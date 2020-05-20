#include "precompiled.h"

inline size_t getFwdParamType(void(*)(int))                     { return FP_CELL;   }
inline size_t getFwdParamType(void(*)(size_t))                  { return FP_CELL;   }
inline size_t getFwdParamType(void(*)(short))                   { return FP_CELL;   }
inline size_t getFwdParamType(void(*)(unsigned short))          { return FP_CELL;   }
inline size_t getFwdParamType(void(*)(bool))                    { return FP_CELL;   }
inline size_t getFwdParamType(void(*)(Vector&))                 { return FP_ARRAY;  }
inline size_t getFwdParamType(void(*)(TeamName))                { return FP_CELL;   }
inline size_t getFwdParamType(void(*)(PLAYER_ANIM))             { return FP_CELL;   }
inline size_t getFwdParamType(void(*)(WeaponIdType))            { return FP_CELL;   }
inline size_t getFwdParamType(void(*)(RewardType))              { return FP_CELL;   }
inline size_t getFwdParamType(void(*)(ScenarioEventEndRound))   { return FP_CELL;   }
inline size_t getFwdParamType(void(*)(ItemID))                  { return FP_CELL;   }
inline size_t getFwdParamType(void(*)(ItemRestType))            { return FP_CELL;   }
inline size_t getFwdParamType(void(*)(ResourceType_e))          { return FP_CELL;   }
inline size_t getFwdParamType(void(*)(float))                   { return FP_FLOAT;  }
inline size_t getFwdParamType(void(*)(float&))                  { return FP_FLOAT;  }
inline size_t getFwdParamType(void(*)(const char *))            { return FP_STRING; }
inline size_t getFwdParamType(void(*)(char *))                  { return FP_STRING; }
inline size_t getFwdParamType(void(*)(IResourceBuffer*))        { return FP_CELL;   }

template<typename T>
inline size_t getFwdParamType(void(*)(T *))                     { return FP_CELL;   }

template<size_t current = 0>
void setupParamTypes(size_t param_types[], void (*)())
{
	param_types[current] = FP_DONE;
}

template<size_t current = 0, typename T, typename ...t_args>
void setupParamTypes(size_t param_types[], void(*)(T, t_args...))
{
	void (*func)(T) = nullptr;
	param_types[current] = getFwdParamType(func);
	void (*next)(t_args...) = nullptr;
	setupParamTypes<current + 1>(param_types, next);
}

template<typename... f_args>
struct regargs
{
	regargs(void (*)(f_args...))
	{
		void (*func)(f_args...) = nullptr;
		setupParamTypes(types, func);
	}

	size_t types[sizeof...(f_args) + 1]; // + FP_DONE
};

struct regfunc
{
	template<typename R, typename T, typename... f_args>
	regfunc(R (*)(T, f_args...))
	{
		func = [](AMX *amx, const char *name) {
			void (*func)(f_args...) = nullptr;
			regargs<f_args...> args(func);
			return g_amxxapi.RegisterSPForwardByName(amx, name, args);
		};
	}

	regfunc(const char *name) { UTIL_SysError("%s doesn't match hook definition", name); }	// to cause a amxx module failure.
	operator regfunc_t() const { return func; }
	regfunc_t func;

	static int current_cell;				// the counter of cells
};

int regfunc::current_cell = 1;

#define ENG(h,...) { {}, {}, #h, "ReHLDS", [](){ return api_cfg.hasReHLDS(); }, ((!(RH_##h & (MAX_REGION_RANGE - 1)) ? regfunc::current_cell = 1, true : false) || (RH_##h & (MAX_REGION_RANGE - 1)) == regfunc::current_cell++) ? regfunc(h##__VA_ARGS__) : regfunc(#h#__VA_ARGS__), [](){ g_RehldsHookchains->h()->registerHook(&h); }, [](){ g_RehldsHookchains->h()->unregisterHook(&h); }}
hook_t hooklist_engine[] = {
	ENG(SV_StartSound),
	ENG(SV_DropClient),
	ENG(SV_ActivateServer),
	ENG(Cvar_DirectSet),
	ENG(SV_WriteFullClientUpdate, _AMXX)
};

#define DLL(h,...) { {}, {}, #h, "ReGameDLL", [](){ return api_cfg.hasReGameDLL(); }, ((!(RG_##h & (MAX_REGION_RANGE - 1)) ? regfunc::current_cell = 1, true : false) || (RG_##h & (MAX_REGION_RANGE - 1)) == regfunc::current_cell++) ? regfunc(h##__VA_ARGS__) : regfunc(#h#__VA_ARGS__), [](){ g_ReGameHookchains->h()->registerHook(&h); }, [](){ g_ReGameHookchains->h()->unregisterHook(&h); }}
hook_t hooklist_gamedll[] = {
	DLL(GetForceCamera),
	DLL(PlayerBlind),
	DLL(RadiusFlash_TraceLine),
	DLL(RoundEnd),
	DLL(PM_Move, _AMXX),
	DLL(PM_AirMove),
	DLL(HandleMenu_ChooseAppearance),
	DLL(HandleMenu_ChooseTeam),
	DLL(ShowMenu),
	DLL(ShowVGUIMenu),
	DLL(BuyGunAmmo),
	DLL(BuyWeaponByWeaponID),
	DLL(ThrowHeGrenade),
	DLL(ThrowFlashbang),
	DLL(ThrowSmokeGrenade),
	DLL(PlantBomb),
	DLL(IsPenetrableEntity),
	DLL(SpawnHeadGib),
	DLL(SpawnRandomGibs),
};

hook_t hooklist_animating[] = {
	DLL(CBaseAnimating_ResetSequenceInfo)
};

hook_t hooklist_player[] = {
	DLL(CBasePlayer_Spawn),
	DLL(CBasePlayer_Precache),
	DLL(CBasePlayer_ObjectCaps),
	DLL(CBasePlayer_Classify),
	DLL(CBasePlayer_TraceAttack),
	DLL(CBasePlayer_TakeDamage),
	DLL(CBasePlayer_TakeHealth),
	DLL(CBasePlayer_Killed),
	DLL(CBasePlayer_AddPoints),
	DLL(CBasePlayer_AddPointsToTeam),
	DLL(CBasePlayer_AddPlayerItem),
	DLL(CBasePlayer_RemovePlayerItem),
	DLL(CBasePlayer_GiveAmmo),
	DLL(CBasePlayer_ResetMaxSpeed),
	DLL(CBasePlayer_Jump),
	DLL(CBasePlayer_Duck),
	DLL(CBasePlayer_PreThink),
	DLL(CBasePlayer_PostThink),
	DLL(CBasePlayer_UpdateClientData),
	DLL(CBasePlayer_ImpulseCommands),
	DLL(CBasePlayer_RoundRespawn),
	DLL(CBasePlayer_Blind),
	DLL(CBasePlayer_SetClientUserInfoModel),
	DLL(CBasePlayer_SetClientUserInfoName),

	DLL(CBasePlayer_Observer_IsValidTarget),
	DLL(CBasePlayer_SetAnimation),
	DLL(CBasePlayer_GiveDefaultItems),
	DLL(CBasePlayer_GiveNamedItem),
	DLL(CBasePlayer_AddAccount),
	DLL(CBasePlayer_GiveShield),
	DLL(CBasePlayer_DropPlayerItem),
	DLL(CBasePlayer_HasRestrictItem),
	DLL(CBasePlayer_DropShield),
	DLL(CBasePlayer_OnSpawnEquip),
	DLL(CBasePlayer_Radio),
	DLL(CBasePlayer_Disappear),
	DLL(CBasePlayer_MakeVIP),
	DLL(CBasePlayer_MakeBomber),
	DLL(CBasePlayer_StartObserver),
	DLL(CBasePlayer_GetIntoGame),
	DLL(CBasePlayer_StartDeathCam),
	DLL(CBasePlayer_SwitchTeam),
	DLL(CBasePlayer_CanSwitchTeam),
	DLL(CBasePlayer_ThrowGrenade),
	DLL(CBasePlayer_SetSpawnProtection),
	DLL(CBasePlayer_RemoveSpawnProtection),
	DLL(CBasePlayer_HintMessageEx),
	DLL(CBasePlayer_UseEmpty),
	DLL(CBasePlayer_DropIdlePlayer),
};

hook_t hooklist_gamerules[] = {
	DLL(CSGameRules_FShouldSwitchWeapon),
	DLL(CSGameRules_GetNextBestWeapon),
	DLL(CSGameRules_FlPlayerFallDamage),
	DLL(CSGameRules_FPlayerCanTakeDamage),
	DLL(CSGameRules_PlayerSpawn),
	DLL(CSGameRules_FPlayerCanRespawn),
	DLL(CSGameRules_GetPlayerSpawnSpot),
	DLL(CSGameRules_ClientUserInfoChanged),
	DLL(CSGameRules_PlayerKilled),
	DLL(CSGameRules_DeathNotice),
	DLL(CSGameRules_CanHavePlayerItem),
	DLL(CSGameRules_DeadPlayerWeapons),
	DLL(CSGameRules_ServerDeactivate),
	DLL(CSGameRules_CheckMapConditions),
	DLL(CSGameRules_CleanUpMap),
	DLL(CSGameRules_RestartRound),
	DLL(CSGameRules_CheckWinConditions),
	DLL(CSGameRules_RemoveGuns),
	DLL(CSGameRules_GiveC4),
	DLL(CSGameRules_ChangeLevel),
	DLL(CSGameRules_GoToIntermission),
	DLL(CSGameRules_BalanceTeams),
	DLL(CSGameRules_OnRoundFreezeEnd),
	DLL(CSGameRules_CanPlayerHearPlayer),
};

hook_t hooklist_grenade[] = {
	DLL(CGrenade_DefuseBombStart),
	DLL(CGrenade_DefuseBombEnd),
	DLL(CGrenade_ExplodeHeGrenade),
	DLL(CGrenade_ExplodeFlashbang),
	DLL(CGrenade_ExplodeSmokeGrenade),
	DLL(CGrenade_ExplodeBomb),
};

hook_t hooklist_weaponbox[] = {
	DLL(CWeaponBox_SetModel),
};

hook_t hooklist_weapon[] = {
	DLL(CBasePlayerWeapon_CanDeploy),
	DLL(CBasePlayerWeapon_DefaultDeploy),
	DLL(CBasePlayerWeapon_DefaultReload),
	DLL(CBasePlayerWeapon_DefaultShotgunReload),
};

hook_t hooklist_gib[] = {
	DLL(CGib_Spawn),
	DLL(CGib_BounceGibTouch),
	DLL(CGib_WaitTillLand),
};

#define RCHECK(h,...) { {}, {}, #h, "ReChecker", [](){ return api_cfg.hasRechecker(); }, ((!(RC_##h & (MAX_REGION_RANGE - 1)) ? regfunc::current_cell = 1, true : false) || (RC_##h & (MAX_REGION_RANGE - 1)) == regfunc::current_cell++) ? regfunc(h##__VA_ARGS__) : regfunc(#h#__VA_ARGS__), [](){ g_RecheckerHookchains->h()->registerHook(&h); }, [](){ g_RecheckerHookchains->h()->unregisterHook(&h); }}
hook_t hooklist_rechecker[] = {
	RCHECK(FileConsistencyProcess, _AMXX),
	RCHECK(FileConsistencyFinal),
	RCHECK(CmdExec, _AMXX)
};

hook_t* hooklist_t::getHookSafe(size_t hook)
{
	#define CASE(h)	case ht_##h: if (likely(index < arraysize(hooklist_##h))) return &hooklist_##h[index]; else break;

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

	return nullptr;
}

void hooklist_t::clear()
{
	#define FOREACH_CLEAR(h) for (auto& h : hooklist_##h) h.clear();

	FOREACH_CLEAR(engine);
	FOREACH_CLEAR(gamedll);
	FOREACH_CLEAR(animating);
	FOREACH_CLEAR(player);
	FOREACH_CLEAR(gamerules);
	FOREACH_CLEAR(rechecker);
	FOREACH_CLEAR(grenade);
	FOREACH_CLEAR(weaponbox);
	FOREACH_CLEAR(weapon);
	FOREACH_CLEAR(gib);
}

void hook_t::clear()
{
	if (pre.size() || post.size()) {
		for (auto h : pre)
			delete h;
		pre.clear();

		for (auto h : post)
			delete h;
		post.clear();

		unregisterHookchain();
	}
}
