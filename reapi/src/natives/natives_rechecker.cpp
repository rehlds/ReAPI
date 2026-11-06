#include "precompiled.h"

/*
* Send request the file for the client to get hash
*
* @param file           The file (Can contain a relative path to the file)
* @param function       The forward to call
* @param flag_type      The request type, can be only RES_TYPE_EXISTS, RES_TYPE_MISSING or RES_TYPE_HASH_ANY
* @param hash           Hash of file to request.
*
* @return               Returns a hook handle. Use UnRegisterQueryFile to unregister the forward
*
* native QueryFileHook:RegisterQueryFile(const file[], const function[], const ResourceType:flag_type, const hash = -1);
*/
cell AMX_NATIVE_CALL RegisterQueryFile(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_file, arg_handler, arg_flag_type, arg_hash };

	ResourceType_e flag = (ResourceType_e)params[arg_flag_type];
	switch (flag)
	{
	case RES_TYPE_NONE:
	case RES_TYPE_IGNORE:
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: invalid flag type \"%i\".", __FUNCTION__, flag);
		return FALSE;
	case RES_TYPE_EXISTS:
		if (params[arg_hash] == -1) {
			flag = RES_TYPE_HASH_ANY;
		}
		break;
	default:
		break;
	}

	char filename[MAX_PATH];
	const char *file = getAmxString(amx, params[arg_file], filename);
	if (!file || file[0] == '\0') {
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: file can not be empty.", __FUNCTION__);
		return FALSE;
	}

	char funcname[256];
	const char *func = getAmxString(amx, params[arg_handler], funcname);
	
	int funcid;
	if (unlikely(g_amxxapi.amx_FindPublic(amx, func, &funcid) != AMX_ERR_NONE))
	{
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: public function \"%s\" not found.", __FUNCTION__, func);
		return FALSE;
	}

	return g_queryFileManager.Add(amx, filename, func, flag, params[arg_hash]);
}

/*
* Unregister the forward.
* Use the return value from RegisterQueryFile as the parameter here!
*
* @param hook           The hook to remove
*
* @return               Returns true if the hook is successfully removed, otherwise false
*
* native UnRegisterQueryFile(const QueryFileHook:index);
*/
cell AMX_NATIVE_CALL UnRegisterQueryFile(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index };
	return g_queryFileManager.Remove(params[arg_index])? TRUE : FALSE;
}

AMX_NATIVE_INFO Rechecker_Natives[] =
{
	{ "RegisterQueryFile",   RegisterQueryFile   },
	{ "UnRegisterQueryFile", UnRegisterQueryFile },

	{ nullptr, nullptr }
};

void RegisterNatives_Rechecker()
{
	if (!api_cfg.hasRechecker())
		fillNatives(Rechecker_Natives, [](AMX *amx, cell *params) -> cell { AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: isn't available", "Rechecker"); return FALSE; });

	g_amxxapi.AddNatives(Rechecker_Natives);
}
