#pragma once

#include <amxxmodule.h>
#include <rehlds_api.h>

#define INDEXENT edictByIndex
#define ENTINDEX indexOfEdict
#define AMX_NULLENT -1

class IGameClient;

extern edict_t* g_pEdicts;
extern IRehldsServerStatic* g_RehldsSvs;

inline size_t indexOfEdict(const edict_t* ed)
{
	return ed - g_pEdicts;
}

inline size_t indexOfEdict(const entvars_t* pev)
{
	return indexOfEdict(pev->pContainingEntity);
}

// safe to nullptr
inline size_t indexOfEdictAmx(const entvars_t* pev)
{
	size_t index = AMX_NULLENT;
	if (likely(pev != nullptr))
		index = indexOfEdict(pev);
	return index;
}

inline size_t indexOfEdictAmx(const edict_t *pEdict, const int INVALID_INDEX = AMX_NULLENT)
{
	size_t index = INVALID_INDEX;
	if (likely(pEdict != nullptr))
		index = indexOfEdict(pEdict);
	return index;
}

// fast
inline edict_t* edictByIndex(const int index)
{
	return g_pEdicts + index;
}

inline IGameClient* clientByIndex(const int index)
{
	IGameClient* cl = nullptr;
	if (likely(index > 0))
		cl = g_RehldsSvs->GetClient(index - 1);
	return cl;
}

// safe to index -1
inline edict_t* edictByIndexAmx(const int index, const int INVALID_INDEX = AMX_NULLENT)
{
	auto ed = g_pEdicts + index;
	if (unlikely(index <= INVALID_INDEX)) // == AMX_NULLENT
		ed = nullptr;
	return ed;
}

template<typename T>
inline T* getPrivate(const int index)
{
	T* pdata = nullptr;
	if (likely(index >= 0)) // != AMX_NULLENT
		pdata = (T *)g_pEdicts[index].pvPrivateData;
	return pdata;
}

template<typename T>
inline T* getPrivate(const edict_t *pEdict)
{
	T* pdata = nullptr;
	if (likely(pEdict != nullptr))
		pdata = (T *)pEdict->pvPrivateData;
	return pdata;
}

inline entvars_t* PEV(const int index)
{
	entvars_t* pvars = nullptr;
	if (likely(index >= 0)) // != AMX_NULLENT
		pvars = &g_pEdicts[index].v;
	return pvars;
}

template<typename T>
inline size_t indexOfPDataAmx(const T* pdata)
{
	size_t index = AMX_NULLENT;
	if (likely(pdata != nullptr))
		index = indexOfEdict(pdata->pev);
	return index;
}

inline cell amx_FloatToCell(REAL x)
{
	return *(cell *)&x;
}

inline float amx_CellToFloat(cell x)
{
	return *(REAL *)&x;
}

inline cell getAmxVector(Vector& vec)
{
	return g_amxxapi.PrepareCellArrayA(reinterpret_cast<cell *>(&vec), 3, true);
}

// HLTypeConversion.h -> AMXModX
template <typename T>
inline T &ref_member(void *ptr, int offset, int element = 0)
{
	return *reinterpret_cast<T *>((reinterpret_cast<int8 *>(ptr) + offset + (element * sizeof(T))));
}

template <typename T>
inline T &get_member(void *ptr, int offset, int element = 0)
{
	return ref_member<T>(ptr, offset, element);
}

template <typename T>
inline T &get_member(edict_t *pEntity, int offset, int element = 0)
{
	return get_member<T>(pEntity->pvPrivateData, offset, element);
}

template <typename T>
inline void set_member(void *ptr, int offset, T value, int element = 0)
{
	ref_member<T>(ptr, offset, element) = value;
}

template <typename T>
inline void set_member(edict_t *pEntity, int offset, T value, int element = 0)
{
	set_member<T>(pEntity->pvPrivateData, offset, value, element);
}

template <typename T>
inline T* get_member_direct(void *ptr, int offset, int element = 0, int size = sizeof(T))
{
	return reinterpret_cast<T *>(reinterpret_cast<int8 *>(ptr) + offset + (element * size));
}

template <typename T>
inline T get_member_direct(edict_t *pEntity, int offset, int element = 0, int size = 0)
{
	return get_member_direct<T>(pEntity->pvPrivateData, offset, element, size);
}

class CTempStrings
{
public:
	CTempStrings();
	char* push(struct tagAMX* amx);
	void pop(size_t count);

	enum
	{
		STRINGS_MAX = 16,
		STRING_SIZE = 1024,
		STRING_LEN = STRING_SIZE - 1
	};

private:
	size_t m_current;
	char m_strings[STRINGS_MAX][STRING_SIZE];
};
