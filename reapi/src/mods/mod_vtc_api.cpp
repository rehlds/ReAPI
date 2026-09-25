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

	if (minorVersion < VOICETRANSCODER_API_VERSION_MINOR)
	{
		// Not fatal: the core VTC natives keep working with API 3.0,
		// only the block API (VTC_BlockClient etc.) requires minor >= 1.
		UTIL_ServerPrint("[%s]: VTC API minor version %d < %d; block natives (VTC_BlockClient/VTC_UnblockClient/VTC_IsClientBlocked) are not available. Please update the VTC up to a minor version API >= %d\n", Plugin_info.logtag, minorVersion, VOICETRANSCODER_API_VERSION_MINOR, VOICETRANSCODER_API_VERSION_MINOR);
	}

	g_pVoiceTranscoderApi->OnClientStartSpeak() += OnClientStartSpeak;
	g_pVoiceTranscoderApi->OnClientStopSpeak() += OnClientStopSpeak;

	return true;
}
