#if defined _reapi_reunion_included
	#endinput
#endif
#define _reapi_reunion_included

#include <reapi_reunion_const>

#define is_user_steam(%0) (REU_GetAuthtype(%0) == CA_TYPE_STEAM)

/*
* Gets client protocol.
*
* @param index  Client index
*
* @return       Client protocol
*/
native REU_GetProtocol(const index);

/*
* Gets client auth type.
*
* @param index  Client index
*
* @return       Client auth type
*/
native client_auth_type:REU_GetAuthtype(const index);

/*
* Get client authkey
*
* @param index  Client index
* @param index  Buffer to copy the authkey
* @param index  Maximum buffer size
*
* @return       Number of cells copied to buffer
*
*/
native REU_GetAuthKey(const index, dest[], maxlen);

/*
* Gets client authkey kind.
*
* @param index  Client index
*
* @return       Authkey kind, see enum reu_authkey_kind (REU_AK_*)
*/
native reu_authkey_kind:REU_GetAuthKeyKind(const index);

/*
* Sets the client connect time.
*
* @param index  Client index
* @param time   Connect time to set
*
* @return       1 on success
*/
native REU_SetConnectTime(const index, const Float:time);

/*
* Gets the client serialized SteamID.
*
* @param index    Client index
* @param type     Variable to store the id type in, see enum AuthIdType (AUTH_IDTYPE_*)
* @param authid   Buffer to copy the serialized id to (at least 64 cells)
*
* @return         true on success, false if the client has no serialized id
*/
native bool:REU_GetSerializedId(const index, &AuthIdType:type, authid[]);

/*
* Gets the client storage SteamID.
*
* @param index    Client index
* @param type     Variable to store the id type in, see enum AuthIdType (AUTH_IDTYPE_*)
* @param authid   Buffer to copy the storage id to (at least 64 cells)
*
* @return         true on success, false if the client has no storage id
*/
native bool:REU_GetStorageId(const index, &AuthIdType:type, authid[]);

/*
* Gets the client display SteamID.
*
* @param index  Client index
*
* @return       Display SteamID as a 64-bit value truncated to a 32-bit cell
* @note         For the full id use REU_GetSerializedId/REU_GetStorageId
*/
native REU_GetDisplaySteamId(const index);

/*
* Check if the client is running RevEmu with limited user rights.
*
* @param index  Client index
*
* @return       1/0
*/
native bool:REU_IsRevemuWithoutAdminRights(const index);
