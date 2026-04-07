#include "precompiled.h"

/*
* Sets a value to an entity's member.
*
* @param index      Entity index
* @param member     The specified member, look at the enums with name *_Members
*
* @return           1 on success.
* native set_member(const index, any:member, any:...);
*/
cell AMX_NATIVE_CALL set_member(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index, arg_member, arg_value, arg_elem };
	member_t *member = memberlist[params[arg_member]];

	if (unlikely(member == nullptr)) {
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: unknown member id %i", __FUNCTION__, params[arg_member]);
		return FALSE;
	}

	edict_t *pEdict = edictByIndexAmx(params[arg_index]);
	if (unlikely(pEdict == nullptr || pEdict->pvPrivateData == nullptr)) {
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: invalid or uninitialized entity", __FUNCTION__);
		return FALSE;
	}

	cell* value = getAmxAddr(amx, params[arg_value]);
	size_t element = (PARAMS_COUNT == 4) ? *getAmxAddr(amx, params[arg_elem]) : 0;

	return set_member(
		amx,
		get_pdata_custom(getPrivate<CBaseEntity>(pEdict), params[arg_member]),
		member,
		value,
		element
	);
}

/*
* Returns a value from an entity's member.
*
* @param index      Entity index
* @param member     The specified member, look at the enums with name *_Members
*
* @return           If an integer or boolean or one byte, array or everything else is passed via the 3rd argument and more, look at the argument list for the specified member
*
* native any:get_member(const index, any:member, any:...);
*/
cell AMX_NATIVE_CALL get_member(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index, arg_member, arg_3, arg_4, arg_5 };
	member_t *member = memberlist[params[arg_member]];

	if (unlikely(member == nullptr)) {
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: unknown member id %i", __FUNCTION__, params[arg_member]);
		return FALSE;
	}

	edict_t *pEdict = edictByIndexAmx(params[arg_index]);
	if (unlikely(pEdict == nullptr || pEdict->pvPrivateData == nullptr)) {
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: invalid or uninitialized entity", __FUNCTION__);
		return FALSE;
	}

	cell* dest;
	size_t element;
	size_t length;

	switch (PARAMS_COUNT)
	{
	case 5:
		dest = getAmxAddr(amx, params[arg_3]);
		length = *getAmxAddr(amx, params[arg_4]);
		element = *getAmxAddr(amx, params[arg_5]);
		break;
	case 4:
		dest = getAmxAddr(amx, params[arg_3]);
		length = *getAmxAddr(amx, params[arg_4]);
		element = 0;
		break;
	case 3:
	{
		cell* arg3 = getAmxAddr(amx, params[arg_3]);
		if (member->isTypeReturnable()) {
			dest = nullptr;
			element = *arg3;
		}
		else {
			dest = arg3;
			element = 0;
		}
		length = 0;
		break;
	}
	default:
		dest = nullptr;
		element = 0;
		length = 0;
		break;
	}

	return get_member(amx,
		get_pdata_custom(getPrivate<CBaseEntity>(pEdict), params[arg_member]),
		member,
		dest,
		element,
		length
	);
}

/*
* Sets a value to an entity's member.
* Safe version, can guarantee that the present member is refers to derived class of the entity.
*
* @param index      Entity index
* @param member     The specified member, look at the enums with name *_Members
*
* @return           1 on success.
* native set_member_safe(const index, any:member, any:...);
*/
cell AMX_NATIVE_CALL set_member_s(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index, arg_member, arg_value, arg_elem };
	member_t *member = memberlist[params[arg_member]];

	if (unlikely(member == nullptr)) {
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: unknown member id %i", __FUNCTION__, params[arg_member]);
		return FALSE;
	}

	edict_t *pEdict = edictByIndexAmx(params[arg_index]);
	if (unlikely(pEdict == nullptr || pEdict->pvPrivateData == nullptr)) {
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: invalid or uninitialized entity", __FUNCTION__);
		return FALSE;
	}

	cell* value = getAmxAddr(amx, params[arg_value]);
	size_t element = (PARAMS_COUNT == 4) ? *getAmxAddr(amx, params[arg_elem]) : 0;
	void *pEntity = get_pdata_custom(getPrivate<CBaseEntity>(pEdict), params[arg_member]);

	if (!member->pfnIsRefsToClass(pEntity))
	{
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: '%s' has no refs to the base class of an entity '%s'", __FUNCTION__, member->name, STRING(pEdict->v.classname));
		return FALSE;
	}

	return set_member(
		amx,
		pEntity,
		member,
		value,
		element
	);
}

/*
* Returns a value from an entity's member.
* Safe version, can guarantee that the present member is refers to derived class of the entity.
*
* @param index      Entity index
* @param member     The specified member, look at the enums with name *_Members
*
* @return           If an integer or boolean or one byte, array or everything else is passed via the 3rd argument and more, look at the argument list for the specified member
*
* native any:get_member_safe(const index, any:member, any:...);
*/
cell AMX_NATIVE_CALL get_member_s(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index, arg_member, arg_3, arg_4, arg_5 };
	member_t *member = memberlist[params[arg_member]];

	if (unlikely(member == nullptr)) {
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: unknown member id %i", __FUNCTION__, params[arg_member]);
		return FALSE;
	}

	edict_t *pEdict = edictByIndexAmx(params[arg_index]);
	if (unlikely(pEdict == nullptr || pEdict->pvPrivateData == nullptr)) {
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: invalid or uninitialized entity", __FUNCTION__);
		return FALSE;
	}

	cell* dest;
	size_t element;
	size_t length;

	switch (PARAMS_COUNT)
	{
	case 5:
		dest = getAmxAddr(amx, params[arg_3]);
		length = *getAmxAddr(amx, params[arg_4]);
		element = *getAmxAddr(amx, params[arg_5]);
		break;
	case 4:
		dest = getAmxAddr(amx, params[arg_3]);
		length = *getAmxAddr(amx, params[arg_4]);
		element = 0;
		break;
	case 3:
	{
		cell* arg3 = getAmxAddr(amx, params[arg_3]);
		if (member->isTypeReturnable()) {
			dest = nullptr;
			element = *arg3;
		}
		else {
			dest = arg3;
			element = 0;
		}
		length = 0;
		break;
	}
	default:
		dest = nullptr;
		element = 0;
		length = 0;
		break;
	}

	void *pEntity = get_pdata_custom(getPrivate<CBaseEntity>(pEdict), params[arg_member]);

	if (!member->pfnIsRefsToClass(pEntity))
	{
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: '%s' has no refs to the base class of an entity '%s'", __FUNCTION__, member->name, STRING(pEdict->v.classname));
		return FALSE;
	}

	return get_member(
		amx,
		pEntity,
		member,
		dest,
		element,
		length
	);
}

/*
* Sets a value to CSGameRules_Members members.
*
* @param member     The specified member, look at the enums with name CSGameRules_Members
*
* @return           1 on success.
*
* native set_member_game(CSGameRules_Members:member, any:...);
*/
cell AMX_NATIVE_CALL set_member_game(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_member, arg_value, arg_elem };

	CHECK_GAMERULES();

	member_t *member = memberlist[params[arg_member]];
	if (unlikely(member == nullptr)) {
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: unknown member id %i", __FUNCTION__, params[arg_member]);
		return FALSE;
	}

	cell* value = getAmxAddr(amx, params[arg_value]);
	size_t element = (PARAMS_COUNT == 4) ? *getAmxAddr(amx, params[arg_elem]) : 0;

	return set_member(amx, g_pGameRules, member, value, element);
}

/*
* Returns a value from CSGameRules_Members members
*
* @param member     The specified member, look at the enums with name CSGameRules_Members
*
* @return           If an integer or boolean or one byte, array or everything else is passed via the 3rd argument and more, look at the argument list for the specified member
*
* native any:get_member_game(CSGameRules_Members:member, any:...);
*/
cell AMX_NATIVE_CALL get_member_game(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_member, arg_2, arg_3 };

	CHECK_GAMERULES();

	member_t *member = memberlist[params[arg_member]];
	if (unlikely(member == nullptr)) {
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: unknown member id %i", __FUNCTION__, params[arg_member]);
		return FALSE;
	}

	cell* dest;
	size_t element;
	size_t length;

	if (PARAMS_COUNT == 3) {
		dest = getAmxAddr(amx, params[arg_2]);
		length = *getAmxAddr(amx, params[arg_3]);
		element = 0;
	}
	else if (PARAMS_COUNT == 2) {
		cell* arg2 = getAmxAddr(amx, params[arg_2]);
		if (member->isTypeReturnable()) {
			dest = nullptr;
			element = *arg2;
		}
		else {
			dest = arg2;
			element = 0;
		}
		length = 0;
	}
	else {
		dest = nullptr;
		element = 0;
		length = 0;
	}

	return get_member(amx, g_pGameRules, member, dest, element, length);
}

/*
* Sets entvars data for an entity.
* Use the var_* EntVars enum
*
* native set_entvar(const index, const EntVars:var, any:...);
*/
cell AMX_NATIVE_CALL set_entvar(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index, arg_var, arg_value, arg_elem };
	member_t *member = memberlist[params[arg_var]];

	if (unlikely(member == nullptr)) {
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: unknown member id %i", __FUNCTION__, params[arg_var]);
		return FALSE;
	}
	CHECK_ISENTITY(arg_index);

	edict_t *pEdict = edictByIndexAmx(params[arg_index]);
	if (unlikely(pEdict == nullptr || pEdict->pvPrivateData == nullptr)) {
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: invalid or uninitialized entity", __FUNCTION__);
		return FALSE;
	}

	cell* value = getAmxAddr(amx, params[arg_value]);
	size_t element = (PARAMS_COUNT == 4) ? *getAmxAddr(amx, params[arg_elem]) : 0;

	return set_member(amx, &pEdict->v, member, value, element);
}

/*
* Returns entvar data from an entity.
* Use the var_* EntVars enum
*
* native any:get_entvar(const index, const EntVars:var, any:...);
*/
cell AMX_NATIVE_CALL get_entvar(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index, arg_var, arg_3, arg_4 };
	member_t *member = memberlist[params[arg_var]];

	if (unlikely(member == nullptr)) {
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: unknown member id %i", __FUNCTION__, params[arg_var]);
		return FALSE;
	}

	CHECK_ISENTITY(arg_index);

	edict_t *pEdict = edictByIndexAmx(params[arg_index]);
	if (unlikely(pEdict == nullptr)) {
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: invalid or uninitialized entity", __FUNCTION__);
		return FALSE;
	}

	cell* dest;
	size_t element;
	size_t length;

	if (PARAMS_COUNT == 4) {
		dest = getAmxAddr(amx, params[arg_3]);
		length = *getAmxAddr(amx, params[arg_4]);
		element = 0;
	}
	else if (PARAMS_COUNT == 3) {
		cell* arg3 = getAmxAddr(amx, params[arg_3]);

		if (member->isTypeReturnable())
		{
			if (member->type == MEMBER_FLOAT) {
				dest = arg3;
				element = 0;
			} else {
				dest = nullptr;
				element = *arg3;
			}
		}
		else {
			dest = arg3;
			element = 0;
		}

		length = 0;
	}
	else {
		dest = nullptr;
		element = 0;
		length = 0;
	}

	return get_member(amx, &pEdict->v, member, dest, element, length);
}

/*
* Sets playermove var.
*
* @param var        The specified playermove, look at the enum PlayerMove
*
* @return           1 on success.
*
* native set_pmove(const PlayerMove:var, any:...);
*/
cell AMX_NATIVE_CALL set_pmove(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_var, arg_value, arg_elem };
	member_t *member = memberlist[params[arg_var]];

	if (unlikely(member == nullptr)) {
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: unknown member id %i", __FUNCTION__, params[arg_var]);
		return FALSE;
	}

	cell* value = getAmxAddr(amx, params[arg_value]);
	size_t element = (PARAMS_COUNT == 3) ? *getAmxAddr(amx, params[arg_elem]) : 0;

	return set_member(amx, g_pMove, member, value, element);
}

/*
* Returns a playermove var.
*
* @param var        The specified playermove var, look at the enums PlayerMove
*
* @return           If an integer or boolean or one byte, array or everything else is passed via the 3rd argument and more, look at the argument list for the specified mvar
*
* native any:get_pmove(const PlayerMove:var, any:...);
*/
cell AMX_NATIVE_CALL get_pmove(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_var, arg_2, arg_3 };
	member_t *member = memberlist[params[arg_var]];

	if (unlikely(member == nullptr)) {
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: unknown member id %i", __FUNCTION__, params[arg_var]);
		return FALSE;
	}

	cell* dest;
	size_t element;
	size_t length;

	if (PARAMS_COUNT == 3)
	{
		if (member->type == MEMBER_STRING)
		{
			dest = getAmxAddr(amx, params[arg_2]);
			length = *getAmxAddr(amx, params[arg_3]);
			element = 0;
		}
		else
		{
			dest = getAmxAddr(amx, params[arg_2]);
			element = *getAmxAddr(amx, params[arg_3]);
			length = 0;
		}
	}
	else if (PARAMS_COUNT == 2)
	{
		cell *arg2 = getAmxAddr(amx, params[arg_2]);
		if (member->isTypeReturnable())
		{
			if (member->type == MEMBER_FLOAT) {
				dest = arg2;
				element = 0;
			} else {
				dest = nullptr;
				element = *arg2;
			}
		}
		else {
			dest = arg2;
			element = 0;
		}

		length = 0;
	}
	else
	{
		dest = nullptr;
		element = 0;
		length = 0;
	}

	return get_member(amx, g_pMove, member, dest, element, length);
}

/*
* Sets a movevar value to a playermove.
*
* @param var        The specified mvar, look at the enum MoveVars
*
* @return           1 on success.
*
* native set_movevar(const MoveVars:var, any:...);
*/
cell AMX_NATIVE_CALL set_movevar(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_var, arg_value };
	member_t *member = memberlist[params[arg_var]];

	if (unlikely(member == nullptr)) {
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: unknown member id %i", __FUNCTION__, params[arg_var]);
		return FALSE;
	}

	cell* value = getAmxAddr(amx, params[arg_value]);
	return set_member(amx, g_pMove->movevars, member, value, 0);
}

/*
* Returns a movevar value from a playermove.
*
* @param var        The specified mvar, look at the enum MoveVars
*
* @return           If an integer or boolean or one byte, array or everything else is passed via the 3rd argument and more, look at the argument list for the specified mvar
*
* native any:get_movevar(const MoveVars:var, any:...);
*/
cell AMX_NATIVE_CALL get_movevar(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_var, arg_2, arg_3 };
	member_t *member = memberlist[params[arg_var]];

	if (unlikely(member == nullptr)) {
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: unknown member id %i", __FUNCTION__, params[arg_var]);
		return FALSE;
	}

	cell* dest;
	size_t element;
	size_t length;

	if (PARAMS_COUNT == 3) {
		dest = getAmxAddr(amx, params[arg_2]);
		length = *getAmxAddr(amx, params[arg_3]);
		element = 0;
	}
	else {
		dest = nullptr;
		element = 0;
		length = 0;
	}

	return get_member(amx, g_pMove->movevars, member, dest, element, length);
}

/*
* Sets usercmd data.
* Use the ucmd_* UCmd enum
*
* native set_ucmd(const ucmd, const UCmd:var, any:...);
*/
cell AMX_NATIVE_CALL set_ucmd(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_cmd, arg_var, arg_value };
	member_t *member = memberlist[params[arg_var]];

	if (unlikely(member == nullptr)) {
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: unknown member id %i", __FUNCTION__, params[arg_var]);
		return FALSE;
	}

	cell* cmd = (cell *)params[arg_cmd];
	cell* value = getAmxAddr(amx, params[arg_value]);
	return set_member(amx, cmd, member, value, 0);
}

/*
* Returns usercmd data from an entity.
* Use the ucmd_* UCmd enum
*
* native any:get_ucmd(const ucmd, const UCmd:var, any:...);
*/
cell AMX_NATIVE_CALL get_ucmd(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_cmd, arg_var, arg_3 };
	member_t *member = memberlist[params[arg_var]];

	if (unlikely(member == nullptr)) {
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: unknown member id %i", __FUNCTION__, params[arg_var]);
		return FALSE;
	}

	cell* dest;
	size_t element;

	if (PARAMS_COUNT == 3) {
		dest = getAmxAddr(amx, params[arg_3]);
		element = 0;
	}
	else {
		dest = nullptr;
		element = 0;
	}

	cell* cmd = (cell *)params[arg_cmd];
	return get_member(amx, cmd, member, dest, element);
}

/*
* Sets a pmtrace var.
*
* @param var        The specified mvar, look at the enum PMTrace
*
* @return           1 on success.
*
* native set_pmtrace(const tracehandle, const PMTrace:var, any:...);
*/
cell AMX_NATIVE_CALL set_pmtrace(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_tr, arg_var, arg_value };
	member_t *member = memberlist[params[arg_var]];

	if (unlikely(member == nullptr)) {
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: unknown member id %i", __FUNCTION__, params[arg_var]);
		return FALSE;
	}

	cell* tr = (cell *)params[arg_tr];
	cell* value = getAmxAddr(amx, params[arg_value]);
	return set_member(amx, tr, member, value, 0);
}

/*
* Returns a pmtrace var
*
* @param var        The specified mvar, look at the enum PMTrace
*
* @return           If an integer or boolean or one byte, array or everything else is passed via the 3rd argument and more, look at the argument list for the specified mvar
*
* native any:get_pmtrace(const tracehandle, const PMTrace:var, any:...);
*/
cell AMX_NATIVE_CALL get_pmtrace(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_tr, arg_var, arg_3 };
	member_t *member = memberlist[params[arg_var]];

	if (unlikely(member == nullptr)) {
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: unknown member id %i", __FUNCTION__, params[arg_var]);
		return FALSE;
	}

	cell* dest;
	size_t element;

	if (PARAMS_COUNT == 3) {
		dest = getAmxAddr(amx, params[arg_3]);
		element = 0;
	}
	else {
		dest = nullptr;
		element = 0;
	}

	cell* tr = (cell *)params[arg_tr];
	return get_member(amx, tr, member, dest, element);
}

/*
* Sets netchan data.
* Use the net_* NetChan enum
*
* native set_netchan(const index, const NetChan:var, any:...);
*/
cell AMX_NATIVE_CALL set_netchan(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index, arg_var, arg_value };

	CHECK_ISPLAYER(arg_index);

	client_t *pClient = clientOfIndex(params[arg_index]);
	CHECK_CLIENT_CONNECTED(pClient, arg_index);

	member_t *member = memberlist[params[arg_var]];
	if (unlikely(member == nullptr)) {
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: unknown member id %i", __FUNCTION__, params[arg_var]);
		return FALSE;
	}

	cell* value = getAmxAddr(amx, params[arg_value]);
	return set_member(amx, &pClient->netchan, member, value, 0);
}

/*
* Returns metchan data from an client.
* Use the net_* NetChan enum
*
* native any:get_netchan(const index, const NetChan:var, any:...);
*/
cell AMX_NATIVE_CALL get_netchan(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index, arg_var, arg_3 };

	CHECK_ISPLAYER(arg_index);

	client_t *pClient = clientOfIndex(params[arg_index]);
	CHECK_CLIENT_CONNECTED(pClient, arg_index);

	member_t *member = memberlist[params[arg_var]];
	if (unlikely(member == nullptr)) {
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: unknown member id %i", __FUNCTION__, params[arg_var]);
		return FALSE;
	}

	cell* dest;
	size_t element;

	if (PARAMS_COUNT == 3) {
		dest = getAmxAddr(amx, params[arg_3]);
		element = 0;
	}
	else {
		dest = nullptr;
		element = 0;
	}

	return get_member(amx, &pClient->netchan, member, dest, element);
}

/*
* Sets a NetAdr var.
*
* @param var        The specified mvar, look at the enum NetAdrVars
*
* @return           1 on success.
*
* native set_netadr(const adr, const NetAdrVars:var, any:...);
*/
cell AMX_NATIVE_CALL set_netadr(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_adr, arg_var, arg_value };
	member_t *member = memberlist[params[arg_var]];

	if (unlikely(member == nullptr)) {
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: unknown member id %i", __FUNCTION__, params[arg_var]);
		return FALSE;
	}

	netadr_t *adr = (netadr_t *)params[arg_adr];
	if (unlikely(adr == nullptr)) {
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: Invalid network address", __FUNCTION__);
		return FALSE;
	}

	switch (params[arg_var])
	{
	case netadr_type:
		adr->type = (netadrtype_t)params[arg_value];
		break;
	case netadr_port:
		adr->port = ntohs(params[arg_value] & 0xFFFF); // cap short
		break;
	case netadr_ip:
		*(size_t *)adr->ip = htonl(params[arg_value] & 0xFFFFFFFF); // cap int
		break;
	default:
		return FALSE;
	}

	return TRUE;
}

/*
* Returns a NetAdr var
*
* @param var        The specified mvar, look at the enum NetAdrVars
*
* @return           If an integer or boolean or one byte, array or everything else is passed via the 3rd argument and more, look at the argument list for the specified mvar
*
* native any:get_netadr(const adr, const NetAdrVars:var, any:...);
*/
cell AMX_NATIVE_CALL get_netadr(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_adr, arg_var, arg_3, arg_4 };
	member_t *member = memberlist[params[arg_var]];

	if (unlikely(member == nullptr)) {
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: unknown member id %i", __FUNCTION__, params[arg_var]);
		return FALSE;
	}

	netadr_t *adr = (netadr_t *)params[arg_adr];
	if (unlikely(adr == nullptr)) {
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: Invalid network address", __FUNCTION__);
		return FALSE;
	}

	switch (params[arg_var])
	{
	case netadr_type: return adr->type;
	case netadr_port: return ntohs(adr->port);
	case netadr_ip:
	{
		if (PARAMS_COUNT == 4)
		{
			cell *dest = getAmxAddr(amx, params[arg_3]);
			size_t length = *getAmxAddr(amx, params[arg_4]);
			setAmxString(dest, NET_AdrToString(*adr, true /*no port*/), length);
		}

		return htonl(*(size_t *)adr->ip);
	}
	default:
		break;
	}

	return FALSE;
}

/*
* Sets a RebuyStruct member.
*
* @param var        The specified RebuyStruct, look at the enum RebuyStruct
*
* @return           1 on success.
*
* native set_rebuy(const rebuyhandle, const RebuyStruct:member, any:value);
*/
cell AMX_NATIVE_CALL set_rebuy(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_handle, arg_member, arg_value };

	member_t *member = memberlist[params[arg_member]];
	if (unlikely(member == nullptr)) {
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: unknown member id %i", __FUNCTION__, params[arg_member]);
		return FALSE;
	}

	cell *handle = (cell *)params[arg_handle];
	if (unlikely(handle == nullptr)) {
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: invalid rebuy handle", __FUNCTION__);
		return FALSE;
	}

	return set_member(amx, handle, member, &params[arg_value], 0);
}

/*
* Returns a RebuyStruct member
*
* @param var        The specified RebuyStruct, look at the enum RebuyStruct
*
* @return           If an integer or boolean or one byte, array or everything else is passed via the 3rd argument and more, look at the argument list for the specified RebuyStruct
*
* native any:get_rebuy(const rebuyhandle, RebuyStruct:member);
*/
cell AMX_NATIVE_CALL get_rebuy(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_handle, arg_member };

	member_t *member = memberlist[params[arg_member]];
	if (unlikely(member == nullptr)) {
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: unknown member id %i", __FUNCTION__, params[arg_member]);
		return FALSE;
	}

	cell *handle = (cell *)params[arg_handle];
	if (unlikely(handle == nullptr)) {
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: invalid rebuy handle", __FUNCTION__);
		return FALSE;
	}

	return get_member(amx, handle, member, nullptr, 0);
}

AMX_NATIVE_INFO EngineVars_Natives[] =
{
	{ "set_entvar", set_entvar },
	{ "get_entvar", get_entvar },

	{ "set_ucmd", set_ucmd },
	{ "get_ucmd", get_ucmd },

	{ "set_netchan", set_netchan },
	{ "get_netchan", get_netchan },

	{ "set_netadr", set_netadr },
	{ "get_netadr", get_netadr },

	{ "set_rebuy", set_rebuy },
	{ "get_rebuy", get_rebuy },

	{ nullptr, nullptr }
};

AMX_NATIVE_INFO ReGameVars_Natives[] =
{
	{ "set_member", set_member },
	{ "get_member", get_member },

	{ "set_member_s", set_member_s },
	{ "get_member_s", get_member_s },

	{ "set_member_game", set_member_game },
	{ "get_member_game", get_member_game },

	{ "set_pmove", set_pmove },
	{ "get_pmove", get_pmove },

	{ "set_movevar", set_movevar },
	{ "get_movevar", get_movevar },

	{ "set_pmtrace", set_pmtrace },
	{ "get_pmtrace", get_pmtrace },

	{ nullptr, nullptr }
};

void RegisterNatives_Members()
{
	if (!api_cfg.hasReGameDLL())
		fillNatives(ReGameVars_Natives, [](AMX *amx, cell *params) -> cell { AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: isn't available", "ReGameDll"); return FALSE; });

	g_amxxapi.AddNatives(ReGameVars_Natives);
	g_amxxapi.AddNatives(EngineVars_Natives);
}

cell set_member(AMX *amx, void* pdata, const member_t *member, cell* value, size_t element)
{
	if (!pdata) {
		return FALSE;
	}

	char string[2048];
	switch (member->type) {
	case MEMBER_CLASSPTR:
		{
			AMX_ENTITY_VALIDATE(*value, member->name);
			// native set_member(_index, any:_member, _value, _elem);
			CBaseEntity *pEntity = getPrivate<CBaseEntity>(*value);
			set_member<CBaseEntity *>(pdata, member->offset, pEntity, element);
			return TRUE;
		}
	case MEMBER_EHANDLE:
		{
			AMX_ENTITY_VALIDATE(*value, member->name);
			// native set_member(_index, any:_member, _value, _elem);
			EHANDLE& ehandle = get_member<EHANDLE>(pdata, member->offset, element);
			edict_t *pEdictValue = edictByIndexAmx(*value);
			ehandle.Set(pEdictValue);
			return TRUE;
		}
	case MEMBER_EDICT:
		{
			AMX_ENTITY_VALIDATE(*value, member->name);
			// native set_member(_index, any:_member, _value, _elem);
			edict_t *pEdictValue = edictByIndexAmx(*value);
			set_member<edict_t *>(pdata, member->offset, pEdictValue, element);
			return TRUE;
		}
	case MEMBER_EVARS:
		{
			AMX_ENTITY_VALIDATE(*value, member->name);
			// native set_member(_index, any:_member, _value, _elem);
			entvars_t *pev = PEV(*value);
			set_member<entvars_t *>(pdata, member->offset, pev, element);
			return TRUE;
		}
	case MEMBER_VECTOR:
		{
			// native set_member(_index, any:_member, Float:_value[3], _elem);
			Vector& pSource = *(Vector *)value;
			set_member<Vector>(pdata, member->offset, pSource, element);
			return TRUE;
		}
	case MEMBER_STRING:
		{
			// native set_member(_index, any:_member, const source[]);
			if (member->max_size > sizeof(char*)) {
				// char []
				const char *source = getAmxString(value, string);
				char *dest = get_member_direct<char>(pdata, member->offset);
				strncpy(dest, source, member->max_size - 1);
				dest[member->max_size - 1] = '\0';

			} else {
				// char *
				const char *source = getAmxString(value, string);
				char *&dest = get_member<char *>(pdata, member->offset);
				g_ReGameFuncs->ChangeString(dest, source);
			}

			return TRUE;
		}
	case MEMBER_QSTRING:
		{
			const char *source = getAmxString(value, string);
			string_t newstr = (source && source[0] != '\0') ? ALLOC_STRING(source) : iStringNull;
			set_member<string_t>(pdata, member->offset, newstr, element);
			return (cell)newstr;
		}
	case MEMBER_FLOAT:
	case MEMBER_INTEGER:
		{
			// native set_member(_index, any:_member, any:_value, _elem);
			set_member<int>(pdata, member->offset, *value, element);
			return TRUE;
		}
	case MEMBER_SHORT:
		{
			// native set_member(_index, any:_member, _value, _elem);
			set_member<short>(pdata, member->offset, *value, element);
			return TRUE;
		}
	case MEMBER_BYTE:
		{
			// native set_member(_index, any:_member, _value, _elem);
			set_member<byte>(pdata, member->offset, *value, element);
			return TRUE;
		}
	case MEMBER_BOOL:
		{
			// native set_member(_index, any:_member, bool:_value, _elem);
			set_member<bool>(pdata, member->offset, *value != 0, element);
			return TRUE;
		}
	case MEMBER_SIGNALS:
		{
			enum { _Signal, _State };

			// native set_member(_index, any:_member, signals[UnifiedSignals]);
			CUnifiedSignals& signal = get_member<CUnifiedSignals>(pdata, member->offset, element);

			int *pSignals = value;
			signal.m_flSignal = pSignals[_Signal];
			signal.m_flState = pSignals[_State];

			return TRUE;
		}
	case MEMBER_DOUBLE:
		{
			// native set_member(_index, any:_member, any:_value, _elem);
			set_member<double>(pdata, member->offset, *(float *)value, element);
			return TRUE;
		}
	case MEMBER_TRACERESULT:
		{
			// native set_member(_index, any:_member, any:_value, _elem);
			set_member<TraceResult>(pdata, member->offset, *(TraceResult *)value, element);
			return TRUE;
		}
	case MEMBER_ENTITY:
	case MEBMER_REBUYSTRUCT:
	case MEMBER_PMTRACE:
	case MEBMER_USERCMD:
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: member type %s (%s) is not supported", __FUNCTION__, member_t::getTypeString(member->type), member->name);
		return FALSE;
	default: break;
	}

	return FALSE;
}

cell get_member(AMX *amx, void* pdata, const member_t *member, cell* dest, size_t element, size_t length)
{
	if (!pdata) {
		return 0;
	}

	switch (member->type)
	{
	case MEMBER_CLASSPTR:
		{
			// native any:get_member(_index, any:_member, element);
			auto& pEntity = get_member<CBaseEntity *>(pdata, member->offset, element);
			return pEntity ? indexOfEdict(pEntity->pev) : AMX_NULLENT;
		}
	case MEMBER_EHANDLE:
		{
			// native any:get_member(_index, any:_member, element);
			EHANDLE ehandle = get_member<EHANDLE>(pdata, member->offset, element);
			edict_t *pEntity = ehandle.Get();
			return pEntity ? indexOfEdict(pEntity) : AMX_NULLENT;
		}
	case MEMBER_EDICT:
		{
			// native any:get_member(_index, any:_member, element);
			edict_t *pEntity = get_member<edict_t *>(pdata, member->offset, element);
			return pEntity ? indexOfEdict(pEntity) : AMX_NULLENT;
		}
	case MEMBER_EVARS:
		{
			// native any:get_member(_index, any:_member, element);
			entvars_t *pev = get_member<entvars_t *>(pdata, member->offset, element);
			return pev ? indexOfEdict(pev) : AMX_NULLENT;
		}
	case MEMBER_VECTOR:
		{
			// native any:get_member(_index, any:_member, any:output[], element);
			if (!dest)
				return 0;

			*(Vector *)dest = get_member<Vector>(pdata, member->offset, element);
			return 1;
		}
	case MEMBER_STRING:
		{
			// native any:get_member(_index, any:_member, any:output[], maxlength);
			if (unlikely(!dest))
				return 0;

			if (member->max_size > sizeof(char*)) {
				// char []
				const char *src = get_member_direct<char>(pdata, member->offset);
				setAmxString(dest, src, length);
			} else {
				// char *
				const char *src = get_member<const char *>(pdata, member->offset);
				if (src == nullptr) {
					setAmxString(dest, "", 1);
					return 0;
				}
				setAmxString(dest, src, length);
			}

			return 1;
		}
	case MEMBER_QSTRING:
		{
			if (unlikely(!dest))
				return 0;

			string_t str = get_member<string_t>(pdata, member->offset, element);
			if (FStringNull(str)) {
				setAmxString(dest, "", 1);
				return 0;
			}

			setAmxString(dest, STRING(str), length);
			return (cell)str;
		}
	case MEMBER_FLOAT:
	case MEMBER_INTEGER:
		{
			auto& ret = get_member<int>(pdata, member->offset, element);
			if (dest)
				*dest = ret;

			return ret;
		}
	case MEMBER_SHORT:
		// native any:get_member(_index, any:_member, element);
		return get_member<short>(pdata, member->offset, element);
	case MEMBER_BYTE:
		// native any:get_member(_index, any:_member, element);
		return get_member<byte>(pdata, member->offset, element);
	case MEMBER_BOOL:
		// native any:get_member(_index, any:_member, element);
		return get_member<bool>(pdata, member->offset, element);
	case MEMBER_DOUBLE:
	{
		// native any:get_member(_index, any:_member, element);
		auto ret = (float)get_member<double>(pdata, member->offset, element);
		return (cell &)ret;
	}
	case MEMBER_SIGNALS:
		{
			enum { _Signal, _State };

			// native any:get_member(_index, any:_member, signals[2]);
			if (unlikely(!dest))
				return 0;

			CUnifiedSignals& signal = get_member<CUnifiedSignals>(pdata, member->offset);

			int *pSignals = dest;
			pSignals[_Signal] = signal.GetSignal();
			pSignals[_State] = signal.GetState();
			return 1;
		}
	case MEMBER_ENTITY:
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: member type %s (%s) is not supported", __FUNCTION__, member_t::getTypeString(member->type), member->name);
		return FALSE;
	case MEMBER_TRACERESULT:
		return (cell)get_member_direct<TraceResult>(pdata, member->offset, element);
	case MEBMER_REBUYSTRUCT:
		return (cell)get_member_direct<RebuyStruct>(pdata, member->offset, element);
	case MEMBER_PMTRACE:
		return (cell)get_member_direct<pmtrace_s>(pdata, member->offset, element);
	case MEBMER_USERCMD:
		return (cell)get_member_direct<usercmd_s>(pdata, member->offset, element);
	case MEMBER_NETADR:
		return (cell)get_member_direct<netadr_t>(pdata, member->offset, element);
	default: break;
	}

	return 0;
}

void *get_pdata_custom(CBaseEntity *pEntity, cell member)
{
	const auto table = memberlist_t::members_tables_e(member / MAX_REGION_RANGE);
	switch (table) {
	case memberlist_t::mt_csentity:
	case memberlist_t::mt_csplayer:
	case memberlist_t::mt_csplayerweapon: {
		if (unlikely(pEntity->m_pEntity == nullptr)) {
			return nullptr;
		}

		return pEntity->m_pEntity;
	}
	default:
		break;
	}

	return pEntity;
}
