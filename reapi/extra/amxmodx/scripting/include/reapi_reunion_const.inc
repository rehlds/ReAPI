#if defined _reapi_reunion_const_included
	#endinput
#endif

#define _reapi_reunion_const_included

/**
* Client auth type
*
* @see                    REU_GetAuthtype()
* @note                   Mirrors reunion's enum dp_authkind_e (reunion_api.h),
*                         extended with CA_TYPE_SSE3 (value 11)
*/
enum client_auth_type
{
	CA_TYPE_NONE = 0,
	CA_TYPE_DPROTO,
	CA_TYPE_STEAM,
	CA_TYPE_STEAMEMU,
	CA_TYPE_REVEMU,
	CA_TYPE_OLDREVEMU,
	CA_TYPE_HLTV,
	CA_TYPE_SC2009,
	CA_TYPE_AVSMP,
	CA_TYPE_SXEI,
	CA_TYPE_REVEMU2013,
	CA_TYPE_SSE3,
};

/**
* Authkey kind
*
* @see                    REU_GetAuthKeyKind()
* @note                   Mirrors reunion's enum reu_authkey_kind (reunion_api.h);
*                         REU_AK_MAX is a sentinel and is not exposed
*/
enum reu_authkey_kind
{
	REU_AK_UNKNOWN = -1,
	REU_AK_STEAM,
	REU_AK_VOLUMEID,
	REU_AK_HDDSN,
	REU_AK_FILEID,
	REU_AK_SXEID,
	REU_AK_OTHER,
};

/**
* Auth id type
*
* @see                    REU_GetSerializedId(), REU_GetStorageId()
* @note                   Mirrors ReHLDS's enum AUTH_IDTYPE
*                         (rehlds/public/rehlds/userid_rehlds.h) - the
*                         USERID_t::idtype field written by the natives above
*/
enum AuthIdType
{
	AUTH_IDTYPE_UNKNOWN = 0,
	AUTH_IDTYPE_STEAM,
	AUTH_IDTYPE_VALVE,
	AUTH_IDTYPE_LOCAL,
};
