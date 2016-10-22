#include "precompiled.h"

/*
* Checks whether the player is talking at this moment
*
* @param index		Client index
* @return		true if client is speaking, false otherwise
*
* native bool:VTC_IsClientSpeaking(const index);
*/
cell AMX_NATIVE_CALL VTC_IsClientSpeaking(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index };

	CHECK_ISPLAYER(arg_index);

	return (cell)g_pVoiceTranscoderApi->IsClientSpeaking((size_t)params[arg_index]);
}

/*
* Mutes this player
*
* @param index		Client index
* @noreturn
*
* native VTC_MuteClient(const index);
*/
cell AMX_NATIVE_CALL VTC_MuteClient(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index };

	CHECK_ISPLAYER(arg_index);

	g_pVoiceTranscoderApi->MuteClient((size_t)params[arg_index]);
	return FALSE;
}

/*
* Unmutes this player
*
* @param index		Client index
* @noreturn
*
* native VTC_UnmuteClient(const index);
*/
cell AMX_NATIVE_CALL VTC_UnmuteClient(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index };

	CHECK_ISPLAYER(arg_index);

	g_pVoiceTranscoderApi->UnmuteClient((size_t)params[arg_index]);
	return FALSE;
}

AMX_NATIVE_INFO Vtc_Natives[] =
{
	{ "VTC_IsClientSpeaking", VTC_IsClientSpeaking },
	{ "VTC_MuteClient", VTC_MuteClient },
	{ "VTC_UnmuteClient", VTC_UnmuteClient },

	{ nullptr, nullptr }
};

/*
* Get client protocol
*
* @param index		Client index
* @return		client protocol
*
* native REU_GetProtocol(const index);
*/
cell AMX_NATIVE_CALL REU_GetProtocol(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index };

	CHECK_ISPLAYER(arg_index);

	return g_ReunionApi->GetClientProtocol(params[arg_index] - 1);
}

/*
* Get client auth type
*
* @param index		Client index
* @return		client auth type
*
* native REU_GetAuthtype(const index);
*/
cell AMX_NATIVE_CALL REU_GetAuthtype(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index };

	CHECK_ISPLAYER(arg_index);

	return g_ReunionApi->GetClientAuthtype(params[arg_index] - 1);
}

/*
* Check if client is a steam player
*
* @param index		Client index
* @return		1/0
*
* native REU_IsUserSteam(const index);
*/
cell AMX_NATIVE_CALL REU_IsUserSteam(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index };

	CHECK_ISPLAYER(arg_index);

	int clientId = params[arg_index] - 1;
	if (g_ReunionApi->GetClientAuthtype(clientId) == DP_AUTH_STEAM)
		return TRUE;
	
	return FALSE;
}

/*
* Check if client running RevEmu with limited user rights
*
* @param index		Client index
* @return		1/0
*
* native REU_IsRevemuWithoutAdminRights(const index);
*/
cell AMX_NATIVE_CALL REU_IsRevemuWithoutAdminRights(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index };

	CHECK_ISPLAYER(arg_index);

	int clientId = params[arg_index] - 1;
	if (g_ReunionApi->GetClientAuthtype(clientId) != DP_AUTH_REVEMU)
		return FALSE;

	char buffer[256];
	size_t size = g_ReunionApi->GetClientAuthdata(clientId, buffer, sizeof buffer);

	for (size_t i = 0; i < size; i++) {
		if (!isdigit(buffer[i]))
			return FALSE;
	}

	return TRUE;
}

AMX_NATIVE_INFO Reunion_Natives[] =
{
	{ "REU_GetProtocol", REU_GetProtocol },
	{ "REU_GetAuthtype", REU_GetAuthtype },
	{ "REU_IsUserSteam", REU_IsUserSteam },
	{ "REU_IsRevemuWithoutAdminRights", REU_IsRevemuWithoutAdminRights },

	{ nullptr, nullptr }
};

void RegisterNatives_Addons()
{
	if (!api_cfg.hasVTC())
		fillNatives(Vtc_Natives, [](AMX *amx, cell *params) -> cell { MF_LogError(amx, AMX_ERR_NATIVE, "%s: isn't available", "VTC"); return FALSE; });

	if (!api_cfg.hasReunion())
		fillNatives(Reunion_Natives, [](AMX *amx, cell *params) -> cell { MF_LogError(amx, AMX_ERR_NATIVE, "%s: isn't available", "Reunion"); return FALSE; });

	g_amxxapi.AddNatives(Vtc_Natives);
	g_amxxapi.AddNatives(Reunion_Natives);
}
