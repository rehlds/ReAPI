#pragma once

#define PARAMS_COUNT            (params[0] / sizeof(cell))

#define CHECK_ISPLAYER(x)               if (unlikely(params[x] <= 0 || params[x] > gpGlobals->maxClients)) { AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: invalid player index %i [%s]", __FUNCTION__, params[x], #x); return FALSE; }
#define CHECK_ISENTITY(x)               if (unlikely(params[x] < 0 || params[x] > gpGlobals->maxEntities)) { AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: invalid entity index %i [%s]", __FUNCTION__, params[x], #x); return FALSE; }
#define CHECK_GAMERULES()               if (unlikely(!g_pGameRules)) { AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: gamerules not initialized", __FUNCTION__); return FALSE; }
#define CHECK_CONNECTED(x, y)           if (unlikely(x == nullptr || x->has_disconnected)) { AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: player %i is not connected", __FUNCTION__, params[y]); return FALSE; }
#define CHECK_CLIENT_CONNECTED(x, y)    if (unlikely(x == nullptr || !(x->active || x->spawned || x->connected))) { AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: player %i is not connected", __FUNCTION__, params[y]); return FALSE; }
#define CHECK_APICLIENT_CONNECTED(x, y) if (unlikely(x == nullptr || !(x->IsActive() || x->IsSpawned() || x->IsConnected()))) { AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: player %i is not connected", __FUNCTION__, params[y]); return FALSE; }
#define CHECK_INSTANCE_OF(x, y)         if (unlikely(dynamic_cast<x *>((x::BaseClass *)y) == nullptr)) { AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: invalid entity %d ('%s'), is not an instance of the base class '%s'", __FUNCTION__, indexOfEdict(y->pev), STRING(y->pev->classname), #x); return FALSE; }
#define CHECK_REQUIREMENTS(x)           if (unlikely(!api_cfg.has##x())) { AMXX_LogError(amx, AMX_ERR_NATIVE, "Native '%s' is not available, %s required.", __FUNCTION__, #x); return FALSE; } if (!g_RehldsMessageManager) { AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: %s message manager not initialized.", __FUNCTION__, #x); return FALSE; }

#define AMX_ENTITY_VALIDATE(x, y)       if (unlikely(x < AMX_NULLENT || x > gpGlobals->maxEntities)) { AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: \"%s\" invalid entity index %i", __FUNCTION__, y, x); return FALSE; }

class CAmxArg
{
public:
	CAmxArg(AMX* amx, cell value) : m_amx(amx), m_value(value) {}
	operator float() const
	{
		return *(float *)&m_value;
	}
	operator Vector&() const
	{
		return *(Vector *)getAmxAddr(m_amx, m_value);
	}
	operator entvars_s*() const
	{
		auto pev = PEV(m_value); // faster
		return m_value < 0 ? nullptr : pev;
	}
	operator int() const
	{
		return m_value;
	}
	operator ULONG() const
	{
		return ULONG(m_value);
	}
	operator size_t() const
	{
		return size_t(m_value);
	}
	operator bool() const
	{
		return m_value != 0;
	}
	operator unsigned short() const
	{
		return (unsigned short)m_value;
	}
	operator CBaseEntity*() const
	{
		if (m_value < 0)
			return nullptr;
		return getPrivate<CBaseEntity>(m_value);
	}
	operator CBasePlayer*() const
	{
		if (m_value < 0)
			return nullptr;
		return UTIL_PlayerByIndex(m_value);
	}
	operator IGameClient *() const
	{
		if (m_value <= 0)
			return nullptr;
		return g_RehldsSvs->GetClient(m_value - 1); // id: 0 - 31
	}
	operator edict_t *() const
	{
		return edictByIndexAmx(m_value);
	}
	operator const float *() const
	{
		return (float *)getAmxAddr(m_amx, m_value);
	}
	operator TeamName() const
	{
		return static_cast<TeamName>(m_value);
	}
	operator ModelName() const
	{
		return static_cast<ModelName>(m_value);
	}
	operator PLAYER_ANIM() const
	{
		return static_cast<PLAYER_ANIM>(m_value);
	}
	operator WeaponIdType() const
	{
		return static_cast<WeaponIdType>(m_value);
	}
	operator TraceResult *() const
	{
		return reinterpret_cast<TraceResult *>(m_value);
	}
	Vector& vector() const
	{
		return operator Vector&();
	}

private:
	AMX*	m_amx;
	cell	m_value;
};

class CAmxArgs
{
public:
	CAmxArgs(AMX* amx, cell* params) : m_amx(amx), m_params(params) {}
	CAmxArg operator[](size_t index) const
	{
		return CAmxArg(m_amx, m_params[index]);
	}

private:
	AMX*	m_amx;
	cell*	m_params;
};

template<size_t N>
const char* getAmxString(cell* src, char (&dest)[N], size_t* len = nullptr)
{
	return getAmxString(src, dest, N - 1, len);
}

template<size_t N>
const char* getAmxString(AMX* amx, cell addr, char (&dest)[N], size_t* len = nullptr)
{
	return getAmxString(getAmxAddr(amx, addr), dest, N - 1, len);
}

template<size_t N>
string_t getAmxStringAlloc(AMX* amx, cell addr, char (&dest)[N], size_t* len = nullptr)
{
	const char* pszDest = getAmxString(getAmxAddr(amx, addr), dest, N - 1, len);
	return (pszDest && pszDest[0] != '\0') ? ALLOC_STRING(pszDest) : iStringNull;
}

inline void fillNatives(AMX_NATIVE_INFO* table, cell (with)(AMX *, cell *))
{
	for (size_t i = 0; table[i].name; i++)
		table[i].func = with;
}
