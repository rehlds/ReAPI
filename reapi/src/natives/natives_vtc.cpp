#include "precompiled.h"

/*
* Checks whether the player is talking at the moment.
*
* @param index      Client index
*
* @return           true if client is speaking, false otherwise
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
* Mutes the player.
*
* @param index      Client index
*
* @noreturn
*
* native VTC_MuteClient(const index);
*/
cell AMX_NATIVE_CALL VTC_MuteClient(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index };

	CHECK_ISPLAYER(arg_index);

	g_pVoiceTranscoderApi->MuteClient((size_t)params[arg_index]);
	return TRUE;
}

/*
* Unmutes the player.
*
* @param index      Client index
*
* @noreturn
*
* native VTC_UnmuteClient(const index);
*/
cell AMX_NATIVE_CALL VTC_UnmuteClient(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index };

	CHECK_ISPLAYER(arg_index);

	g_pVoiceTranscoderApi->UnmuteClient((size_t)params[arg_index]);
	return TRUE;
}

/*
* Checks whether the player is muted at the moment.
*
* @param index      Client index
*
* @return           true if client is muted, false otherwise
*
* native bool:VTC_IsClientMuted(const index);
*/
cell AMX_NATIVE_CALL VTC_IsClientMuted(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index };

	CHECK_ISPLAYER(arg_index);

	return (cell)g_pVoiceTranscoderApi->IsClientMuted((size_t)params[arg_index]);
}

/*
* Play the audio file via the voice stream.
*
* @param receiver       Receiver index (0 = play to all clients)
* @param soundFilePath  The path to the sound file, relative to the game directory; must be a valid PCM WAV file (8 or 16 bit)
*
* @note Usage example:
*       VTC_PlaySound(id, "sound/ambience/Opera.wav");
*
* @noreturn
*
* native VTC_PlaySound(const receiver, const soundFilePath[]);
*/
cell AMX_NATIVE_CALL VTC_PlaySound(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index, arg_audio_pathfile };

	// 0 is a valid receiver: VTC plays the sound to all clients (broadcast)
	if (unlikely(params[arg_index] < 0 || params[arg_index] > gpGlobals->maxClients)) {
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: invalid receiver index %i [arg_index]", __FUNCTION__, params[arg_index]);
		return FALSE;
	}

	char pathfile[MAX_PATH];
	g_pVoiceTranscoderApi->PlaySound((size_t)params[arg_index], getAmxString(amx, params[arg_audio_pathfile], pathfile));
	return TRUE;
}

/*
* Blocks the player's voice packets entirely.
* Unlike VTC_MuteClient, a blocked player is not treated as speaking:
* VTC_OnClientStartSpeak/VTC_OnClientStopSpeak are not called for this player.
*
* @param index      Client index
*
* @noreturn
*
* @error            Requires VoiceTranscoder API 3.1 (minor >= 1)
*
* native VTC_BlockClient(const index);
*/
cell AMX_NATIVE_CALL VTC_BlockClient(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index };

	CHECK_ISPLAYER(arg_index);

	if (g_VtcApiMinorVersion < 1) {
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: requires VoiceTranscoder API 3.1 (minor >= 1)", __FUNCTION__);
		return FALSE;
	}

	g_pVoiceTranscoderApi->BlockClient((size_t)params[arg_index]);
	return TRUE;
}

/*
* Unblocks the player's voice packets.
*
* @param index      Client index
*
* @noreturn
*
* @error            Requires VoiceTranscoder API 3.1 (minor >= 1)
*
* native VTC_UnblockClient(const index);
*/
cell AMX_NATIVE_CALL VTC_UnblockClient(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index };

	CHECK_ISPLAYER(arg_index);

	if (g_VtcApiMinorVersion < 1) {
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: requires VoiceTranscoder API 3.1 (minor >= 1)", __FUNCTION__);
		return FALSE;
	}

	g_pVoiceTranscoderApi->UnblockClient((size_t)params[arg_index]);
	return TRUE;
}

/*
* Checks whether the player's voice is blocked at the moment.
*
* @param index      Client index
*
* @return           true if client's voice is blocked, false otherwise
*
* @error            Requires VoiceTranscoder API 3.1 (minor >= 1), returns false if not available
*
* native bool:VTC_IsClientBlocked(const index);
*/
cell AMX_NATIVE_CALL VTC_IsClientBlocked(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_index };

	CHECK_ISPLAYER(arg_index);

	if (g_VtcApiMinorVersion < 1) {
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: requires VoiceTranscoder API 3.1 (minor >= 1)", __FUNCTION__);
		return FALSE;
	}

	return (cell)g_pVoiceTranscoderApi->IsClientBlocked((size_t)params[arg_index]);
}

AMX_NATIVE_INFO VTC_Natives[] =
{
	{ "VTC_IsClientSpeaking", VTC_IsClientSpeaking },
	{ "VTC_MuteClient",       VTC_MuteClient       },
	{ "VTC_UnmuteClient",     VTC_UnmuteClient     },
	{ "VTC_IsClientMuted",    VTC_IsClientMuted    },
	{ "VTC_PlaySound",        VTC_PlaySound        },
	{ "VTC_BlockClient",      VTC_BlockClient      },
	{ "VTC_UnblockClient",    VTC_UnblockClient    },
	{ "VTC_IsClientBlocked",  VTC_IsClientBlocked  },

	{ nullptr, nullptr }
};

void RegisterNatives_VTC()
{
	if (!api_cfg.hasVTC())
		fillNatives(VTC_Natives, [](AMX *amx, cell *params) -> cell { AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: isn't available", "VTC"); return FALSE; });

	g_amxxapi.AddNatives(VTC_Natives);
}
