#pragma once

#define MAX_HOOK_FORWARDS		1024
#define MAX_REGION_RANGE		1024

#define BEGIN_FUNC_REGION(x)		(MAX_REGION_RANGE * hooklist_t::hooks_tables_e::ht_##x)

typedef bool (*reqfunc_t) ();
typedef int  (*regfunc_t) (AMX *, const char *);
typedef void (*regchain_t)();

struct hook_t
{
	std::vector<class CAmxxHookBase *> pre;   // pre forwards
	std::vector<class CAmxxHookBase *> post;  // post forwards

	const char *func_name;                  // function name
	const char *depend_name;                // platform dependency

	reqfunc_t checkRequirements;
	regfunc_t registerForward;              // AMXX forward registration function
	regchain_t registerHookchain;           // register re* API hook
	regchain_t unregisterHookchain;         // unregister re* API hook

	void clear();
};

extern hook_t hooklist_engine[];
extern hook_t hooklist_gamedll[];
extern hook_t hooklist_animating[];
extern hook_t hooklist_player[];
extern hook_t hooklist_gamerules[];
extern hook_t hooklist_rechecker[];
extern hook_t hooklist_grenade[];
extern hook_t hooklist_weaponbox[];
extern hook_t hooklist_weapon[];
extern hook_t hooklist_gib[];

enum
{
	INVALID_HOOKCHAIN = 0
};

struct hooklist_t
{
	hook_t *operator[](size_t hook) const
	{
		#define CASE(h)	case ht_##h: return &hooklist_##h[index];

		const auto table = hooks_tables_e(hook / MAX_REGION_RANGE);
		const auto index = hook & (MAX_REGION_RANGE - 1);

		switch (table) {
			CASE(engine)
			CASE(gamedll)
			CASE(animating)
			CASE(player)
			CASE(gamerules)
			CASE(rechecker)
			CASE(grenade)
			CASE(weaponbox)
			CASE(weapon)
			CASE(gib)
		}

		#undef CASE

		return nullptr;
	}

	static hook_t *getHookSafe(size_t hook);
	static void clear();

	enum hooks_tables_e
	{
		ht_engine,
		ht_gamedll,
		ht_animating,
		ht_player,
		ht_gamerules,
		ht_rechecker,
		ht_grenade,
		ht_weaponbox,
		ht_weapon,
		ht_gib,
	};
};

enum EngineFunc
{
	RH_SV_StartSound = BEGIN_FUNC_REGION(engine),
	RH_SV_DropClient,
	RH_SV_ActivateServer,
	RH_Cvar_DirectSet,
	RH_SV_WriteFullClientUpdate

	// [...]
};

enum GamedllFunc
{
	RG_GetForceCamera = BEGIN_FUNC_REGION(gamedll),
	RG_PlayerBlind,
	RG_RadiusFlash_TraceLine,
	RG_RoundEnd,

	RG_PM_Move,
	RG_PM_AirMove,
	RG_HandleMenu_ChooseAppearance,
	RG_HandleMenu_ChooseTeam,
	RG_ShowMenu,
	RG_ShowVGUIMenu,

	RG_BuyGunAmmo,
	RG_BuyWeaponByWeaponID,

	RG_ThrowHeGrenade,
	RG_ThrowFlashbang,
	RG_ThrowSmokeGrenade,
	RG_PlantBomb,
	RG_IsPenetrableEntity,

	RG_SpawnHeadGib,
	RG_SpawnRandomGibs,

	// [...]
};

enum GamedllFunc_CBaseAnimating
{
	RG_CBaseAnimating_ResetSequenceInfo = BEGIN_FUNC_REGION(animating),

	// [...]
};

enum GamedllFunc_CBasePlayer
{
	// CBasePlayer virtual
	RG_CBasePlayer_Spawn = BEGIN_FUNC_REGION(player),
	RG_CBasePlayer_Precache,
	RG_CBasePlayer_ObjectCaps,
	RG_CBasePlayer_Classify,
	RG_CBasePlayer_TraceAttack,
	RG_CBasePlayer_TakeDamage,
	RG_CBasePlayer_TakeHealth,
	RG_CBasePlayer_Killed,
	RG_CBasePlayer_AddPoints,
	RG_CBasePlayer_AddPointsToTeam,
	RG_CBasePlayer_AddPlayerItem,
	RG_CBasePlayer_RemovePlayerItem,
	RG_CBasePlayer_GiveAmmo,
	RG_CBasePlayer_ResetMaxSpeed,
	RG_CBasePlayer_Jump,
	RG_CBasePlayer_Duck,
	RG_CBasePlayer_PreThink,
	RG_CBasePlayer_PostThink,
	RG_CBasePlayer_UpdateClientData,
	RG_CBasePlayer_ImpulseCommands,
	RG_CBasePlayer_RoundRespawn,
	RG_CBasePlayer_Blind,

	RG_CBasePlayer_SetClientUserInfoModel,
	RG_CBasePlayer_SetClientUserInfoName,
	RG_CBasePlayer_Observer_IsValidTarget,
	RG_CBasePlayer_SetAnimation,
	RG_CBasePlayer_GiveDefaultItems,
	RG_CBasePlayer_GiveNamedItem,
	RG_CBasePlayer_AddAccount,
	RG_CBasePlayer_GiveShield,
	RG_CBasePlayer_DropPlayerItem,
	RG_CBasePlayer_HasRestrictItem,

	RG_CBasePlayer_DropShield,
	RG_CBasePlayer_OnSpawnEquip,
	RG_CBasePlayer_Radio,
	RG_CBasePlayer_Disappear,
	RG_CBasePlayer_MakeVIP,
	RG_CBasePlayer_MakeBomber,
	RG_CBasePlayer_StartObserver,
	RG_CBasePlayer_GetIntoGame,
	RG_CBasePlayer_StartDeathCam,
	RG_CBasePlayer_SwitchTeam,
	RG_CBasePlayer_CanSwitchTeam,
	RG_CBasePlayer_ThrowGrenade,
	RG_CBasePlayer_SetSpawnProtection,
	RG_CBasePlayer_RemoveSpawnProtection,
	RG_CBasePlayer_HintMessageEx,
	RG_CBasePlayer_UseEmpty,
	RG_CBasePlayer_DropIdlePlayer,

	// [...]
};

enum GamedllFunc_CGrenade
{
	RG_CGrenade_DefuseBombStart = BEGIN_FUNC_REGION(grenade),
	RG_CGrenade_DefuseBombEnd,
	RG_CGrenade_ExplodeHeGrenade,
	RG_CGrenade_ExplodeFlashbang,
	RG_CGrenade_ExplodeSmokeGrenade,
	RG_CGrenade_ExplodeBomb,

	// [...]
};

enum GamedllFunc_CWeaponBox
{
	RG_CWeaponBox_SetModel = BEGIN_FUNC_REGION(weaponbox),

	// [...]
};

enum GamedllFunc_CBasePlayerWeapon
{
	RG_CBasePlayerWeapon_CanDeploy = BEGIN_FUNC_REGION(weapon),
	RG_CBasePlayerWeapon_DefaultDeploy,
	RG_CBasePlayerWeapon_DefaultReload,
	RG_CBasePlayerWeapon_DefaultShotgunReload,

	// [...]
};

enum GamedllFunc_CSGameRules
{
	// CSGameRules virtual
	RG_CSGameRules_FShouldSwitchWeapon = BEGIN_FUNC_REGION(gamerules),
	RG_CSGameRules_GetNextBestWeapon,
	RG_CSGameRules_FlPlayerFallDamage,
	RG_CSGameRules_FPlayerCanTakeDamage,
	RG_CSGameRules_PlayerSpawn,
	RG_CSGameRules_FPlayerCanRespawn,
	RG_CSGameRules_GetPlayerSpawnSpot,
	RG_CSGameRules_ClientUserInfoChanged,
	RG_CSGameRules_PlayerKilled,
	RG_CSGameRules_DeathNotice,
	RG_CSGameRules_CanHavePlayerItem,
	RG_CSGameRules_DeadPlayerWeapons,
	RG_CSGameRules_ServerDeactivate,
	RG_CSGameRules_CheckMapConditions,
	RG_CSGameRules_CleanUpMap,
	RG_CSGameRules_RestartRound,
	RG_CSGameRules_CheckWinConditions,
	RG_CSGameRules_RemoveGuns,
	RG_CSGameRules_GiveC4,
	RG_CSGameRules_ChangeLevel,
	RG_CSGameRules_GoToIntermission,
	RG_CSGameRules_BalanceTeams,
	RG_CSGameRules_OnRoundFreezeEnd,
	RG_CSGameRules_CanPlayerHearPlayer,

	// [...]
};

enum GamedllFunc_CGib
{
	RG_CGib_Spawn = BEGIN_FUNC_REGION(gib),
	RG_CGib_BounceGibTouch,
	RG_CGib_WaitTillLand,

	// [...]
};

enum ReCheckerFunc
{
	RC_FileConsistencyProcess = BEGIN_FUNC_REGION(rechecker),
	RC_FileConsistencyFinal,
	RC_CmdExec,

	// [...]
};
