#if defined _reapi_gamedll_included
	#endinput
#endif

#define _reapi_gamedll_included

#include <reapi_gamedll_const>

/*
* Sets Think callback for entity
*
* @param entity     Entity index
* @param callback   The forward to call
* @param params     Optional set of data to pass through to callback
* @param len        Optional size of data
*
* @note Use "" to reset callback
* @note Callback should be contains passing arguments as "public Think_Callback(const ent)"
*
* @noreturn
*/
native SetThink(const ent, const callback[], const params[] = "", const len = 0);

/*
* Sets Touch callback for entity
*
* @param entity     Entity index
* @param callback   The forward to call
* @param params     Optional set of data to pass through to callback
* @param len        Optional size of data
*
* @note Use "" to reset callback
* @note Callback should be contains passing arguments as "public Touch_Callback(const ent, const other)"
*
* @noreturn
*/
native SetTouch(const ent, const callback[], const params[] = "", const len = 0);

/*
* Sets Use callback for entity
*
* @param entity     Entity index
* @param callback   The forward to call
* @param params     Optional set of data to pass through to callback
* @param len        Optional size of data
*
* @note Use "" to reset callback
* @note Callback should be contains passing arguments as "public Use_Callback(const ent, const activator, const caller, USE_TYPE:useType, Float:value)"
*
* @noreturn
*/
native SetUse(const ent, const callback[], const params[] = "", const len = 0);

/*
* Sets Blocked callback for entity
*
* @param entity     Entity index
* @param callback   The forward to call
* @param params     Optional set of data to pass through to callback
* @param len        Optional size of data
*
* @note Use "" to reset callback
* @note Callback should be contains passing arguments as "public Blocked_Callback(const ent, const other)"
*
* @noreturn
*/
native SetBlocked(const ent, const callback[], const params[] = "", const len = 0);

/*
* Sets MoveDone callback for entity
*
* @param entity     Entity index
* @param callback   The forward to call
* @param params     Optional set of data to pass through to callback
* @param len        Optional size of data
*
* @note Use "" to reset callback
* @note Entity should be inherited from CBaseToggle, otherwise server can crash
* @note Callback should be contains passing arguments as "public MoveDone_Callback(const ent)"
*
* @noreturn
*/
native SetMoveDone(const ent, const callback[], const params[] = "", const len = 0);

/*
* Sets a value to CSGameRules_Members members.
*
* @param member     The specified member, look at the enums with name CSGameRules_Members
*
* @return           1 on success.
*/
native set_member_game(CSGameRules_Members:member, any:...);

/*
* Returns a value from CSGameRules_Members members
*
* @param member     The specified member, look at the enums with name CSGameRules_Members
*
* @return           If an integer or boolean or one byte, array or everything else is passed via the 3rd argument and more, look at the argument list for the specified member
*/
native any:get_member_game(CSGameRules_Members:member, any:...);

/*
* Sets a value to an entity's member.
*
* @param index      Entity index
* @param member     The specified member, look at the enums with name *_Members
*
* @return           1 on success.
*/
native set_member(const index, any:member, any:...);

/*
* Returns a value from an entity's member.
*
* @param index      Entity index
* @param member     The specified member, look at the enums with name *_Members
*
* @return           If an integer or boolean or one byte, array or everything else is passed via the 3rd argument and more, look at the argument list for the specified member
*/
native any:get_member(const index, any:member, any:...);

/*
* Sets a value to an entity's member.
* Safe version, can guarantee that the present member is refers to derived class of the entity.
*
* @param index      Entity index
* @param member     The specified member, look at the enums with name *_Members
*
* @return           1 on success.
*/
native set_member_s(const index, any:member, any:...);

/*
* Returns a value from an entity's member.
* Safe version, can guarantee that the present member is refers to derived class of the entity.
*
* @param index      Entity index
* @param member     The specified member, look at the enums with name *_Members
*
* @return           If an integer or boolean or one byte, array or everything else is passed via the 3rd argument and more, look at the argument list for the specified member
*/
native any:get_member_s(const index, any:member, any:...);

/*
* Sets playermove var.
*
* @param var        The specified playermove, look at the enum PlayerMove
*
* @return           1 on success.
*/
native set_pmove(const PlayerMove:var, any:...);

/*
* Returns a playermove var.
*
* @param var        The specified playermove var, look at the enums PlayerMove
*
* @return           If an integer or boolean or one byte, array or everything else is passed via the 3rd argument and more, look at the argument list for the specified mvar
*/
native any:get_pmove(const PlayerMove:var, any:...);

/*
* Sets a movevar value to a playermove.
*
* @param var        The specified mvar, look at the enum MoveVars
*
* @return           1 on success.
*/
native set_movevar(const MoveVars:var, any:...);

/*
* Returns a movevar value from a playermove.
*
* @param var        The specified mvar, look at the enum MoveVars
*
* @return           If an integer or boolean or one byte, array or everything else is passed via the 3rd argument and more, look at the argument list for the specified mvar
*/
native any:get_movevar(const MoveVars:var, any:...);

/*
* Sets a pmtrace var.
*
* @param var        The specified mvar, look at the enum PMTrace
*
* @return           1 on success.
*/
native set_pmtrace(const tracehandle, const PMTrace:var, any:...);

/*
* Returns a pmtrace var
*
* @param var        The specified mvar, look at the enum PMTrace
*
* @return           If an integer or boolean or one byte, array or everything else is passed via the 3rd argument and more, look at the argument list for the specified mvar
*/
native any:get_pmtrace(const tracehandle, const PMTrace:var, any:...);

/*
* Sets a RebuyStruct member.
*
* @param var        The specified RebuyStruct, look at the enum RebuyStruct
*
* @return           1 on success.
*/
native set_rebuy(const RebuyHandle:rebuyhandle, const RebuyStruct:member, any:value);

/*
* Returns a RebuyStruct member
*
* @param var        The specified RebuyStruct, look at the enum RebuyStruct
*
* @return           If an integer or boolean or one byte, array or everything else is passed via the 3rd argument and more, look at the argument list for the specified RebuyStruct
*/
native any:get_rebuy(const RebuyHandle:rebuyhandle, RebuyStruct:member);

/*
* Assign the number of the player's animation.
*
* @param index          Client index
* @param playerAnim     Specific animation number
*
* @noreturn
*/
native rg_set_animation(const index, PLAYER_ANIM:playerAnim);

/*
* Adds money to player's account.
*
* @param index          Client index
* @param amount         The amount of money
* @param bTrackChange   If bTrackChange is 1, the amount of money added will also be displayed.
*
* @noreturn
*/
native rg_add_account(const index, amount, AccountSet:typeSet = AS_ADD, const bool:bTrackChange = true);

/*
* Gives the player an item.
*
* @param index      Client index
* @param pszName    Item classname
* @param type       Look at the enums with name GiveType
*
* @return           Index of entity if successfull, -1 otherwise
*/
native rg_give_item(const index, const pszName[], GiveType:type = GT_APPEND);

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
*/
native rg_give_custom_item(const index, const pszName[], GiveType:type = GT_APPEND, const uid = 0);

/*
* Give the default items to a player.
*
* @param index      Client index
*
* @noreturn
*/
native rg_give_default_items(const index);

/*
* Gives the player a shield
*
* @param index          Client index
* @param bDeploy        To deploy the shield
*
* @noreturn
*/
native rg_give_shield(const index, const bool:bDeploy = true);

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
*/
native rg_dmg_radius(Float:vecSrc[3], const inflictor, const attacker, const Float:flDamage, const Float:flRadius, const iClassIgnore, const bitsDamageType);

/*
* Resets the global multi damage accumulator.
*
* @noreturn
*/
native rg_multidmg_clear();

/*
* Inflicts contents of global multi damage registered on victim.
*
* @param inflictor      Inflictor is the entity that caused the damage (such as a gun)
* @param attacker       Attacker is the entity that triggered the damage (such as the gun's owner)
*
* @noreturn
*/
native rg_multidmg_apply(const inflictor, const attacker);

/*
* Adds damage to the accumulator.
*
* @param inflictor          Inflictor is the entity that caused the damage (such as a gun)
* @param victim             The victim that takes damage
* @param flDamage           The amount of damage
* @param bitsDamageType     Damage type DMG_*
*
* @noreturn
*/
native rg_multidmg_add(const inflictor, const victim, const Float:flDamage, const bitsDamageType);

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
* @note: This native doesn't create a decal effect
* @note: Decal creation is handled by PlaybackEvent, including shot animation and shot sound
*
* @noreturn
*/
native rg_fire_bullets(const inflictor, const attacker, const shots, Float:vecSrc[3], Float:vecDirShooting[3], Float:vecSpread[3], const Float:flDistance, const Bullet:iBulletType, const iTracerFreq, const iDamage);

/*
* Fires buckshots from entity (used at XM1014 and M3 weapons).
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
* @note: This native doesn't create a decal effect
* @note: Decal creation is handled by PlaybackEvent, including shot animation and shot sound
*
* @noreturn
*/
native rg_fire_buckshots(const inflictor, const attacker, const shots, Float:vecSrc[3], Float:vecDirShooting[3], Float:vecSpread[3], const Float:flDistance, const iTracerFreq, const iDamage);

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
* @note: This native doesn't create a decal effect
* @note: Decal creation is handled by PlaybackEvent, including shot animation and shot sound
*
* @return Float:[3]         The spread result
*/
native Float:[3] rg_fire_bullets3(const inflictor, const attacker, Float:vecSrc[3], Float:vecDirShooting[3], const Float:vecSpread, const Float:flDistance, const iPenetration, const Bullet:iBulletType, const iDamage, const Float:flRangeModifier, const bool:bPistol, const shared_rand);

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
* @return           1 on success, 0 otherwise
*/
native rg_round_end(const Float:tmDelay, const WinStatus:st, const ScenarioEventEndRound:event = ROUND_NONE, const message[] = "default", const sentence[] = "default", const bool:trigger = false);

/*
* Updates current scores.
*
* @param iCtsWins   The amount of wins for counter-terrorists
* @param iTsWins    The amount of wins for terrorists
* @param bAdd       Adds the score to the current amount
*
* @noreturn
*/
native rg_update_teamscores(const iCtsWins = 0, const iTsWins = 0, const bool:bAdd = true);

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
*/
native rg_create_entity(const classname[], const bool:useHashTable = false);

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
*/
native rg_find_ent_by_class(start_index, const classname[], const bool:useHashTable = false);

/*
* Finds an entity in the world using Counter-Strike's custom FindEntityByString wrapper, matching by owner.
*
* @param start_index    Entity index to start searching from. AMX_NULLENT (-1) to start from the first entity
* @param classname      Classname to search for
*
* @return               true if found, false otherwise
*/
native bool:rg_find_ent_by_owner(&start_index, const classname[], owner);

/*
* Finds the weapon by name in the player's inventory.
*
* @param index      Client index
* @param weapon     Weapon name
*
* @return           Weapon's entity index, 0 otherwise
*/
native rg_find_weapon_bpack_by_name(const index, const weapon[]);

/*
* Checks if the player has the item.
*
* @param index      Client index
* @param item       Item name
*
* @return           true if he does, false otherwise
*/
native bool:rg_has_item_by_name(const index, const item[]);

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
*/
native any:rg_get_weapon_info(any:...);

/*
* Sets specific weapon info values.
*
* @param weapon_id      Weapon id, see WEAPON_* constants
* @param type           Info type, see WI_* constants
*
* @return               1 on success, 0 otherwise
*/
native rg_set_weapon_info(const {WeaponIdType,_}:weapon_id, WpnInfo:type, any:...);

/*
* Remove all the player's stuff in a specific slot.
*
* @param index          Client index
* @param slot           The slot that will be emptied
* @param removeAmmo     Remove ammunition
*
* @return               1 - successful removal of all items in the slot or the slot is empty
*                       0 - if at least one item failed to remove
*/
native rg_remove_items_by_slot(const index, const InventorySlotType:slot, const bool:removeAmmo = true);

/*
* Drop to floor all the player's stuff by specific slot.
*
* @param index      Client index
* @param slot       Specific slot for remove of each item.
*
* @return           1 - successful drop of all items in the slot or the slot is empty
*                   0 - if at least one item failed to drop
*/
native rg_drop_items_by_slot(const index, const InventorySlotType:slot);

/*
* Remove all of the player's items.
*
* @param index      Client index
* @param removeSuit Remove suit
*
* @return           1 on success, 0 otherwise
*/
native rg_remove_all_items(const index, const bool:removeSuit = false);

/*
* Forces the player to drop the specified item classname.
*
* @param index      Client index
* @param item_name  Item classname, if no name, the active item classname
*
* @return           Entity index of weaponbox, AMX_NULLENT (-1) otherwise
*
*/
native rg_drop_item(const index, const item_name[]);

/*
* Executes a client command on the gamedll side.
*
* @param index      Client index
* @param command    Client command to execute
* @param arg        Optional command arguments
*
* @return           1 on success, 0 otherwise
*/
native rg_internal_cmd(const index, const cmd[], const arg[] = "");

/*
* Removes the specified item classname from the player
*
* @param index      Client index
* @param item_name  Item classname
* @param removeAmmo Remove ammunition
*
* @return           1 if found and remove, 0 otherwise
*/
native rg_remove_item(const index, const item_name[], const bool:removeAmmo = false);

/*
* Sets the amount of ammo in the client's backpack for a specific weapon.
*
* @param index      Client index
* @param weapon     Weapon id
* @param amount     New backpack ammo amount to set
*
* @noreturn
*/
native rg_set_user_bpammo(const index, WeaponIdType:weapon, amount);

/*
* Returns the amount of ammo in the client's backpack for a specific weapon.
*
* @param index      Client index
* @param weapon     Weapon id
*
* @return           Amount of ammo in backpack
*/
native rg_get_user_bpammo(const index, WeaponIdType:weapon);

/*
* Sets the amount of clip ammo for a specific weapon.
*
* @param index      Client index
* @param weapon     Weapon id
* @param amount     New clip ammo amount to set
*
* @noreturn
*/
native rg_set_user_ammo(const index, WeaponIdType:weapon, amount);

/*
* Returns the amount of clip ammo for a specific weapon.
*
* @param index      Client index
* @param weapon     Weapon id
*
* @return           Amount of clip ammo
*/
native rg_get_user_ammo(const index, WeaponIdType:weapon);

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
*/
native rg_give_defusekit(const index, const bool:bDefusekit = true, const Float:color[] = {0.0, 160.0, 0.0}, const icon[] = "defuser", const bool:bFlash = false);

/*
* Returns the client's armor value and retrieves the type of armor.
*
* @param index        Client index
* @param armortype    Variable to store armor type in
*
* @return             Amount of armor, 0 if the client has no armor
*/
native rg_get_user_armor(const index, &ArmorType:armortype = ARMOR_NONE);

/*
* Sets the client's armor value and the type of armor.
*
* @param index          Client index
* @param armorvalue     Amount of armor to set
* @param armortype      Armor type to set
*
* @noreturn
*/
native rg_set_user_armor(const index, armorvalue, ArmorType:armortype);

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
*/
native rg_set_user_team(const index, {TeamName,_}:team, {ModelName,_}:model = MODEL_AUTO, const bool:send_teaminfo = true, const bool:check_win_conditions = false);

/*
* Sets the client's player model.
*
* @param index          Client index
* @param model          Model name
* @param update_index   If true, the modelindex is updated as well
*
* @return               1 on success, 0 otherwise
*/
native rg_set_user_model(const index, const model[], const bool:update_index = false);

/*
* Resets the client's model.
*
* @param index          Client index
* @param update_index   If true, the modelindex is reset as well
*
* @return               1 on success, 0 otherwise
*/
native rg_reset_user_model(const index, const bool:update_index = false);

/*
* Enable/Disable player's footsteps.
*
* @param index      Client index
* @param silent     To enable silent footsteps
*
* @return           1 on success, 0 otherwise
*/
native rg_set_user_footsteps(const index, bool:silent = false);

/*
* Get the current footsteps state of the player.
*
* @param index      Client index
*
* @return           1 if the player has silent footsteps, 0 otherwise
*/
native rg_get_user_footsteps(const index);

/*
* Transfers C4 from one player to another.
*
* @param index      Client index
* @param receiver   Receiver index, if 0 it will transfer to a random player
*
* @return           Index of player entity if successfull, 0 otherwise
*/
native rg_transfer_c4(const index, const receiver = 0);

/*
* Instantly reload client's weapons.
*
* @param index      Client index
* @param weapon     Weapon entity-index, if 0 then all weapons will be reloaded
*
* @return           1 on success, 0 otherwise
*/
native rg_instant_reload_weapons(const index, const weapon = 0);

/*
* Plant a bomb.
*
* @param index      Owner index or 0 for worldspawn.
* @param origin     The origin of the bomb where it will be planted.
* @param angles     The angles of the planted bomb.
*
* @return           Index of bomb entity or AMX_NULLENT (-1) otherwise
*/
native rg_plant_bomb(const index, Float:vecOrigin[3], Float:vecAngles[3] = {0.0,0.0,0.0});

/*
* Sets the amount of reward in the game account for all players.
*
* @param rules_index    Look at the enum with name RewardRules
* @param amount         The money amount
*
* @noreturn
*/
native rg_set_account_rules(const RewardRules:rules_index, const amount);

/*
* Gets the specified reward rule's money amount.
*
* @param rules_index    Look at the enum with name RewardRules
*
* @return               The amount of reward
*
*/
native rg_get_account_rules(const RewardRules:rules_index);

/*
* Checks if the bomb is planted.
*
* @return           true if the bomb is planted, false otherwise
*/
native bool:rg_is_bomb_planted();

/*
* Forces a player to join a team.
*
* @param index      Client index
* @param team       Team id
*
* @return           1 on success, 0 otherwise
*/
native rg_join_team(const index, const TeamName:team);

/*
* Instantly balances the teams.
*
* @noreturn
*/
native rg_balance_teams();

/*
* Swaps players' teams without reseting frags, deaths and wins.
*
* @noreturn
*/
native rg_swap_all_players();

/*
* Instantly switches the player to his opposite team.
* @note Switch from CT to TERRORIST is also opposite.
*
* @param index      Client index
*
* @noreturn
*/
native rg_switch_team(const index);

/*
* Forces the player to switch to a specific weapon.
*
* @param index      Client index
* @param weapon     Weapon entity-index
*
* @return           1 on success, 0 otherwise
*/
native rg_switch_weapon(const index, const weapon);

/*
* Gets which team has a higher join priority.
*
* @return           Returns the Team Name
*/
native TeamName:rg_get_join_team_priority();

/*
* Checks whether the player can take damage from the attacker.
*
* @param index      Client index
* @param attacker   Attacker index
*
* @return           true if player can take damage, false otherwise
*/
native bool:rg_is_player_can_takedamage(const index, const attacker);

/*
* Checks whether the player can respawn.
*
* @param index      Client index
*
* @return           true if player can respawn, false otherwise
*/
native bool:rg_is_player_can_respawn(const index);

/*
* Gets WeaponIdType from weaponbox
*
* @param entity     Weaponbox entity
*
* @return           return enum of WeaponIdType
*/
native WeaponIdType:rg_get_weaponbox_id(const entity);

/*
* Respawn on round for players/bots.
*
* @param index      Client index
*
* @noreturn
*/
native rg_round_respawn(const index);

/*
* Resets player's maxspeed.
*
* @param index      Client index
*
* @noreturn
*/
native rg_reset_maxspeed(const index);

/*
* Draws a HUD progress bar which fills from 0% to 100% for the time duration in seconds.
* @note: Set the duration to 0 to hide the bar.
*
* @param index      Client index
* @param time       Duration
* @param observer   Send for everyone who is observing the player
*
* @noreturn
*/
native rg_send_bartime(const index, const duration, const bool:observer = true);

/*
* Same as BarTime, but StartPercent specifies how much of the bar is (already) filled.
*
* @param index          Client index
* @param time           Duration
* @param startPercent   Start percent
* @param observer       Send for everyone who is observing the player
*
* @noreturn
*/
native rg_send_bartime2(const index, const duration, const Float:startPercent, const bool:observer = true);

/*
* Sends the SendAudio message - plays the specified audio.
*
* @param index      Receiver index or use 0 for everyone
* @param sample     Sound file to play
* @param pitch      Sound pitch
*
* @noreturn
*/
native rg_send_audio(const index, const sample[], const pitch = PITCH_NORM);

/**
* Sets a parameter of the member CSPlayerItem::m_ItemInfo
*
* @param entity Entity index
* @param type   Item info type. See ItemInfo constants.
*
*/
native rg_set_iteminfo(const entity, ItemInfo:type, any:...);

/**
* Gets a parameter of the member CSPlayerItem::m_ItemInfo
*
* @param entity Entity index
* @param type   Item info type. See ItemInfo constants.
*
*/
native rg_get_iteminfo(const ent, ItemInfo:type, any:...);

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
*/
native rg_set_global_iteminfo(const {WeaponIdType,_}:weapon_id, ItemInfo:type, any:...);

/**
* Gets a parameter of the global CBasePlayerItem::m_ItemInfoArray array
*
* @param weapon_id Weapon id, see WEAPON_* constants
* @param type      Item info type. See ItemInfo constants.
*
*/
native rg_get_global_iteminfo(const {WeaponIdType,_}:weapon_id, ItemInfo:type, any:...);

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
*/
native bool:rg_hint_message(const index, const message[], Float:duration = 6.0, bool:bDisplayIfPlayerDead = false, bool:bOverride = false);

/*
* Instantly restart round.
*
* @noreturn
*/
native rg_restart_round();

/*
* Instantly check win conditions.
*
* @noreturn
*/
native rg_check_win_conditions();

/*
* Instantly initialize player counts.
*
* @param num_alive_terrorist   Count alive terrorists
* @param num_alive_ct          Count alive counter-terrorists
* @param num_dead_terrorist    Count dead terrorists
* @param num_dead_ct           Count dead counter-terrorists
*
* @noreturn
*/
native rg_initialize_player_counts(&num_alive_terrorist = 0, &num_alive_ct = 0, &num_dead_terrorist = 0, &num_dead_ct = 0);

/*
* Reset if player can hear another player.
*
* @param index                 Player id
* @noreturn
*/
native rg_reset_can_hear_player(const index);

/*
* Set if player can hear another player
*
* @param listener              Listener player id
* @param sender                Sender player id
* @param can_hear              Can listener hear sender
*
* @noreturn
*/
native rg_set_can_hear_player(const listener, const sender, const bool:can_hear);

/*
* Get if player can hear another player
*
* @param listener              Listener player id
* @param sender                Sender player id
*
* @return boolean
*/
native bool:rg_get_can_hear_player(const listener, const sender);

/*
* Spawn a head gib
*
* @param index                 Entity id
*
* @return                      Index of head gib entity or AMX_NULLENT (-1) otherwise
*/
native rg_spawn_head_gib(const index);

/*
* Spawn random gibs
*
* @param index                 Entity id
* @param cGibs                 Count gibs
* @param bHuman                Set gibs of a human or alien
*
* @noreturn
*/
native rg_spawn_random_gibs(const index, const cGibs, const bool:bHuman = true);

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
native rg_spawn_grenade(WeaponIdType:weaponId, pevOwner, Float:vecSrc[3], Float:vecThrow[3], Float:time, TeamName:iTeam, usEvent = 0);

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
native rg_create_weaponbox(const pItem, const pPlayerOwner, const modelName[], Float:origin[3], Float:angles[3], Float:velocity[3], Float:lifeTime, bool:packAmmo);

/*
* Removes an entity using gamedll's UTIL_Remove function, which sets a frame delay to ensure its removal.
*
* @param pEntity               Entity index to remove
*
* @return                      1 on success, 0 otherwise
*/
native rg_remove_entity(const pEntity);

/*
* Creates a Decal in world based on a traceresult.
*
* @param ptr                   Traceresult pointer, use Fakemeta's create_tr2 to instantiate one
* @param decalNumber           Number of decal to spray, see DECAL_ constants on cssdk_const.inc
*
* @noreturn
*/
native rg_decal_trace(const ptr, Decal:decalNumber);

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
native rg_emit_texture_sound(const ptr, Float:vecSrc[3], Float:vecEnd[3], Bullet:iBulletType);

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
native rg_add_ammo_registry(const szAmmoname[]);

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
native rg_weapon_deploy(const entity, const szViewModel[], const szWeaponModel[], iAnim, const szAnimExt[], skiplocal = 0);

/*
* Reloads a weapon or a player's active weapon using the CBasePlayerWeapon::DefaultReload function.
*
* @param entity                Weapon to reload (> MaxClients) OR player index to reload his current active weapon (>= 1 & <= MaxClients).
* @param iClipSize             Weapon max clip to check. 0 = weapon max clip stored in ItemInfo
* @param iAnim                 Weapon view model animation to play (often "reload", use HLMV to see anim index)
* @param fDelay                Player reload duration before clip refill.
*
* @return                      1 on successful weapon reload, 0 otherwise.
*/
native rg_weapon_reload(const entity, iClipSize, iAnim, Float:fDelay);

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
native rg_weapon_shotgun_reload(const entity, iAnim, iStartAnim, Float:fDelay, Float:fStartDelay, const pszReloadSound1[] = "", const pszReloadSound2[] = "");

/*
* Sends a weapon animation using the CBasePlayerWeapon::SendWeaponAnim function.
*
* @param entity                Weapon to send animation on owner (> MaxClients) OR player index to send animation (>= 1 & <= MaxClients).
* @param iAnim                 Weapon view model animation to play (use HLMV to see anim index)
* @param skiplocal             If 0, weapon animation will be forced to play on client ignoring active client prediction.
*
* @noreturn
*/
native rg_weapon_send_animation(const entity, iAnim, skiplocal = 0);

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
native rg_weapon_kickback(const entity, Float:up_base, Float:lateral_base, Float:up_modifier, Float:lateral_modifier, Float:up_max, Float:lateral_max, direction_change);

/*
* Switches player current weapon into the best one on its inventory using the CHalfLifeMultiplay::GetNextBestWeapon function.
*
* @param player                Player index.
* @param currentWeapon         Current player active weapon. 0 = retrieve from m_pActiveItem member
*
* @note Weapon selection is based on weapon's Weight attribute from ItemInfo structure.
* @return                      1 if weapon was found and switched to, 0 otherwise
*/
native rg_switch_best_weapon(const player, const currentWeapon = 0);

/*
* Disappear a player from the world. Used when VIP reaches escape zone. Basically a silent kill.
*
* @param player                Player index.
*
* @noreturn
*/
native rg_disappear(const player);

/*
* Sets player current Observer mode.
* @note Player must be a valid observer (m_afPhysicsFlags & PFLAG_OBSERVER).
*
* @param player                Player index.
* @param mode                  Observer mode, see OBS_* constants in cssdk_const.inc
*
* @noreturn
*/
native rg_set_observer_mode(const player, const mode);

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
native rg_observer_find_next_player(const player, const bool:bReverse = false, const name[] = "");

/*
* Emits a death notice (logs, DeathMsg event, win conditions check)
*
* @param pVictim               Player index.
* @param pKiller               Killer entity.
* @param pevInflictor          Inflictor entity. 0 = world
*
* @noreturn
*/
native rg_death_notice(const pVictim, const pKiller, const pevInflictor);

/*
* Checks a player relationship with another reference
*
* @param player     Player index
* @param target     Target index
*
* @return           Match player relationship, see GR_* constants in cssdk_const.inc
*/
native rg_player_relationship(const player, const target);

/*
* Sends death messages to all players, including info about the killer, victim, weapon used,
* extra death flags, death position, assistant, and kill rarity using the CHalfLifeMultiplay::SendDeathMessage function.
*
* @param pKiller                Killer index (if any).
* @param pVictim                Victim index.
* @param pAssister              Assisting player index (if any).
* @param pevInflictor           Inflictor entity. 0 = world
* @param killerWeaponName       The name of the weapon used by the killer.
* @param iDeathMessageFlags     Flags indicating extra death message info, see DeathMessageFlags enum in cssdk_const.inc
* @param iRarityOfKill          An bitsums representing the rarity classification of the kill, see KillRarity enum in cssdk_const.inc
*
* @noreturn
*/
native rg_send_death_message(const pKiller, const pVictim, const pAssister, const pevInflictor, const killerWeaponName[], const DeathMessageFlags:iDeathMessageFlags, const KillRarity:iRarityOfKill);

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
native rg_player_takedamage_impulse(const player, const attacker, const Float:flKnockbackForce, const Float:flVelModifier);
