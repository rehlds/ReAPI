#include "precompiled.h"

/*
* Assign the number of the player's animation.
*
* @param index          Client index
* @param playerAnim     Specific animation number
*
* @noreturn
*
* native rg_set_animation(const index, PLAYER_ANIM:playerAnim);
*/
cell AMX_NATIVE_CALL rg_set_animation(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index, arg_anim };

	CHECK_ISPLAYER(arg_index);

	CBasePlayer *pPlayer = UTIL_PlayerByIndex(params[arg_index]);
	CHECK_CONNECTED(pPlayer, arg_index);

	pPlayer->CSPlayer()->SetAnimation(CAmxArg(amx, params[arg_anim]));
	return TRUE;
}

enum AccountSet { AS_SET, AS_ADD };

/*
* Adds money to player's account.
*
* @param index          Client index
* @param amount         The amount of money
* @param bTrackChange   If bTrackChange is 1, the amount of money added will also be displayed.
*
* @noreturn
*
* native rg_add_account(const index, amount, AccountSet:typeSet = AS_ADD, const bool:bTrackChange = true);
*/
cell AMX_NATIVE_CALL rg_add_account(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index, arg_amount, arg_typeSet, arg_track_change };

	CHECK_ISPLAYER(arg_index);

	CBasePlayer *pPlayer = UTIL_PlayerByIndex(params[arg_index]);
	CHECK_CONNECTED(pPlayer, arg_index);

	if (static_cast<AccountSet>(params[arg_typeSet]) == AS_SET) {
		pPlayer->m_iAccount = 0;
	}

	pPlayer->CSPlayer()->AddAccount(params[arg_amount], RT_NONE, params[arg_track_change] != 0);
	return TRUE;
}

/*
* Gives the player an item.
*
* @param index      Client index
* @param pszName    Item classname
* @param type       Look at the enums with name GiveType
*
* @return           Index of entity if successfull, -1 otherwise
*
* native rg_give_item(const index, const pszName[], GiveType:type = GT_APPEND);
*/
cell AMX_NATIVE_CALL rg_give_item(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index, arg_item, arg_type };

	CHECK_ISPLAYER(arg_index);

	CBasePlayer *pPlayer = UTIL_PlayerByIndex(params[arg_index]);
	CHECK_CONNECTED(pPlayer, arg_index);

	char namebuf[256];
	const char *itemName = getAmxString(amx, params[arg_item], namebuf);

	GiveType type = static_cast<GiveType>(params[arg_type]);
	if (type > GT_APPEND)
	{
		auto pInfo = g_ReGameApi->GetWeaponSlot(itemName);
		if (pInfo)
		{
			pPlayer->ForEachItem(pInfo->slot, [pPlayer, type](CBasePlayerItem *pItem) {
				RemoveOrDropItem(pPlayer, pItem, type);
				return false;
			});
		}
	}

	auto pEntity = pPlayer->CSPlayer()->GiveNamedItemEx(itemName);
	if (pEntity)
		return indexOfPDataAmx(pEntity);

	return AMX_NULLENT;
}

/*
* Gives the player an custom item, this means that don't handled API things.
*
* @example rg_give_custom_item(id, "weapon_c4"); doesn't sets the member m_bHasC4 to true, as the rg_give_item does.
*
* @param index      Client index
* @param pszName    Item classname
* @param type       Look at the enums with name GiveType
* @param uid        Sets a unique index for the entity
*
* @return           Index of entity if successfull, -1 otherwise
*
* native rg_give_custom_item(const index, const pszName[], GiveType:type = GT_APPEND, const uid = 0);
*/
cell AMX_NATIVE_CALL rg_give_custom_item(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index, arg_item, arg_type, arg_uid };

	CHECK_ISPLAYER(arg_index);

	CBasePlayer *pPlayer = UTIL_PlayerByIndex(params[arg_index]);
	CHECK_CONNECTED(pPlayer, arg_index);

	char namebuf[256];
	const char *itemName = getAmxString(amx, params[arg_item], namebuf);

	GiveType type = static_cast<GiveType>(params[arg_type]);
	if (type > GT_APPEND)
	{
		auto pInfo = g_ReGameApi->GetWeaponSlot(itemName);
		if (pInfo)
		{
			pPlayer->ForEachItem(pInfo->slot, [pPlayer, type](CBasePlayerItem *pItem) {
				RemoveOrDropItem(pPlayer, pItem, type);
				return false;
			});
		}
	}

	auto pEntity = GiveNamedItemInternal(amx, pPlayer, itemName, params[arg_uid]);
	if (pEntity)
		return indexOfPDataAmx(pEntity);

	return AMX_NULLENT;
}

/*
* Give the default items to a player.
*
* @param index      Client index
*
* @noreturn
*
* native rg_give_default_items(const index);
*/
cell AMX_NATIVE_CALL rg_give_default_items(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index };

	CHECK_ISPLAYER(arg_index);

	CBasePlayer *pPlayer = UTIL_PlayerByIndex(params[arg_index]);
	CHECK_CONNECTED(pPlayer, arg_index);

	pPlayer->CSPlayer()->GiveDefaultItems();
	return TRUE;
}

/*
* Gives the player a shield
*
* @param index          Client index
* @param bDeploy        To deploy the shield
*
* @noreturn
*
* native rg_give_shield(const index, const bool:bDeploy = true);
*/
cell AMX_NATIVE_CALL rg_give_shield(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index, arg_deploy };

	CHECK_ISPLAYER(arg_index);

	CBasePlayer *pPlayer = UTIL_PlayerByIndex(params[arg_index]);
	CHECK_CONNECTED(pPlayer, arg_index);

	pPlayer->CSPlayer()->GiveShield(params[arg_deploy] != 0);
	return TRUE;
}

/*
* Inflicts damage in a radius from the source position.
*
* @param vecSrc             The source position
* @param inflictor          Inflictor is the entity that caused the damage (such as a gun)
* @param attacker           Attacker is the entity that triggered the damage (such as the gun's owner)
* @param flDamage           The amount of damage
* @param flRadius           Damage radius
* @param iClassIgnore       To specify classes that are immune to damage
* @param bitsDamageType     Damage type DMG_*
*
* @noreturn
*
* native rg_dmg_radius(Float:vecSrc[3], const inflictor, const attacker, const Float:flDamage, const Float:flRadius, const iClassIgnore, const bitsDamageType);
*/
cell AMX_NATIVE_CALL rg_dmg_radius(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_vec, arg_inflictor, arg_attacker, arg_damage, arg_radius, arg_ignore_class, arg_dmg_type };

	CHECK_ISENTITY(arg_inflictor);
	CHECK_ISENTITY(arg_attacker);

	CAmxArgs args(amx, params);
	g_ReGameFuncs->RadiusDamage(args[arg_vec], args[arg_inflictor], args[arg_attacker], args[arg_damage], args[arg_radius], args[arg_ignore_class], args[arg_dmg_type]);

	return TRUE;
}

/*
* Resets the global multi damage accumulator.
*
* @noreturn
*
* native rg_multidmg_clear();
*/
cell AMX_NATIVE_CALL rg_multidmg_clear(AMX *amx, cell *params)
{
	g_ReGameFuncs->ClearMultiDamage();
	return TRUE;
}

/*
* Inflicts contents of global multi damage registered on victim.
*
* @param inflictor      Inflictor is the entity that caused the damage (such as a gun)
* @param attacker       Attacker is the entity that triggered the damage (such as the gun's owner)
*
* @noreturn
*
* native rg_multidmg_apply(const inflictor, const attacker);
*/
cell AMX_NATIVE_CALL rg_multidmg_apply(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_inflictor, arg_attacker };

	CHECK_ISENTITY(arg_inflictor);
	CHECK_ISENTITY(arg_attacker);

	CAmxArgs args(amx, params);
	g_ReGameFuncs->ApplyMultiDamage(args[arg_inflictor], args[arg_attacker]);

	return TRUE;
}

/*
* Adds damage to the accumulator.
*
* @param inflictor          Inflictor is the entity that caused the damage (such as a gun)
* @param victim             The victim that takes damage
* @param flDamage           The amount of damage
* @param bitsDamageType     Damage type DMG_*
*
* @noreturn
*
* native rg_multidmg_add(const inflictor, const victim, const Float:flDamage, const bitsDamageType);
*/
cell AMX_NATIVE_CALL rg_multidmg_add(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_inflictor, arg_victim, arg_damage, arg_dmg_type };

	CHECK_ISENTITY(arg_inflictor);
	CHECK_ISENTITY(arg_victim);

	if (params[arg_victim] < 0) { // null
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: victim == null", __FUNCTION__);
		return FALSE;
	}

	CAmxArgs args(amx, params);
	g_ReGameFuncs->AddMultiDamage(args[arg_inflictor], args[arg_victim], args[arg_damage], args[arg_dmg_type]);

	return TRUE;
}

/*
* Fires bullets from entity.
*
* @param inflictor          Inflictor is the entity that caused the damage (such as a gun)
* @param attacker           Attacker is the entity that triggered the damage (such as the gun's owner)
* @param shots              The number of shots
* @param vecSrc             The source position of the barrel
* @param vecDirShooting     Shooting direction
* @param vecSpread          Spread
* @param flDistance         Max shot distance
* @param iBulletType        Bullet type, look at the enum with name Bullet in cssdk_const.inc
* @param iTracerFreq        Tracer frequency
* @param iDamage            Damage amount
*
* @noreturn
*
* native rg_fire_bullets(const inflictor, const attacker, const shots, Float:vecSrc[3], Float:vecDirShooting[3], Float:vecSpread[3], const Float:flDistance, const Bullet:iBulletType, const iTracerFreq, const iDamage);
*/
cell AMX_NATIVE_CALL rg_fire_bullets(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_inflictor, arg_attacker, arg_shots, arg_vecSrc, arg_dir, arg_spread, arg_dist, arg_bullet_type, arg_tracefrq, arg_dmg };

	CHECK_ISENTITY(arg_inflictor);
	CHECK_ISENTITY(arg_attacker);

	CAmxArgs args(amx, params);
	CBaseEntity *pInflictor = args[arg_inflictor];

	pInflictor->m_pEntity->FireBullets
	(
		args[arg_shots],
		args[arg_vecSrc],
		args[arg_dir],
		args[arg_spread],
		args[arg_dist],
		args[arg_bullet_type],
		args[arg_tracefrq],
		args[arg_dmg],
		args[arg_attacker]
	);

	return TRUE;
}

/*
* Fires buckshots from entity.
*
* @param inflictor          Inflictor is the entity that caused the damage (such as a gun)
* @param attacker           Attacker is the entity that triggered the damage (such as the gun's owner)
* @param shots              The number of shots
* @param vecSrc             The source position of the barrel
* @param vecDirShooting     Shooting direction
* @param vecSpread          Spread
* @param flDistance         Max shot distance
* @param iTracerFreq        Tracer frequency
* @param iDamage            Damage amount
*
* @noreturn
*
* native rg_fire_buckshots(const inflictor, const attacker, const shots, Float:vecSrc[3], Float:vecDirShooting[3], Float:vecSpread[3], const Float:flDistance, const iTracerFreq, const iDamage);
*/
cell AMX_NATIVE_CALL rg_fire_buckshots(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_inflictor, arg_attacker, arg_shots, arg_vecSrc, arg_dir, arg_spread, arg_dist, arg_tracefrq, arg_dmg };

	CHECK_ISENTITY(arg_inflictor);
	CHECK_ISENTITY(arg_attacker);

	CAmxArgs args(amx, params);
	CBaseEntity *pInflictor = args[arg_inflictor];

	pInflictor->m_pEntity->FireBuckshots
	(
		args[arg_shots],
		args[arg_vecSrc],
		args[arg_dir],
		args[arg_spread],
		args[arg_dist],
		args[arg_tracefrq],
		args[arg_dmg],
		args[arg_attacker]
	);

	return TRUE;
}

/*
* Fires bullets from player's weapon.
*
* @param inflictor          Inflictor is the entity that caused the damage (such as a gun)
* @param attacker           Attacker is the entity that triggered the damage (such as the gun's owner)
* @param vecSrc             The source position of the barrel
* @param vecDirShooting     Shooting direction
* @param vecSpread          Spread
* @param flDistance         Max shot distance
* @param iPenetration       The amount of penetration
* @param iBulletType        Bullet type, look at the enum with name Bullet in cssdk_const.inc
* @param iDamage            Damage amount
* @param flRangeModifier    Damage range modifier
* @param bPistol            Pistol shot
* @param shared_rand        Use player's random seed, get circular gaussian spread
*
* @return Float:[3]         The spread result
*
* native Float:[3] rg_fire_bullets3(const inflictor, const attacker, Float:vecSrc[3], Float:vecDirShooting[3], const Float:vecSpread, const Float:flDistance, const iPenetration, const Bullet:iBulletType, const iDamage, const Float:flRangeModifier, const bool:bPistol, const shared_rand);
*/
cell AMX_NATIVE_CALL rg_fire_bullets3(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_inflictor, arg_attacker, arg_vecSrc, arg_dir, arg_spread, arg_dist, arg_penetration, arg_bullet_type, arg_dmg, arg_range_mod, arg_pistol, arg_rand, arg_out };

	CHECK_ISENTITY(arg_inflictor);
	CHECK_ISENTITY(arg_attacker);

	CAmxArgs args(amx, params);
	CBaseEntity *pInflictor = args[arg_inflictor];
	entvars_t *pAttacker = args[arg_attacker];

	args[arg_out].vector() = pInflictor->m_pEntity->FireBullets3
	(
		args[arg_vecSrc],
		args[arg_dir],
		args[arg_spread],
		args[arg_dist],
		args[arg_penetration],
		args[arg_bullet_type],
		args[arg_dmg],
		args[arg_range_mod],
		pAttacker, // icc fix
		args[arg_pistol],
		args[arg_rand]
	);

	return TRUE;
}

struct {
	const char *msg;
	const char *sentence;
	size_t status;
} msg_sentence_list[] = {
	{ "", "" },																		// ROUND_NONE
	{ "#Target_Bombed",                     "terwin",     WINSTATUS_TERRORISTS  },	// ROUND_TARGET_BOMB
	{ "#VIP_Escaped",                       "ctwin",      WINSTATUS_CTS         },	// ROUND_VIP_ESCAPED
	{ "#VIP_Assassinated",                  "terwin",     WINSTATUS_TERRORISTS  },	// ROUND_VIP_ASSASSINATED
	{ "#Terrorists_Escaped",                "terwin",     WINSTATUS_TERRORISTS  },	// ROUND_TERRORISTS_ESCAPED
	{ "#CTs_PreventEscape",                 "ctwin",      WINSTATUS_CTS         },	// ROUND_CTS_PREVENT_ESCAPE
	{ "#Escaping_Terrorists_Neutralized",   "ctwin",      WINSTATUS_CTS         },	// ROUND_ESCAPING_TERRORISTS_NEUTRALIZED
	{ "#Bomb_Defused",                      "ctwin",      WINSTATUS_CTS         },	// ROUND_BOMB_DEFUSED
	{ "#CTs_Win",                           "ctwin",      WINSTATUS_CTS         },	// ROUND_CTS_WIN
	{ "#Terrorists_Win",                    "terwin",     WINSTATUS_TERRORISTS  },	// ROUND_TERRORISTS_WIN
	{ "#Round_Draw",                        "rounddraw",  WINSTATUS_DRAW        },	// ROUND_END_DRAW
	{ "#All_Hostages_Rescued",              "ctwin",      WINSTATUS_CTS         },	// ROUND_ALL_HOSTAGES_RESCUED
	{ "#Target_Saved",                      "ctwin",      WINSTATUS_CTS         },	// ROUND_TARGET_SAVED
	{ "#Hostages_Not_Rescued",              "terwin",     WINSTATUS_TERRORISTS  },	// ROUND_HOSTAGE_NOT_RESCUED
	{ "#Terrorists_Not_Escaped",            "ctwin",      WINSTATUS_CTS         },	// ROUND_TERRORISTS_NOT_ESCAPED
	{ "#VIP_Not_Escaped",                   "terwin",     WINSTATUS_TERRORISTS  },	// ROUND_VIP_NOT_ESCAPED
	{ "#Game_Commencing",                   "",           WINSTATUS_DRAW        },	// ROUND_GAME_COMMENCE
	{ "",                                   "",           WINSTATUS_DRAW        },	// ROUND_GAME_RESTART
	{ "#Cstrike_Tutor_Round_Over",          "rounddraw",  WINSTATUS_DRAW        },	// ROUND_GAME_OVER
};

/*
* Forces the round to end.
*
* @param tmDelay    Delay before the onset of a new round
* @param st         Which team won
* @param event      The event is the end of the round
* @param message    The message on round end
* @param sentence   The sound at the end of the round
* @param trigger    This will trigger to all hooks on that function
*                   Be very careful about recursion!
*
* @noreturn
* native rg_round_end(const Float:tmDelay, const WinStatus:st, const ScenarioEventEndRound:event = ROUND_NONE, const message[] = "default", const sentence[] = "default", const bool:trigger = false);
*/
cell AMX_NATIVE_CALL rg_round_end(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_delay, arg_win, arg_event, arg_message, arg_sentence, arg_trigger };

	CHECK_GAMERULES();

	auto winstatus = params[arg_win];

	if (winstatus < WINSTATUS_NONE || winstatus > WINSTATUS_DRAW) {
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: invalid winstatus %i, bounds(%i, %i)", __FUNCTION__, winstatus, WINSTATUS_NONE, WINSTATUS_DRAW);
		return FALSE;
	}

	char sentencebuf[190], messagebuf[190];
	const char *sentence = getAmxString(amx, params[arg_sentence], sentencebuf);
	const char *message  = getAmxString(amx, params[arg_message], messagebuf);

	ScenarioEventEndRound event = static_cast<ScenarioEventEndRound>(params[arg_event]);
	if (event != ROUND_NONE)
	{
		if (event < ROUND_NONE || event > ROUND_GAME_OVER) {
			AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: invalid event %i, bounds(%i, %i)", __FUNCTION__, event, ROUND_NONE, ROUND_GAME_OVER);
			return FALSE;
		}

		auto& lst = msg_sentence_list[event];
		if (strcmp(sentence, "default") == 0)
			sentence = lst.sentence;
		if (strcmp(message, "default") == 0)
			message = lst.msg;
	}

	if (sentence[0] != '\0')
	{
		Broadcast(sentence);
	}

	float tmDelay = CAmxArg(amx, params[arg_delay]);
	if (params[arg_trigger] != 0)
	{
		return callForward<bool>(RG_RoundEnd,
				[&message](int _winStatus, ScenarioEventEndRound _event, float _tmDelay)
				{
					CSGameRules()->EndRoundMessage(message, _event);
					CSGameRules()->TerminateRound(_tmDelay, _winStatus);
					return true;
				},
			winstatus, event, tmDelay);
	}
	else
	{
		CSGameRules()->EndRoundMessage(message, event);
		CSGameRules()->TerminateRound(tmDelay, winstatus);
	}

	return TRUE;
}

/*
* Updates current scores.
*
* @param iCtsWins   The amount of wins for counter-terrorists
* @param iTsWins    The amount of wins for terrorists
* @param bAdd       Adds the score to the current amount
*
* @noreturn
* native rg_update_teamscores(const iCtsWins = 0, const iTsWins = 0, const bool:bAdd = true);
*/
cell AMX_NATIVE_CALL rg_update_teamscores(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_cts, arg_ts, arg_add };

	CHECK_GAMERULES();

	CSGameRules()->m_iNumCTWins = ((params[arg_add] != 0) ? CSGameRules()->m_iNumCTWins : 0) + params[arg_cts];
	CSGameRules()->m_iNumTerroristWins = ((params[arg_add] != 0) ? CSGameRules()->m_iNumTerroristWins : 0) + params[arg_ts];
	CSGameRules()->UpdateTeamScores();

	return TRUE;
}

/*
* Creates an entity using Counter-Strike's custom CreateNamedEntity wrapper.
*
* @param classname      Entity classname
* @param useHashTable   Use this only for known game entities
* @note: Do not use this if you plan to change custom classname an entity after creation,
*        otherwise it will never be release from hash table even if an entity was destroyed,
*        and that to lead table to inflate/memory leaks
*
* @return               Index of the created entity or 0 otherwise
*
* native rg_create_entity(const classname[], const bool:useHashTable = false);
*/
cell AMX_NATIVE_CALL rg_create_entity(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_classname, arg_hashtable };

	char classname[256];
	string_t iClass = getAmxStringAlloc(amx, params[arg_classname], classname);

	edict_t	*pEntity;
	if (params[arg_hashtable] != 0)
		pEntity = g_ReGameFuncs->CREATE_NAMED_ENTITY2(iClass);
	else
		pEntity = CREATE_NAMED_ENTITY(iClass);

	if (pEntity) {
		return indexOfEdict(pEntity);
	}

	return 0;
}

/*
* Finds an entity in the world using Counter-Strike's custom FindEntityByString wrapper.
*
* @param start_index        Entity index to start searching from. -1 to start from the first entity
* @param classname          Classname to search for
* @param useHashTable       Use this only for known game entities
*
* @note: Do not use this if you use a custom classname
*
* @return                   Entity index > 0 if found, 0 otherwise
*
* native rg_find_ent_by_class(start_index, const classname[], const bool:useHashTable = false);
*/
cell AMX_NATIVE_CALL rg_find_ent_by_class(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_start_index, arg_classname, arg_hashtable };

	char classname[256];
	const char* value = getAmxString(amx, params[arg_classname], classname);

	if (params[arg_hashtable] != 0)
	{
		auto pStartEntity = getPrivate<CBaseEntity>(params[arg_start_index]);
		auto pEntity = g_ReGameFuncs->UTIL_FindEntityByString(pStartEntity, "classname", value);
		if (pEntity) {
			return indexOfEdict(pEntity->pev);
		}

		return 0;
	}

	auto pStartEntity = edictByIndexAmx(params[arg_start_index]);
	auto pEdict = FIND_ENTITY_BY_STRING(pStartEntity, "classname", value);
	if (pEdict) {
		return indexOfEdict(pEdict);
	}

	return 0;
}

/*
* Finds an entity in the world using Counter-Strike's custom FindEntityByString wrapper, matching by owner.
*
* @param start_index    Entity index to start searching from. AMX_NULLENT (-1) to start from the first entity
* @param classname      Classname to search for
*
* @return               true if found, false otherwise
*
* native bool:rg_find_ent_by_owner(&start_index, const classname[], owner);
*/
cell AMX_NATIVE_CALL rg_find_ent_by_owner(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_start_index, arg_classname, arg_onwer };

	CHECK_ISENTITY(arg_onwer);

	char classname[256];

	cell& startIndex = *getAmxAddr(amx, params[arg_start_index]);
	startIndex = clamp(startIndex, AMX_NULLENT, gpGlobals->maxEntities - 1);

	const char* value = getAmxString(amx, params[arg_classname], classname);
	edict_t* pOwner = edictByIndexAmx(params[arg_onwer]);

	for (int i = startIndex + 1; i < gpGlobals->maxEntities; i++)
	{
		edict_t *pEntity = edictByIndex(i);
		if (pEntity->v.owner != pOwner)
			continue;

		// yet not allocated
		if (!pEntity->pvPrivateData || pEntity->free)
			continue;

		if (FClassnameIs(pEntity, value))
		{
			startIndex = i;
			return TRUE;
		}
	}

	return FALSE;
}

/*
* Finds the weapon by name in the player's inventory.
*
* @param index      Client index
* @param weapon     Weapon name
*
* @return           Weapon's entity index, 0 otherwise
*
* native rg_find_weapon_bpack_by_name(const index, const weapon[]);
*/
cell AMX_NATIVE_CALL rg_find_weapon_bpack_by_name(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index, arg_weapon };

	CHECK_ISPLAYER(arg_index);

	CBasePlayer *pPlayer = UTIL_PlayerByIndex(params[arg_index]);
	CHECK_CONNECTED(pPlayer, arg_index);

	char wname[256];
	const char *pszWeaponName = getAmxString(amx, params[arg_weapon], wname);
	auto pInfo = g_ReGameApi->GetWeaponSlot(pszWeaponName);
	if (pInfo != nullptr)
	{
		auto pItem = pPlayer->m_rgpPlayerItems[ pInfo->slot ];
		while (pItem)
		{
			if (FClassnameIs(pItem->pev, pszWeaponName)) {
				return indexOfEdict(pItem->pev);
			}

			pItem = pItem->m_pNext;
		}
	}

	return FALSE;
}

struct {
	const char* pszItemName;
	bool(*hasItem)(CBasePlayer* pl);
} itemInfoStruct[] = {
	{ "item_thighpack",     [](CBasePlayer* pl) -> bool { return pl->m_bHasDefuser; } },
	{ "item_longjump",      [](CBasePlayer* pl) -> bool { return pl->m_fLongJump == TRUE; } },
	{ "item_assaultsuit",   [](CBasePlayer* pl) -> bool { return pl->m_iKevlar == ARMOR_VESTHELM; } },
	{ "item_kevlar",        [](CBasePlayer* pl) -> bool { return pl->m_iKevlar == ARMOR_KEVLAR; } },
	{ "weapon_shield",      [](CBasePlayer* pl) -> bool { return pl->m_bOwnsShield; } },
};

/*
* Checks if the player has the item.
*
* @param index      Client index
* @param item       Item name
*
* @return           true if he does, false otherwise
*
* native bool:rg_has_item_by_name(const index, const item[]);
*/
cell AMX_NATIVE_CALL rg_has_item_by_name(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index, arg_item };

	CHECK_ISPLAYER(arg_index);

	CBasePlayer *pPlayer = UTIL_PlayerByIndex(params[arg_index]);
	CHECK_CONNECTED(pPlayer, arg_index);

	char iname[256];
	const char *pszItemName = getAmxString(amx, params[arg_item], iname);
	// item_* and weapon_shield
	for (auto& inf : itemInfoStruct) {
		if (FStrEq(inf.pszItemName, pszItemName)) {
			return (cell)inf.hasItem(pPlayer);
		}
	}

	// weapon_*
	auto pInfo = g_ReGameApi->GetWeaponSlot(pszItemName);
	if (pInfo)
	{
		auto pItem = pPlayer->m_rgpPlayerItems[ pInfo->slot ];
		while (pItem)
		{
			if (FClassnameIs(pItem->pev, pszItemName)) {
				return TRUE;
			}

			pItem = pItem->m_pNext;
		}
	}

	return FALSE;
}

/*
* Returns specific information about the weapon.
*
* @param weapon name or id      Weapon id, see WEAPON_* constants, WeaponIdType or weapon_* name
* @param WpnInfo:type           Info type, see WI_* constants
*
* @note weapon_* name can only be used to get WI_ID
*
* @return                       Weapon information
* @error                        If weapon_id or type are out of bounds, an error will be thrown
*
* native any:rg_get_weapon_info(any:...);
*/
cell AMX_NATIVE_CALL rg_get_weapon_info(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_weapon_id, arg_type, arg_3, arg_4 };

	WeaponIdType weaponId = static_cast<WeaponIdType>(*getAmxAddr(amx, params[arg_weapon_id]));
	WpnInfo info_type = static_cast<WpnInfo>(*getAmxAddr(amx, params[arg_type]));

	if (!GetWeaponInfoRange(weaponId, false) && info_type != WI_ID)
	{
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: invalid weapon id %i", __FUNCTION__, weaponId);
		return 0;
	}

	char wname[256];
	const char* szWeaponName = getAmxString(amx, params[arg_weapon_id], wname);
	WeaponInfoStruct* info = g_ReGameApi->GetWeaponInfo(weaponId);

	switch (info_type)
	{
	case WI_ID:
		if (szWeaponName != nullptr) {
			auto infoName = g_ReGameApi->GetWeaponInfo(szWeaponName);
			if (infoName != nullptr) {
				return infoName->id;
			}
		}

		return WEAPON_NONE;
	case WI_COST:
		return info->cost;
	case WI_CLIP_COST:
		return info->clipCost;
	case WI_BUY_CLIP_SIZE:
		return info->buyClipSize;
	case WI_GUN_CLIP_SIZE:
		return info->gunClipSize;
	case WI_MAX_ROUNDS:
		return info->maxRounds;
	case WI_AMMO_TYPE:
		return info->ammoType;
	case WI_AMMO_NAME:
		{
			if (PARAMS_COUNT != arg_4) {
				AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: bad parameter count, got %i, expected %i", __FUNCTION__, PARAMS_COUNT, arg_4);
				return -1;
			}

			// native rg_get_weapon_info(id, WI_AMMO_NAME, output[], maxlength);
			cell* dest = getAmxAddr(amx, params[arg_3]);
			size_t length = *getAmxAddr(amx, params[arg_4]);

			if (info->ammoName1 == nullptr) {
				setAmxString(dest, "", 1);
				return 0;
			}

			setAmxString(dest, info->ammoName1, length);
			return 1;
		}
	case WI_NAME:
		{
			if (PARAMS_COUNT != arg_4) {
				AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: bad parameter count, got %i, expected %i", __FUNCTION__, PARAMS_COUNT, arg_4);
				return -1;
			}

			// native rg_get_weapon_info(id, WI_NAME, output[], maxlength);
			cell* dest = getAmxAddr(amx, params[arg_3]);
			size_t length = *getAmxAddr(amx, params[arg_4]);

			if (info->entityName == nullptr) {
				setAmxString(dest, "", 1);
				return 0;
			}

			setAmxString(dest, info->entityName, length);
			return 1;
		}
	case WI_SLOT:
		{
			auto pInfo = g_ReGameApi->GetWeaponSlot(weaponId);
			if (pInfo) {
				return pInfo->slot;
			}

			return NONE_SLOT;
		}
	default:
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: unknown type statement %i, params count %i", __FUNCTION__, info_type, PARAMS_COUNT);
		return -1;
	}
}

/*
* Sets specific weapon info values.
*
* @param weapon_id      Weapon id, see WEAPON_* constants
* @param type           Info type, see WI_* constants
*
* @return               1 on success, 0 otherwise
*
* native rg_set_weapon_info(const {WeaponIdType,_}:weapon_id, WpnInfo:type, any:...);
*/
cell AMX_NATIVE_CALL rg_set_weapon_info(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_weapon_id, arg_type, arg_value };

	WeaponIdType weaponId = static_cast<WeaponIdType>(params[arg_weapon_id]);
	if (!GetWeaponInfoRange(weaponId, true))
	{
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: invalid weapon id %i", __FUNCTION__, weaponId);
		return 0;
	}

	cell* value = getAmxAddr(amx, params[arg_value]);
	WeaponInfoStruct *info = g_ReGameApi->GetWeaponInfo(weaponId);
	WpnInfo info_type = static_cast<WpnInfo>(params[arg_type]);

	switch (info_type)
	{
	case WI_COST:
		info->cost = *value;
		break;
	case WI_CLIP_COST:
		info->clipCost = *value;
		break;
	case WI_BUY_CLIP_SIZE:
		info->buyClipSize = *value;
		break;
	case WI_GUN_CLIP_SIZE:
		info->gunClipSize= *value;
		break;
	case WI_MAX_ROUNDS:
		info->maxRounds = *value;
		break;
	case WI_AMMO_TYPE:
		info->ammoType = *value;
		break;
	case WI_AMMO_NAME:
	case WI_NAME:
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: this change will have no effect, type statement %i", __FUNCTION__, info_type);
		return 0;
	case WI_SLOT:
		{
			auto pInfo = g_ReGameApi->GetWeaponSlot(weaponId);
			pInfo->slot = static_cast<InventorySlotType>(*value);
			break;
		}
	default:
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: unknown type statement %i, params count %i", __FUNCTION__, info_type, PARAMS_COUNT);
		return 0;
	}

	return 1;
}

/*
* Remove all the player's stuff in a specific slot.
*
* @param index          Client index
* @param slot           The slot that will be emptied
* @param removeAmmo     Remove ammunition
*
* @return               1 - successful removal of all items in the slot or the slot is empty
*                       0 - if at least one item failed to remove
*
* native rg_remove_items_by_slot(const index, const InventorySlotType:slot, const bool:removeAmmo = true);
*/
cell AMX_NATIVE_CALL rg_remove_items_by_slot(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index, arg_slot, arg_remammo };

	CHECK_ISPLAYER(arg_index);

	CBasePlayer *pPlayer = UTIL_PlayerByIndex(params[arg_index]);
	CHECK_CONNECTED(pPlayer, arg_index);

	bool success = true;

	pPlayer->ForEachItem(params[arg_slot], [&](CBasePlayerItem *pItem)
	{
		// Compatible with older versions of the plugin,
		// which still only pass two parameters
		success &= pPlayer->CSPlayer()->RemovePlayerItemEx(STRING(pItem->pev->classname), (PARAMS_COUNT < 3 || params[arg_remammo] != 0)) ? true : false;
		return false;
	});

	return success ? TRUE : FALSE;
}

/*
* Drop to floor all the player's stuff by specific slot.
*
* @param index      Client index
* @param slot       Specific slot for remove of each item.
*
* @return           1 - successful drop of all items in the slot or the slot is empty
*                   0 - if at least one item failed to drop
*
* native rg_drop_items_by_slot(const index, const InventorySlotType:slot);
*/
cell AMX_NATIVE_CALL rg_drop_items_by_slot(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index, arg_slot };

	CHECK_ISPLAYER(arg_index);

	CBasePlayer *pPlayer = UTIL_PlayerByIndex(params[arg_index]);
	CHECK_CONNECTED(pPlayer, arg_index);

	bool success = true;

	pPlayer->ForEachItem(params[arg_slot], [&](CBasePlayerItem *pItem) {
		success &= pPlayer->CSPlayer()->DropPlayerItem(STRING(pItem->pev->classname)) ? true : false;
		return false;
	});

	return success ? TRUE : FALSE;
}

/*
* Remove all of the player's items.
*
* @param index      Client index
* @param removeSuit Remove suit
*
* @return           1 on success, 0 otherwise
*
* native rg_remove_all_items(const index, const bool:removeSuit = false);
*/
cell AMX_NATIVE_CALL rg_remove_all_items(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index, arg_suit };

	CHECK_ISPLAYER(arg_index);

	CBasePlayer *pPlayer = UTIL_PlayerByIndex(params[arg_index]);
	CHECK_CONNECTED(pPlayer, arg_index);

	pPlayer->CSPlayer()->RemoveAllItems(params[arg_suit] != 0);
	return TRUE;
}

/*
* Forces the player to drop the specified item classname.
*
* @param index      Client index
* @param item_name  Item classname, if no name, the active item classname
*
* @return           Entity index of weaponbox, AMX_NULLENT (-1) otherwise
*
* native rg_drop_item(const index, const item_name[]);
*/
cell AMX_NATIVE_CALL rg_drop_item(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index, arg_item_name };

	CHECK_ISPLAYER(arg_index);

	CBasePlayer *pPlayer = UTIL_PlayerByIndex(params[arg_index]);
	CHECK_CONNECTED(pPlayer, arg_index);

	char item[256];
	auto pEntity = pPlayer->CSPlayer()->DropPlayerItem(getAmxString(amx, params[arg_item_name], item));

	if (pEntity)
		return indexOfPDataAmx(pEntity);

	return AMX_NULLENT;
}

/*
* Executes a client command on the gamedll side.
*
* @param index      Client index
* @param command    Client command to execute
* @param arg        Optional command arguments
*
* @return           1 on success, 0 otherwise
*
* native rg_internal_cmd(const index, const cmd[], const arg[] = "");
*/
cell AMX_NATIVE_CALL rg_internal_cmd(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index, arg_cmd, arg_arg };

	CHECK_ISPLAYER(arg_index);

	CBasePlayer *pPlayer = UTIL_PlayerByIndex(params[arg_index]);
	if (unlikely(pPlayer == nullptr || pPlayer->has_disconnected)) {
		return FALSE;
	}

	char cmd[256], arg[256];
	pPlayer->CSPlayer()->ClientCommand(getAmxString(amx, params[arg_cmd], cmd), getAmxString(amx, params[arg_arg], arg));
	return TRUE;
}

/*
* Removes the specified item classname from the player
*
* @param index      Client index
* @param item_name  Item classname
* @param removeAmmo Remove ammunition
*
* @return           1 if found and remove, 0 otherwise
*
* native rg_remove_item(const index, const item_name[], const bool:removeAmmo = false);
*/
cell AMX_NATIVE_CALL rg_remove_item(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index, arg_item_name, arg_remammo };

	CHECK_ISPLAYER(arg_index);

	CBasePlayer *pPlayer = UTIL_PlayerByIndex(params[arg_index]);
	CHECK_CONNECTED(pPlayer, arg_index);

	char iname[256];
	const char* pszItemName = getAmxString(amx, params[arg_item_name], iname);
	if (pPlayer->CSPlayer()->RemovePlayerItemEx(pszItemName, params[arg_remammo] != 0)) {
		return TRUE;
	}

	return FALSE;
}

/*
* Sets the amount of ammo in the client's backpack for a specific weapon.
*
* @param index      Client index
* @param weapon     Weapon id
* @param amount     New backpack ammo amount to set
*
* @noreturn
*
* native rg_set_user_bpammo(const index, WeaponIdType:weapon, amount);
*/
cell AMX_NATIVE_CALL rg_set_user_bpammo(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index, arg_weapon_id, arg_amount };

	CHECK_ISPLAYER(arg_index);

	CBasePlayer *pPlayer = UTIL_PlayerByIndex(params[arg_index]);
	CHECK_CONNECTED(pPlayer, arg_index);

	auto pInfo = g_ReGameApi->GetWeaponSlot(static_cast<WeaponIdType>(params[arg_weapon_id]));
	if (!pInfo) {
		return FALSE;
	}

	auto pWeapon = pPlayer->ForEachItem<CBasePlayerWeapon>([pInfo](CBasePlayerWeapon *pWeapon) {
		return (pWeapon->IsWeapon() && pWeapon->m_iId == pInfo->id);
	});

	if (pWeapon) {
		pPlayer->m_rgAmmo[ pWeapon->m_iPrimaryAmmoType ] = params[arg_amount];
		return TRUE;
	}

	return FALSE;
}

/*
* Returns the amount of ammo in the client's backpack for a specific weapon.
*
* @param index      Client index
* @param weapon     Weapon id
*
* @return           Amount of ammo in backpack
*
* native rg_get_user_bpammo(const index, WeaponIdType:weapon);
*/
cell AMX_NATIVE_CALL rg_get_user_bpammo(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index, arg_weapon_id };

	CHECK_ISPLAYER(arg_index);

	CBasePlayer *pPlayer = UTIL_PlayerByIndex(params[arg_index]);
	CHECK_CONNECTED(pPlayer, arg_index);

	auto pInfo = g_ReGameApi->GetWeaponSlot(static_cast<WeaponIdType>(params[arg_weapon_id]));
	if (!pInfo) {
		return FALSE;
	}

	auto pWeapon = pPlayer->ForEachItem<CBasePlayerWeapon>([pInfo](CBasePlayerWeapon *pWeapon) {
		return (pWeapon->IsWeapon() && pWeapon->m_iId == pInfo->id);
	});

	if (pWeapon) {
		return (cell)pPlayer->m_rgAmmo[ pWeapon->m_iPrimaryAmmoType ];
	}

	return FALSE;
}

/*
* Sets the amount of clip ammo for a specific weapon.
*
* @param index      Client index
* @param weapon     Weapon id
* @param amount     New clip ammo amount to set
*
* @noreturn
*
* native rg_set_user_ammo(const index, WeaponIdType:weapon, amount);
*/
cell AMX_NATIVE_CALL rg_set_user_ammo(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index, arg_weapon_id, arg_amount };

	CHECK_ISPLAYER(arg_index);

	CBasePlayer *pPlayer = UTIL_PlayerByIndex(params[arg_index]);
	CHECK_CONNECTED(pPlayer, arg_index);

	auto pInfo = g_ReGameApi->GetWeaponSlot(static_cast<WeaponIdType>(params[arg_weapon_id]));
	if (!pInfo) {
		return FALSE;
	}

	auto pWeapon = pPlayer->ForEachItem<CBasePlayerWeapon>(pInfo->slot, [pInfo](CBasePlayerWeapon *pWeapon) {
		return (pWeapon->IsWeapon() && pWeapon->m_iId == pInfo->id);
	});

	if (pWeapon) {
		pWeapon->m_iClip = params[arg_amount];
		return TRUE;
	}

	return FALSE;
}

/*
* Returns the amount of clip ammo for a specific weapon.
*
* @param index      Client index
* @param weapon     Weapon id
*
* @return           Amount of clip ammo
*
* native rg_get_user_ammo(const index, WeaponIdType:weapon);
*/
cell AMX_NATIVE_CALL rg_get_user_ammo(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index, arg_weapon_id };

	CHECK_ISPLAYER(arg_index);

	CBasePlayer *pPlayer = UTIL_PlayerByIndex(params[arg_index]);
	CHECK_CONNECTED(pPlayer, arg_index);

	auto pInfo = g_ReGameApi->GetWeaponSlot(static_cast<WeaponIdType>(params[arg_weapon_id]));
	if (!pInfo) {
		return FALSE;
	}

	auto pWeapon = pPlayer->ForEachItem<CBasePlayerWeapon>(pInfo->slot, [pInfo](CBasePlayerWeapon *pWeapon) {
		return (pWeapon->IsWeapon() && pWeapon->m_iId == pInfo->id);
	});

	if (pWeapon) {
		return (cell)pWeapon->m_iClip;
	}

	return FALSE;
}

/*
* Sets the client's defusekit status and allows to set a custom HUD icon and color.
*
* @param index      Client index
* @param defusekit  If nonzero the client will have a defusekit, otherwise it will be removed
* @param color      Color RGB
* @param icon       HUD sprite to use as an icon
* @param flash      If nonzero the icon will flash red
*
* @noreturn
*
* native rg_give_defusekit(const index, const bool:bDefusekit = true, const Float:color[] = {0.0, 160.0, 0.0}, const icon[] = "defuser", const bool:bFlash = false);
*/
cell AMX_NATIVE_CALL rg_give_defusekit(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index, arg_def, arg_color, arg_icon, arg_flash };

	CHECK_GAMERULES();
	CHECK_ISPLAYER(arg_index);

	CBasePlayer *pPlayer = UTIL_PlayerByIndex(params[arg_index]);
	CHECK_CONNECTED(pPlayer, arg_index);

	// on the map there is not bomb places
	if (CSGameRules() != nullptr && !CSGameRules()->m_bMapHasBombTarget && !CSGameRules()->m_bMapHasBombZone) {
		return FALSE;
	}

	if (pPlayer->m_iTeam != CT) {
		return FALSE;
	}

	pPlayer->m_bHasDefuser = params[arg_def] ? true : false;
	pPlayer->pev->body     = params[arg_def] ? 1 : 0;

	if (params[arg_def] != 0)
	{
		Vector* color = (Vector *)getAmxAddr(amx, params[arg_color]);

		char iconbuf[256];
		const char* icon = getAmxString(amx, params[arg_icon], iconbuf);

		MESSAGE_BEGIN(MSG_ONE, gmsgStatusIcon, nullptr, pPlayer->pev);
			WRITE_BYTE(params[arg_flash] != 0 ? STATUSICON_FLASH : STATUSICON_SHOW);
			WRITE_STRING(icon);
			WRITE_BYTE(color->x);
			WRITE_BYTE(color->y);
			WRITE_BYTE(color->z);
		MESSAGE_END();
	}
	else
	{
		MESSAGE_BEGIN(MSG_ONE, gmsgStatusIcon, nullptr, pPlayer->pev);
			WRITE_BYTE(STATUSICON_HIDE);
			WRITE_STRING("defuser");
		MESSAGE_END();
	}

	return TRUE;
}

/*
* Returns the client's armor value and retrieves the type of armor.
*
* @param index        Client index
* @param armortype    Variable to store armor type in
*
* @return             Amount of armor, 0 if the client has no armor
*
* native rg_get_user_armor(const index, &ArmorType:armortype = ARMOR_NONE);
*/
cell AMX_NATIVE_CALL rg_get_user_armor(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index, arg_armortype };

	CHECK_ISPLAYER(arg_index);

	CBasePlayer *pPlayer = UTIL_PlayerByIndex(params[arg_index]);
	CHECK_CONNECTED(pPlayer, arg_index);

	*getAmxAddr(amx, params[arg_armortype]) = pPlayer->m_iKevlar;
	return static_cast<cell>(pPlayer->pev->armorvalue);
}

/*
* Sets the client's armor value and the type of armor.
*
* @param index          Client index
* @param armorvalue     Amount of armor to set
* @param armortype      Armor type to set
*
* @noreturn
*
* native rg_set_user_armor(const index, armorvalue, ArmorType:armortype);
*/
cell AMX_NATIVE_CALL rg_set_user_armor(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index, arg_armorvalue, arg_armortype };

	CHECK_ISPLAYER(arg_index);

	CBasePlayer *pPlayer = UTIL_PlayerByIndex(params[arg_index]);
	CHECK_CONNECTED(pPlayer, arg_index);

	ArmorType armorType = static_cast<ArmorType>(params[arg_armortype]);

	pPlayer->pev->armorvalue = params[arg_armorvalue];
	pPlayer->m_iKevlar = armorType;

	if (armorType == ARMOR_KEVLAR || armorType == ARMOR_VESTHELM) {
		MESSAGE_BEGIN(MSG_ONE, gmsgArmorType, nullptr, pPlayer->pev);
			WRITE_BYTE(armorType == ARMOR_VESTHELM ? 1 : 0);
		MESSAGE_END();
	}

	return TRUE;
}

/*
* Sets the client's team without killing the player and sets the client's model.
* @note To obtain a TeamName use the following:
*       new TeamName:team = get_member(id, m_iTeam);
*
* @param index          Client index
* @param team           Team id
* @param model          Internal model, use MODEL_AUTO for a random appearance or MODEL_UNASSIGNED to not update it
*
* @param send_teaminfo          If true, a TeamInfo message will be sent
* @param check_win_conditions   If true, a CheckWinConditions will be call
*
* @return               1 on success, 0 otherwise
*
* native rg_set_user_team(const index, {TeamName,_}:team, {ModelName,_}:model = MODEL_AUTO, const bool:send_teaminfo = true, const bool:check_win_conditions = false);
*/
cell AMX_NATIVE_CALL rg_set_user_team(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index, arg_team, arg_model, arg_sendinfo, arg_check_win_conditions };

	CHECK_GAMERULES();
	CHECK_ISPLAYER(arg_index);

	CBasePlayer *pPlayer = UTIL_PlayerByIndex(params[arg_index]);
	CHECK_CONNECTED(pPlayer, arg_index);

	CAmxArgs args(amx, params);

	TeamName prevTeam = pPlayer->m_iTeam;
	pPlayer->m_iTeam = args[arg_team];

	if (prevTeam != args[arg_team]) {
		// next team
		switch (pPlayer->m_iTeam) {
		case TERRORIST:
			CSGameRules()->m_iNumTerrorist++;
			break;
		case CT:
			CSGameRules()->m_iNumCT++;
			break;
		default:
			break;
		}

		// previous team
		switch (prevTeam) {
		case TERRORIST:
			CSGameRules()->m_iNumTerrorist--;
			if (pPlayer->m_bHasC4 && !CSGameRules()->m_flRestartRoundTime && CSGameRules()->m_bMapHasBombTarget)
			{
				if (CSGameRules()->m_iNumTerrorist > 0 && pPlayer->CSPlayer()->RemovePlayerItem("weapon_c4")) {
					pPlayer->m_bHasC4 = false;
					pPlayer->pev->body = 0;
					pPlayer->CSPlayer()->SetBombIcon();
					pPlayer->CSPlayer()->SetProgressBarTime(0);
					CSGameRules()->GiveC4();
				} else if (pPlayer->IsAlive()) {// are still alive
					pPlayer->CSPlayer()->DropPlayerItem("weapon_c4");
				}
			}
			break;
		case CT:
			CSGameRules()->m_iNumCT--;
			if (pPlayer->m_bHasDefuser) {
				pPlayer->m_bHasDefuser = false;
				pPlayer->pev->body = 0;

				MESSAGE_BEGIN(MSG_ONE, gmsgStatusIcon, nullptr, pPlayer->pev);
					WRITE_BYTE(STATUSICON_HIDE);
					WRITE_STRING("defuser");
				MESSAGE_END();

				pPlayer->CSPlayer()->SendItemStatus();
			}
			break;
		default:
			break;
		}
	}

	if (args[arg_model] > MODEL_UNASSIGNED) {
		if (args[arg_model] != MODEL_AUTO) {
			pPlayer->m_iModelName = args[arg_model];
		} else {
			pPlayer->m_iModelName = GetModelAuto(pPlayer->m_iTeam);
		}

		pPlayer->CSPlayer()->SetPlayerModel(pPlayer->m_bHasC4);
	}

	if (params[arg_sendinfo] != 0) {
		pPlayer->CSPlayer()->TeamChangeUpdate();
	}

	g_amxxapi.SetPlayerTeamInfo(args[arg_index], args[arg_team], GetTeamName(args[arg_team]));

	// If team was changed to spectator, then player should be able immediately to spectate
	if (args[arg_team] == SPECTATOR && !pPlayer->IsAlive()) {
		pPlayer->CSPlayer()->StartDeathCam();
	}

	if (PARAMS_COUNT >= 5 && args[arg_check_win_conditions]) {
		CSGameRules()->CheckWinConditions();
	}

	return TRUE;
}

/*
* Sets the client's player model.
*
* @param index          Client index
* @param model          Model name
* @param update_index   If true, the modelindex is updated as well
*
* @return               1 on success, 0 otherwise
*
* native rg_set_user_model(const index, const model[], const bool:update_index = false);
*/
cell AMX_NATIVE_CALL rg_set_user_model(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index, arg_model, arg_update };

	CHECK_ISPLAYER(arg_index);

	CBasePlayer *pPlayer = UTIL_PlayerByIndex(params[arg_index]);
	CHECK_CONNECTED(pPlayer, arg_index);

	char modelbuf[256];
	const char* newModel = getAmxString(amx, params[arg_model], modelbuf);
	if (*newModel == '\0') {
		AMXX_LogError(amx, AMX_ERR_NATIVE, "Model can not be empty");
		return FALSE;
	}

	pPlayer->CSPlayer()->SetPlayerModelEx(newModel);
	pPlayer->CSPlayer()->SetPlayerModel(pPlayer->m_bHasC4);

	if (params[arg_update] != 0)
	{
		char model[MAX_PATH];
		snprintf(model, sizeof(model), "models/player/%s/%s.mdl", newModel, newModel);
		pPlayer->CSPlayer()->SetNewPlayerModel(model);
	}

	return TRUE;
}

/*
* Resets the client's model.
*
* @param index          Client index
* @param update_index   If true, the modelindex is reset as well
*
* @return               1 on success, 0 otherwise
*
* native rg_reset_user_model(const index, const bool:update_index = false);
*/
cell AMX_NATIVE_CALL rg_reset_user_model(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index, arg_update };

	CHECK_ISPLAYER(arg_index);

	CBasePlayer *pPlayer = UTIL_PlayerByIndex(params[arg_index]);
	CHECK_CONNECTED(pPlayer, arg_index);

	pPlayer->CSPlayer()->SetPlayerModelEx("");
	pPlayer->CSPlayer()->SetPlayerModel(pPlayer->m_bHasC4);

	if (params[arg_update] != 0)
	{
		char *infobuffer = GET_INFO_BUFFER(pPlayer->edict());
		char *pModel = GET_KEY_VALUE(infobuffer, "model");

		char model[MAX_PATH];
		snprintf(model, sizeof(model), "models/player/%s/%s.mdl", pModel, pModel);
		pPlayer->CSPlayer()->SetNewPlayerModel(model);
	}

	return TRUE;
}

/*
* Enable/Disable player's footsteps.
*
* @param index      Client index
* @param silent     To enable silent footsteps
*
* @return           1 on success, 0 otherwise
*
* native rg_set_user_footsteps(const index, bool:silent = false);
*/
cell AMX_NATIVE_CALL rg_set_user_footsteps(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index, arg_silent };

	CHECK_ISPLAYER(arg_index);

	CBasePlayer *pPlayer = UTIL_PlayerByIndex(params[arg_index]);
	CHECK_CONNECTED(pPlayer, arg_index);

	if (params[arg_silent]) {
		pPlayer->m_flTimeStepSound = 999;
		pPlayer->pev->flTimeStepSound = 999;
	} else {
		pPlayer->m_flTimeStepSound = 0;
		pPlayer->pev->flTimeStepSound = 400;
	}

	return TRUE;
}

/*
* Get the current footsteps state of the player.
*
* @param index      Client index
*
* @return           1 if the player has silent footsteps, 0 otherwise
*
* native rg_get_user_footsteps(const index);
*/
cell AMX_NATIVE_CALL rg_get_user_footsteps(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index };

	CHECK_ISPLAYER(arg_index);

	CBasePlayer *pPlayer = UTIL_PlayerByIndex(params[arg_index]);
	CHECK_CONNECTED(pPlayer, arg_index);

	return (cell)(pPlayer->m_flTimeStepSound == 999);
}

/*
* Transfers C4 from one player to another.
*
* @param index      Client index
* @param receiver   Receiver index, if 0 it will transfer to a random player
*
* @return           Index of player entity if successfull, 0 otherwise
*
* native rg_transfer_c4(const index, const receiver = 0);
*/

cell AMX_NATIVE_CALL rg_transfer_c4(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index, arg_receiver };

	CHECK_GAMERULES();
	CHECK_ISPLAYER(arg_index);

	CBasePlayer *pPlayer = UTIL_PlayerByIndex(params[arg_index]);
	CHECK_CONNECTED(pPlayer, arg_index);

	if (!pPlayer->m_bHasC4) {
		return FALSE;
	}

	CBasePlayer *pReceiver = nullptr;

	if (params[arg_receiver] > 0 && params[arg_receiver] <= gpGlobals->maxClients) {
		pReceiver = UTIL_PlayerByIndex(params[arg_receiver]);
		CHECK_CONNECTED(pReceiver, arg_receiver);

		if (!pPlayer->CSPlayer()->RemovePlayerItemEx("weapon_c4", true)) {
			return FALSE;
		}

		if (!pReceiver->CSPlayer()->MakeBomber()) {
			return FALSE;
		}
	}
	else {
		int NumDeadCT, NumDeadTerrorist, NumAliveTerrorist, NumAliveCT;
		CSGameRules()->InitializePlayerCounts(NumAliveTerrorist, NumAliveCT, NumDeadTerrorist, NumDeadCT);

		if (pPlayer->m_iTeam == CT && NumAliveTerrorist < 1) {
			return FALSE;
		}

		if (pPlayer->m_iTeam == TERRORIST && NumAliveTerrorist < 2) {
			return FALSE;
		}

		if (!pPlayer->CSPlayer()->RemovePlayerItemEx("weapon_c4", true)) {
			return FALSE;
		}

		auto flags = pPlayer->pev->flags;
		pPlayer->pev->flags |= FL_DORMANT;
		pReceiver = CSGameRules()->GiveC4();
		pPlayer->pev->flags = flags;
	}

	if (pReceiver)
		return indexOfPDataAmx(pReceiver);

	return FALSE;
}

/*
* Instantly reload client's weapons.
*
* @param index      Client index
* @param weapon     Weapon entity-index, if 0 then all weapons will be reloaded
*
* @return           1 on success, 0 otherwise
*
* native rg_instant_reload_weapons(const index, const weapon = 0);
*/
cell AMX_NATIVE_CALL rg_instant_reload_weapons(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index, arg_weapon, arg_force_reload, arg_force_refill };

	CHECK_ISPLAYER(arg_index);

	CBasePlayer *pPlayer = UTIL_PlayerByIndex(params[arg_index]);
	CHECK_CONNECTED(pPlayer, arg_index);

	CBasePlayerWeapon *pWeapon = nullptr;
	if (params[arg_weapon] != 0)
	{
		pWeapon = getPrivate<CBasePlayerWeapon>(params[arg_weapon]);
		if (unlikely(pWeapon == nullptr)) {
			AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: invalid or uninitialized entity", __FUNCTION__);
			return FALSE;
		}

		CHECK_INSTANCE_OF(CBasePlayerWeapon, pWeapon);

		if (!pWeapon->IsWeapon()) {
			AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: #%d entity is not a weapon.", __FUNCTION__, indexOfEdict(pWeapon->pev));
			return FALSE;
		}
	}

	pPlayer->CSPlayer()->ReloadWeapons(pWeapon, true, true);
	return TRUE;
}

/*
* Plant a bomb.
*
* @param index      Owner index or 0 for worldspawn.
* @param origin     The origin of the bomb where it will be planted.
* @param angles     The angles of the planted bomb.
*
* @return           Index of bomb entity or AMX_NULLENT (-1) otherwise
*
* native rg_plant_bomb(const index, Float:vecOrigin[3], Float:vecAngles[3] = {0.0,0.0,0.0});
*/
cell AMX_NATIVE_CALL rg_plant_bomb(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index, arg_origin, arg_angles };
	CAmxArgs args(amx, params);

	entvars_t *pevOwner = nullptr;

	if (params[arg_index] != 0)
	{
		CHECK_ISPLAYER(arg_index);

		CBasePlayer *pPlayer = UTIL_PlayerByIndex(params[arg_index]);
		CHECK_CONNECTED(pPlayer, arg_index);
		pevOwner = pPlayer->pev;
	}

	CGrenade *pBomb = g_ReGameFuncs->PlantBomb(pevOwner, args[arg_origin], args[arg_angles]);

	// Sanity check anyway
	if (pBomb)
		return indexOfPDataAmx(pBomb);

	return AMX_NULLENT;
}

/*
* Sets the amount of reward in the game account for all players.
*
* @param rules_index    Look at the enum with name RewardRules
* @param amount         The money amount
*
* @noreturn
*
* native rg_set_account_rules(const RewardRules:rules_index, const amount);
*/
cell AMX_NATIVE_CALL rg_set_account_rules(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_rules_index, arg_amount };

	CHECK_GAMERULES();

	CSGameRules()->SetAccountRules(static_cast<RewardRules>(params[arg_rules_index]), params[arg_amount]);
	return TRUE;
}

/*
* Gets the specified reward rule's money amount.
*
* @param rules_index    Look at the enum with name RewardRules
*
* @return               The amount of reward
*
* native rg_get_account_rules(const RewardRules:rules_index);
*/
cell AMX_NATIVE_CALL rg_get_account_rules(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_rules_index };

	CHECK_GAMERULES();

	return (cell)CSGameRules()->GetAccountRules(static_cast<RewardRules>(params[arg_rules_index]));
}

/*
* Checks if the bomb is planted.
*
* @return           true if the bomb is planted, false otherwise
*
* native bool:rg_is_bomb_planted();
*/
cell AMX_NATIVE_CALL rg_is_bomb_planted(AMX *amx, cell *params)
{
	CHECK_GAMERULES();

	return (cell)CSGameRules()->IsBombPlanted();
}

/*
* Forces a player to join a team.
*
* @param index      Client index
* @param team       Team id
*
* @return           1 on success, 0 otherwise
*
* native rg_join_team(const index, const TeamName:team);
*/
cell AMX_NATIVE_CALL rg_join_team(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index, arg_team };

	CHECK_ISPLAYER(arg_index);

	CBasePlayer *pPlayer = UTIL_PlayerByIndex(params[arg_index]);
	CHECK_CONNECTED(pPlayer, arg_index);

	return (cell)pPlayer->CSPlayer()->JoinTeam(static_cast<TeamName>(params[arg_team]));
}

/*
* Instantly balances the teams.
*
* @noreturn
*
* native rg_balance_teams();
*/
cell AMX_NATIVE_CALL rg_balance_teams(AMX *amx, cell *params)
{
	CHECK_GAMERULES();

	CSGameRules()->BalanceTeams();
	return TRUE;
}

/*
* Swaps players' teams without reseting frags, deaths and wins.
*
* @noreturn
*
* native rg_swap_all_players();
*/
cell AMX_NATIVE_CALL rg_swap_all_players(AMX *amx, cell *params)
{
	CHECK_GAMERULES();

	CSGameRules()->SwapAllPlayers();
	return TRUE;
}

/*
* Instantly switches the player to his opposite team.
* @note Switch from CT to TERRORIST is also opposite.
*
* @param index      Client index
*
* @noreturn
*
* native rg_switch_team(const index);
*/
cell AMX_NATIVE_CALL rg_switch_team(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index };

	CHECK_ISPLAYER(arg_index);

	CBasePlayer *pPlayer = UTIL_PlayerByIndex(params[arg_index]);
	CHECK_CONNECTED(pPlayer, arg_index);

	pPlayer->CSPlayer()->SwitchTeam();
	return TRUE;
}

/*
* Forces the player to switch to a specific weapon.
*
* @param index      Client index
* @param weapon     Weapon entity-index
*
* @return           1 on success, 0 otherwise
*
* native rg_switch_weapon(const index, const weapon);
*/
cell AMX_NATIVE_CALL rg_switch_weapon(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index, arg_weapon };

	CHECK_ISPLAYER(arg_index);

	CBasePlayer *pPlayer = UTIL_PlayerByIndex(params[arg_index]);
	CHECK_CONNECTED(pPlayer, arg_index);

	CBasePlayerWeapon *pWeapon = getPrivate<CBasePlayerWeapon>(params[arg_weapon]);
	if (unlikely(pWeapon == nullptr)) {
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: invalid or uninitialized entity", __FUNCTION__);
		return FALSE;
	}

	CHECK_INSTANCE_OF(CBasePlayerWeapon, pWeapon);

	if (!pWeapon->IsWeapon()) {
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: #%d entity is not a weapon.", __FUNCTION__, indexOfEdict(pWeapon->pev));
		return FALSE;
	}

	return (cell)pPlayer->CSPlayer()->SwitchWeapon(pWeapon);
}

/*
* Gets which team has a higher join priority.
*
* @return           Returns the Team Name
*
* native TeamName:rg_get_join_team_priority();
*/
cell AMX_NATIVE_CALL rg_get_join_team_priority(AMX *amx, cell *params)
{
	CHECK_GAMERULES();

	return CSGameRules()->SelectDefaultTeam();
}

/*
* Checks whether the player can take damage from the attacker.
*
* @param index      Client index
* @param attacker   Attacker index
*
* @return           true if player can take damage, false otherwise
*
* native bool:rg_is_player_can_takedamage(const index, const attacker);
*/
cell AMX_NATIVE_CALL rg_is_player_can_takedamage(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index, arg_attacker };

	CHECK_GAMERULES();
	CHECK_ISPLAYER(arg_index);

	CBasePlayer *pPlayer = UTIL_PlayerByIndex(params[arg_index]);
	CHECK_CONNECTED(pPlayer, arg_index);

	CBaseEntity *pAttacker = getPrivate<CBaseEntity>(params[arg_attacker]);
	if (!pAttacker) {
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: Invalid entity attacker", __FUNCTION__);
		return FALSE;
	}

	return CSGameRules()->FPlayerCanTakeDamage(pPlayer, pAttacker);
}

/*
* Checks whether the player can respawn.
*
* @param index      Client index
*
* @return           true if player can respawn, false otherwise
*
* native bool:rg_is_player_can_respawn(const index);
*/
cell AMX_NATIVE_CALL rg_is_player_can_respawn(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index };

	CHECK_GAMERULES();
	CHECK_ISPLAYER(arg_index);

	CBasePlayer *pPlayer = UTIL_PlayerByIndex(params[arg_index]);
	CHECK_CONNECTED(pPlayer, arg_index);

	return CSGameRules()->FPlayerCanRespawn(pPlayer);
}

/*
* Gets WeaponIdType from weaponbox
*
* @param entity     Weaponbox entity
*
* @return           return enum of WeaponIdType
*
* native WeaponIdType:rg_get_weaponbox_id(const entity);
*/
cell AMX_NATIVE_CALL rg_get_weaponbox_id(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_entity };

	CHECK_ISENTITY(arg_entity);

	CWeaponBox *pEntityBox = getPrivate<CWeaponBox>(params[arg_entity]);
	if (pEntityBox == nullptr) {
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: Invalid entity weaponbox", __FUNCTION__);
		return FALSE;
	}

	for (auto item : pEntityBox->m_rgpPlayerItems) {
		if (item) {
			return item->m_iId;
		}
	}

	return WEAPON_NONE;
}

/*
* Respawn on round for players/bots.
*
* @param index      Client index
*
* @noreturn
*
* native rg_round_respawn(const index);
*/
cell AMX_NATIVE_CALL rg_round_respawn(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index };

	CBasePlayer *pPlayer = UTIL_PlayerByIndex(params[arg_index]);
	CHECK_CONNECTED(pPlayer, arg_index);

	pPlayer->RoundRespawn();
	return TRUE;
}

/*
* Resets player's maxspeed.
*
* @param index      Client index
*
* @noreturn
*
* native rg_reset_maxspeed(const index);
*/
cell AMX_NATIVE_CALL rg_reset_maxspeed(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index };

	CBasePlayer *pPlayer = UTIL_PlayerByIndex(params[arg_index]);
	CHECK_CONNECTED(pPlayer, arg_index);

	pPlayer->ResetMaxSpeed();
	return TRUE;
}

/*
* Draws a HUD progress bar which fills from 0% to 100% for the time duration in seconds.
* @note: Set the duration to 0 to hide the bar.
*
* @param index      Client index
* @param time       Duration
* @param observer   Send for everyone who is observing the player
*
* @noreturn
*
* native rg_send_bartime(const index, const duration, const bool:observer = true);
*/
cell AMX_NATIVE_CALL rg_send_bartime(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index, arg_time, arg_observer };

	CBasePlayer *pPlayer = UTIL_PlayerByIndex(params[arg_index]);
	CHECK_CONNECTED(pPlayer, arg_index);

	CAmxArgs args(amx, params);
	if (!args[arg_observer]) {
		EMESSAGE_BEGIN(MSG_ONE_UNRELIABLE, gmsgBarTime, nullptr, pPlayer->edict());
			EWRITE_SHORT(args[arg_time]);
		EMESSAGE_END();
		return TRUE;
	}

	pPlayer->CSPlayer()->SetProgressBarTime(args[arg_time]);
	return TRUE;
}

/*
* Same as BarTime, but StartPercent specifies how much of the bar is (already) filled.
*
* @param index          Client index
* @param time           Duration
* @param startPercent   Start percent
* @param observer       Send for everyone who is observing the player
*
* @noreturn
*
* native rg_send_bartime2(const index, const duration, const Float:startPercent, const bool:observer = true);
*/
cell AMX_NATIVE_CALL rg_send_bartime2(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index, arg_time, arg_start_percent, arg_observer };

	CBasePlayer *pPlayer = UTIL_PlayerByIndex(params[arg_index]);
	CHECK_CONNECTED(pPlayer, arg_index);

	CAmxArgs args(amx, params);
	int duration = args[arg_time];
	float startPercent = args[arg_start_percent];
	if (!args[arg_observer]) {
		EMESSAGE_BEGIN(MSG_ONE_UNRELIABLE, gmsgBarTime2, nullptr, pPlayer->edict());
			EWRITE_SHORT(duration);
			EWRITE_SHORT(startPercent);
		EMESSAGE_END();
		return TRUE;
	}

	float timeElapsed = (startPercent / 100.0f) * duration;
	pPlayer->CSPlayer()->SetProgressBarTime2(duration, timeElapsed);
	return TRUE;
}

/*
* Sends the SendAudio message - plays the specified audio.
*
* @param index      Receiver index or use 0 for everyone
* @param sample     Sound file to play
* @param pitch      Sound pitch
*
* @noreturn
*
* native rg_send_audio(const index, const sample[], const pitch = PITCH_NORM);
*/
cell AMX_NATIVE_CALL rg_send_audio(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index, arg_sample, arg_pitch };

	int nIndex = params[arg_index];
	if (nIndex < 0)
		nIndex = 0;

	char sample[256];
	const char *szSample = getAmxString(amx, params[arg_sample], sample);
	auto pEdict = (nIndex == 0) ? nullptr : edictByIndexAmx(nIndex);

	EMESSAGE_BEGIN(nIndex ? MSG_ONE_UNRELIABLE : MSG_BROADCAST, gmsgSendAudio, nullptr, pEdict);
		EWRITE_BYTE(nIndex);
		EWRITE_STRING(szSample);
		EWRITE_SHORT(params[arg_pitch]);
	EMESSAGE_END();

	return TRUE;
}

enum ItemInfo_e
{
	ItemInfo_iSlot,
	ItemInfo_iPosition,
	ItemInfo_pszAmmo1,
	ItemInfo_iMaxAmmo1,
	ItemInfo_pszAmmo2,
	ItemInfo_iMaxAmmo2,
	ItemInfo_pszName,
	ItemInfo_iMaxClip,
	ItemInfo_iId,
	ItemInfo_iFlags,
	ItemInfo_iWeight
};

/**
* Sets a parameter of the member CSPlayerItem::m_ItemInfo
*
* @param entity Entity index
* @param type   Item info type. See ItemInfo constants.
*
* native rg_set_iteminfo(const entity, ItemInfo:type, any:...);
*/
cell AMX_NATIVE_CALL rg_set_iteminfo(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index, arg_type, arg_value };

	CHECK_ISENTITY(arg_index);

	CBasePlayerWeapon *pWeapon = getPrivate<CBasePlayerWeapon>(params[arg_index]);
	if (unlikely(pWeapon == nullptr)) {
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: invalid or uninitialized entity", __FUNCTION__);
		return FALSE;
	}

	CHECK_INSTANCE_OF(CBasePlayerWeapon, pWeapon);

	if (!pWeapon->IsWeapon()) {
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: #%d entity is not a weapon.", __FUNCTION__, indexOfEdict(pWeapon->pev));
		return FALSE;
	}

	CCSPlayerItem *pItem = pWeapon->CSPlayerItem();
	if (unlikely(pItem == nullptr)) {
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: invalid or uninitialized m_pEntity.", __FUNCTION__);
		return FALSE;
	}

	char itembuf[256];
	cell *ptr = getAmxAddr(amx, params[arg_value]);

	ItemInfo_e type = static_cast<ItemInfo_e>(params[arg_type]);
	switch (type)
	{
	case ItemInfo_iSlot:     pItem->m_ItemInfo.iSlot     = *ptr; break;
	case ItemInfo_iPosition: pItem->m_ItemInfo.iPosition = *ptr; break;
	case ItemInfo_iMaxAmmo1: pItem->m_ItemInfo.iMaxAmmo1 = *ptr; break;
	case ItemInfo_iMaxAmmo2: pItem->m_ItemInfo.iMaxAmmo2 = *ptr; break;
	case ItemInfo_iMaxClip:  pItem->m_ItemInfo.iMaxClip  = *ptr; break;
	case ItemInfo_iId:       pItem->m_ItemInfo.iId       = *ptr; break;
	case ItemInfo_iFlags:    pItem->m_ItemInfo.iFlags    = *ptr; break;
	case ItemInfo_iWeight:   pItem->m_ItemInfo.iWeight   = *ptr; break;
	case ItemInfo_pszAmmo1:  pItem->m_ItemInfo.pszAmmo1  = STRING(getAmxStringAlloc(amx, params[arg_value], itembuf)); break;
	case ItemInfo_pszAmmo2:  pItem->m_ItemInfo.pszAmmo2  = STRING(getAmxStringAlloc(amx, params[arg_value], itembuf)); break;
	case ItemInfo_pszName:   pItem->m_ItemInfo.pszName   = STRING(getAmxStringAlloc(amx, params[arg_value], itembuf)); break;

	default:
		AMXX_LogError(amx, AMX_ERR_NATIVE, "Unknown ItemInfo type %d", type);
		return FALSE;
	}

	return TRUE;
}

/**
* Gets a parameter of the member CSPlayerItem::m_ItemInfo
*
* @param entity Entity index
* @param type   Item info type. See ItemInfo constants.
*
* native rg_get_iteminfo(const ent, ItemInfo:type, any:...);
*/
cell AMX_NATIVE_CALL rg_get_iteminfo(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index, arg_type, arg_output, arg_length };

	CHECK_ISENTITY(arg_index);

	CBasePlayerWeapon *pWeapon = getPrivate<CBasePlayerWeapon>(params[arg_index]);
	if (unlikely(pWeapon == nullptr)) {
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: invalid or uninitialized entity", __FUNCTION__);
		return FALSE;
	}

	ItemInfo_e type = static_cast<ItemInfo_e>(params[arg_type]);
	if ((type == ItemInfo_pszAmmo1 || type == ItemInfo_pszAmmo2 || type == ItemInfo_pszName) && PARAMS_COUNT != 4) {
		AMXX_LogError(amx, AMX_ERR_NATIVE, "Bad arg count. Expected %d, got %d.", 4, PARAMS_COUNT);
		return FALSE;
	}

	CHECK_INSTANCE_OF(CBasePlayerWeapon, pWeapon);

	if (!pWeapon->IsWeapon()) {
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: #%d entity is not a weapon.", __FUNCTION__, indexOfEdict(pWeapon->pev));
		return FALSE;
	}

	CCSPlayerItem *pItem = pWeapon->CSPlayerItem();
	if (unlikely(pItem == nullptr)) {
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: invalid or uninitialized m_pEntity.", __FUNCTION__);
		return FALSE;
	}

	cell *dest = getAmxAddr(amx, params[arg_output]);
	size_t length = (PARAMS_COUNT == 4) ? *getAmxAddr(amx, params[arg_length]) : 0;

	switch (type)
	{
	case ItemInfo_iSlot:     return pItem->m_ItemInfo.iSlot;
	case ItemInfo_iPosition: return pItem->m_ItemInfo.iPosition;
	case ItemInfo_iMaxAmmo1: return pItem->m_ItemInfo.iMaxAmmo1;
	case ItemInfo_iMaxAmmo2: return pItem->m_ItemInfo.iMaxAmmo2;
	case ItemInfo_iMaxClip:  return pItem->m_ItemInfo.iMaxClip;
	case ItemInfo_iId:       return pItem->m_ItemInfo.iId;
	case ItemInfo_iFlags:    return pItem->m_ItemInfo.iFlags;
	case ItemInfo_iWeight:   return pItem->m_ItemInfo.iWeight;
	case ItemInfo_pszAmmo1:
	{
		if (pItem->m_ItemInfo.pszAmmo1 == nullptr) {
			setAmxString(dest, "", 1);
			break;
		}

		setAmxString(dest, pItem->m_ItemInfo.pszAmmo1, length);
		break;
	}
	case ItemInfo_pszAmmo2:
	{
		if (pItem->m_ItemInfo.pszAmmo2 == nullptr) {
			setAmxString(dest, "", 1);
			break;
		}

		setAmxString(dest, pItem->m_ItemInfo.pszAmmo2, length);
		break;
	}
	case ItemInfo_pszName:
	{
		if (pItem->m_ItemInfo.pszName == nullptr) {
			setAmxString(dest, "", 1);
			break;
		}

		setAmxString(dest, pItem->m_ItemInfo.pszName, length);
		break;
	}
	default:
		AMXX_LogError(amx, AMX_ERR_NATIVE, "Unknown ItemInfo type %d", type);
		return FALSE;
	}

	return TRUE;
}

/**
* Sets a parameter of the global CBasePlayerItem::m_ItemInfoArray array
* @note To have effect on client side (i.g. ammo size on HUD) you should
*       alter this value BEFORE WeaponList message is sent to client, or
*		force it's alteration by sending again to the specific client.
*		Hooking WeaponList message with AMXX's register_message is a choice.
*
* @param weapon_id Weapon id, see WEAPON_* constants
* @param type      Item info type. See ItemInfo constants.
*
* native rg_set_global_iteminfo(const {WeaponIdType,_}:weapon_id, ItemInfo:type, any:...);
*/
cell AMX_NATIVE_CALL rg_set_global_iteminfo(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_weapon_id, arg_type, arg_value };

	WeaponIdType weaponId = static_cast<WeaponIdType>(params[arg_weapon_id]);
	if (!GetWeaponInfoRange(weaponId, false))
	{
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: invalid weapon id %i", __FUNCTION__, weaponId);
		return FALSE;
	}

	ItemInfo* II = g_ReGameApi->GetItemInfo(weaponId);

	char itembuf[256];
	cell *ptr = getAmxAddr(amx, params[arg_value]);

	ItemInfo_e type = static_cast<ItemInfo_e>(params[arg_type]);
	switch (type)
	{
	case ItemInfo_iSlot:     II->iSlot     = *ptr; break;
	case ItemInfo_iPosition: II->iPosition = *ptr; break;
	case ItemInfo_iMaxAmmo1: II->iMaxAmmo1 = *ptr; break;
	case ItemInfo_iMaxAmmo2: II->iMaxAmmo2 = *ptr; break;
	case ItemInfo_iMaxClip:  II->iMaxClip  = *ptr; break;
	case ItemInfo_iId:       II->iId       = *ptr; break;
	case ItemInfo_iFlags:    II->iFlags    = *ptr; break;
	case ItemInfo_iWeight:   II->iWeight   = *ptr; break;
	case ItemInfo_pszAmmo1:  II->pszAmmo1  = STRING(getAmxStringAlloc(amx, params[arg_value], itembuf)); break;
	case ItemInfo_pszAmmo2:  II->pszAmmo2  = STRING(getAmxStringAlloc(amx, params[arg_value], itembuf)); break;
	case ItemInfo_pszName:   II->pszName   = STRING(getAmxStringAlloc(amx, params[arg_value], itembuf)); break;

	default:
		AMXX_LogError(amx, AMX_ERR_NATIVE, "Unknown ItemInfo type %d", type);
		return FALSE;
	}

	return TRUE;
}

/**
* Gets a parameter of the global CBasePlayerItem::m_ItemInfoArray array
*
* @param weapon_id Weapon id, see WEAPON_* constants
* @param type      Item info type. See ItemInfo constants.
*
* native rg_get_global_iteminfo(const {WeaponIdType,_}:weapon_id, ItemInfo:type, any:...);
*/
cell AMX_NATIVE_CALL rg_get_global_iteminfo(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_weapon_id, arg_type, arg_output, arg_length };

	WeaponIdType weaponId = static_cast<WeaponIdType>(params[arg_weapon_id]);
	if (!GetWeaponInfoRange(weaponId, false))
	{
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: invalid weapon id %i", __FUNCTION__, weaponId);
		return FALSE;
	}

	ItemInfo_e type = static_cast<ItemInfo_e>(params[arg_type]);
	if ((type == ItemInfo_pszAmmo1 || type == ItemInfo_pszAmmo2 || type == ItemInfo_pszName) && PARAMS_COUNT != 4)
	{
		AMXX_LogError(amx, AMX_ERR_NATIVE, "Bad arg count. Expected %d, got %d.", 4, PARAMS_COUNT);
		return FALSE;
	}

	ItemInfo* II = g_ReGameApi->GetItemInfo(weaponId);

	cell *dest = getAmxAddr(amx, params[arg_output]);
	size_t length = (PARAMS_COUNT == 4) ? *getAmxAddr(amx, params[arg_length]) : 0;

	switch (type)
	{
	case ItemInfo_iSlot:     return II->iSlot;
	case ItemInfo_iPosition: return II->iPosition;
	case ItemInfo_iMaxAmmo1: return II->iMaxAmmo1;
	case ItemInfo_iMaxAmmo2: return II->iMaxAmmo2;
	case ItemInfo_iMaxClip:  return II->iMaxClip;
	case ItemInfo_iId:       return II->iId;
	case ItemInfo_iFlags:    return II->iFlags;
	case ItemInfo_iWeight:   return II->iWeight;
	case ItemInfo_pszAmmo1:
	{
		if (II->pszAmmo1 == nullptr) {
			setAmxString(dest, "", 1);
			break;
		}

		setAmxString(dest, II->pszAmmo1, length);
		break;
	}
	case ItemInfo_pszAmmo2:
	{
		if (II->pszAmmo2 == nullptr) {
			setAmxString(dest, "", 1);
			break;
		}

		setAmxString(dest, II->pszAmmo2, length);
		break;
	}
	case ItemInfo_pszName:
	{
		if (II->pszName == nullptr) {
			setAmxString(dest, "", 1);
			break;
		}

		setAmxString(dest, II->pszName, length);
		break;
	}
	default:
		AMXX_LogError(amx, AMX_ERR_NATIVE, "Unknown ItemInfo type %d", type);
		return FALSE;
	}

	return TRUE;
}

/*
* Adds hint message to the queue.
*
* @param index                 Client index
* @param message               The message hint
* @param duration              The time duration in seconds stays on screen
* @param bDisplayIfPlayerDead  Whether to print hint for dead players?
* @param bOverride             Whether to override previous messages?
*
* @return                      true if prints, false otherwise
*
* native bool:rg_hint_message(const index, const message[], Float:duration = 6.0, bool:bDisplayIfPlayerDead = false, bool:bOverride = false);
*/
cell AMX_NATIVE_CALL rg_hint_message(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index, arg_message, arg_duration, arg_displayIfPlayerDead, arg_override };

	CHECK_ISPLAYER(arg_index);

	CBasePlayer *pPlayer = UTIL_PlayerByIndex(params[arg_index]);
	CHECK_CONNECTED(pPlayer, arg_index);

	char messagebuf[190];
	const char *message  = getAmxString(amx, params[arg_message], messagebuf);

	if (!message || message[0] == '\0') {
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: sending an empty hint message is meaningless. rework your code.", __FUNCTION__);
		return FALSE;
	}

	CAmxArgs args(amx, params);
	return pPlayer->CSPlayer()->HintMessageEx(message, args[arg_duration], args[arg_displayIfPlayerDead], args[arg_override]) ? TRUE : FALSE;
}

/*
* Instantly restart round.
*
* @noreturn
*
* native rg_restart_round();
*/
cell AMX_NATIVE_CALL rg_restart_round(AMX *amx, cell *params)
{
	CHECK_GAMERULES();

	CSGameRules()->RestartRound();
	return TRUE;
}

/*
* Instantly check win conditions.
*
* @noreturn
*
* native rg_check_win_conditions();
*/
cell AMX_NATIVE_CALL rg_check_win_conditions(AMX *amx, cell *params)
{
	CHECK_GAMERULES();

	CSGameRules()->CheckWinConditions();
	return TRUE;
}

/*
* Instantly initialize player counts.
*
* @param num_alive_terrorist   Count alive terrorists
* @param num_alive_ct          Count alive counter-terrorists
* @param num_dead_terrorist    Count dead terrorists
* @param num_dead_ct           Count dead counter-terrorists
*
* @noreturn
*
* native rg_initialize_player_counts(&num_alive_terrorist = 0, &num_alive_ct = 0, &num_dead_terrorist = 0, &num_dead_ct = 0);
*/
cell AMX_NATIVE_CALL rg_initialize_player_counts(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_num_alive_terrorist, arg_num_alive_ct, arg_num_dead_terrorist, arg_num_dead_ct };

	CHECK_GAMERULES();

	cell& numAliveTerrorist = *getAmxAddr(amx, params[arg_num_alive_terrorist]);
	cell& numAliveCT        = *getAmxAddr(amx, params[arg_num_alive_ct]);
	cell& numDeadTerrorist  = *getAmxAddr(amx, params[arg_num_dead_terrorist]);
	cell& numDeadCT         = *getAmxAddr(amx, params[arg_num_dead_ct]);

	CSGameRules()->InitializePlayerCounts(numAliveTerrorist, numAliveCT, numDeadTerrorist, numDeadCT);
	return TRUE;
}

/*
* Reset if player can hear another player.
*
* @param index                 Player id
* @noreturn
*
* native rg_reset_can_hear_player(const index);
*/
cell AMX_NATIVE_CALL rg_reset_can_hear_player(AMX* amx, cell* params)
{
	enum args_e { arg_count, arg_index };

	CHECK_GAMERULES();
	CHECK_ISPLAYER(arg_index);

	CBasePlayer* pPlayer = UTIL_PlayerByIndex(params[arg_index]);
	CHECK_CONNECTED(pPlayer, arg_index);

	CSGameRules()->m_VoiceGameMgr.m_pHelper->ResetCanHearPlayer(pPlayer->edict());

	return TRUE;
}

/*
* Set if player can hear another player
*
* @param listener              Listener player id
* @param sender                Sender player id
* @param can_hear              Can listener hear sender
*
* @noreturn
*
* native rg_set_can_hear_player(const listener, const sender, const bool:can_hear);
*/
cell AMX_NATIVE_CALL rg_set_can_hear_player(AMX* amx, cell* params)
{
	enum args_e { arg_count, arg_listener, arg_sender, arg_can_hear };

	CHECK_GAMERULES();
	CHECK_ISPLAYER(arg_listener);
	CHECK_ISPLAYER(arg_sender);

	CBasePlayer* pListener = UTIL_PlayerByIndex(params[arg_listener]);
	CHECK_CONNECTED(pListener, arg_listener);

	CBasePlayer* pSender = UTIL_PlayerByIndex(params[arg_sender]);
	CHECK_CONNECTED(pSender, arg_sender);

	CSGameRules()->m_VoiceGameMgr.m_pHelper->SetCanHearPlayer(pListener, pSender, params[arg_can_hear] != 0);

	return TRUE;
}

/*
* Get if player can hear another player
*
* @param listener              Listener player id
* @param sender                Sender player id
*
* @return boolean
*
* native bool:rg_get_can_hear_player(const listener, const sender);
*/
cell AMX_NATIVE_CALL rg_get_can_hear_player(AMX* amx, cell* params)
{
	enum args_e { arg_count, arg_listener, arg_sender };

	CHECK_GAMERULES();
	CHECK_ISPLAYER(arg_listener);
	CHECK_ISPLAYER(arg_sender);

	CBasePlayer* pListener = UTIL_PlayerByIndex(params[arg_listener]);
	CHECK_CONNECTED(pListener, arg_listener);

	CBasePlayer* pSender = UTIL_PlayerByIndex(params[arg_sender]);
	CHECK_CONNECTED(pSender, arg_sender);

	return CSGameRules()->m_VoiceGameMgr.m_pHelper->GetCanHearPlayer(pListener, pSender);
}

/*
* Spawn a head gib
*
* @param index                 Entity id
*
* @return                      Index of head gib entity or AMX_NULLENT (-1) otherwise
*
* native rg_spawn_head_gib(const index);
*/
cell AMX_NATIVE_CALL rg_spawn_head_gib(AMX* amx, cell* params)
{
	enum args_e { arg_count, arg_index };

	CHECK_ISENTITY(arg_index);

	CBaseEntity *pEntity = getPrivate<CBaseEntity>(params[arg_index]);
	if (unlikely(pEntity == nullptr)) {
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: invalid or uninitialized entity", __FUNCTION__);
		return FALSE;
	}

	entvars_t *pevEntity = pEntity->pev;
	CGib *pHeadGib = g_ReGameFuncs->SpawnHeadGib(pevEntity);

	// Sanity check anyway
	if (pHeadGib)
		return indexOfPDataAmx(pHeadGib);

	return AMX_NULLENT;
}

/*
* Spawn random gibs
*
* @param index                 Entity id
* @param cGibs                 Count gibs
* @param bHuman                Set gibs of a human or alien
*
* @noreturn
*
* native rg_spawn_random_gibs(const index, const cGibs, const bool:bHuman = true);
*/
cell AMX_NATIVE_CALL rg_spawn_random_gibs(AMX* amx, cell* params)
{
	enum args_e { arg_count, arg_index, arg_gibs, arg_human };

	CHECK_ISENTITY(arg_index);

	CBaseEntity *pEntity = getPrivate<CBaseEntity>(params[arg_index]);
	if (unlikely(pEntity == nullptr)) {
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: invalid or uninitialized entity", __FUNCTION__);
		return FALSE;
	}

	entvars_t *pevEntity = pEntity->pev;

	g_ReGameFuncs->SpawnRandomGibs(pevEntity, params[arg_gibs], params[arg_human]);
	return TRUE;
}

/*
* Spawn a grenade (HEGrenade, Flashbang, SmokeGrenade, or C4)
*
* @param weaponId              WEAPON_HEGRENADE, WEAPON_SMOKEGRENADE, WEAPON_FLASHBANG or WEAPON_C4
* @param pevOwner              Grenade owner
* @param vecSrc                Grenade spawn position
* @param vecThrow              Grenade velocity vector
* @param time                  Grenade explosion time
* @param iTeam                 Grenade team, see TEAM_* constants
* @param usEvent               Event index related to grenade (returned value of precache_event)
*
* @return                      Entity index on success, AMX_NULLENT (-1) otherwise
*/
cell AMX_NATIVE_CALL rg_spawn_grenade(AMX* amx, cell* params)
{
	enum args_e { arg_count, arg_weapon_id, arg_index, arg_origin, arg_velocity, arg_time, arg_team, arg_event };

	CHECK_ISPLAYER(arg_index);

	CBasePlayer *pPlayer = UTIL_PlayerByIndex(params[arg_index]);
	CHECK_CONNECTED(pPlayer, arg_index);

	CAmxArgs args(amx, params);

	CGrenade *pBomb = g_ReGameFuncs->SpawnGrenade(args[arg_weapon_id],
						pPlayer->pev,
						args[arg_origin],
						args[arg_velocity],
						args[arg_time],
						args[arg_team],
						args[arg_event]);

	// Sanity check anyway
	if (pBomb)
		return indexOfPDataAmx(pBomb);

	return AMX_NULLENT;
}

/*
* Spawn a weaponbox entity with its properties
*
* @param pItem                 Weapon entity index to attach
* @param pPlayerOwner          Player index to remove pItem entity (0 = no weapon owner)
* @param modelName             Model name ("models/w_*.mdl")
* @param origin                Weaponbox origin position
* @param angles                Weaponbox angles
* @param velocity              Weaponbox initial velocity vector
* @param lifeTime              Time to stay in world (< 0.0 = use mp_item_staytime cvar value)
* @param packAmmo              Set if ammo should be removed from weapon owner
*
* @return                      Weaponbox ent index on success, AMX_NULLENT (-1) otherwise
*/
cell AMX_NATIVE_CALL rg_create_weaponbox(AMX* amx, cell* params)
{
	enum args_e { arg_count, arg_item, arg_player, arg_modelname, arg_origin, arg_angles, arg_velocity, arg_lifetime, arg_packammo };

	CHECK_ISENTITY(arg_item);

	CBasePlayerItem *pItem = getPrivate<CBasePlayerItem>(params[arg_item]);
	if (unlikely(pItem == nullptr))
	{
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: invalid or uninitialized entity", __FUNCTION__);
		return FALSE;
	}

	CBasePlayer *pPlayer = nullptr;

	if (params[arg_player] != 0)
	{
		CHECK_ISPLAYER(arg_player);

		pPlayer = getPrivate<CBasePlayer>(params[arg_player]);
		CHECK_CONNECTED(pPlayer, arg_player);
	}

	CAmxArgs args(amx, params);

	char modelStr[MAX_PATH];
	const char *modelName = getAmxString(amx, params[arg_modelname], modelStr);
	CWeaponBox *pBox = g_ReGameFuncs->CreateWeaponBox(pItem, pPlayer, modelName, args[arg_origin], args[arg_angles], args[arg_velocity], args[arg_lifetime], args[arg_packammo]);

	if (pBox)
		return indexOfPDataAmx(pBox);

	return AMX_NULLENT;
}

/*
* Removes an entity using gamedll's UTIL_Remove function, which sets a frame delay to ensure its removal.
*
* @param pEntity               Entity index to remove
*
* @return                      1 on success, 0 otherwise
*/
cell AMX_NATIVE_CALL rg_remove_entity(AMX* amx, cell* params)
{
	enum args_e { arg_count, arg_entity };

	CHECK_ISENTITY(arg_entity);

	auto pEntity = getPrivate<CBaseEntity>(params[arg_entity]);
	if (!pEntity || (pEntity->pev->flags & FL_KILLME) != 0)
	{
		return FALSE;
	}

	g_ReGameFuncs->UTIL_Remove(pEntity);

	if ((pEntity->pev->flags & FL_KILLME) != 0)
	{
		return TRUE;
	}

	return FALSE;
}

/*
* Creates a Decal in world based on a traceresult.
*
* @param ptr                   Traceresult pointer, use Fakemeta's create_tr2 to instantiate one
* @param decalNumber           Number of decal to spray, see DECAL_ constants on cssdk_const.inc
*
* @noreturn
*/
cell AMX_NATIVE_CALL rg_decal_trace(AMX* amx, cell* params)
{
	enum args_e { arg_count, arg_trace, arg_decal };

	CAmxArgs args(amx, params);
	g_ReGameFuncs->UTIL_DecalTrace(args[arg_trace], args[arg_decal]);
	return TRUE;
}

/*
* Emits a sound based on a traceresult simulating a bullet hit (metal, wood, concrete, etc.).
* @note Used mostly on trace attacks (bullets, knife).
*
* @param ptr                   Traceresult pointer, use Fakemeta's create_tr2 to instantiate one
* @param vecSrc                Start position
* @param vecEnd                End position, must match ptr's vecEndPos member
* @param iBulletType           Bullet type, see BULLET_* constants in cssdk_const.inc
*
* @noreturn
*/
cell AMX_NATIVE_CALL rg_emit_texture_sound(AMX* amx, cell* params)
{
	enum args_e { arg_count, arg_trace, arg_start, arg_end, arg_bullet_type };

	CAmxArgs args(amx, params);
	g_ReGameFuncs->TextureTypePlaySound(args[arg_trace], args[arg_start], args[arg_end], args[arg_bullet_type]);
	return TRUE;
}

/*
* Generates an ammo slot in game's logic
* @note To see a visual effect, WeaponList message should be sent using the custom ammo name,
*       where ammo icon HUD will be the one listed in "sprites/weapon_<name>.txt" file.
*
* @param szAmmoname            Ammo name to create.
*
* @note Maximum ammo index is 31, after that every ammo instantiation will start from 1 overriding existing ones.
* @return                      New ammo index. If name already exists, will return the matched index from memory.
*/
cell AMX_NATIVE_CALL rg_add_ammo_registry(AMX* amx, cell* params)
{
	enum args_e { arg_count, arg_ammoname };

	char ammonamebuf[190];
	string_t ammoname = getAmxStringAlloc(amx, params[arg_ammoname], ammonamebuf);

	if (!ammonamebuf || ammonamebuf[0] == '\0')
	{
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: empty ammo name", __FUNCTION__);
		return FALSE;
	}

	return (cell)g_ReGameFuncs->AddAmmoNameToAmmoRegistry(STRING(ammoname));
}

/*
* Deploys a weapon attached to a player using the CBasePlayerWeapon::DefaultDeploy function.
*
* @param entity                Weapon to deploy. Must be attached to a player.
* @param szViewModel           Weapon view model name ("models/v_*.mdl")
* @param szWeaponModel         Weapon world model bame ("models/p_*.mdl")
* @param iAnim                 Weapon view model animation to play (often "deploy", use HLMV to see anim index)
* @param szAnimExt             Player anim extension name to assign. Examples: "carbine", "shotgun", "knife", etc.
*                              Use HLMV on a player model to see animext names.
* @param skiplocal             If 0, weapon animation will be forced to play on client ignoring active client prediction.
*
* @return                      1 on successful weapon deploy, 0 otherwise.
*/
cell AMX_NATIVE_CALL rg_weapon_deploy(AMX* amx, cell* params)
{
	enum args_e { arg_count, arg_weapon, arg_viewmodel, arg_weaponmodel, arg_anim, arg_animextension, arg_skiplocal };

	CHECK_ISENTITY(arg_weapon);
	CBasePlayerWeapon *pWeapon = getPrivate<CBasePlayerWeapon>(params[arg_weapon]);

	if (unlikely(pWeapon == nullptr))
	{
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: invalid or uninitialized entity", __FUNCTION__);
		return FALSE;
	}

	CHECK_INSTANCE_OF(CBasePlayerWeapon, pWeapon);

	if (!pWeapon->IsWeapon())
	{
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: #%d entity is not a weapon.", __FUNCTION__, indexOfEdict(pWeapon->pev));
		return FALSE;
	}

	CCSPlayerWeapon *pCSWeapon = pWeapon->CSPlayerWeapon();
	if (unlikely(pCSWeapon == nullptr))
	{
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: invalid or uninitialized m_pEntity.", __FUNCTION__);
		return FALSE;
	}

	char viewmodelbuffer[MAX_PATH], weaponmodelbuffer[MAX_PATH], animextbuffer[32];
	getAmxString(amx, params[arg_viewmodel], viewmodelbuffer);
	getAmxString(amx, params[arg_weaponmodel], weaponmodelbuffer);
	getAmxString(amx, params[arg_animextension], animextbuffer);

	return pCSWeapon->DefaultDeploy(viewmodelbuffer, weaponmodelbuffer, (int)params[arg_anim], animextbuffer, (int)params[arg_skiplocal]) ? TRUE : FALSE;
}

/*
* Reloads a weapon or a player's active weapon using the CBasePlayerWeapon::DefaultReload function.
*
* @param entity                Weapon to reload (> MaxPlayers) OR player index to reload his current active weapon (>= 1 & <= MaxPlayers).
* @param iClipSize             Weapon max clip to check. 0 = weapon max clip stored in ItemInfo
* @param iAnim                 Weapon view model animation to play (often "reload", use HLMV to see anim index)
* @param fDelay                Player reload duration before clip refill.
*
* @return                      1 on successful weapon reload, 0 otherwise.
*/
cell AMX_NATIVE_CALL rg_weapon_reload(AMX* amx, cell* params)
{
	enum args_e { arg_count, arg_weapon, arg_clipsize, arg_anim, arg_delay };

	CBasePlayerWeapon *pWeapon;

	if (params[arg_weapon] > 0 && params[arg_weapon] <= gpGlobals->maxClients)
	{
		CHECK_ISPLAYER(arg_weapon);
		CBasePlayer *pPlayer = UTIL_PlayerByIndex(params[arg_weapon]);
		CHECK_CONNECTED(pPlayer, arg_weapon);

		if (!pPlayer->IsAlive())
		{
			AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: player %d not alive", __FUNCTION__, params[arg_weapon]);
			return FALSE;
		}

		pWeapon = static_cast<CBasePlayerWeapon *>(pPlayer->m_pActiveItem);
	}
	else
	{
		CHECK_ISENTITY(arg_weapon);

		pWeapon = getPrivate<CBasePlayerWeapon>(params[arg_weapon]);
	}

	if (unlikely(pWeapon == nullptr))
	{
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: invalid or uninitialized entity", __FUNCTION__);
		return FALSE;
	}

	CHECK_INSTANCE_OF(CBasePlayerWeapon, pWeapon);

	if (!pWeapon->IsWeapon())
	{
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: #%d entity is not a weapon.", __FUNCTION__, indexOfEdict(pWeapon->pev));
		return FALSE;
	}

	CCSPlayerWeapon *pCSWeapon = pWeapon->CSPlayerWeapon();
	if (unlikely(pCSWeapon == nullptr))
	{
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: invalid or uninitialized m_pEntity.", __FUNCTION__);
		return FALSE;
	}

	CAmxArgs args(amx, params);

	int clipsize = args[arg_clipsize];

	if (clipsize <= 0)
		clipsize = pWeapon->CSPlayerItem()->m_ItemInfo.iMaxClip;

	return pCSWeapon->DefaultReload(clipsize, args[arg_anim], args[arg_delay]) ? TRUE : FALSE;
}

/*
* Forces shotgun reload thinking on a weapon or a player's active weapon using the CBasePlayerWeapon::DefaultShotgunReload function.
*
* @param entity                Weapon to reload (> MaxClients) OR player index to reload his current active weapon (>= 1 & <= MaxClients).
* @param iAnim                 Weapon view model "insert" animation to play (use HLMV to see anim index)
* @param iStartAnim            Weapon view model "start reload" animation to play (use HLMV to see anim index)
* @param fDelay                Delay between each buckshot inserted
* @param fStartDelay           Delay before buckshots insertion starts
* @param pszReloadSound1       Sound to play on every insertion
* @param pszReloadSound2       Another sound to play on every insertion
*
* @note This is used inside weapon's Reload function and is often called every frame player is pressing IN_RELOAD button.
* @return                      1 while weapon not in delay and with ammo remaining to load, 0 otherwise.
*/
cell AMX_NATIVE_CALL rg_weapon_shotgun_reload(AMX* amx, cell* params)
{
	enum args_e { arg_count, arg_weapon, arg_anim, arg_startanim, arg_delay, arg_startdelay, arg_reloadsound1, arg_reloadsound2 };

	CBasePlayerWeapon *pWeapon;

	if (params[arg_weapon] > 0 && params[arg_weapon] <= gpGlobals->maxClients)
	{
		CHECK_ISPLAYER(arg_weapon);
		CBasePlayer *pPlayer = UTIL_PlayerByIndex(params[arg_weapon]);
		CHECK_CONNECTED(pPlayer, arg_weapon);

		if (!pPlayer->IsAlive())
		{
			AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: player %d not alive", __FUNCTION__, params[arg_weapon]);
			return FALSE;
		}

		pWeapon = static_cast<CBasePlayerWeapon *>(pPlayer->m_pActiveItem);
	}
	else
	{
		CHECK_ISENTITY(arg_weapon);

		pWeapon = getPrivate<CBasePlayerWeapon>(params[arg_weapon]);
	}

	if (unlikely(pWeapon == nullptr))
	{
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: invalid or uninitialized entity", __FUNCTION__);
		return FALSE;
	}

	CHECK_INSTANCE_OF(CBasePlayerWeapon, pWeapon);

	if (!pWeapon->IsWeapon())
	{
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: #%d entity is not a weapon.", __FUNCTION__, indexOfEdict(pWeapon->pev));
		return FALSE;
	}

	CCSPlayerWeapon *pCSWeapon = pWeapon->CSPlayerWeapon();
	if (unlikely(pCSWeapon == nullptr))
	{
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: invalid or uninitialized m_pEntity.", __FUNCTION__);
		return FALSE;
	}

	CAmxArgs args(amx, params);

	char sound1buffer[MAX_PATH], sound2buffer[MAX_PATH];
	const char *reloadsound1 = getAmxString(amx, params[arg_reloadsound1], sound1buffer);
	const char *reloadsound2 = getAmxString(amx, params[arg_reloadsound2], sound2buffer);

	return pCSWeapon->DefaultShotgunReload(args[arg_anim], args[arg_startanim], args[arg_delay], args[arg_startdelay], reloadsound1, reloadsound2) ? TRUE : FALSE;
}

/*
* Sends a weapon animation using the CBasePlayerWeapon::SendWeaponAnim function.
*
* @param entity                Weapon to send animation on owner (> MaxClients) OR player index to send animation (>= 1 & <= MaxClients).
* @param iAnim                 Weapon view model animation to play (use HLMV to see anim index)
* @param skiplocal             If 0, weapon animation will be forced to play on client ignoring active client prediction.
*
* @noreturn
*/
cell AMX_NATIVE_CALL rg_weapon_send_animation(AMX* amx, cell* params)
{
	enum args_e { arg_count, arg_weapon, arg_anim, arg_skiplocal };

	CBasePlayerWeapon *pWeapon;

	if (params[arg_weapon] > 0 && params[arg_weapon] <= gpGlobals->maxClients)
	{
		CHECK_ISPLAYER(arg_weapon);
		CBasePlayer *pPlayer = UTIL_PlayerByIndex(params[arg_weapon]);
		CHECK_CONNECTED(pPlayer, arg_weapon);

		if (!pPlayer->IsAlive())
		{
			AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: player %d not alive", __FUNCTION__, params[arg_weapon]);
			return FALSE;
		}

		pWeapon = static_cast<CBasePlayerWeapon *>(pPlayer->m_pActiveItem);
	}
	else
	{
		CHECK_ISENTITY(arg_weapon);

		pWeapon = getPrivate<CBasePlayerWeapon>(params[arg_weapon]);
	}

	if (unlikely(pWeapon == nullptr))
	{
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: invalid or uninitialized entity", __FUNCTION__);
		return FALSE;
	}

	CHECK_INSTANCE_OF(CBasePlayerWeapon, pWeapon);

	if (!pWeapon->IsWeapon())
	{
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: #%d entity is not a weapon.", __FUNCTION__, indexOfEdict(pWeapon->pev));
		return FALSE;
	}

	CCSPlayerWeapon *pCSWeapon = pWeapon->CSPlayerWeapon();
	if (unlikely(pCSWeapon == nullptr))
	{
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: invalid or uninitialized m_pEntity.", __FUNCTION__);
		return FALSE;
	}

	CAmxArgs args(amx, params);

	pCSWeapon->SendWeaponAnim(args[arg_anim], args[arg_skiplocal]);
	return TRUE;
}

/*
* Emits a "recoil" effect on weapon's player using the CBasePlayerWeapon::KickBack function.
*
* @param entity                Weapon to reload (> MaxClients) OR player index to reload his current active weapon (>= 1 & <= MaxClients).
* @param up_base               Minimum vertical punchangle
* @param lateral_base          Minimum horizontal punchangle
* @param up_modifier           Vertical punchangle units to multiply to m_iShotsFired member
* @param lateral_modifier      Horizontal punchangle units to multiply to m_iShotsFired member
* @param up_max                Maximum vertical punchangle
* @param lateral_max           Maximum horizontal punchangle
* @param direction_change      Probability to change punchangle orientation (positive or negative). 0 = 100% (1/1), 1 = 50% (1/2), 2 = 33.3% (1/3), ...
*
* @noreturn
*/
cell AMX_NATIVE_CALL rg_weapon_kickback(AMX* amx, cell* params)
{
	enum args_e { arg_count, arg_weapon, arg_up_base, arg_lateral_base, arg_up_modifier, arg_lateral_modifier, arg_up_max, arg_lateral_max, arg_direction_change };

	CBasePlayerWeapon *pWeapon;

	if (params[arg_weapon] > 0 && params[arg_weapon] <= gpGlobals->maxClients)
	{
		CHECK_ISPLAYER(arg_weapon);
		CBasePlayer *pPlayer = UTIL_PlayerByIndex(params[arg_weapon]);
		CHECK_CONNECTED(pPlayer, arg_weapon);

		if (!pPlayer->IsAlive())
		{
			AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: player %d not alive", __FUNCTION__, params[arg_weapon]);
			return FALSE;
		}

		pWeapon = static_cast<CBasePlayerWeapon *>(pPlayer->m_pActiveItem);
	}
	else
	{
		CHECK_ISENTITY(arg_weapon);

		pWeapon = getPrivate<CBasePlayerWeapon>(params[arg_weapon]);
	}

	if (unlikely(pWeapon == nullptr)) {
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: invalid or uninitialized entity", __FUNCTION__);
		return FALSE;
	}

	CHECK_INSTANCE_OF(CBasePlayerWeapon, pWeapon);

	if (!pWeapon->IsWeapon())
	{
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: #%d entity is not a weapon.", __FUNCTION__, indexOfEdict(pWeapon->pev));
		return FALSE;
	}

	CCSPlayerWeapon *pCSWeapon = pWeapon->CSPlayerWeapon();
	if (unlikely(pCSWeapon == nullptr))
	{
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: invalid or uninitialized m_pEntity.", __FUNCTION__);
		return FALSE;
	}

	CAmxArgs args(amx, params);

	pCSWeapon->KickBack(args[arg_up_base], args[arg_lateral_base], args[arg_up_modifier], args[arg_lateral_modifier], args[arg_up_max], args[arg_lateral_max], args[arg_direction_change]);
	return TRUE;
}

/*
* Switches player current weapon into the best one on its inventory using the CHalfLifeMultiplay::GetNextBestWeapon function.
*
* @param player                Player index.
* @param currentWeapon         Current player active weapon. 0 = retrieve from m_pActiveItem member
*
* @note Weapon selection is based on weapon's Weight attribute from ItemInfo structure.
* @return                      1 if weapon was found and switched to, 0 otherwise
*/
cell AMX_NATIVE_CALL rg_switch_best_weapon(AMX* amx, cell* params)
{
	enum args_e { arg_count, arg_index, arg_weapon };

	CHECK_GAMERULES();

	CHECK_ISPLAYER(arg_index);

	CBasePlayer *pPlayer = UTIL_PlayerByIndex(params[arg_index]);
	CHECK_CONNECTED(pPlayer, arg_index);

	if (!pPlayer->IsAlive())
		return FALSE;

	CBasePlayerWeapon *pWeapon;

	if (params[arg_weapon] != 0)
	{
		CHECK_ISENTITY(arg_weapon);

		pWeapon = getPrivate<CBasePlayerWeapon>(params[arg_weapon]);

		if (unlikely(pWeapon == nullptr)) {
			AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: invalid or uninitialized entity", __FUNCTION__);
			return FALSE;
		}
	}
	else
	{
		pWeapon = static_cast<CBasePlayerWeapon *>(pPlayer->m_pActiveItem);

		if (unlikely(pWeapon == nullptr)) {
			AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: player %d has invalid m_pActiveItem", __FUNCTION__, params[arg_index]);
			return FALSE;
		}
	}

	CHECK_INSTANCE_OF(CBasePlayerWeapon, pWeapon);

	if (!pWeapon->IsWeapon()) {
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: #%d entity is not a weapon.", __FUNCTION__, indexOfEdict(pWeapon->pev));
		return FALSE;
	}

	return CSGameRules()->GetNextBestWeapon(pPlayer, pWeapon);
}

/*
* Disappear a player from the world. Used when VIP reaches escape zone. Basically a silent kill.
*
* @param player                Player index.
*
* @noreturn
*/
cell AMX_NATIVE_CALL rg_disappear(AMX* amx, cell* params)
{
	enum args_e { arg_count, arg_index };

	CHECK_ISPLAYER(arg_index)

	CBasePlayer *pPlayer = UTIL_PlayerByIndex(params[arg_index]);
	CHECK_CONNECTED(pPlayer, arg_index);

	pPlayer->CSPlayer()->Disappear();
	return TRUE;
}

/*
* Sets player current Observer mode.
* @note Player must be a valid observer (m_afPhysicsFlags & PFLAG_OBSERVER).
*
* @param player                Player index.
* @param mode                  Observer mode, see OBS_* constants in cssdk_const.inc
*
* @noreturn
*/
cell AMX_NATIVE_CALL rg_set_observer_mode(AMX* amx, cell* params)
{
	enum args_e { arg_count, arg_index, arg_mode };

	CHECK_ISPLAYER(arg_index)

	CBasePlayer *pPlayer = UTIL_PlayerByIndex(params[arg_index]);
	CHECK_CONNECTED(pPlayer, arg_index);

	pPlayer->CSPlayer()->Observer_SetMode((int)params[arg_mode]);
	return TRUE;
}

/*
* Call origin function CBasePlayer::Observer_FindNextPlayer()
* @note Player must be a valid observer (m_afPhysicsFlags & PFLAG_OBSERVER).
*
* @param player                Player index.
* @param bReverse              If bReverse is true, finding order will be reversed
* @param name                  Player name to find.
*
* @noreturn
*/
cell AMX_NATIVE_CALL rg_observer_find_next_player(AMX* amx, cell* params)
{
	enum args_e { arg_count, arg_index, arg_bReverse, arg_name };

	CHECK_ISPLAYER(arg_index)

	CBasePlayer *pPlayer = UTIL_PlayerByIndex(params[arg_index]);
	CHECK_CONNECTED(pPlayer, arg_index);

	char nameBuf[MAX_PLAYER_NAME_LENGTH];
	const char* name = getAmxString(amx, params[arg_name], nameBuf);
	if (strcmp(name, "") == 0)
		name = nullptr;

	pPlayer->CSPlayer()->Observer_FindNextPlayer(params[arg_bReverse] != 0, name);
	return TRUE;
}

/*
* Emits a death notice (logs, DeathMsg event, win conditions check)
*
* @param pVictim               Player index.
* @param pKiller               Killer entity.
* @param pevInflictor          Inflictor entity. 0 = world
*
* @noreturn
*/
cell AMX_NATIVE_CALL rg_death_notice(AMX* amx, cell* params)
{
	enum args_e { arg_count, arg_victim, arg_killer, arg_inflictor };

	CHECK_GAMERULES();

	CHECK_ISPLAYER(arg_victim)

	CBasePlayer *pPlayer = UTIL_PlayerByIndex(params[arg_victim]);
	CHECK_CONNECTED(pPlayer, arg_victim);

	CHECK_ISENTITY(arg_killer)

	CAmxArgs args(amx, params);
	CSGameRules()->DeathNotice(pPlayer, args[arg_killer], args[arg_inflictor]);
	return TRUE;
}

/*
* Checks a player relationship with another reference
*
* @param player     Player index
* @param target     Target index
*
* @return           Match player relationship, see GR_* constants in cssdk_const.inc
*/
cell AMX_NATIVE_CALL rg_player_relationship(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_player, arg_target };

	CHECK_GAMERULES();
	CHECK_ISPLAYER(arg_player);
	CHECK_ISENTITY(arg_target);

	CBasePlayer *pPlayer = UTIL_PlayerByIndex(params[arg_player]);
	CHECK_CONNECTED(pPlayer, arg_player);

	CBaseEntity *pTarget = getPrivate<CBaseEntity>(params[arg_target]);

	return CSGameRules()->PlayerRelationship(pPlayer, pTarget);
}

/*
* Sends death messages to all players, including info about the killer, victim, weapon used,
* extra death flags, death position, assistant, and kill rarity using the CHalfLifeMultiplay::SendDeathMessage function.
*
* @param pKiller                The entity who performed the kill (Note: The killer may be a non-player)
* @param pVictim                The player who was killed
* @param pAssister              The assisting player (if any)
* @param pevInflictor           Inflictor entity. 0 = world
* @param killerWeaponName       The name of the weapon used by the killer
* @param iDeathMessageFlags     Flags indicating extra death message info
* @param iRarityOfKill          An bitsums representing the rarity classification of the kill
*
* @noreturn
*/
cell AMX_NATIVE_CALL rg_send_death_message(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_killer, arg_victim, arg_assister, arg_inflictor, arg_weaponname, arg_deathmsgflags, arg_rarityofkill };

	CHECK_GAMERULES();

	CHECK_ISPLAYER(arg_victim);
	CBasePlayer *pVictim = UTIL_PlayerByIndex(params[arg_victim]);
	CHECK_CONNECTED(pVictim, arg_victim);

	CBasePlayer *pKiller = nullptr;
	CBasePlayer *pAssister = nullptr;

	// Check if the killer is a player
	if (params[arg_killer])
	{
		CHECK_ISPLAYER(arg_killer);
		pKiller = UTIL_PlayerByIndex(params[arg_killer]);
		CHECK_CONNECTED(pKiller, arg_killer);
	}

	// Check if the assister is a player
	if (params[arg_assister])
	{
		CHECK_ISPLAYER(arg_assister);
		pAssister = UTIL_PlayerByIndex(params[arg_assister]);
		CHECK_CONNECTED(pAssister, arg_assister);
	}

	CAmxArgs args(amx, params);

	char weaponStr[32];
	const char *weaponName = getAmxString(amx, params[arg_weaponname], weaponStr);

	CSGameRules()->SendDeathMessage(pKiller, pVictim, pAssister, args[arg_inflictor], weaponName, args[arg_deathmsgflags], args[arg_rarityofkill]);
	return TRUE;
}

/*
* Adds impulse to the player.
*
* @param player                 Player index.
* @param attacker               Attacker index.
* @param flKnockbackForce       Knockback force.
* @param flVelModifier          Velocity modifier.
*
* @noreturn
*/
cell AMX_NATIVE_CALL rg_player_takedamage_impulse(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index, arg_attacker, arg_knockback_force, arg_vel_modifier };

	CHECK_ISPLAYER(arg_index);
	CHECK_ISPLAYER(arg_attacker);

	CBasePlayer *pPlayer = UTIL_PlayerByIndex(params[arg_index]);
	CHECK_CONNECTED(pPlayer, arg_index);

	if (!pPlayer->IsAlive())
	{
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: player %d not alive", __FUNCTION__, params[arg_index]);
		return FALSE;
	}

	CBasePlayer *pAttacker = UTIL_PlayerByIndex(params[arg_attacker]);
	CHECK_CONNECTED(pAttacker, arg_attacker);

	if (!pAttacker->IsAlive())
	{
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: attacker %d not alive", __FUNCTION__, params[arg_attacker]);
		return FALSE;
	}

	CAmxArgs args(amx, params);
	pPlayer->CSPlayer()->TakeDamageImpulse(pAttacker, args[arg_knockback_force], args[arg_vel_modifier]);

	return TRUE;
}

AMX_NATIVE_INFO Misc_Natives_RG[] =
{
	{ "rg_set_animation",             rg_set_animation             },
	{ "rg_add_account",               rg_add_account               },
	{ "rg_give_item",                 rg_give_item                 },
	{ "rg_give_custom_item",          rg_give_custom_item          },
	{ "rg_give_default_items",        rg_give_default_items        },
	{ "rg_give_shield",               rg_give_shield               },

	{ "rg_dmg_radius",                rg_dmg_radius                },
	{ "rg_multidmg_clear",            rg_multidmg_clear            },
	{ "rg_multidmg_apply",            rg_multidmg_apply            },
	{ "rg_multidmg_add",              rg_multidmg_add              },

	{ "rg_fire_bullets",              rg_fire_bullets              },
	{ "rg_fire_buckshots",            rg_fire_buckshots            },
	{ "rg_fire_bullets3",             rg_fire_bullets3             },

	{ "rg_round_end",                 rg_round_end                 },
	{ "rg_update_teamscores",         rg_update_teamscores         },

	{ "rg_create_entity",             rg_create_entity             },
	{ "rg_find_ent_by_class",         rg_find_ent_by_class         },
	{ "rg_find_ent_by_owner",         rg_find_ent_by_owner         },
	{ "rg_find_weapon_bpack_by_name", rg_find_weapon_bpack_by_name },
	{ "rg_has_item_by_name",          rg_has_item_by_name          },

	{ "rg_get_weapon_info",           rg_get_weapon_info           },
	{ "rg_set_weapon_info",           rg_set_weapon_info           },

	{ "rg_remove_items_by_slot",      rg_remove_items_by_slot      },
	{ "rg_drop_items_by_slot",        rg_drop_items_by_slot        },

	{ "rg_remove_all_items",          rg_remove_all_items          },
	{ "rg_remove_item",               rg_remove_item               },
	{ "rg_drop_item",                 rg_drop_item                 },
	{ "rg_internal_cmd",              rg_internal_cmd              },

	{ "rg_give_defusekit",            rg_give_defusekit            },

	{ "rg_set_user_bpammo",           rg_set_user_bpammo           },
	{ "rg_get_user_bpammo",           rg_get_user_bpammo           },
	{ "rg_set_user_ammo",             rg_set_user_ammo             },
	{ "rg_get_user_ammo",             rg_get_user_ammo             },

	{ "rg_get_user_armor",            rg_get_user_armor            },
	{ "rg_set_user_armor",            rg_set_user_armor            },
	{ "rg_set_user_team",             rg_set_user_team             },
	{ "rg_set_user_model",            rg_set_user_model            },
	{ "rg_reset_user_model",          rg_reset_user_model          },

	{ "rg_set_user_footsteps",        rg_set_user_footsteps        },
	{ "rg_get_user_footsteps",        rg_get_user_footsteps        },

	{ "rg_transfer_c4",               rg_transfer_c4               },
	{ "rg_instant_reload_weapons",    rg_instant_reload_weapons    },
	{ "rg_plant_bomb",                rg_plant_bomb                },

	{ "rg_set_account_rules",         rg_set_account_rules         },
	{ "rg_get_account_rules",         rg_get_account_rules         },

	{ "rg_is_bomb_planted",           rg_is_bomb_planted           },
	{ "rg_join_team",                 rg_join_team                 },
	{ "rg_balance_teams",             rg_balance_teams             },
	{ "rg_swap_all_players",          rg_swap_all_players          },
	{ "rg_switch_team",               rg_switch_team               },
	{ "rg_switch_weapon",             rg_switch_weapon             },
	{ "rg_get_join_team_priority",    rg_get_join_team_priority    },
	{ "rg_is_player_can_takedamage",  rg_is_player_can_takedamage  },
	{ "rg_is_player_can_respawn",     rg_is_player_can_respawn     },
	{ "rg_get_weaponbox_id",          rg_get_weaponbox_id          },
	{ "rg_round_respawn",             rg_round_respawn             },
	{ "rg_reset_maxspeed",            rg_reset_maxspeed            },

	{ "rg_send_bartime",              rg_send_bartime              },
	{ "rg_send_bartime2",             rg_send_bartime2             },
	{ "rg_send_audio",                rg_send_audio                },

	{ "rg_set_iteminfo",              rg_set_iteminfo              },
	{ "rg_get_iteminfo",              rg_get_iteminfo              },
	{ "rg_set_global_iteminfo",       rg_set_global_iteminfo       },
	{ "rg_get_global_iteminfo",       rg_get_global_iteminfo       },

	{ "rg_hint_message",              rg_hint_message              },

	{ "rg_restart_round",             rg_restart_round             },
	{ "rg_check_win_conditions",      rg_check_win_conditions      },
	{ "rg_initialize_player_counts",  rg_initialize_player_counts  },

	{ "rg_reset_can_hear_player",     rg_reset_can_hear_player     },
	{ "rg_set_can_hear_player",       rg_set_can_hear_player       },
	{ "rg_get_can_hear_player",       rg_get_can_hear_player       },

	{ "rg_spawn_head_gib",            rg_spawn_head_gib            },
	{ "rg_spawn_random_gibs",         rg_spawn_random_gibs         },

	{ "rg_spawn_grenade",             rg_spawn_grenade             },
	{ "rg_create_weaponbox",          rg_create_weaponbox          },
	{ "rg_remove_entity",             rg_remove_entity             },
	{ "rg_decal_trace",               rg_decal_trace               },
	{ "rg_emit_texture_sound",        rg_emit_texture_sound        },
	{ "rg_add_ammo_registry",         rg_add_ammo_registry         },
	{ "rg_weapon_deploy",             rg_weapon_deploy             },
	{ "rg_weapon_reload",             rg_weapon_reload             },
	{ "rg_weapon_shotgun_reload",     rg_weapon_shotgun_reload     },
	{ "rg_weapon_send_animation",     rg_weapon_send_animation     },
	{ "rg_weapon_kickback",           rg_weapon_kickback           },
	{ "rg_switch_best_weapon",        rg_switch_best_weapon        },
	{ "rg_disappear",                 rg_disappear                 },
	{ "rg_set_observer_mode",         rg_set_observer_mode         },
	{ "rg_observer_find_next_player", rg_observer_find_next_player },
	{ "rg_death_notice",              rg_death_notice              },
	{ "rg_player_relationship",       rg_player_relationship       },

	{ "rg_send_death_message",        rg_send_death_message        },
	{ "rg_player_takedamage_impulse", rg_player_takedamage_impulse },

	{ nullptr, nullptr }
};

/*
* Sets the name of the map.
*
* @param mapname    New map name.
*
* @noreturn
*
* native rh_set_mapname(const mapname[]);
*/
cell AMX_NATIVE_CALL rh_set_mapname(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_mapname };

	char mapbuf[256];
	const char *mapname = getAmxString(amx, params[arg_mapname], mapbuf);
	g_RehldsData->SetName(mapname);
	g_pFunctionTable->pfnResetGlobalState = ResetGlobalState;
	return TRUE;
}

enum MapNameType { MNT_TRUE, MNT_SET };

/*
* Gets the name of the map.
*
* @param output     Buffer to copy map name to
* @param len        Maximum buffer size
* @param type       MNT_SET will return the name of the current map
*                   MNT_TRUE will return the original map name independant of the name set with via rh_set_mapname
*
* @noreturn
*
* native rh_get_mapname(output[], len, MapNameType:type = MNT_SET);
*/
cell AMX_NATIVE_CALL rh_get_mapname(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_output, arg_maxlen, arg_type };

	cell* dest = getAmxAddr(amx, params[arg_output]);

	switch ((MapNameType)params[arg_type])
	{
	case MNT_TRUE:
		setAmxString(dest, g_szMapName, params[arg_maxlen]);
		break;
	case MNT_SET:
		setAmxString(dest, g_RehldsData->GetName(), params[arg_maxlen]);
		break;
	}

	return TRUE;
}

/*
* Reverts back the original map name.
*
* @noreturn
*
* native rh_reset_mapname();
*/
cell AMX_NATIVE_CALL rh_reset_mapname(AMX *amx, cell *params)
{
	g_RehldsData->SetName(g_szMapName);
	g_pFunctionTable->pfnResetGlobalState = nullptr;
	return TRUE;
}

/*
* Emits a sound from an entity from the engine.
*
* @param entity     Entity index or use 0 to emit from worldspawn at the specified position
* @param recipient  Recipient index or use 0 to make all clients hear it
* @param channel    Channel to emit from
* @param sample     Sound file to emit
* @param vol        Volume in percents
* @param attn       Sound attenuation
* @param flags      Emit flags
* @param pitch      Sound pitch
* @param emitFlags  Additional Emit2 flags, look at the defines like SND_EMIT2_*
* @param origin     Specify origin and only on "param" entity worldspawn that is 0
*
* @return           true if the emission was successfull, false otherwise
*
* native bool:rh_emit_sound2(const entity, const recipient, const channel, const sample[], Float:vol = VOL_NORM, Float:attn = ATTN_NORM, const flags = 0, const pitch = PITCH_NORM, emitFlags = 0, const Float:origin[3] = {0.0,0.0,0.0});
*/
cell AMX_NATIVE_CALL rh_emit_sound2(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_entity, arg_recipient, arg_channel, arg_sample, arg_vol, arg_attn, arg_flags, arg_pitch, arg_emitFlags, arg_origin };

	CBaseEntity *pRecipient = getPrivate<CBaseEntity>(params[arg_recipient]);
	CHECK_CONNECTED(pRecipient, arg_recipient);

	// ignore recipient of non-player
	if (params[arg_recipient] != 0 && pRecipient && !pRecipient->IsNetClient()) {
		return FALSE;
	}

	CAmxArgs args(amx, params);
	char samplebuf[256];
	const char *sample = getAmxString(amx, params[arg_sample], samplebuf);

	return (cell)g_RehldsFuncs->SV_EmitSound2
	(
		args[arg_entity],		// entity
		args[arg_recipient],	// recipient
		args[arg_channel],		// channel
		sample,					// sample
		args[arg_vol],			// volume
		args[arg_attn],			// attn
		args[arg_flags],		// flags
		args[arg_pitch],		// pitch
		args[arg_emitFlags],	// emitFlags
		args[arg_origin]		// pOrigin
	);
}

/*
* Forces an userinfo update
*
* @param index      Client index
*
* @noreturn
*/
cell AMX_NATIVE_CALL rh_update_user_info(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index };

	CHECK_ISPLAYER(arg_index);

	IGameClient *pClient = clientByIndex(params[arg_index]);
	CHECK_APICLIENT_CONNECTED(pClient, arg_index);

	g_RehldsFuncs->SV_UpdateUserInfo(pClient);

	return TRUE;
}

/*
* Kicks a client from server with message
*
* @param index      Client index
* @param message    Message that will be sent to client when it is deleted from server
*
* @noreturn
*
* native rh_drop_client(const index, const message[] = "");
*/
cell AMX_NATIVE_CALL rh_drop_client(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index, arg_msg };

	CHECK_ISPLAYER(arg_index);

	IGameClient *pClient = clientByIndex(params[arg_index]);
	CHECK_APICLIENT_CONNECTED(pClient, arg_index);

	char messagebuf[256];
	g_RehldsFuncs->DropClient(pClient, false, getAmxString(amx, params[arg_msg], messagebuf));
	return TRUE;
}

/*
* -
*
* @param output     Buffer to copy the ip address
* @param len        Maximum buffer size
*
* @noreturn
*
* native rh_get_net_from(output[], len);
*/
cell AMX_NATIVE_CALL rh_get_net_from(AMX* amx, cell* params)
{
	enum args_e { arg_count, arg_output, arg_maxlen };

	cell *dest = getAmxAddr(amx, params[arg_output]);
	const char *addr = NET_AdrToString(*g_RehldsData->GetNetFrom());

	setAmxString(dest, addr, params[arg_maxlen]);

	return TRUE;
}

/*
* Get real game time throughout the entire server lifecycle.
*
* @return           Real game time
*
* native Float:rh_get_realtime();
*/
cell AMX_NATIVE_CALL rh_get_realtime(AMX* amx, cell* params)
{
	enum args_e { arg_count };

	float realtime = static_cast<float>(g_RehldsFuncs->GetRealTime());
	return *(cell *)&realtime;
}

/*
* Returns client's netchan playing time in seconds.
*
* @param index      Client index
*
* @return           Netchan connection time in seconds or 0 if client index is invalid or client is not connected
*
* native rh_get_client_connect_time(const index);
*/
cell AMX_NATIVE_CALL rh_get_client_connect_time(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index };

	CHECK_ISPLAYER(arg_index);

	client_t *pClient = clientOfIndex(params[arg_index]);
	CHECK_CLIENT_CONNECTED(pClient, arg_index);

	return (cell)(g_RehldsFuncs->GetRealTime() - pClient->netchan.connect_time);
}

/*
* Checks if a specific entity is present in the host's outgoing entity table for a given frame,
* indicating it has passed the visibility check (AddToFullPack) and is ready for client transmission.
*
* @param host       Host index for whom we are checking the entity. (Host cannot be a fake client)
* @param entity     Entity index to find in the table of entities for the given frame.
* @param frame      Frame index where to look. Default is -1, which checks the previous frame.
* @note             To check in the current frame, this native should be called at the end of the server frame.
*
* @return           Returns true if the entity is present in the host's outgoing entity table and
*                   ready to be sent to all clients in the given frame, otherwise false.
*
* native bool:rh_is_entity_fullpacked(const host, const entity, const frame = -1);
*/
cell AMX_NATIVE_CALL rh_is_entity_fullpacked(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_host, arg_entity, arg_frame };

	const int SV_UPDATE_BACKUP = (gpGlobals->maxClients == 1) ? SINGLEPLAYER_BACKUP : MULTIPLAYER_BACKUP;
	const int SV_UPDATE_MASK   = (SV_UPDATE_BACKUP - 1);

	CHECK_ISPLAYER(arg_host);

	client_t *pHost = clientOfIndex(params[arg_host]);
	CHECK_CLIENT_CONNECTED(pHost, arg_host);

	if (pHost->fakeclient)
	{
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: Entity checking for fake client (#%d) is invalid. Fake clients do not process entity updates.", __FUNCTION__, params[arg_host]);
		return FALSE;
	}

	int iEntity = params[arg_entity];
	int iFrame = params[arg_frame];

	client_frame_t *frame = &pHost->frames[(pHost->netchan.outgoing_sequence + iFrame) & SV_UPDATE_MASK];
	packet_entities_t *fullpack = &frame->entities;

	for (int i = 0; i < fullpack->num_entities; i++)
	{
		const entity_state_t *es = &fullpack->entities[i];
		if (es->number == iEntity)
			return TRUE; // GOTCHA! The given entity was found in the fullpack of entities
	}

	return FALSE;
}

/*
* Checks if server paused
*
* @return           Returns true if paused, otherwise false.
*
* native bool:rh_is_server_paused();
*/
cell AMX_NATIVE_CALL rh_is_server_paused(AMX *amx, cell *params)
{
	return g_RehldsData->IsPaused() ? TRUE : FALSE;
}

/*
* Set server pause state
*
* @param st    pause state
*
* @noreturn
*
* native rh_set_server_pause(const bool:status);
*/
cell AMX_NATIVE_CALL rh_set_server_pause(AMX *amx, cell *params)
{
	enum { arg_count, arg_status };
	g_RehldsFuncs->SetServerPause(params[arg_status] != 0);
	return TRUE;
}

AMX_NATIVE_INFO Misc_Natives_RH[] =
{
	{ "rh_set_mapname",             rh_set_mapname             },
	{ "rh_get_mapname",             rh_get_mapname             },
	{ "rh_reset_mapname",           rh_reset_mapname           },
	{ "rh_emit_sound2",             rh_emit_sound2             },
	{ "rh_update_user_info",        rh_update_user_info        },
	{ "rh_drop_client",             rh_drop_client             },
	{ "rh_get_net_from",            rh_get_net_from            },
	{ "rh_get_realtime",            rh_get_realtime            },
	{ "rh_is_entity_fullpacked",    rh_is_entity_fullpacked    },
	{ "rh_get_client_connect_time", rh_get_client_connect_time },
	{ "rh_is_server_paused",        rh_is_server_paused        },
	{ "rh_set_server_pause",        rh_set_server_pause        },

	{ nullptr, nullptr }
};

void RegisterNatives_Misc()
{
	if (!api_cfg.hasReGameDLL())
		fillNatives(Misc_Natives_RG, [](AMX *amx, cell *params) -> cell { AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: isn't available", "ReGameDll"); return FALSE; });

	if (!api_cfg.hasReHLDS())
		fillNatives(Misc_Natives_RH, [](AMX *amx, cell *params) -> cell { AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: isn't available", "ReHlds"); return FALSE; });

	g_amxxapi.AddNatives(Misc_Natives_RG);
	g_amxxapi.AddNatives(Misc_Natives_RH);
}
