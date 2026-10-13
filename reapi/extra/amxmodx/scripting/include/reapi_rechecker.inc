#if defined _reapi_rechecker_included
	#endinput
#endif
#define _reapi_rechecker_included

enum QueryFileHook
{
	INVALID_QUERYFILE_HOOK = 0
};

enum ResourceType
{
	RES_TYPE_NONE = 0,
	RES_TYPE_EXISTS,    // compare with the specified hash value
	RES_TYPE_MISSING,   // check that the file is missing on the client
	RES_TYPE_IGNORE,    // ignore the specified hash value
	RES_TYPE_HASH_ANY,  // any file with any hash value
};

enum ReCheckerFunc
{
	/*
	* Description:   -
	* Params:        (const client, const filename[], const cmd[], const ResourceType:type, const responseHash, bool:isBreak)
	*/
	RC_FileConsistencyProcess = BEGIN_FUNC_REGION(rechecker),

	/*
	* Description:   -
	* Params:        (const client)
	*/
	RC_FileConsistencyFinal,

	/*
	* Description:   -
	* Params:        (const client, const filename[], const cmd[], const responseHash)
	*/
	RC_CmdExec,

	// [...]
};

/*
* Send request the file for the client to get hash
*
* @param file           The file (Can contain a relative path to the file)
* @param function       The forward to call
* @param type           The request type, can be only RES_TYPE_EXISTS, RES_TYPE_MISSING or RES_TYPE_HASH_ANY
* @param hash           Hash of file to request. With RES_TYPE_EXISTS, -1 means "any hash" (the request is treated as RES_TYPE_HASH_ANY).
*
* @return               Returns a hook handle. Use UnRegisterQueryFile to remove the forward
*/
native QueryFileHook:RegisterQueryFile(const file[], const function[], const ResourceType:type, const hash = -1);

/*
* Unregister the forward.
* Use the return value from RegisterQueryFile as the parameter here!
*
* @param hook           The hook to remove
*
* @return               Returns true if the hook is successfully removed, otherwise false
*/
native bool:UnRegisterQueryFile(QueryFileHook:hook);
