#if defined _reapi_included
	#endinput
#endif

#define _reapi_included

#if AMXX_VERSION_NUM >= 175
	#pragma reqlib reapi
	#if !defined AMXMODX_NOAUTOLOAD
		#pragma loadlib reapi
	#endif
#else
	#pragma library reapi
#endif

enum hooks_tables_e
{
	ht_engine,
	ht_gamedll,
	ht_animating,
	ht_player,
	ht_gamerules,
	ht_rechecker,
	ht_grenade,
	ht_weaponbox,
	ht_weapon,
	ht_gib,
	ht_cbaseentity,
	ht_botmanager
};

enum members_tables_e
{
	mt_gamerules,
	mt_base,
	mt_animating,
	mt_basemonster,
	mt_player,
	mt_entvars,
	mt_playermove,
	mt_movevars,
	mt_usercmd,
	mt_pmtrace,
	mt_csplayer,
	mt_baseitem,
	mt_baseweapon,
	mt_weaponbox,
	mt_armoury,
	mt_grenade,
	mt_p228,
	mt_scout,
	mt_hegrenade,
	mt_xm1014,
	mt_c4,
	mt_mac10,
	mt_aug,
	mt_smokegrenade,
	mt_elite,
	mt_fiveseven,
	mt_ump45,
	mt_sg550,
	mt_galil,
	mt_famas,
	mt_usp,
	mt_glock18,
	mt_awp,
	mt_mp5n,
	mt_m249,
	mt_m3,
	mt_m4a1,
	mt_tmp,
	mt_g3sg1,
	mt_deagle,
	mt_sg552,
	mt_ak47,
	mt_knife,
	mt_p90,
	mt_shield,
	mt_rebuystruct,
	mt_mapinfo,
	mt_csplayerweapon,
	mt_gib,
	mt_netadr,
	mt_csentity,
	mt_netchan
};

#define ReAPIFunc 				{EngineFunc, GamedllFunc, GamedllFunc_CBaseAnimating, GamedllFunc_CBasePlayer, GamedllFunc_CSGameRules, GamedllFunc_CGrenade, GamedllFunc_CWeaponBox, ReCheckerFunc, GamedllFunc_CBasePlayerWeapon, GamedllFunc_CGib, GamedllFunc_CBaseEntity, GamedllFunc_CBotManager}

// Is like FNullEnt
#define is_nullent(%0)          (%0 == 0 || is_entity(%0) == false)

#define MAX_REGION_RANGE        1024

#define BEGIN_FUNC_REGION(%0)   (any:MAX_REGION_RANGE * hooks_tables_e:ht_%0)
#define BEGIN_MEMBER_REGION(%0) (any:MAX_REGION_RANGE * members_tables_e:mt_%0)

#include <cssdk_const>

#include <reapi_version>
#include <reapi_engine>  // @note: only for ReHLDS
#include <reapi_gamedll> // @note: only for gamedll Counter-Strike (ReGameDLL_CS)

// If you want to use s/get_member unsafe version,
// then macro MEMBER_UNSAFE must be defined before including header reapi.inc
#if !defined(MEMBER_UNSAFE)
	#define set_member set_member_s
	#define get_member get_member_s
#endif

// addons
#include <reapi_vtc>
#include <reapi_reunion>
#include <reapi_rechecker>

/**
* Hookchain return types
*/
enum
{
	HC_CONTINUE = 0, // Plugin didn't take any action
	HC_SUPERCEDE,    // Skip real function, use my return value
	HC_BREAK,        // Skip all forwards and real function, use my return value
                     // @note Warning: Be very careful, using this type of return will skip calls for all following AMXX plugins

	HC_BYPASS        // Skip calls for all following AMXX plugins, but call the original function
                     // @note Warning: In PRE skips all forwards including POST forwards
};

/**
* Hookchain argument types
*/
enum AType
{
	ATYPE_INTEGER = 0,
	ATYPE_FLOAT,
	ATYPE_STRING,
	ATYPE_CLASSPTR,
	ATYPE_EDICT,
	ATYPE_EVARS,
	ATYPE_BOOL,
	ATYPE_VECTOR,
	ATYPE_TRACE
};

enum HookChain
{
	INVALID_HOOKCHAIN = 0
};

/*
* Hook API function that are available into enum.
* Look at the enums for parameter lists.
*
* @param function   The function to hook
* @param callback   The forward to call
* @param post       Whether or not to forward this in post
*
* @return           Returns a hook handle. Use EnableHookChain/DisableHookChain to toggle the forward on or off
*/
native HookChain:RegisterHookChain(ReAPIFunc:function_id, const callback[], post = 0);

/*
* Stops a hook from triggering.
* Use the return value from RegisterHookChain as the parameter here!
*
* @param hook       The hook to stop
*
*/
native bool:DisableHookChain(HookChain:hook);

/*
* Starts a hook back up.
* Use the return value from RegisterHookChain as the parameter here!
*
* @param hook       The hook to re-enable
*
* @return           Returns true if the function is successfully executed, otherwise false
*/
native bool:EnableHookChain(HookChain:hook);

/*
* Sets the return value of a hookchain.
*
* @param type       To specify the ATYPE_* parameter, look at the enum AType
* @param value      The value to set the return to
*
*/
native SetHookChainReturn(AType:type, any:...);

/*
* Gets the return value of the current hookchain.
* This has no effect in pre hookchain.
*
* @param type       To specify the ATYPE_* parameter, look at the enum AType
* @param [maxlen]   Max length of string (optional)
*
* @return           If an integer or boolean or one byte or float, array or everything else is passed via 1st argument and more
*/
native any:GetHookChainReturn(AType:type, any:...);

/*
* Set hookchain argument.
* This has no effect in post hookchain.
*
* @param number     Number of argument
* @param value      New value
* @param [maxlen]   Max length of string (optional)
*
* @return           Returns true if the function is successfully executed, otherwise false
*/
native SetHookChainArg(number, AType:type, any:...);

/*
* Return call state of original API function (that are available into enum).
* Look at the enums for parameter lists.
*
* @param func       The function to get state
*
* @return           Returns true if the original function was called, otherwise false
*/
native bool:IsReapiHookOriginalWasCalled(ReAPIFunc:function_id);

/*
* Returns the current hookchain handle.
*
* @return           Returns the hook handle
*/
native HookChain:GetCurrentHookChainHandle();

/*
* Compares the entity to a specified classname.
* @note This native also checks the validity of an entity.
*
* @return       true/false
*/
native bool:FClassnameIs(const entityIndex, const className[]);

/*
* To get WeaponIdType from grenade entity
*
* @param entity     Grenade entity
*
* @return           return enum's of WeaponIdType
*/
native WeaponIdType:GetGrenadeType(const entityIndex);

/*
* Sets the view entity on a client.
* This allows pfnSetView able to hooks.
*
* @param index       Client index
* @param viewEntity  Entity index
*
*/
native engset_view(const index, const viewEntity);

/*
* Gets the return index of the current view entity on a client.
*
* @param index  Client index
*
*/
native get_viewent(const index);

/*
* Check if the entity is valid.
*
* @return       true/false
*/
native bool:is_entity(const entityIndex);

/*
* Check if ReHLDS is available.
*
* @return       true/false
*/
native bool:is_rehlds();

/*
* Check if ReGameDLL is available.
*
* @return       true/false
*/
native bool:is_regamedll();

/*
* Check if Reunion is available.
*
* @return       true/false
*
*/
native bool:has_reunion();

/*
* Check if VTC is available.
*
* @return       true/false
*/
native bool:has_vtc();

/*
* Check if Rechecker is available.
*
* @return       true/false
*/
native bool:has_rechecker();

/*
* This is the callback from the module that gives major/minor versions for verifying compatibility for ReAPI versions.
* If an AMXX plugin gets a failure, then you do need to upgrade to the latest version of the ReAPI module or update the files included for AMXX plugins.
* Do not modify this!
*/
public __reapi_version_check(const majorVersion, const minorVersion)
{
	if (majorVersion != REAPI_VERSION_MAJOR)
	{
		new temp[512];
		formatex(temp, sizeof temp - 1, "[ReAPI]: Api major version mismatch; expected %d, real %d", REAPI_VERSION_MAJOR, majorVersion);
		set_fail_state(temp);
		return;
	}

	if (minorVersion < REAPI_VERSION_MINOR)
	{
		new temp[512];
		formatex(temp, sizeof temp - 1, "[ReAPI]: Api minor version mismatch; expected at least %d, real %d", REAPI_VERSION_MINOR, minorVersion);
		set_fail_state(temp);
		return;
	}
}
