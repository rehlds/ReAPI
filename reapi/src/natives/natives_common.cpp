#include "precompiled.h"

/*
* Compares the entity to specified a classname.
* @note This native also checks the validity of an entity.
*
* @return       true/false
*
* native bool:FClassnameIs(const entityIndex, const className[]);
*/
cell AMX_NATIVE_CALL amx_FClassnameIs(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index, arg_classname };

	int nEntityIndex = params[arg_index];
	if (nEntityIndex < 0 || nEntityIndex > gpGlobals->maxEntities) {
		return FALSE;
	}

	char classname[256];

	edict_t *pEdict = edictByIndex(nEntityIndex);
	if (pEdict && FClassnameIs(pEdict, getAmxString(amx, params[arg_classname], classname))) {
		return TRUE;
	}

	return FALSE;
}

/*
* To get WeaponIdType from grenade entity
*
* @param entity     Grenade entity
*
* @return           return enum's of WeaponIdType
*
* native WeaponIdType:GetGrenadeType(const entityIndex);
*/
cell AMX_NATIVE_CALL amx_GetGrenadeType(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index };

	CHECK_ISENTITY(arg_index);

	edict_t *pEdict = edictByIndex(params[arg_index]);
	if (!pEdict || !FClassnameIs(pEdict, "grenade")) {
		return WEAPON_NONE;
	}

	CGrenade *pGrenade = getPrivate<CGrenade>(pEdict);
	if (!pGrenade) {
		return WEAPON_NONE;
	}

	if (pGrenade->m_bIsC4) {
		return WEAPON_C4;
	}

	if (!pGrenade->m_usEvent) {
		return WEAPON_FLASHBANG;
	}

	static unsigned short usCreateExplosion = 0;
	if (!usCreateExplosion) {
		usCreateExplosion = PRECACHE_EVENT(1, "events/createexplo.sc");
	}

	return (pGrenade->m_usEvent == usCreateExplosion) ? WEAPON_HEGRENADE : WEAPON_SMOKEGRENADE;
}

/*
* Sets the view entity on a client.
* This allows pfnSetView able to hooks.
*
* @param index       Client index
* @param viewEntity  Entity index
*
* native engset_view(const index, const viewEntity);
*/
cell AMX_NATIVE_CALL amx_engset_view(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index, arg_entity };

	CHECK_ISPLAYER(arg_index);

	CBasePlayer *pPlayer = UTIL_PlayerByIndex(params[arg_index]);
	CHECK_CONNECTED(pPlayer, arg_index);

	g_pengfuncsTable->pfnSetView(pPlayer->edict(), edictByIndexAmx(params[arg_entity]));
	return TRUE;
}

/*
* Gets the return index of the current view entity on a client.
*
* @param index       Client index
*
* native get_viewent(const index);
*/
cell AMX_NATIVE_CALL amx_get_viewent(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index };

	CHECK_ISPLAYER(arg_index);

	client_t *pClient = clientOfIndex(params[arg_index]);
	if (unlikely(pClient == nullptr || !(pClient->active | pClient->spawned | pClient->connected)))
	{
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: player %i is not connected", __FUNCTION__, params[arg_index]);
		return FALSE;
	}

	return indexOfEdictAmx(pClient->pViewEntity);
}

/*
* Sets value string to entire buffer
*
* @param buffer     Pointer to buffer
* @param value      Value to set
* @param maxlen     Maximum size of the value buffer to set, -1 means copy all characters
*
* @return           1 on success, 0 otherwise
*
* native set_key_value_buffer(const pbuffer, const value[], const maxlen = -1);
*/
cell AMX_NATIVE_CALL amx_set_key_value_buffer(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_buffer, arg_value, arg_maxlen };

	char *buffer = reinterpret_cast<char *>(params[arg_buffer]);
	if (!buffer)
	{
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: Invalid buffer", __FUNCTION__);
		return FALSE;
	}

	size_t maxlen = (params[arg_maxlen] > 0) ? min(params[arg_maxlen], MAX_INFO_STRING) : MAX_INFO_STRING;
	char infobuf[MAX_INFO_STRING];
	const char *value = getAmxString(amx, params[arg_value], infobuf);
	size_t len = min<size_t>(Q_strlen(value) + 1, maxlen);
	Q_strnlcpy(buffer, value, len);
	return TRUE;
}

/*
* Gets an AMXX string buffer from a infobuffer pointer
*
* @param pbuffer    Info string pointer
* @param output     String to copy value to
* @param maxlen     Maximum size of the output buffer
*
* @return           Number of cells written to the output buffer
*
* native get_key_value_buffer(const pbuffer, const output[], const maxlen);
*/
cell AMX_NATIVE_CALL amx_get_key_value_buffer(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_buffer, arg_output, arg_maxlen };

	char *buffer = reinterpret_cast<char *>(params[arg_buffer]);
	if (!buffer)
	{
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: Invalid buffer", __FUNCTION__);
		return FALSE;
	}

	return g_amxxapi.SetAmxString(amx, params[arg_output], buffer, params[arg_maxlen]);
}

/*
* Gets value for key in buffer
*
* @param pbuffer    Pointer to buffer
* @param key        Key string
* @param value      Buffer to copy value to
* @param maxlen     Maximum size of the buffer
*
* @return           Number of cells written to buffer
* @error            If invalid buffer handler provided, an error will be thrown.
*
* native get_key_value(const pbuffer, const key[], const value[], const maxlen);
*/
cell AMX_NATIVE_CALL amx_get_key_value(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_buffer, arg_key, arg_value, arg_maxlen };

	char *buffer = reinterpret_cast<char *>(params[arg_buffer]);
	if (!buffer)
	{
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: Invalid buffer", __FUNCTION__);
		return FALSE;
	}

	char keybuf[MAX_KV_LEN];
	auto key = getAmxString(amx, params[arg_key], keybuf);

	return g_amxxapi.SetAmxString(amx, params[arg_value], g_engfuncs.pfnInfoKeyValue(buffer, key), params[arg_maxlen]);
}

/*
* Sets value for key in buffer
*
* @param pbuffer    Pointer to buffer
* @param key        Key string
* @param value      Value to set
*
* @noreturn
* @error            If invalid buffer handler provided, an error will be thrown.
*
* native set_key_value(const &pbuffer, const key[], const value[]);
*/
cell AMX_NATIVE_CALL amx_set_key_value(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_buffer, arg_key, arg_value };

	char *buffer = reinterpret_cast<char *>(params[arg_buffer]);
	if (!buffer)
	{
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: Invalid buffer", __FUNCTION__);
		return FALSE;
	}

	char keybuf[MAX_KV_LEN], valuebuf[MAX_KV_LEN];
	auto key = getAmxString(amx, params[arg_key], keybuf);
	auto value = getAmxString(amx, params[arg_value], valuebuf);

	if (!key[0])
	{
		buffer[0] = '\0';
		return TRUE;
	}

	Info_SetValueForStarKey(buffer, key, value, MAX_INFO_STRING);

	// TODO: this function doesn't let me sets another buffer
	//g_engfuncs.pfnSetKeyValue(buffer, key, value);
	return TRUE;
}

/*
* Gets the position of the bone
*
* @param entity     Entity index
* @param bone       Number of the bone
* @param vecOrigin  Array to store origin in
* @param vecAngles  Array to store angles in
*
* @return           1 on success, 0 otherwise
* @error            If the index is not within the range of 1 to maxEntities or
*                   the entity is not valid, an error will be thrown.
*
* native GetBonePosition(const entity, const bone, Float:vecOrigin[3], Float:vecAngles[3] = {0.0, 0.0, 0.0});
*/
cell AMX_NATIVE_CALL amx_GetBonePosition(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index, arg_bone, arg_origin, arg_angles };

	CHECK_ISENTITY(arg_index);

	CBaseEntity *pEntity = getPrivate<CBaseEntity>(params[arg_index]);
	if (unlikely(pEntity == nullptr)) {
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: invalid or uninitialized entity", __FUNCTION__);
		return FALSE;
	}

	if (FNullEnt(params[arg_index])) {
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: worldspawn not allowed", __FUNCTION__);
		return FALSE;
	}

	Vector *pVecOrigin = (Vector *)getAmxAddr(amx, params[arg_origin]);
	Vector *pVecAngles = (PARAMS_COUNT == 4) ? (Vector *)getAmxAddr(amx, params[arg_angles]) : nullptr;
	GetBonePosition(pEntity, params[arg_bone], pVecOrigin, pVecAngles);
	return TRUE;
}

/*
* Gets the position of the attachment
*
* @param entity     Entity index
* @param attachment Number of the attachment
* @param vecOrigin  Array to store origin in
* @param vecAngles  Array to store angles in
*
* @return           1 on success, 0 otherwise
* @error            If the index is not within the range of 1 to maxEntities or
*                   the entity is not valid, an error will be thrown.
*
* native GetAttachment(const entity, const attachment, Float:vecOrigin[3], Float:vecAngles[3] = {0.0, 0.0, 0.0});
*/
cell AMX_NATIVE_CALL amx_GetAttachment(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index, arg_attachment, arg_origin, arg_angles };

	CHECK_ISENTITY(arg_index);

	CBaseEntity *pEntity = getPrivate<CBaseEntity>(params[arg_index]);
	if (unlikely(pEntity == nullptr)) {
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: invalid or uninitialized entity", __FUNCTION__);
		return FALSE;
	}

	if (FNullEnt(params[arg_index])) {
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: worldspawn not allowed", __FUNCTION__);
		return FALSE;
	}

	Vector *pVecOrigin = (Vector *)getAmxAddr(amx, params[arg_origin]);
	Vector *pVecAngles = (PARAMS_COUNT == 4) ? (Vector *)getAmxAddr(amx, params[arg_angles]) : nullptr;
	GetAttachment(pEntity, params[arg_attachment], pVecOrigin, pVecAngles);
	return TRUE;
}

/*
* Gets body group value based on entity's model group
*
* @param entity     Entity index
* @param group      Number of entity's model group index
*
* @return           Body group value
* @error            If the index is not within the range of 1 to maxEntities or
*                   the entity is not valid, an error will be thrown.
*
*/
cell AMX_NATIVE_CALL amx_GetBodygroup(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index, arg_group };

	CHECK_ISENTITY(arg_index);

	CBaseEntity *pEntity = getPrivate<CBaseEntity>(params[arg_index]);
	if (unlikely(pEntity == nullptr)) {
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: invalid or uninitialized entity", __FUNCTION__);
		return 0;
	}

	if (FNullEnt(params[arg_index])) {
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: worldspawn not allowed", __FUNCTION__);
		return 0;
	}

	return (cell)GetBodygroup(pEntity, params[arg_group]);
}

/*
* Sets body group value based on entity's model group
*
* @param entity     Entity index
* @param group      Number of entity's model group index
* @param value      Value to assign
*
* @return           1 on success, 0 otherwise
* @error            If the index is not within the range of 1 to maxEntities or
*                   the entity is not valid, an error will be thrown.
*
*/
cell AMX_NATIVE_CALL amx_SetBodygroup(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index, arg_group, arg_value };

	CHECK_ISENTITY(arg_index);

	CBaseEntity *pEntity = getPrivate<CBaseEntity>(params[arg_index]);
	if (unlikely(pEntity == nullptr)) {
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: invalid or uninitialized entity", __FUNCTION__);
		return FALSE;
	}

	if (FNullEnt(params[arg_index])) {
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: worldspawn not allowed", __FUNCTION__);
		return FALSE;
	}

	SetBodygroup(pEntity, params[arg_group], params[arg_value]);
	return TRUE;
}

/*
* Gets sequence information based on entity's model current sequence index
*
* @param entity             Entity index
* @param piFlags            Sequence flags (1 = sequence loops)
* @param pflFrameRate       Sequence framerate
* @param pflGroundSpeed     Sequence ground speed
*
* @return                   True on success, false otherwise
* @error                    If the index is not within the range of 1 to maxEntities or
*                           the entity is not valid, an error will be thrown.
*
*/
cell AMX_NATIVE_CALL amx_GetSequenceInfo(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index, arg_flags, arg_framerate, arg_groundspeed };

	CHECK_ISENTITY(arg_index);

	CBaseEntity *pEntity = getPrivate<CBaseEntity>(params[arg_index]);
	if (unlikely(pEntity == nullptr)) {
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: invalid or uninitialized entity", __FUNCTION__);
		return FALSE;
	}

	if (FNullEnt(params[arg_index])) {
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: worldspawn not allowed", __FUNCTION__);
		return FALSE;
	}

	int* pflags = reinterpret_cast<int*>(getAmxAddr(amx, params[arg_flags]));
	float* pframerate = reinterpret_cast<float*>(getAmxAddr(amx, params[arg_framerate]));
	float* pgroundspeed = reinterpret_cast<float*>(getAmxAddr(amx, params[arg_groundspeed]));

	return (cell)GetSequenceInfo2(pEntity, pflags, pframerate, pgroundspeed);
}

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
*
* native SetThink(const ent, const callback[], const params[] = "", const len = 0);
*/
cell AMX_NATIVE_CALL amx_SetThink(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index, arg_handler, arg_params, arg_len };

	CHECK_ISENTITY(arg_index);

	CBaseEntity *pEntity = getPrivate<CBaseEntity>(params[arg_index]);
	if (unlikely(pEntity == nullptr)) {
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: invalid or uninitialized entity", __FUNCTION__);
		return FALSE;
	}

	char namebuf[256];
	const char *funcname = getAmxString(amx, params[arg_handler], namebuf);
	if (unlikely(funcname == nullptr || funcname[0] == '\0')) {
		EntityCallbackDispatcher().UnsetThink(amx, pEntity);
		return TRUE;
	}

	int funcid;
	if (unlikely(g_amxxapi.amx_FindPublic(amx, funcname, &funcid) != AMX_ERR_NONE)) {
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: public function \"%s\" not found.", __FUNCTION__, funcname);
		return FALSE;
	}

	cell *pParams = (PARAMS_COUNT >= 3) ? getAmxAddr(amx, params[arg_params]) : nullptr;
	return (cell)EntityCallbackDispatcher().SetThink(amx, pEntity, funcname, pParams, params[arg_len]);
}

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
*
* native SetTouch(const ent, const callback[], const params[] = "", const len = 0);
*/
cell AMX_NATIVE_CALL amx_SetTouch(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index, arg_handler, arg_params, arg_len };

	CHECK_ISENTITY(arg_index);

	CBaseEntity *pEntity = getPrivate<CBaseEntity>(params[arg_index]);
	if (unlikely(pEntity == nullptr)) {
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: invalid or uninitialized entity", __FUNCTION__);
		return FALSE;
	}

	char namebuf[256];
	const char *funcname = getAmxString(amx, params[arg_handler], namebuf);
	if (unlikely(funcname == nullptr || funcname[0] == '\0')) {
		EntityCallbackDispatcher().UnsetTouch(amx, pEntity);
		return TRUE;
	}

	int funcid;
	if (unlikely(g_amxxapi.amx_FindPublic(amx, funcname, &funcid) != AMX_ERR_NONE)) {
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: public function \"%s\" not found.", __FUNCTION__, funcname);
		return FALSE;
	}

	cell *pParams = (PARAMS_COUNT >= 3) ? getAmxAddr(amx, params[arg_params]) : nullptr;
	return (cell)EntityCallbackDispatcher().SetTouch(amx, pEntity, funcname, pParams, params[arg_len]);
}

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
*
* native SetUse(const ent, const callback[], const params[] = "", const len = 0);
*/
cell AMX_NATIVE_CALL amx_SetUse(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index, arg_handler, arg_params, arg_len };

	CHECK_ISENTITY(arg_index);

	CBaseEntity *pEntity = getPrivate<CBaseEntity>(params[arg_index]);
	if (unlikely(pEntity == nullptr)) {
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: invalid or uninitialized entity", __FUNCTION__);
		return FALSE;
	}

	char namebuf[256];
	const char *funcname = getAmxString(amx, params[arg_handler], namebuf);
	if (unlikely(funcname == nullptr || funcname[0] == '\0')) {
		EntityCallbackDispatcher().UnsetUse(amx, pEntity);
		return TRUE;
	}

	int funcid;
	if (unlikely(g_amxxapi.amx_FindPublic(amx, funcname, &funcid) != AMX_ERR_NONE)) {
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: public function \"%s\" not found.", __FUNCTION__, funcname);
		return FALSE;
	}

	cell *pParams = (PARAMS_COUNT >= 3) ? getAmxAddr(amx, params[arg_params]) : nullptr;
	return (cell)EntityCallbackDispatcher().SetUse(amx, pEntity, funcname, pParams, params[arg_len]);
}

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
*
* native SetBlocked(const ent, const callback[], const params[] = "", const len = 0);
*/
cell AMX_NATIVE_CALL amx_SetBlocked(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index, arg_handler, arg_params, arg_len };

	CHECK_ISENTITY(arg_index);

	CBaseEntity *pEntity = getPrivate<CBaseEntity>(params[arg_index]);
	if (unlikely(pEntity == nullptr)) {
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: invalid or uninitialized entity", __FUNCTION__);
		return FALSE;
	}

	char namebuf[256];
	const char *funcname = getAmxString(amx, params[arg_handler], namebuf);
	if (unlikely(funcname == nullptr || funcname[0] == '\0')) {
		EntityCallbackDispatcher().UnsetBlocked(amx, pEntity);
		return TRUE;
	}

	int funcid;
	if (unlikely(g_amxxapi.amx_FindPublic(amx, funcname, &funcid) != AMX_ERR_NONE)) {
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: public function \"%s\" not found.", __FUNCTION__, funcname);
		return FALSE;
	}

	cell *pParams = (PARAMS_COUNT >= 3) ? getAmxAddr(amx, params[arg_params]) : nullptr;
	return (cell)EntityCallbackDispatcher().SetBlocked(amx, pEntity, funcname, pParams, params[arg_len]);
}

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
*
* native SetMoveDone(const ent, const callback[], const params[] = "", const len = 0);
*/
cell AMX_NATIVE_CALL amx_SetMoveDone(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index, arg_handler, arg_params, arg_len };

	CHECK_ISENTITY(arg_index);

	CBaseEntity *pEntity = getPrivate<CBaseEntity>(params[arg_index]);
	if (unlikely(pEntity == nullptr)) {
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: invalid or uninitialized entity", __FUNCTION__);
		return FALSE;
	}

	char namebuf[256];
	const char *funcname = getAmxString(amx, params[arg_handler], namebuf);
	if (unlikely(funcname == nullptr || funcname[0] == '\0')) {
		EntityCallbackDispatcher().UnsetMoveDone(amx, pEntity);
		return TRUE;
	}

	int funcid;
	if (unlikely(g_amxxapi.amx_FindPublic(amx, funcname, &funcid) != AMX_ERR_NONE)) {
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: public function \"%s\" not found.", __FUNCTION__, funcname);
		return FALSE;
	}

	cell *pParams = (PARAMS_COUNT >= 3) ? getAmxAddr(amx, params[arg_params]) : nullptr;
	return (cell)EntityCallbackDispatcher().SetMoveDone(amx, pEntity, funcname, pParams, params[arg_len]);
}

enum class CheckVisibilityType {
	PVS = 0, // Check in Potentially Visible Set (PVS)
	PAS      // Check in Potentially Audible Set (PAS)
};

/*
* Test visibility of an entity from a given origin using either PVS or PAS
*
* @param entity     Entity index
* @param origin     Vector representing the origin from which visibility is checked
* @param type       Type of visibility check: VisibilityInPVS (Potentially Visible Set) or VisibilityInPAS (Potentially Audible Set)
*
* @return           0 - Not visible
*                   1 - Visible, passed by a leafnum
*                   2 - Visible, passed by a headnode
*
* @remarks          This function checks the visibility of the specified entity from the given origin, using either
*                   the Potentially Visible Set (PVS) or the Potentially Audible Set (PAS) depending on the provided type
*
* native CheckVisibilityInOrigin(const ent, Float:origin[3], CheckVisibilityType:type = VisibilityInPVS);
*/
cell AMX_NATIVE_CALL amx_CheckVisibilityInOrigin(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index, arg_origin, arg_type };

	CHECK_ISENTITY(arg_index);

	CBaseEntity *pEntity = getPrivate<CBaseEntity>(params[arg_index]);
	if (unlikely(pEntity == nullptr)) {
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: invalid or uninitialized entity", __FUNCTION__);
		return FALSE;
	}

	CheckVisibilityType type = static_cast<CheckVisibilityType>(params[arg_type]);
	if (type < CheckVisibilityType::PVS || type > CheckVisibilityType::PAS) {
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: invalid visibility check type %d. Use either VisibilityInPVS or VisibilityInPAS.", __FUNCTION__, params[arg_type]);
		return FALSE;
	}

	Vector &origin = *(Vector *)getAmxAddr(amx, params[arg_origin]);

	unsigned char *pSet = NULL;
	switch (type)
	{
	case CheckVisibilityType::PVS: pSet = ENGINE_SET_PVS(origin); break;
	case CheckVisibilityType::PAS: pSet = ENGINE_SET_PAS(origin); break;
	default: break;
	}

	return ENGINE_CHECK_VISIBILITY(pEntity->edict(), pSet);
}

AMX_NATIVE_INFO Natives_Common[] =
{
	{ "FClassnameIs",         amx_FClassnameIs         },
	{ "GetGrenadeType",       amx_GetGrenadeType       },
	{ "engset_view",          amx_engset_view          },
	{ "get_viewent",          amx_get_viewent          },
	{ "get_key_value",        amx_get_key_value        },
	{ "set_key_value",        amx_set_key_value        },
	{ "get_key_value_buffer", amx_get_key_value_buffer },
	{ "set_key_value_buffer", amx_set_key_value_buffer },
	{ "GetBonePosition",      amx_GetBonePosition      },
	{ "GetAttachment",        amx_GetAttachment        },
	{ "GetBodygroup",         amx_GetBodygroup         },
	{ "SetBodygroup",         amx_SetBodygroup         },
	{ "GetSequenceInfo",      amx_GetSequenceInfo      },
	{ "SetThink",             amx_SetThink             },
	{ "SetTouch",             amx_SetTouch             },
	{ "SetUse",               amx_SetUse               },
	{ "SetBlocked",           amx_SetBlocked           },
	{ "SetMoveDone",          amx_SetMoveDone          },

	{ "CheckVisibilityInOrigin", amx_CheckVisibilityInOrigin },

	{ nullptr, nullptr }
};

/*
* Check if the entity is valid
*
* @return       true/false
*
* native bool:is_entity(const entityIndex);
*/
cell AMX_NATIVE_CALL is_entity(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index };

	int nIndex = params[arg_index];
	if (nIndex < 0 || nIndex > gpGlobals->maxEntities) {
		return FALSE;
	}

	auto pEntity = getPrivate<CBaseEntity>(nIndex);
	if (!pEntity) {
		return FALSE;
	}

	// if it is the index of the player
	if (pEntity->IsPlayer() && pEntity->has_disconnected) {
		return FALSE;
	}

	return TRUE;
}

/*
* Check if the rehlds is available
*
* @return       true/false
*
* native bool:is_rehlds();
*/
cell AMX_NATIVE_CALL is_rehlds(AMX *amx, cell *params)
{
	return (cell)api_cfg.hasReHLDS();
}

/*
* Check if the regamedll is available
*
* @return       true/false
*
* native bool:is_regamedll();
*/
cell AMX_NATIVE_CALL is_regamedll(AMX *amx, cell *params)
{
	return (cell)api_cfg.hasReGameDLL();
}

/*
* Check if the reunion is available
*
* @return       true/false
*
* native bool:has_reunion();
*/
cell AMX_NATIVE_CALL has_reunion(AMX *amx, cell *params)
{
	return (cell)api_cfg.hasReunion();
}

/*
* Check if the vtc is available
*
* @return       true/false
*
* native bool:has_vtc();
*/
cell AMX_NATIVE_CALL has_vtc(AMX *amx, cell *params)
{
	return (cell)api_cfg.hasVTC();
}

/*
* Check if the rechecker is available
*
* @return       true/false
*
* native bool:has_rechecker();
*/
cell AMX_NATIVE_CALL has_rechecker(AMX *amx, cell *params)
{
	return (cell)api_cfg.hasRechecker();
}

AMX_NATIVE_INFO Natives_Checks[] =
{
	{ "is_entity",     is_entity     },
	{ "is_rehlds",     is_rehlds     },
	{ "is_regamedll",  is_regamedll  },
	{ "has_reunion",   has_reunion   },
	{ "has_vtc",       has_vtc       },
	{ "has_rechecker", has_rechecker },
};

void RegisterNatives_Common()
{
	g_amxxapi.AddNatives(Natives_Common);
	g_amxxapi.AddNatives(Natives_Checks);
}
