#ifndef REUNION_API_H
#define REUNION_API_H

#define REUNION_API_VERSION_MAJOR		1
#define REUNION_API_VERSION_MINOR		4

enum dp_authkind_e
{
	DP_AUTH_NONE = 0,
	DP_AUTH_DPROTO = 1,
	DP_AUTH_STEAM = 2,
	DP_AUTH_STEAMEMU = 3,
	DP_AUTH_REVEMU = 4,
	DP_AUTH_OLDREVEMU = 5,
	DP_AUTH_HLTV = 6,
	DP_AUTH_SC2009 = 7,
	DP_AUTH_AVSMP = 8,
	DP_AUTH_SXEI = 9,
	DP_AUTH_REVEMU2013 = 10
};

enum reu_authkey_kind
{
	REU_AK_UNKNOWN = -1,
	REU_AK_STEAM,
	REU_AK_VOLUMEID,
	REU_AK_HDDSN,
	REU_AK_FILEID,
	REU_AK_SXEID,
	REU_AK_OTHER,
	REU_AK_MAX
};

class IReunionApi
{
public:
	// NOTE: GetMajorVersion, GetMinorVersion have been since version 0.1.0.92c
	int version_major;
	int version_minor;

	enum
	{
		LONG_AUTHID_LEN = 16
	};

	virtual int GetClientProtocol(int index) = 0; // index: 0-31
	virtual dp_authkind_e GetClientAuthtype(int index) = 0; // index: 0-31

	/* Deprecated */virtual size_t GetClientAuthdata(int index, void *data, int maxlen) = 0; // get auth data as binary. index: 0-31
	/* Deprecated */virtual const char *GetClientAuthdataString(int index, char *data, int maxlen) = 0; // get auth data as string. index: 0-31

	virtual int GetMajorVersion() = 0;
	virtual int GetMinorVersion() = 0;

	virtual void GetLongAuthId(int index, unsigned char(&authId)[LONG_AUTHID_LEN]) = 0;
	virtual reu_authkey_kind GetAuthKeyKind(int index) = 0;

	// index: 0-31
	virtual void SetConnectTime(int index, double time) = 0;
	virtual USERID_t* GetSerializedId(int index) const = 0;
	virtual USERID_t* GetStorageId(int index) const = 0;
	virtual uint64 GetDisplaySteamId(int index) const = 0;
};

#endif // REUNION_API_H
