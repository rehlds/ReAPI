#include "precompiled.h"

IReunionApi* g_ReunionApi;

bool ReunionApi_Init()
{
	if (!g_RehldsApi)
		return false;

	g_ReunionApi = (IReunionApi *)g_RehldsApi->GetFuncs()->GetPluginApi("reunion");

	if (!g_ReunionApi)
		return false;

	if (g_ReunionApi->GetMajorVersion() != REUNION_API_VERSION_MAJOR)
	{
		UTIL_ServerPrint("[%s]: Reunion API major version mismatch; expected %d, real %d\n", Plugin_info.logtag, REUNION_API_VERSION_MAJOR, g_ReunionApi->GetMajorVersion());

		// need to notify that it is necessary to update the Reunion.
		if (g_ReunionApi->GetMajorVersion() < REUNION_API_VERSION_MAJOR)
		{
			UTIL_ServerPrint("[%s]: Please update the Reunion up to a major version API >= %d\n", Plugin_info.logtag, REUNION_API_VERSION_MAJOR);
		}

		// need to notify that it is necessary to update the module.
		else if (g_ReunionApi->GetMajorVersion() > REUNION_API_VERSION_MAJOR)
		{
			UTIL_ServerPrint("[%s]: Please update the %s up to a major version API >= %d\n", Plugin_info.logtag, Plugin_info.logtag, g_ReunionApi->GetMajorVersion());
		}

		return false;
	}

	if (g_ReunionApi->GetMinorVersion() < REUNION_API_VERSION_MINOR)
	{
		UTIL_ServerPrint("[%s]: Reunion API minor version mismatch; expected at least %d, real %d\n", Plugin_info.logtag, REUNION_API_VERSION_MINOR, g_ReunionApi->GetMinorVersion());
		UTIL_ServerPrint("[%s]: Please update the Reunion up to a minor version API >= %d\n", Plugin_info.logtag, REUNION_API_VERSION_MINOR);
		return false;
	}

	return true;
}
