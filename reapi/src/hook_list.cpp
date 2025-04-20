#include "precompiled.h"

inline size_t getFwdParamType(void(*)(int))                     { return FP_CELL;   }
inline size_t getFwdParamType(void(*)(size_t))                  { return FP_CELL;   }
inline size_t getFwdParamType(void(*)(short))                   { return FP_CELL;   }
inline size_t getFwdParamType(void(*)(unsigned short))          { return FP_CELL;   }
inline size_t getFwdParamType(void(*)(ULONG))                   { return FP_CELL;   }
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
inline size_t getFwdParamType(void(*)(unsigned char))           { return FP_CELL;   }
inline size_t getFwdParamType(void(*)(resourcetype_t))          { return FP_CELL;   }
inline size_t getFwdParamType(void(*)(cmd_source_t))            { return FP_CELL;   }
inline size_t getFwdParamType(void(*)(GameEventType))           { return FP_CELL;   }
inline size_t getFwdParamType(void(*)(float*))                  { return FP_ARRAY;  }

template <typename T>
inline size_t getFwdParamType(void(*)(T *))                     { return FP_CELL;   }

struct regfunc
{
	template <typename ...f_args>
	struct regargs
	{
		regargs()
		{
			void (*func)(f_args...) = nullptr;
			setArgs(args, func);
		}

		template <size_t ...Is>
		int Register(AMX *amx, const char *name, std::index_sequence<Is...>) const
		{
			return g_amxxapi.RegisterSPForwardByName(amx, name, args[Is]...);
		}

	protected:
		template <size_t current = 0>
		void setArgs(size_t param_types[], void (*)())
		{
			param_types[current] = FP_DONE;
		}

		template <size_t current = 0, typename T, typename ...t_args>
		void setArgs(size_t param_types[], void (*)(T, t_args...))
		{
			void (*func)(T) = nullptr;
			param_types[current] = getFwdParamType(func);
			void (*next)(t_args...) = nullptr;
			setArgs<current + 1>(param_types, next);
		}

	private:
		size_t args[sizeof...(f_args) + 1]; // + FP_DONE
	};

	template <typename R, typename T, typename ...f_args>
	regfunc(R (*)(T, f_args...))
	{
		func = [](AMX *amx, const char *name)
		{
			regargs<f_args...> args;
			return args.Register(amx, name, std::make_index_sequence<sizeof...(f_args) + 1>{});
		};
	}

	regfunc(const char *name) { UTIL_SysError("%s doesn't match hook definition", name); }	// to cause a amxx module failure.
	operator regfunc_t() const { return func; }
	regfunc_t func;

	static int current_cell;				// the counter of cells
};

int regfunc::current_cell = 1;

#define ENG(h,...) { {}, {}, #h, "ReHLDS", [](){ return api_cfg.hasReHLDS(); }, ((!(RH_##h & (MAX_REGION_RANGE - 1)) ? regfunc::current_cell = 1, true : false) || (RH_##h & (MAX_REGION_RANGE - 1)) == regfunc::current_cell++) ? regfunc(h##__VA_ARGS__) : regfunc(#h#__VA_ARGS__), [](){ g_RehldsHookchains->h()->registerHook(&h); }, [](){ g_RehldsHookchains->h()->unregisterHook(&h); }, false}
hook_t hooklist_engine[] = {
	ENG(SV_StartSound),
	ENG(SV_DropClient),
	ENG(SV_ActivateServer),
	ENG(Cvar_DirectSet),
	ENG(SV_WriteFullClientUpdate, _AMXX),
	ENG(GetEntityInit),
	ENG(ClientConnected),
	ENG(SV_ConnectClient),
	ENG(SV_EmitPings, _AMXX),
	ENG(ED_Alloc),
	ENG(ED_Free),
	ENG(Con_Printf),
	ENG(SV_CheckUserInfo, _AMXX),
	ENG(PF_precache_generic_I),
	ENG(PF_precache_model_I),
	ENG(PF_precache_sound_I),
	ENG(EV_Precache, _AMXX),
	ENG(SV_AddResource),
	ENG(SV_ClientPrintf),
	ENG(SV_AllowPhysent),
	ENG(ExecuteServerStringCmd),
	ENG(SV_SendResources, _AMXX),
};

#define DLL(h,...) { {}, {}, #h, "ReGameDLL", [](){ return api_cfg.hasReGameDLL(); }, ((!(RG_##h & (MAX_REGION_RANGE - 1)) ? regfunc::current_cell = 1, true : false) || (RG_##h & (MAX_REGION_RANGE - 1)) == regfunc::current_cell++) ? regfunc(h##__VA_ARGS__) : regfunc(#h#__VA_ARGS__), [](){ g_ReGameHookchains->h()->registerHook(&h); }, [](){ g_ReGameHookchains->h()->unregisterHook(&h); }, false}
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
	DLL(CreateWeaponBox),
	DLL(PM_LadderMove, _AMXX),
	DLL(PM_WaterJump, _AMXX),
	DLL(PM_CheckWaterJump, _AMXX),
	DLL(PM_Jump, _AMXX),
	DLL(PM_Duck, _AMXX),
	DLL(PM_UnDuck, _AMXX),
	DLL(PM_PlayStepSound, _AMXX),
	DLL(PM_AirAccelerate, _AMXX),
	DLL(ClearMultiDamage),
	DLL(AddMultiDamage),
	DLL(ApplyMultiDamage),
	DLL(BuyItem),
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

	DLL(CBasePlayer_Observer_SetMode),
	DLL(CBasePlayer_Observer_FindNextPlayer),

	DLL(CBasePlayer_Pain),
	DLL(CBasePlayer_DeathSound),
	DLL(CBasePlayer_JoiningThink),

	DLL(CBasePlayer_CheckTimeBasedDamage),
	DLL(CBasePlayer_EntSelectSpawnPoint),

	DLL(CBasePlayer_PlayerDeathThink),
	DLL(CBasePlayer_Observer_Think),
	DLL(CBasePlayer_RemoveAllItems),
	DLL(CBasePlayer_UpdateStatusBar),
	DLL(CBasePlayer_TakeDamageImpulse),
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
	DLL(CSGameRules_Think),
	DLL(CSGameRules_TeamFull),
	DLL(CSGameRules_TeamStacked),
	DLL(CSGameRules_PlayerGotWeapon),
	DLL(CSGameRules_SendDeathMessage),
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
	DLL(CBasePlayerWeapon_ItemPostFrame),
	DLL(CBasePlayerWeapon_KickBack),
	DLL(CBasePlayerWeapon_SendWeaponAnim),
};

hook_t hooklist_gib[] = {
	DLL(CGib_Spawn),
	DLL(CGib_BounceGibTouch),
	DLL(CGib_WaitTillLand),
};

hook_t hooklist_cbaseentity[] = {
	DLL(CBaseEntity_FireBullets),
	DLL(CBaseEntity_FireBuckshots),
	DLL(CBaseEntity_FireBullets3),
};

hook_t hooklist_botmanager[] = {
	DLL(CBotManager_OnEvent),
};

#define RCHECK(h,...) { {}, {}, #h, "ReChecker", [](){ return api_cfg.hasRechecker(); }, ((!(RC_##h & (MAX_REGION_RANGE - 1)) ? regfunc::current_cell = 1, true : false) || (RC_##h & (MAX_REGION_RANGE - 1)) == regfunc::current_cell++) ? regfunc(h##__VA_ARGS__) : regfunc(#h#__VA_ARGS__), [](){ g_RecheckerHookchains->h()->registerHook(&h); }, [](){ g_RecheckerHookchains->h()->unregisterHook(&h); }, false}
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
		CASE(cbaseentity)
		CASE(botmanager)
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
	FOREACH_CLEAR(cbaseentity);
	FOREACH_CLEAR(botmanager);
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
