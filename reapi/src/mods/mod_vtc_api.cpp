#include "precompiled.h"

IVoiceTranscoderAPI *g_pVoiceTranscoderApi;
size_t g_VtcApiMinorVersion = 0;

bool VTC_Api_Init()
{
	if (!g_RehldsApi)
		return false;

	g_pVoiceTranscoderApi = (IVoiceTranscoderAPI *)g_RehldsApi->GetFuncs()->GetPluginApi("VoiceTranscoder");

	if (!g_pVoiceTranscoderApi)
		return false;

	size_t majorVersion = g_pVoiceTranscoderApi->MajorVersion();
	size_t minorVersion = g_pVoiceTranscoderApi->MinorVersion();
	g_VtcApiMinorVersion = minorVersion;

	if (majorVersion != VOICETRANSCODER_API_VERSION_MAJOR)
	{
		UTIL_ServerPrint("[%s]: VTC API major version mismatch; expected %d, real %d\n", Plugin_info.logtag, VOICETRANSCODER_API_VERSION_MAJOR, majorVersion);

		// need to notify that it is necessary to update the VTC.
		if (majorVersion < VOICETRANSCODER_API_VERSION_MAJOR)
		{
			UTIL_ServerPrint("[%s]: Please update the VTC up to a major version API >= %d\n", Plugin_info.logtag, VOICETRANSCODER_API_VERSION_MAJOR);
		}

		// need to notify that it is necessary to update the module.
		else if (majorVersion > VOICETRANSCODER_API_VERSION_MAJOR)
		{
			UTIL_ServerPrint("[%s]: Please update the %s up to a major version API >= %d\n", Plugin_info.logtag, Plugin_info.logtag, majorVersion);
		}

		return false;
	}

	// A minor version mismatch is not fatal and is not reported here: running an
	// older VTC is the server owner's choice, and the only natives that need a
	// newer minor version report the error themselves when they are called.

	g_pVoiceTranscoderApi->OnClientStartSpeak() += OnClientStartSpeak;
	g_pVoiceTranscoderApi->OnClientStopSpeak() += OnClientStopSpeak;

	return true;
}
