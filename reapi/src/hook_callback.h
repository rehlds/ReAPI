#pragma once

#include <tuple>

// hookchain return type
enum HookChainState
{
	HC_CONTINUE = 0,	// Plugin didn't take any action
	HC_SUPERCEDE,		// Skip real function, use my return value
	HC_BREAK,			// Skip all forwards and real function, use my return value
						// @note Warning: Be very careful using this type of return will skip calls for all following AMXX the plugins

	HC_BYPASS			// Skip calls for all following AMXX plugins, but call the original function
						// @note Warning: In PRE skips all forwards including POST forwards
};

// api types
enum AType : uint8
{
	ATYPE_INTEGER = 0,
	ATYPE_FLOAT,
	ATYPE_STRING,
	ATYPE_CLASSPTR,
	ATYPE_EDICT,
	ATYPE_EVARS,
	ATYPE_BOOL,
	ATYPE_VECTOR,
	ATYPE_TRACE
};

struct retval_t
{
	bool set;
	AType type;

	union
	{
		char*			_string;
		float			_float;
		int				_integer;
		CBaseEntity*	_classptr;
		edict_t*		_edict;
		entvars_t*		_pev;
		Vector*			_vector;
	};
};

inline AType getApiType(int)            { return ATYPE_INTEGER; }
inline AType getApiType(unsigned)       { return ATYPE_INTEGER; }
inline AType getApiType(ULONG)          { return ATYPE_INTEGER; }
inline AType getApiType(float)          { return ATYPE_FLOAT; }
inline AType getApiType(const char *)   { return ATYPE_STRING; }
inline AType getApiType(char[])         { return ATYPE_STRING; }
inline AType getApiType(CBaseEntity *)  { return ATYPE_CLASSPTR; }
inline AType getApiType(edict_t *)      { return ATYPE_EDICT; }
inline AType getApiType(entvars_t *)    { return ATYPE_EVARS; }
inline AType getApiType(bool)           { return ATYPE_BOOL; }
inline AType getApiType(Vector)         { return ATYPE_VECTOR; }
inline AType getApiType(ENTITYINIT)     { return ATYPE_INTEGER; }
inline AType getApiType(TraceResult*)   { return ATYPE_TRACE; }

template<typename T>
inline AType getApiType(T *) { return ATYPE_INTEGER; }

inline bool hasStringArgs() { return false; }

template <typename T, typename ...f_args>
bool hasStringArgs(T, f_args... args)
{
	static_assert(sizeof(T) <= sizeof(int), "invalid hookchain argument size > sizeof(int)");

	if (getApiType(T()) == ATYPE_STRING)
		return true;

	return hasStringArgs(args...);
}

#define MAX_HOOKCHAIN_ARGS 12u

struct hookctx_t
{
	template<typename ...t_args>
	hookctx_t(size_t arg_count, t_args&&... args)
	{
		if (hasStringArgs(args...)) {
			tempstrings_used = 0;
		}

		args_count = min(arg_count, MAX_HOOKCHAIN_ARGS);
		setArgs(std::forward_as_tuple(args...));
	}

	template <size_t current = 0,
			typename tuple_t,
			size_t size = std::tuple_size<typename std::decay<tuple_t>::type>::value,
			std::enable_if_t<current >= size>* = nullptr> // if current >= size
	void setArgs(tuple_t &&t)
	{
	}

	template <size_t current = 0,
			typename tuple_t,
			size_t size = std::tuple_size<typename std::decay<tuple_t>::type>::value,
			std::enable_if_t<current < size>* = nullptr> // if current < size
	void setArgs(tuple_t &&t)
	{
		// current iteration
		if (current < MAX_HOOKCHAIN_ARGS)
		{
			auto &arg = std::get<current>(std::forward<tuple_t>(t));
			args[current].handle = (size_t)&arg;
			args[current].type   = getApiType(arg);
			setArgs<current + 1>(std::forward<tuple_t>(t)); // call next
		}
	}

	void reset(AType ret_type = ATYPE_INTEGER)
	{
		retVal.set = false;
		retVal.type = ret_type;
	}

	char* get_temp_string(AMX* amx)
	{
		auto ptr = s_temp_strings.push(amx);
		if (likely(ptr)) {
			tempstrings_used++;
			return ptr;
		}

		static char fatalErr[] = "<reapi error>";
		return fatalErr;
	}

	void SetId(int id) { index = id; }
	void ResetId()     { index = 0;  }

	void clear_temp_strings() const
	{
		s_temp_strings.pop(tempstrings_used);
	}

	int index                       = 0;
	retval_t retVal                 = {false,ATYPE_INTEGER};
	size_t tempstrings_used         = 0;

	struct args_t
	{
		size_t handle;
		AType  type;
	};

	size_t args_count               = 0;
	args_t args[MAX_HOOKCHAIN_ARGS] = {};
	static CTempStrings s_temp_strings;
};

extern hookctx_t* g_hookCtx;

template <typename original_t, typename ...f_args>
NOINLINE void DLLEXPORT _callVoidForward(hook_t* hook, original_t original, f_args&&... args)
{
	auto hookCtx = g_hookCtx;
	hookCtx->reset();
	int hc_state = HC_CONTINUE;

	hook->wasCalled = false;

	for (auto fwd : hook->pre)
	{
		if (likely(fwd->GetState() == FSTATE_ENABLED))
		{
			hookCtx->SetId(fwd->GetIndex()); // set current handler hook
			int ret = g_amxxapi.ExecuteForward(fwd->GetFwdIndex(), std::forward<f_args &&>(args)...);
			hookCtx->ResetId();

			if (unlikely(ret == HC_BREAK))
				return;

			if (unlikely(ret > hc_state))
				hc_state = ret;

			if (unlikely(ret == HC_BYPASS))
				break;
		}
	}

	if (hc_state != HC_SUPERCEDE) {
		g_hookCtx = nullptr;
		original(std::forward<f_args &&>(args)...);
		g_hookCtx = hookCtx;
		hook->wasCalled = true;
	}

	if (hc_state != HC_BYPASS)
	{
		for (auto fwd : hook->post)
		{
			if (likely(fwd->GetState() == FSTATE_ENABLED))
			{
				hookCtx->SetId(fwd->GetIndex()); // set current handler hook
				int ret = g_amxxapi.ExecuteForward(fwd->GetFwdIndex(), std::forward<f_args &&>(args)...);
				hookCtx->ResetId();

				if (unlikely(ret == HC_BREAK || ret == HC_BYPASS))
					break;
			}
		}
	}

	hook->wasCalled = false;
}

template <typename original_t, typename ...f_args>
void callVoidForward(size_t func, original_t original, f_args&&... args)
{
	hookctx_t hookCtx(sizeof...(args), args...);
	hookctx_t* save = g_hookCtx;

	g_hookCtx = &hookCtx;
	_callVoidForward(g_hookManager.getHookFast(func), original, args...);
	g_hookCtx = save;

	if (hasStringArgs(args...)) {
		hookCtx.clear_temp_strings();
	}
}

template <typename R, typename original_t, typename ...f_args>
NOINLINE R DLLEXPORT _callForward(hook_t* hook, original_t original, f_args&&... args)
{
	using R2 = typename std::decay<R>::type;

	auto hookCtx = g_hookCtx;
	hookCtx->reset(getApiType(R2()));

	int hc_state = HC_CONTINUE;

	hook->wasCalled = false;

	for (auto fwd : hook->pre)
	{
		if (likely(fwd->GetState() == FSTATE_ENABLED))
		{
			hookCtx->SetId(fwd->GetIndex()); // set current handler hook
			auto ret = g_amxxapi.ExecuteForward(fwd->GetFwdIndex(), std::forward<f_args &&>(args)...);
			hookCtx->ResetId();

			if (unlikely(ret != HC_SUPERCEDE && ret != HC_BREAK)) {
				continue;
			}

			if (unlikely(!hookCtx->retVal.set)) {
				fwd->Error(AMX_ERR_ASSERT, "Can't suppress original function call without new return value set, so you must call SetHookChainReturn.");
				continue;
			}

			if (unlikely(ret == HC_BREAK)) {
				return *(R2 *)&hookCtx->retVal._integer;
			}

			if (unlikely(ret > hc_state))
				hc_state = ret;
		}
	}

	if (likely(hc_state != HC_SUPERCEDE))
	{
		g_hookCtx = nullptr;
		R retVal = original(std::forward<f_args &&>(args)...);
		g_hookCtx = hookCtx;
		hook->wasCalled = true;

		if (unlikely(!hookCtx->retVal.set)) {
			switch (sizeof retVal) {
			case sizeof(int8):
				hookCtx->retVal._integer = *(int8 *)&retVal;
				break;
			case sizeof(int16):
				hookCtx->retVal._integer = *(int16 *)&retVal;
				break;
			case sizeof(int32):
				hookCtx->retVal._integer = *(int32 *)&retVal;
				break;
			case sizeof(Vector):
				hookCtx->retVal._vector = (Vector *)&retVal;
				break;
			}
			hookCtx->retVal.set = true;
		}
	}

	for (auto fwd : hook->post)
	{
		if (likely(fwd->GetState() == FSTATE_ENABLED))
		{
			hookCtx->SetId(fwd->GetIndex()); // set current handler hook
			auto ret = g_amxxapi.ExecuteForward(fwd->GetFwdIndex(), std::forward<f_args &&>(args)...);
			hookCtx->ResetId();

			if (unlikely(ret == HC_BREAK))
				break;
		}
	}

	hook->wasCalled = false;

	if (std::is_reference<R>::value)
		return *(R2 *)hookCtx->retVal._integer;
	else
		return *(R2 *)&hookCtx->retVal._integer;
}

template <typename R, typename original_t, typename ...f_args>
R callForward(size_t func, original_t original, f_args&&... args)
{
	hookctx_t hookCtx(sizeof...(args), args...);
	hookctx_t* save = g_hookCtx;

	g_hookCtx = &hookCtx;
	R ret = _callForward<R>(g_hookManager.getHookFast(func), original, args...);
	g_hookCtx = save;

	if (hasStringArgs(args...)) {
		hookCtx.clear_temp_strings();
	}

	return ret;
}

template<typename T, typename A>
struct hookdata_t
{
	hookdata_t(T chain, A args) : m_chain(chain), m_args(args) {}

	T m_chain;
	A m_args;
};

/*
* ReHLDS functions
*/
void SV_StartSound(IRehldsHook_SV_StartSound *chain, int recipients, edict_t *entity, int channel, const char *sample, int volume, float attenuation, int fFlags, int pitch);
void SV_DropClient(IRehldsHook_SV_DropClient *chain, IGameClient *cl, bool crash, const char *fmt);
void SV_ActivateServer(IRehldsHook_SV_ActivateServer *chain, int runPhysics);
void Cvar_DirectSet(IRehldsHook_Cvar_DirectSet *chain, cvar_t *var, const char *value);
void ClientConnected(IRehldsHook_ClientConnected* chain, IGameClient* cl);
void SV_ConnectClient(IRehldsHook_SV_ConnectClient* chain);
BOOL SV_CheckUserInfo(IRehldsHook_SV_CheckUserInfo *chain, netadr_t *adr, char *userinfo, qboolean bIsReconnecting, int iReconnectSlot, char *name);
BOOL SV_CheckUserInfo_AMXX(IRehldsHook_SV_CheckUserInfo *chain, netadr_t *adr, size_t userinfo, qboolean bIsReconnecting, int iReconnectSlot, char *name);

struct SV_WriteFullClientUpdate_args_t
{
	SV_WriteFullClientUpdate_args_t(sizebuf_t *msg, int size) : message(msg), maxlen(size) {}

	sizebuf_t *message;
	int maxlen;
};

using SV_WriteFullClientUpdate_t = hookdata_t<IRehldsHook_SV_WriteFullClientUpdate *, SV_WriteFullClientUpdate_args_t &>;
void SV_WriteFullClientUpdate_AMXX(SV_WriteFullClientUpdate_t *data, IGameClient *client, size_t buffer, IGameClient *receiver);
void SV_WriteFullClientUpdate(IRehldsHook_SV_WriteFullClientUpdate *chain, IGameClient *client, char *buffer, size_t maxlen, sizebuf_t *sb, IGameClient *receiver);
ENTITYINIT GetEntityInit(IRehldsHook_GetEntityInit *chain, char *classname);
void ExecuteServerStringCmd(IRehldsHook_ExecuteServerStringCmd* chain, const char* cmdName, cmd_source_t src, IGameClient* client);

struct SV_EmitPings_args_t
{
	SV_EmitPings_args_t(IGameClient* cl, sizebuf_t *msg) : client(cl), message(msg) {}

	IGameClient *client;
	sizebuf_t* message;
};

using SV_EmitPings_t = hookdata_t<IRehldsHook_SV_EmitPings *, SV_EmitPings_args_t &>;
void SV_EmitPings_AMXX(SV_EmitPings_t *data, IGameClient *client);
void SV_EmitPings(IRehldsHook_SV_EmitPings *chain, IGameClient *client, sizebuf_t *msg);
void Con_Printf(IRehldsHook_Con_Printf *chain, const char *string);
int PF_precache_generic_I(IRehldsHook_PF_precache_generic_I *chain, const char *s);
int PF_precache_model_I(IRehldsHook_PF_precache_model_I *chain, const char *s);
int PF_precache_sound_I(IRehldsHook_PF_precache_sound_I *chain, const char *s);

using SV_SendResources_t = hookdata_t<IRehldsHook_SV_SendResources *, sizebuf_t *>;
void SV_SendResources_AMXX(SV_SendResources_t *data, IGameClient *cl);
void SV_SendResources(IRehldsHook_SV_SendResources *chain, sizebuf_t *msg);

struct EventPrecache_args_t
{
	EventPrecache_args_t(int _type) : type(_type) {}
	int type;
};

using EventPrecache_t = hookdata_t<IRehldsHook_EV_Precache *, EventPrecache_args_t &>;
unsigned short EV_Precache_AMXX(EventPrecache_t *data, const char *psz);
unsigned short EV_Precache(IRehldsHook_EV_Precache *chain, int type, const char *psz);
void SV_AddResource(IRehldsHook_SV_AddResource *chain, resourcetype_t type, const char *name, int size, unsigned char flags, int index);
edict_t *ED_Alloc(IRehldsHook_ED_Alloc* chain);
void ED_Free(IRehldsHook_ED_Free* chain, edict_t *entity);
void SV_ClientPrintf(IRehldsHook_SV_ClientPrintf* chain, const char *string);
bool SV_AllowPhysent(IRehldsHook_SV_AllowPhysent* chain, edict_t* check, edict_t* sv_player);

/*
* ReGameDLL functions
*/
void CBasePlayer_Spawn(IReGameHook_CBasePlayer_Spawn *chain, CBasePlayer *pthis);
void CBasePlayer_Precache(IReGameHook_CBasePlayer_Precache *chain, CBasePlayer *pthis);
int CBasePlayer_ObjectCaps(IReGameHook_CBasePlayer_ObjectCaps *chain, CBasePlayer *pthis);
int CBasePlayer_Classify(IReGameHook_CBasePlayer_Classify *chain, CBasePlayer *pthis);
void CBasePlayer_TraceAttack(IReGameHook_CBasePlayer_TraceAttack *chain, CBasePlayer *pthis, entvars_t *pevAttacker, float flDamage, Vector& vecDir, TraceResult *ptr, int bitsDamageType);
int CBasePlayer_TakeDamage(IReGameHook_CBasePlayer_TakeDamage *chain, CBasePlayer *pthis, entvars_t *pevInflictor, entvars_t *pevAttacker, float& flDamage, int bitsDamageType);
int CBasePlayer_TakeHealth(IReGameHook_CBasePlayer_TakeHealth *chain, CBasePlayer *pthis, float flHealth, int bitsDamageType);
void CBasePlayer_Killed(IReGameHook_CBasePlayer_Killed *chain, CBasePlayer *pthis, entvars_t *pevAttacker, int iGib);
void CBasePlayer_AddPoints(IReGameHook_CBasePlayer_AddPoints *chain, CBasePlayer *pthis, int score, BOOL bAllowNegativeScore);
void CBasePlayer_AddPointsToTeam(IReGameHook_CBasePlayer_AddPointsToTeam *chain, CBasePlayer *pthis, int score, BOOL bAllowNegativeScore);
BOOL CBasePlayer_AddPlayerItem(IReGameHook_CBasePlayer_AddPlayerItem *chain, CBasePlayer *pthis, CBasePlayerItem *pItem);
BOOL CBasePlayer_RemovePlayerItem(IReGameHook_CBasePlayer_RemovePlayerItem *chain, CBasePlayer *pthis, CBasePlayerItem *pItem);
int CBasePlayer_GiveAmmo(IReGameHook_CBasePlayer_GiveAmmo *chain, CBasePlayer *pthis, int iAmount, const char *szName, int iMax);
void CBasePlayer_ResetMaxSpeed(IReGameHook_CBasePlayer_ResetMaxSpeed *chain, CBasePlayer *pthis);
void CBasePlayer_Jump(IReGameHook_CBasePlayer_Jump *chain, CBasePlayer *pthis);
void CBasePlayer_Duck(IReGameHook_CBasePlayer_Duck *chain, CBasePlayer *pthis);
void CBasePlayer_PreThink(IReGameHook_CBasePlayer_PreThink *chain, CBasePlayer *pthis);
void CBasePlayer_PostThink(IReGameHook_CBasePlayer_PostThink *chain, CBasePlayer *pthis);
void CBasePlayer_UpdateClientData(IReGameHook_CBasePlayer_UpdateClientData *chain, CBasePlayer *pthis);
void CBasePlayer_ImpulseCommands(IReGameHook_CBasePlayer_ImpulseCommands *chain, CBasePlayer *pthis);
void CBasePlayer_RoundRespawn(IReGameHook_CBasePlayer_RoundRespawn *chain, CBasePlayer *pthis);
void CBasePlayer_Blind(IReGameHook_CBasePlayer_Blind *chain, CBasePlayer *pthis, float flUntilTime, float flHoldTime, float flFadeTime, int iAlpha);

CBasePlayer *CBasePlayer_Observer_IsValidTarget(IReGameHook_CBasePlayer_Observer_IsValidTarget *chain, CBasePlayer *pthis, int iPlayerIndex, bool bSameTeam);
void CBasePlayer_SetAnimation(IReGameHook_CBasePlayer_SetAnimation *chain, CBasePlayer *pthis, PLAYER_ANIM playerAnim);
void CBasePlayer_GiveDefaultItems(IReGameHook_CBasePlayer_GiveDefaultItems *chain, CBasePlayer *pthis);
CBaseEntity *CBasePlayer_GiveNamedItem(IReGameHook_CBasePlayer_GiveNamedItem *chain, CBasePlayer *pthis, const char *pszName);
void CBasePlayer_AddAccount(IReGameHook_CBasePlayer_AddAccount *chain, CBasePlayer *pthis, int amount, RewardType type, bool bTrackChange);
void CBasePlayer_GiveShield(IReGameHook_CBasePlayer_GiveShield *chain, CBasePlayer *pthis, bool bDeploy);
void CBasePlayer_SetClientUserInfoModel(IReGameHook_CBasePlayer_SetClientUserInfoModel *chain, CBasePlayer *pthis, char *infobuffer, char *szNewModel);
bool CBasePlayer_SetClientUserInfoName(IReGameHook_CBasePlayer_SetClientUserInfoName *chain, CBasePlayer *pthis, char *infobuffer, char *szNewName);
bool CBasePlayer_HasRestrictItem(IReGameHook_CBasePlayer_HasRestrictItem *chain, CBasePlayer *pthis, ItemID item, ItemRestType type);
CBaseEntity *CBasePlayer_DropPlayerItem(IReGameHook_CBasePlayer_DropPlayerItem *chain, CBasePlayer *pthis, const char *pszItemName);
CBaseEntity *CBasePlayer_DropShield(IReGameHook_CBasePlayer_DropShield *chain, CBasePlayer *pthis, bool bDeploy);
void CBasePlayer_OnSpawnEquip(IReGameHook_CBasePlayer_OnSpawnEquip *chain, CBasePlayer *pthis, bool addDefault, bool equipGame);
void CBasePlayer_Radio(IReGameHook_CBasePlayer_Radio *chain, CBasePlayer *pthis, const char *msg_id, const char *msg_verbose, short pitch, bool showIcon);
void CBasePlayer_Disappear(IReGameHook_CBasePlayer_Disappear *chain, CBasePlayer *pthis);
void CBasePlayer_MakeVIP(IReGameHook_CBasePlayer_MakeVIP *chain, CBasePlayer *pthis);
bool CBasePlayer_MakeBomber(IReGameHook_CBasePlayer_MakeBomber *chain, CBasePlayer *pthis);
void CBasePlayer_StartObserver(IReGameHook_CBasePlayer_StartObserver *chain, CBasePlayer *pthis, Vector &vecPosition, Vector &vecViewAngle);
bool CBasePlayer_GetIntoGame(IReGameHook_CBasePlayer_GetIntoGame *chain, CBasePlayer *pthis);

void CBaseAnimating_ResetSequenceInfo(IReGameHook_CBaseAnimating_ResetSequenceInfo *chain, CBaseAnimating *pthis);

int GetForceCamera(IReGameHook_GetForceCamera *chain, CBasePlayer *pObserver);
void PlayerBlind(IReGameHook_PlayerBlind *chain, CBasePlayer *pPlayer, entvars_t *pevInflictor, entvars_t *pevAttacker, float fadeTime, float fadeHold, int alpha, Vector& color);
void RadiusFlash_TraceLine(IReGameHook_RadiusFlash_TraceLine *chain, CBasePlayer *pPlayer, entvars_t *pevInflictor, entvars_t *pevAttacker, Vector& vecSrc, Vector& vecSpot, TraceResult *ptr);
bool RoundEnd(IReGameHook_RoundEnd *chain, int winStatus, ScenarioEventEndRound event, float tmDelay);
// TODO: pending InstallGameRules
// TODO: pending PM_Init
struct Move_args_t
{
	Move_args_t(playermove_t *_ppmove, int _server) : ppmove(_ppmove), server(_server) {}

	playermove_t *ppmove;
	int server;
};
using Move_t = hookdata_t<IReGameHook_PM_Move *, Move_args_t &>;
void PM_Move_AMXX(Move_t *data, int playerIndex);
void PM_Move(IReGameHook_PM_Move *chain, playermove_t *ppmove, int server);
void PM_AirMove(IReGameHook_PM_AirMove *chain, int playerIndex);
void HandleMenu_ChooseAppearance(IReGameHook_HandleMenu_ChooseAppearance *chain, CBasePlayer *pPlayer, int slot);
BOOL HandleMenu_ChooseTeam(IReGameHook_HandleMenu_ChooseTeam *chain, CBasePlayer *pPlayer, int slot);
void ShowMenu(IReGameHook_ShowMenu *chain, CBasePlayer *pPlayer, int bitsValidSlots, int nDisplayTime, BOOL fNeedMore, char *pszText);
void ShowVGUIMenu(IReGameHook_ShowVGUIMenu *chain, CBasePlayer *pPlayer, int MenuType, int BitMask, char *szOldMenu);
bool BuyGunAmmo(IReGameHook_BuyGunAmmo *chain, CBasePlayer *player, CBasePlayerItem *weapon, bool bBlinkMoney);
CBaseEntity *BuyWeaponByWeaponID(IReGameHook_BuyWeaponByWeaponID *chain, CBasePlayer *pPlayer, WeaponIdType weaponID);
// TODO: pending InternalCommand

BOOL CSGameRules_FShouldSwitchWeapon(IReGameHook_CSGameRules_FShouldSwitchWeapon *chain, CBasePlayer *pPlayer, CBasePlayerItem *pWeapon);
BOOL CSGameRules_GetNextBestWeapon(IReGameHook_CSGameRules_GetNextBestWeapon *chain, CBasePlayer *pPlayer, CBasePlayerItem *pCurrentWeapon);
float CSGameRules_FlPlayerFallDamage(IReGameHook_CSGameRules_FlPlayerFallDamage *chain, CBasePlayer *pPlayer);
BOOL CSGameRules_FPlayerCanTakeDamage(IReGameHook_CSGameRules_FPlayerCanTakeDamage *chain, CBasePlayer *pPlayer, CBaseEntity *pAttacker);
void CSGameRules_PlayerSpawn(IReGameHook_CSGameRules_PlayerSpawn *chain, CBasePlayer *pPlayer);
BOOL CSGameRules_FPlayerCanRespawn(IReGameHook_CSGameRules_FPlayerCanRespawn *chain, CBasePlayer *pPlayer);
edict_t *CSGameRules_GetPlayerSpawnSpot(IReGameHook_CSGameRules_GetPlayerSpawnSpot *chain, CBasePlayer *pPlayer);
void CSGameRules_ClientUserInfoChanged(IReGameHook_CSGameRules_ClientUserInfoChanged *chain, CBasePlayer *pPlayer, char *infobuffer);
void CSGameRules_PlayerKilled(IReGameHook_CSGameRules_PlayerKilled *chain, CBasePlayer *pVictim, entvars_t *pevKiller, entvars_t *pevInflictor);
void CSGameRules_DeathNotice(IReGameHook_CSGameRules_DeathNotice *chain, CBasePlayer *pVictim, entvars_t *pevKiller, entvars_t *pevInflictor);
int CSGameRules_CanHavePlayerItem(IReGameHook_CSGameRules_CanHavePlayerItem *chain, CBasePlayer *pPlayer, CBasePlayerItem *pItem);
int CSGameRules_DeadPlayerWeapons(IReGameHook_CSGameRules_DeadPlayerWeapons *chain, CBasePlayer *pPlayer);
void CSGameRules_ServerDeactivate(IReGameHook_CSGameRules_ServerDeactivate *chain);
void CSGameRules_CheckMapConditions(IReGameHook_CSGameRules_CheckMapConditions *chain);
void CSGameRules_CleanUpMap(IReGameHook_CSGameRules_CleanUpMap *chain);
void CSGameRules_RestartRound(IReGameHook_CSGameRules_RestartRound *chain);
void CSGameRules_CheckWinConditions(IReGameHook_CSGameRules_CheckWinConditions *chain);
void CSGameRules_RemoveGuns(IReGameHook_CSGameRules_RemoveGuns *chain);
CBasePlayer *CSGameRules_GiveC4(IReGameHook_CSGameRules_GiveC4 *chain);
void CSGameRules_ChangeLevel(IReGameHook_CSGameRules_ChangeLevel *chain);
void CSGameRules_GoToIntermission(IReGameHook_CSGameRules_GoToIntermission *chain);
void CSGameRules_BalanceTeams(IReGameHook_CSGameRules_BalanceTeams *chain);
void CSGameRules_OnRoundFreezeEnd(IReGameHook_CSGameRules_OnRoundFreezeEnd *chain);
// TODO: pending PM_UpdateStepSound
void CBasePlayer_StartDeathCam(IReGameHook_CBasePlayer_StartDeathCam *chain, CBasePlayer *pthis);
void CBasePlayer_SwitchTeam(IReGameHook_CBasePlayer_SwitchTeam *chain, CBasePlayer *pthis);
bool CBasePlayer_CanSwitchTeam(IReGameHook_CBasePlayer_CanSwitchTeam *chain, CBasePlayer *pthis, TeamName teamToSwap);
CGrenade *CBasePlayer_ThrowGrenade(IReGameHook_CBasePlayer_ThrowGrenade *chain, CBasePlayer *pthis, CBasePlayerWeapon *pWeapon, Vector &vecSrc, Vector &vecThrow, float time, unsigned short usEvent);
bool CSGameRules_CanPlayerHearPlayer(IReGameHook_CSGameRules_CanPlayerHearPlayer *chain, CBasePlayer *pListener, CBasePlayer *pSender);
void CWeaponBox_SetModel(IReGameHook_CWeaponBox_SetModel *chain, CWeaponBox *pthis, const char *pszModelName);
void CGrenade_DefuseBombStart(IReGameHook_CGrenade_DefuseBombStart *chain, CGrenade *pthis, CBasePlayer *pPlayer);
void CGrenade_DefuseBombEnd(IReGameHook_CGrenade_DefuseBombEnd *chain, CGrenade *pthis, CBasePlayer *pPlayer, bool bDefused);
void CGrenade_ExplodeHeGrenade(IReGameHook_CGrenade_ExplodeHeGrenade *chain, CGrenade *pthis, TraceResult *ptr, int bitsDamageType);
void CGrenade_ExplodeFlashbang(IReGameHook_CGrenade_ExplodeFlashbang *chain, CGrenade *pthis, TraceResult *ptr, int bitsDamageType);
void CGrenade_ExplodeSmokeGrenade(IReGameHook_CGrenade_ExplodeSmokeGrenade *chain, CGrenade *pthis);
void CGrenade_ExplodeBomb(IReGameHook_CGrenade_ExplodeBomb *chain, CGrenade *pthis, TraceResult *ptr, int bitsDamageType);
CGrenade *ThrowHeGrenade(IReGameHook_ThrowHeGrenade *chain, entvars_t *pevOwner, Vector &vecStart, Vector &vecVelocity, float time, int iTeam, unsigned short usEvent);
CGrenade *ThrowFlashbang(IReGameHook_ThrowFlashbang *chain, entvars_t *pevOwner, Vector &vecStart, Vector &vecVelocity, float time);
CGrenade *ThrowSmokeGrenade(IReGameHook_ThrowSmokeGrenade *chain, entvars_t *pevOwner, Vector &vecStart, Vector &vecVelocity, float time, unsigned short usEvent);
CGrenade *PlantBomb(IReGameHook_PlantBomb *chain, entvars_t *pevOwner, Vector &vecStart, Vector &vecVelocity);
void CBasePlayer_RemoveSpawnProtection(IReGameHook_CBasePlayer_RemoveSpawnProtection *chain, CBasePlayer *pthis);
void CBasePlayer_SetSpawnProtection(IReGameHook_CBasePlayer_SetSpawnProtection *chain, CBasePlayer *pthis, float flProtectionTime);
bool IsPenetrableEntity(IReGameHook_IsPenetrableEntity *chain, Vector &vecSrc, Vector &vecEnd, entvars_t *pevAttacker, edict_t *pHit);
bool CBasePlayer_HintMessageEx(IReGameHook_CBasePlayer_HintMessageEx *chain, CBasePlayer *pthis, const char *pMessage, float duration, bool bDisplayIfPlayerDead, bool bOverride);
void CBasePlayer_UseEmpty(IReGameHook_CBasePlayer_UseEmpty *chain, CBasePlayer *pthis);
BOOL CBasePlayerWeapon_CanDeploy(IReGameHook_CBasePlayerWeapon_CanDeploy *chain, CBasePlayerWeapon *pthis);
BOOL CBasePlayerWeapon_DefaultDeploy(IReGameHook_CBasePlayerWeapon_DefaultDeploy *chain, CBasePlayerWeapon *pthis, char *szViewModel, char *szWeaponModel, int iAnim, char *szAnimExt, int skiplocal);
int CBasePlayerWeapon_DefaultReload(IReGameHook_CBasePlayerWeapon_DefaultReload *chain, CBasePlayerWeapon *pthis, int iClipSize, int iAnim, float fDelay);
bool CBasePlayerWeapon_DefaultShotgunReload(IReGameHook_CBasePlayerWeapon_DefaultShotgunReload *chain, CBasePlayerWeapon *pthis, int iAnim, int iStartAnim, float fDelay, float fStartDelay, const char *pszReloadSound1, const char *pszReloadSound2);
void CBasePlayer_DropIdlePlayer(IReGameHook_CBasePlayer_DropIdlePlayer *chain, CBasePlayer *pthis, const char *reason);
CWeaponBox *CreateWeaponBox(IReGameHook_CreateWeaponBox *chain, CBasePlayerItem *pItem, CBasePlayer *pPlayerOwner, const char *modelName, Vector &origin, Vector &angles, Vector &velocity, float lifeTime, bool packAmmo);

CGib *SpawnHeadGib(IReGameHook_SpawnHeadGib *chain, entvars_t *pevVictim);
void SpawnRandomGibs(IReGameHook_SpawnRandomGibs *chain, entvars_t *pevVictim, int cGibs, int human);
void CGib_Spawn(IReGameHook_CGib_Spawn *chain, CGib *pthis, const char *szGibModel);
void CGib_BounceGibTouch(IReGameHook_CGib_BounceGibTouch *chain, CGib *pthis, CBaseEntity *pOther);
void CGib_WaitTillLand(IReGameHook_CGib_WaitTillLand *chain, CGib *pthis);

void CBaseEntity_FireBullets(IReGameHook_CBaseEntity_FireBullets *chain, CBaseEntity *pEntity, ULONG cShots, Vector &vecSrc, Vector &vecDirShooting, Vector &vecSpread, float flDistance, int iBulletType, int iTracerFreq, int iDamage, entvars_t *pevAttacker);
void CBaseEntity_FireBuckshots(IReGameHook_CBaseEntity_FireBuckshots *chain, CBaseEntity *pEntity, ULONG cShots, Vector &vecSrc, Vector &vecDirShooting, Vector &vecSpread, float flDistance, int iTracerFreq, int iDamage, entvars_t *pevAttacker);
Vector &CBaseEntity_FireBullets3(IReGameHook_CBaseEntity_FireBullets3 *chain, CBaseEntity *pEntity, Vector &vecSrc, Vector &vecDirShooting, float vecSpread, float flDistance, int iPenetration, int iBulletType, int iDamage, float flRangeModifier, entvars_t *pevAttacker, bool bPistol, int shared_rand);

void CBasePlayer_Observer_SetMode(IReGameHook_CBasePlayer_Observer_SetMode *chain, CBasePlayer *pthis, int iMode);
void CBasePlayer_Observer_FindNextPlayer(IReGameHook_CBasePlayer_Observer_FindNextPlayer *chain, CBasePlayer *pthis, bool bReverse, const char *name);

void CBasePlayer_Pain(IReGameHook_CBasePlayer_Pain *chain, CBasePlayer *pthis, int iLastHitGroup, bool bHasArmour);
void CBasePlayer_DeathSound(IReGameHook_CBasePlayer_DeathSound *chain, CBasePlayer *pthis);
void CBasePlayer_JoiningThink(IReGameHook_CBasePlayer_JoiningThink *chain, CBasePlayer *pthis);

// TODO: pending FreeGameRules
void PM_LadderMove_AMXX(IReGameHook_PM_LadderMove *chain, physent_t *pLadder, int playerIndex);
void PM_LadderMove(IReGameHook_PM_LadderMove *chain, physent_t *pLadder);
void PM_WaterJump_AMXX(IReGameHook_PM_WaterJump *chain, int playerIndex);
void PM_WaterJump(IReGameHook_PM_WaterJump *chain);
void PM_CheckWaterJump_AMXX(IReGameHook_PM_CheckWaterJump *chain, int playerIndex);
void PM_CheckWaterJump(IReGameHook_PM_CheckWaterJump *chain);
void PM_Jump_AMXX(IReGameHook_PM_Jump *chain, int playerIndex);
void PM_Jump(IReGameHook_PM_Jump *chain);
void PM_Duck_AMXX(IReGameHook_PM_Duck *chain, int playerIndex);
void PM_Duck(IReGameHook_PM_Duck *chain);
void PM_UnDuck_AMXX(IReGameHook_PM_UnDuck *chain, int playerIndex);
void PM_UnDuck(IReGameHook_PM_UnDuck *chain);
void PM_PlayStepSound_AMXX(IReGameHook_PM_PlayStepSound *chain, int step, float fvol, int playerIndex);
void PM_PlayStepSound(IReGameHook_PM_PlayStepSound *chain, int step, float fvol);
void PM_AirAccelerate_AMXX(IReGameHook_PM_AirAccelerate *chain, vec_t *wishdir, float wishspeed, float accel, int playerIndex);
void PM_AirAccelerate(IReGameHook_PM_AirAccelerate *chain, vec_t *wishdir, float wishspeed, float accel);
void ClearMultiDamage(IReGameHook_ClearMultiDamage *chain);
void AddMultiDamage(IReGameHook_AddMultiDamage *chain, entvars_t *pevInflictor, CBaseEntity *pEntity, float flDamage, int bitsDamageType);
void ApplyMultiDamage(IReGameHook_ApplyMultiDamage *chain, entvars_t *pevInflictor, entvars_t *pevAttacker);
void BuyItem(IReGameHook_BuyItem *chain, CBasePlayer *pPlayer, int iSlot);
void CSGameRules_Think(IReGameHook_CSGameRules_Think *chain);
BOOL CSGameRules_TeamFull(IReGameHook_CSGameRules_TeamFull *chain, int team_id);
BOOL CSGameRules_TeamStacked(IReGameHook_CSGameRules_TeamStacked *chain, int newTeam_id, int curTeam_id);
void CSGameRules_PlayerGotWeapon(IReGameHook_CSGameRules_PlayerGotWeapon *chain, CBasePlayer *pPlayer, CBasePlayerItem *pWeapon);
void CBotManager_OnEvent(IReGameHook_CBotManager_OnEvent *chain, GameEventType event, CBaseEntity* pEntity, CBaseEntity* pOther);
void CBasePlayer_CheckTimeBasedDamage(IReGameHook_CBasePlayer_CheckTimeBasedDamage *chain, CBasePlayer *pthis);
edict_t *CBasePlayer_EntSelectSpawnPoint(IReGameHook_CBasePlayer_EntSelectSpawnPoint *chain, CBasePlayer *pthis);
void CBasePlayerWeapon_ItemPostFrame(IReGameHook_CBasePlayerWeapon_ItemPostFrame *chain, CBasePlayerWeapon *pthis);
void CBasePlayerWeapon_KickBack(IReGameHook_CBasePlayerWeapon_KickBack *chain, CBasePlayerWeapon *pthis, float up_base, float lateral_base, float up_modifier, float lateral_modifier, float up_max, float lateral_max, int direction_change);
void CBasePlayerWeapon_SendWeaponAnim(IReGameHook_CBasePlayerWeapon_SendWeaponAnim *chain, CBasePlayerWeapon *pthis, int iAnim, int skiplocal);
void CBasePlayer_PlayerDeathThink(IReGameHook_CBasePlayer_PlayerDeathThink *chain, CBasePlayer *pthis);
void CBasePlayer_Observer_Think(IReGameHook_CBasePlayer_Observer_Think *chain, CBasePlayer *pthis);
void CBasePlayer_RemoveAllItems(IReGameHook_CBasePlayer_RemoveAllItems *chain, CBasePlayer *pthis, BOOL removeSuit);
void CSGameRules_SendDeathMessage(IReGameHook_CSGameRules_SendDeathMessage *chain, CBaseEntity *pKiller, CBasePlayer *pVictim, CBasePlayer *pAssister, entvars_t *pevInflictor, const char *killerWeaponName, int iDeathMessageFlags, int iRarityOfKill);
void CBasePlayer_UpdateStatusBar(IReGameHook_CBasePlayer_UpdateStatusBar *chain, CBasePlayer *pthis);
void CBasePlayer_TakeDamageImpulse(IReGameHook_CBasePlayer_TakeDamageImpulse *chain, CBasePlayer *pthis, CBasePlayer *pAttacker, float flKnockbackForce, float flVelModifier);

/*
* VTC functions
*/
extern int g_iClientStartSpeak;
extern int g_iClientStopSpeak;

void OnClientStartSpeak(size_t clientIndex);
void OnClientStopSpeak(size_t clientIndex);

/*
* ReChecker functions
*/

using CmdExec_t = hookdata_t<IRecheckerHook_CmdExec *, IResourceBuffer *>;
void CmdExec_AMXX(CmdExec_t *chain, IGameClient *cl, const char *filename, char *cmd, uint32 responseHash);
void CmdExec(IRecheckerHook_CmdExec *chain, IGameClient *cl, IResourceBuffer *res, char *cmd, uint32 responseHash);

using FileConsistencyProcess_t = hookdata_t<IRecheckerHook_FileConsistencyProcess *, IResourceBuffer *>;
void FileConsistencyProcess_AMXX(FileConsistencyProcess_t *data, IGameClient *cl, const char *filename, const char *cmd, ResourceType_e type, uint32 responseHash, bool isBreak);
void FileConsistencyProcess(IRecheckerHook_FileConsistencyProcess *chain, IGameClient *cl, IResourceBuffer *res, ResourceType_e typeFind, uint32 responseHash);

void FileConsistencyFinal(IRecheckerHook_FileConsistencyFinal *chain, IGameClient *cl);

