// vim: set ts=4 sw=4 tw=99 noet:
//
// AMX Mod X, based on AMX Mod by Aleksander Naszko ("OLO").
// Copyright (C) The AMX Mod X Development Team.
// Parts Copyright (C) 2001-2003 Will Day <willday@hpgx.net>
//
// This software is licensed under the GNU General Public License, version 3 or higher.
// Additional exceptions apply. For full license details, see LICENSE.txt or visit:
//     https://alliedmods.net/amxmodx-license

//
// Module SDK
//

#include "precompiled.h"

// Module info
static amxx_module_info_s g_ModuleInfo =
{
	Plugin_info.name,
	Plugin_info.author,
	Plugin_info.version,
	FALSE,
	Plugin_info.logtag,
	"reapi",
	"reapi"
};

// Storage for the requested functions
amxxapi_t g_amxxapi;

#define DECLARE_REQ(x)	{#x, offsetof(amxxapi_t, x)}

static struct funcreq_t
{
	const char * name;
	size_t offset;
} g_funcrequests[] =
{
	DECLARE_REQ(AddNatives),
	//DECLARE_REQ(AddNewNatives),
	//DECLARE_REQ(BuildPathname),
	//DECLARE_REQ(BuildPathnameR),
	DECLARE_REQ(GetAmxAddr),
	//DECLARE_REQ(GetAmxVectorNull),			// AMXX 1.8.3-dev
	//DECLARE_REQ(PrintSrvConsole),
	//DECLARE_REQ(GetModname),
	DECLARE_REQ(GetAmxScriptName),
	//DECLARE_REQ(GetAmxScript),
	DECLARE_REQ(FindAmxScriptByAmx),
	//DECLARE_REQ(FindAmxScriptByName),
	DECLARE_REQ(SetAmxString),
	//DECLARE_REQ(SetAmxStringUTF8Char),			// AMXX 1.8.3-dev
	//DECLARE_REQ(SetAmxStringUTF8Cell),			// AMXX 1.8.3-dev
	DECLARE_REQ(GetAmxString),
	//DECLARE_REQ(GetAmxStringNull),			// AMXX 1.8.3-dev
	//DECLARE_REQ(GetAmxStringLen),
	//DECLARE_REQ(FormatAmxString),
	//DECLARE_REQ(CopyAmxMemory),
	DECLARE_REQ(Log),
	DECLARE_REQ(LogError),
	//DECLARE_REQ(RaiseAmxError),
	DECLARE_REQ(RegisterForward),
	DECLARE_REQ(ExecuteForward),
	//DECLARE_REQ(PrepareCellArray),
	//DECLARE_REQ(PrepareCharArray),
	DECLARE_REQ(PrepareCellArrayA),
	//DECLARE_REQ(PrepareCharArrayA),
	//DECLARE_REQ(IsPlayerValid),
	//DECLARE_REQ(GetPlayerName),
	//DECLARE_REQ(GetPlayerIP),
	//DECLARE_REQ(IsPlayerInGame),
	//DECLARE_REQ(IsPlayerBot),
	//DECLARE_REQ(IsPlayerAuthorized),
	//DECLARE_REQ(GetPlayerTime),
	//DECLARE_REQ(GetPlayerPlayTime),
	//DECLARE_REQ(GetPlayerCurweapon),
	//DECLARE_REQ(GetPlayerTeam),
	//DECLARE_REQ(GetPlayerTeamID),
	//DECLARE_REQ(GetPlayerDeaths),
	//DECLARE_REQ(GetPlayerMenu),
	//DECLARE_REQ(GetPlayerKeys),
	//DECLARE_REQ(IsPlayerAlive),
	//DECLARE_REQ(GetPlayerFrags),
	//DECLARE_REQ(IsPlayerConnecting),
	//DECLARE_REQ(IsPlayerHLTV),
	//DECLARE_REQ(GetPlayerArmor),
	//DECLARE_REQ(GetPlayerHealth),
	//DECLARE_REQ(amx_Exec),
	//DECLARE_REQ(amx_Execv),
	//DECLARE_REQ(amx_Allot),
	DECLARE_REQ(amx_FindPublic),
	//DECLARE_REQ(LoadAmxScript),
	//DECLARE_REQ(UnloadAmxScript),
	//DECLARE_REQ(RealToCell),
	//DECLARE_REQ(CellToReal),
	DECLARE_REQ(RegisterSPForward),
	DECLARE_REQ(RegisterSPForwardByName),
	DECLARE_REQ(UnregisterSPForward),
	//DECLARE_REQ(MergeDefinitionFile),
	//DECLARE_REQ(amx_FindNative),
	//DECLARE_REQ(GetPlayerFlags),
	//DECLARE_REQ(GetPlayerEdict),
	//DECLARE_REQ(Format),
	//DECLARE_REQ(RegisterFunction),
	//DECLARE_REQ(RequestFunction),
	//DECLARE_REQ(amx_Push),
	DECLARE_REQ(SetPlayerTeamInfo),
	//DECLARE_REQ(PlayerPropAddr),
	//DECLARE_REQ(RegAuthFunc),
	//DECLARE_REQ(UnregAuthFunc),
	//DECLARE_REQ(FindLibrary),
	//DECLARE_REQ(AddLibraries),
	//DECLARE_REQ(RemoveLibraries),
	//DECLARE_REQ(OverrideNatives),
	//DECLARE_REQ(GetLocalInfo),
	//DECLARE_REQ(AmxReregister),
	//DECLARE_REQ(RegisterFunctionEx),
	//DECLARE_REQ(MessageBlock),
};

C_DLLEXPORT int AMXX_Query(int *interfaceVersion, amxx_module_info_s *moduleInfo)
{
	// check parameters
	if (!interfaceVersion || !moduleInfo)
		return AMXX_PARAM;

	// check interface version
	if (*interfaceVersion != AMXX_INTERFACE_VERSION)
	{
		// Tell amxx core our interface version
		*interfaceVersion = AMXX_INTERFACE_VERSION;
		return AMXX_IFVERS;
	}

	// copy module info
	memcpy(moduleInfo, &g_ModuleInfo, sizeof(amxx_module_info_s));
	return AMXX_OK;
}

C_DLLEXPORT int AMXX_CheckGame(const char *game)
{
	return AMXX_GAME_OK;
}

C_DLLEXPORT int AMXX_Attach(PFN_REQ_FNPTR reqFnptrFunc)
{
	// Check pointer
	if (!reqFnptrFunc)
		return AMXX_PARAM;

	for (size_t i = 0; i < ARRAYSIZE(g_funcrequests); i++)
	{
		void* fptr = reqFnptrFunc(g_funcrequests[i].name);
		if (fptr == NULL)
		{
			return AMXX_FUNC_NOT_PRESENT;
		}
		*(void **)((unsigned long)&g_amxxapi + g_funcrequests[i].offset) = fptr;
	}

	OnAmxxAttach();

	RegisterNatives_HookChains();
	RegisterNatives_HookMessage();
	RegisterNatives_Members();
	RegisterNatives_Misc();
	RegisterNatives_VTC();
	RegisterNatives_Rechecker();
	RegisterNatives_Reunion();
	RegisterNatives_Common();

	return AMXX_OK;
}

C_DLLEXPORT int AMXX_Detach()
{
	return AMXX_OK;
}

C_DLLEXPORT int AMXX_PluginsLoaded()
{
	int iFwd = g_amxxapi.RegisterForward("__reapi_version_check", ET_IGNORE, FP_CELL, FP_CELL, FP_DONE);
	g_amxxapi.ExecuteForward(iFwd, REAPI_VERSION_MAJOR, REAPI_VERSION_MINOR);

	if (api_cfg.hasVTC()) {

		g_iClientStartSpeak = g_amxxapi.RegisterForward("VTC_OnClientStartSpeak", ET_IGNORE, FP_CELL, FP_DONE);
		g_iClientStopSpeak = g_amxxapi.RegisterForward("VTC_OnClientStopSpeak", ET_IGNORE, FP_CELL, FP_DONE);
	}

	return AMXX_OK;
}

C_DLLEXPORT void AMXX_PluginsUnloaded()
{
}

C_DLLEXPORT void AMXX_PluginsUnloading()
{
}

NOINLINE void AMXX_Log(const char *fmt, ...)
{
	char msg[2048];
	va_list arglst;
	va_start(arglst, fmt);
	vsnprintf(msg, sizeof msg, fmt, arglst);
	va_end(arglst);

	g_amxxapi.Log("[%s] %s", g_ModuleInfo.logtag, msg);
}

NOINLINE void AMXX_LogError(AMX *amx, int err, const char *fmt, ...)
{
	char msg[2048];
	va_list arglst;
	va_start(arglst, fmt);
	vsnprintf(msg, sizeof msg, fmt, arglst);
	va_end(arglst);

	g_amxxapi.LogError(amx, err, "[%s] %s", g_ModuleInfo.logtag, msg);
}

char* getAmxString(cell* src, char* dest, size_t max, size_t* len)
{
	char* start = dest;

	while (*src && --max)
		*dest++ = (char)*src++;
	*dest = '\0';

	if (len)
		*len = dest - start;

	return start;
}

void setAmxString(cell* dest, const char* string, size_t max)
{
	if (!dest || max == 0)
		return;

	// reserve 1 cell for term null
	size_t limit = max - 1;

	while (*string && limit--)
		*dest++ = (cell)*string++;

	*dest = 0;
}
