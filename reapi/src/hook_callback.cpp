#include "precompiled.h"

hookctx_t* g_hookCtx = nullptr;
CTempStrings hookctx_t::s_temp_strings;

/*
* ReHLDS functions
*/
void SV_StartSound(IRehldsHook_SV_StartSound *chain, int recipients, edict_t *entity, int channel, const char *sample, int volume, float attenuation, int fFlags, int pitch)
{
	auto original = [chain](int _recipients, int _entity, int _channel, const char *_sample, int _volume, float _attenuation, int _fFlags, int _pitch)
	{
		chain->callNext(_recipients, edictByIndexAmx(_entity), _channel, _sample, _volume, _attenuation, _fFlags, _pitch);
	};

	callVoidForward(RH_SV_StartSound, original, recipients, indexOfEdict(entity), channel, sample, volume, attenuation, fFlags, pitch);
}

void SV_DropClient(IRehldsHook_SV_DropClient *chain, IGameClient *cl, bool crash, const char *fmt)
{
	auto original = [chain](int _cl, bool _crash, const char *_fmt)
	{
		chain->callNext(clientByIndex(_cl), _crash, _fmt);
	};

	callVoidForward(RH_SV_DropClient, original, cl->GetId() + 1, crash, fmt);
}

void SV_ActivateServer(IRehldsHook_SV_ActivateServer *chain, int runPhysics)
{
	auto original = [chain](int _runPhysics)
	{
		chain->callNext(_runPhysics);
	};

	callVoidForward(RH_SV_ActivateServer, original, runPhysics);
}

void Cvar_DirectSet(IRehldsHook_Cvar_DirectSet *chain, cvar_t *var, const char *value)
{
	auto original = [chain](cvar_t *_var, const char *_value)
	{
		chain->callNext(_var, _value);
	};

	callVoidForward(RH_Cvar_DirectSet, original, var, value);
}

void SV_WriteFullClientUpdate_AMXX(SV_WriteFullClientUpdate_t *data, IGameClient *client, size_t buffer, IGameClient *receiver)
{
	auto original = [data](int _client, size_t _buffer, int _receiver)
	{
		data->m_chain->callNext(clientByIndex(_client), (char *)_buffer, data->m_args.maxlen, data->m_args.message, clientByIndex(_receiver));
	};

	callVoidForward(RH_SV_WriteFullClientUpdate, original, client->GetId() + 1, buffer, receiver ? receiver->GetId() + 1 : AMX_NULLENT);
}

void SV_WriteFullClientUpdate(IRehldsHook_SV_WriteFullClientUpdate *chain, IGameClient *client, char *buffer, size_t maxlen, sizebuf_t *sb, IGameClient *receiver)
{
	SV_WriteFullClientUpdate_args_t args(sb, maxlen);
	SV_WriteFullClientUpdate_t data(chain, args);
	SV_WriteFullClientUpdate_AMXX(&data, client, (size_t)buffer, receiver);
}

void Con_Printf(IRehldsHook_Con_Printf *chain, const char *string)
{
	auto original = [chain](const char *_string)
	{
		chain->callNext(_string);
	};

	callVoidForward(RH_Con_Printf, original, string);
}

ENTITYINIT GetEntityInit(IRehldsHook_GetEntityInit *chain, char *classname)
{
	auto original = [chain](char *_classname)
	{
		return chain->callNext(_classname);
	};

	return callForward<ENTITYINIT>(RH_GetEntityInit, original, classname);
}

void ClientConnected(IRehldsHook_ClientConnected* chain, IGameClient* cl)
{
	auto original = [chain](int client)
	{
		chain->callNext(clientByIndex(client));
	};

	callVoidForward(RH_ClientConnected, original, cl->GetId() + 1);
}

void SV_ConnectClient(IRehldsHook_SV_ConnectClient *chain)
{
	auto original = [chain]()
	{
		chain->callNext();
	};

	callVoidForward(RH_SV_ConnectClient, original);
}

void SV_EmitPings_AMXX(SV_EmitPings_t* data, IGameClient* cl)
{
	auto original = [data](int _cl)
	{
		data->m_chain->callNext(clientByIndex(_cl), data->m_args.message);
	};

	callVoidForward(RH_SV_EmitPings, original, cl->GetId() + 1);
}

void SV_EmitPings(IRehldsHook_SV_EmitPings *chain, IGameClient *cl, sizebuf_t *msg)
{
	SV_EmitPings_args_t args(cl, msg);
	SV_EmitPings_t data(chain, args);
	SV_EmitPings_AMXX(&data, cl);
}

edict_t *ED_Alloc(IRehldsHook_ED_Alloc* chain)
{
	auto original = [chain]()
	{
		return indexOfEdict(chain->callNext());
	};

	return edictByIndexAmx(callForward<size_t>(RH_ED_Alloc, original));
}

void ED_Free(IRehldsHook_ED_Free* chain, edict_t *entity)
{
	auto original = [chain](int _entity)
	{
		chain->callNext(edictByIndexAmx(_entity));
	};

	callVoidForward(RH_ED_Free, original, indexOfEdict(entity));
}

bool SV_AllowPhysent(IRehldsHook_SV_AllowPhysent* chain, edict_t* check, edict_t* sv_player)
{
	auto original = [chain](int _check, int _sv_player)
	{
		return chain->callNext(edictByIndexAmx(_check), edictByIndexAmx(_sv_player));
	};

	return callForward<bool>(RH_SV_AllowPhysent, original, indexOfEdict(check), indexOfEdict(sv_player));
}

BOOL SV_CheckUserInfo_AMXX(IRehldsHook_SV_CheckUserInfo *chain, netadr_t *adr, size_t userinfo, qboolean bIsReconnecting, int iReconnectSlot, char *name)
{
	auto original = [chain](netadr_t *_adr, cell _userinfo, qboolean _bIsReconnecting, int _iReconnectSlot, char *_name)
	{
		return chain->callNext(_adr, (char *)_userinfo, _bIsReconnecting, _iReconnectSlot, _name);
	};

	return callForward<BOOL>(RH_SV_CheckUserInfo, original, adr, userinfo, bIsReconnecting, iReconnectSlot, name);
}

BOOL SV_CheckUserInfo(IRehldsHook_SV_CheckUserInfo *chain, netadr_t *adr, char *userinfo, qboolean bIsReconnecting, int iReconnectSlot, char *name)
{
	return SV_CheckUserInfo_AMXX(chain, adr, (size_t)userinfo, bIsReconnecting, iReconnectSlot, name);
}

int PF_precache_generic_I(IRehldsHook_PF_precache_generic_I *chain, const char *s)
{
	auto original = [chain](const char *_s)
	{
		return chain->callNext(_s);
	};

	return callForward<int>(RH_PF_precache_generic_I, original, s);
}

int PF_precache_model_I(IRehldsHook_PF_precache_model_I *chain, const char *s)
{
	auto original = [chain](const char *_s)
	{
		return chain->callNext(_s);
	};

	return callForward<int>(RH_PF_precache_model_I, original, s);
}

int PF_precache_sound_I(IRehldsHook_PF_precache_sound_I *chain, const char *s)
{
	auto original = [chain](const char *_s)
	{
		return chain->callNext(_s);
	};

	return callForward<int>(RH_PF_precache_sound_I, original, s);
}

unsigned short EV_Precache_AMXX(EventPrecache_t *data, const char *psz)
{
	auto original = [data](const char *_psz)
	{
		return data->m_chain->callNext(data->m_args.type, _psz);
	};

	return callForward<unsigned short>(RH_EV_Precache, original, psz);
}

unsigned short EV_Precache(IRehldsHook_EV_Precache *chain, int type, const char *psz)
{
	EventPrecache_args_t args(type);
	EventPrecache_t data(chain, args);
	return EV_Precache_AMXX(&data, psz);
}

void SV_AddResource(IRehldsHook_SV_AddResource *chain, resourcetype_t type, const char *name, int size, unsigned char flags, int index)
{
	auto original = [chain](resourcetype_t _type, const char *_name, int _size, unsigned char _flags, int _index)
	{
		chain->callNext(_type, _name, _size, _flags, _index);
	};

	callVoidForward(RH_SV_AddResource, original, type, name, size, flags, index);
}

void SV_ClientPrintf(IRehldsHook_SV_ClientPrintf *chain, const char *string)
{
	auto original = [chain](const char *_string)
	{
		chain->callNext(_string);
	};

	callVoidForward(RH_SV_ClientPrintf, original, string);
}

void ExecuteServerStringCmd(IRehldsHook_ExecuteServerStringCmd* chain, const char* cmdName, cmd_source_t cmdSrc, IGameClient* cl)
{
	auto original = [chain](const char* _cmdName, cmd_source_t _cmdSrc, int client)
	{
		chain->callNext(_cmdName, _cmdSrc, _cmdSrc == src_client ? clientByIndex(client) : nullptr);
	};

	callVoidForward(RH_ExecuteServerStringCmd, original, cmdName, cmdSrc, cmdSrc == src_client ? cl->GetId() + 1 : AMX_NULLENT);
}

void SV_SendResources_AMXX(SV_SendResources_t *data, IGameClient *cl)
{
	auto original = [data](int _cl)
	{
		data->m_chain->callNext(data->m_args);
	};

	callVoidForward(RH_SV_SendResources, original, cl ? cl->GetId() + 1 : AMX_NULLENT);
}

void SV_SendResources(IRehldsHook_SV_SendResources *chain, sizebuf_t *msg)
{
	SV_SendResources_t data(chain, msg);
	SV_SendResources_AMXX(&data, g_RehldsFuncs->GetHostClient());
}

/*
* ReGameDLL functions
*/
void CBasePlayer_Spawn(IReGameHook_CBasePlayer_Spawn *chain, CBasePlayer *pthis)
{
	auto original = [chain](int _pthis)
	{
		chain->callNext(getPrivate<CBasePlayer>(_pthis));
	};

	callVoidForward(RG_CBasePlayer_Spawn, original, indexOfEdict(pthis->pev));
}

void CBasePlayer_Precache(IReGameHook_CBasePlayer_Precache *chain, CBasePlayer *pthis)
{
	auto original = [chain](int _pthis)
	{
		chain->callNext(getPrivate<CBasePlayer>(_pthis));
	};

	callVoidForward(RG_CBasePlayer_Precache, original, indexOfEdict(pthis->pev));
}

int CBasePlayer_ObjectCaps(IReGameHook_CBasePlayer_ObjectCaps *chain, CBasePlayer *pthis)
{
	auto original = [chain](int _pthis)
	{
		return chain->callNext(getPrivate<CBasePlayer>(_pthis));
	};

	return callForward<int>(RG_CBasePlayer_ObjectCaps, original, indexOfEdict(pthis->pev));
}

int CBasePlayer_Classify(IReGameHook_CBasePlayer_Classify *chain, CBasePlayer *pthis)
{
	auto original = [chain](int _pthis)
	{
		return chain->callNext(getPrivate<CBasePlayer>(_pthis));
	};

	return callForward<int>(RG_CBasePlayer_Classify, original, indexOfEdict(pthis->pev));
}

void CBasePlayer_TraceAttack(IReGameHook_CBasePlayer_TraceAttack *chain, CBasePlayer *pthis, entvars_t *pevAttacker, float flDamage, Vector& vecDir, TraceResult *ptr, int bitsDamageType)
{
	Vector vecDirCopy(vecDir);

	auto original = [chain, &vecDirCopy](int _pthis, int _pevAttacker, float _flDamage, cell _vecDir, TraceResult *_ptr, int _bitsDamageType)
	{
		chain->callNext(getPrivate<CBasePlayer>(_pthis), PEV(_pevAttacker), _flDamage, vecDirCopy, _ptr, _bitsDamageType);
	};

	callVoidForward(RG_CBasePlayer_TraceAttack, original, indexOfEdict(pthis->pev), indexOfEdict(pevAttacker), flDamage, getAmxVector(vecDirCopy), ptr, bitsDamageType);
}

int CBasePlayer_TakeDamage(IReGameHook_CBasePlayer_TakeDamage *chain, CBasePlayer *pthis, entvars_t *pevInflictor, entvars_t *pevAttacker, float& flDamage, int bitsDamageType)
{
	auto original = [chain](int _pthis, int _pevInflictor, int _pevAttacker, volatile float& _flDamage, int _bitsDamageType)
	{
		return chain->callNext(getPrivate<CBasePlayer>(_pthis), PEV(_pevInflictor), PEV(_pevAttacker), const_cast<float&>(_flDamage), _bitsDamageType);
	};

	return callForward<int>(RG_CBasePlayer_TakeDamage, original, indexOfEdict(pthis->pev), indexOfEdict(pevInflictor), indexOfEdict(pevAttacker), flDamage, bitsDamageType);
}

int CBasePlayer_TakeHealth(IReGameHook_CBasePlayer_TakeHealth *chain, CBasePlayer *pthis, float flHealth, int bitsDamageType)
{
	auto original = [chain](int _pthis, float _flHealth, int _bitsDamageType)
	{
		return chain->callNext(getPrivate<CBasePlayer>(_pthis), _flHealth, _bitsDamageType);
	};

	return callForward<int>(RG_CBasePlayer_TakeHealth, original, indexOfEdict(pthis->pev), flHealth, bitsDamageType);
}

void CBasePlayer_Killed(IReGameHook_CBasePlayer_Killed *chain, CBasePlayer *pthis, entvars_t *pevAttacker, int iGib)
{
	auto original = [chain](int _pthis, int _pevAttacker, int _iGib)
	{
		chain->callNext(getPrivate<CBasePlayer>(_pthis), PEV(_pevAttacker), _iGib);
	};

	callVoidForward(RG_CBasePlayer_Killed, original, indexOfEdict(pthis->pev), indexOfEdict(pevAttacker), iGib);
}

void CBasePlayer_AddPoints(IReGameHook_CBasePlayer_AddPoints *chain, CBasePlayer *pthis, int score, BOOL bAllowNegativeScore)
{
	auto original = [chain](int _pthis, int _score, BOOL _bAllowNegativeScore)
	{
		chain->callNext(getPrivate<CBasePlayer>(_pthis), _score, _bAllowNegativeScore);
	};

	callVoidForward(RG_CBasePlayer_AddPoints, original, indexOfEdict(pthis->pev), score, bAllowNegativeScore);
}

void CBasePlayer_AddPointsToTeam(IReGameHook_CBasePlayer_AddPointsToTeam *chain, CBasePlayer *pthis, int score, BOOL bAllowNegativeScore)
{
	auto original = [chain](int _pthis, int _score, BOOL _bAllowNegativeScore)
	{
		chain->callNext(getPrivate<CBasePlayer>(_pthis), _score, _bAllowNegativeScore);
	};

	callVoidForward(RG_CBasePlayer_AddPointsToTeam, original, indexOfEdict(pthis->pev), score, bAllowNegativeScore);
}

BOOL CBasePlayer_AddPlayerItem(IReGameHook_CBasePlayer_AddPlayerItem *chain, CBasePlayer *pthis, CBasePlayerItem *pItem)
{
	auto original = [chain](int _pthis, int _pItem)
	{
		return chain->callNext(getPrivate<CBasePlayer>(_pthis), getPrivate<CBasePlayerItem>(_pItem));
	};

	return callForward<BOOL>(RG_CBasePlayer_AddPlayerItem, original, indexOfEdict(pthis->pev), indexOfEdict(pItem->pev));
}

BOOL CBasePlayer_RemovePlayerItem(IReGameHook_CBasePlayer_RemovePlayerItem *chain, CBasePlayer *pthis, CBasePlayerItem *pItem)
{
	auto original = [chain](int _pthis, int _pItem)
	{
		return chain->callNext(getPrivate<CBasePlayer>(_pthis), getPrivate<CBasePlayerItem>(_pItem));
	};

	return callForward<BOOL>(RG_CBasePlayer_RemovePlayerItem, original, indexOfEdict(pthis->pev), indexOfEdict(pItem->pev));
}

int CBasePlayer_GiveAmmo(IReGameHook_CBasePlayer_GiveAmmo *chain, CBasePlayer *pthis, int iAmount, const char *szName, int iMax)
{
	auto original = [chain](int _pthis, int _iAmount, const char *_szName, int _iMax)
	{
		return chain->callNext(getPrivate<CBasePlayer>(_pthis), _iAmount, _szName, _iMax);
	};

	return callForward<int>(RG_CBasePlayer_GiveAmmo, original, indexOfEdict(pthis->pev), iAmount, szName, iMax);
}

void CBasePlayer_ResetMaxSpeed(IReGameHook_CBasePlayer_ResetMaxSpeed *chain, CBasePlayer *pthis)
{
	auto original = [chain](int _pthis)
	{
		chain->callNext(getPrivate<CBasePlayer>(_pthis));
	};

	callVoidForward(RG_CBasePlayer_ResetMaxSpeed, original, indexOfEdict(pthis->pev));
}

void CBasePlayer_Jump(IReGameHook_CBasePlayer_Jump *chain, CBasePlayer *pthis)
{
	auto original = [chain](int _pthis)
	{
		chain->callNext(getPrivate<CBasePlayer>(_pthis));
	};

	callVoidForward(RG_CBasePlayer_Jump, original, indexOfEdict(pthis->pev));
}

void CBasePlayer_Duck(IReGameHook_CBasePlayer_Duck *chain, CBasePlayer *pthis)
{
	auto original = [chain](int _pthis)
	{
		chain->callNext(getPrivate<CBasePlayer>(_pthis));
	};

	callVoidForward(RG_CBasePlayer_Duck, original, indexOfEdict(pthis->pev));
}

void CBasePlayer_PreThink(IReGameHook_CBasePlayer_PreThink *chain, CBasePlayer *pthis)
{
	auto original = [chain](int _pthis)
	{
		chain->callNext(getPrivate<CBasePlayer>(_pthis));
	};

	callVoidForward(RG_CBasePlayer_PreThink, original, indexOfEdict(pthis->pev));
}

void CBasePlayer_PostThink(IReGameHook_CBasePlayer_PostThink *chain, CBasePlayer *pthis)
{
	auto original = [chain](int _pthis)
	{
		chain->callNext(getPrivate<CBasePlayer>(_pthis));
	};

	callVoidForward(RG_CBasePlayer_PostThink, original, indexOfEdict(pthis->pev));
}

void CBasePlayer_UpdateClientData(IReGameHook_CBasePlayer_UpdateClientData *chain, CBasePlayer *pthis)
{
	auto original = [chain](int _pthis)
	{
		chain->callNext(getPrivate<CBasePlayer>(_pthis));
	};

	callVoidForward(RG_CBasePlayer_UpdateClientData, original, indexOfEdict(pthis->pev));
}

void CBasePlayer_ImpulseCommands(IReGameHook_CBasePlayer_ImpulseCommands *chain, CBasePlayer *pthis)
{
	auto original = [chain](int _pthis)
	{
		chain->callNext(getPrivate<CBasePlayer>(_pthis));
	};

	callVoidForward(RG_CBasePlayer_ImpulseCommands, original, indexOfEdict(pthis->pev));
}

void CBasePlayer_RoundRespawn(IReGameHook_CBasePlayer_RoundRespawn *chain, CBasePlayer *pthis)
{
	auto original = [chain](int _pthis)
	{
		chain->callNext(getPrivate<CBasePlayer>(_pthis));
	};

	callVoidForward(RG_CBasePlayer_RoundRespawn, original, indexOfEdict(pthis->pev));
}

void CBasePlayer_Blind(IReGameHook_CBasePlayer_Blind *chain, CBasePlayer *pthis, float flUntilTime, float flHoldTime, float flFadeTime, int iAlpha)
{
	auto original = [chain](int _pthis, float _flUntilTime, float _flHoldTime, float _flFadeTime, int _iAlpha)
	{
		chain->callNext(getPrivate<CBasePlayer>(_pthis), _flUntilTime, _flHoldTime, _flFadeTime, _iAlpha);
	};

	callVoidForward(RG_CBasePlayer_Blind, original, indexOfEdict(pthis->pev), flUntilTime, flHoldTime, flFadeTime, iAlpha);
}

CBasePlayer *CBasePlayer_Observer_IsValidTarget(IReGameHook_CBasePlayer_Observer_IsValidTarget *chain, CBasePlayer *pthis, int iPlayerIndex, bool bSameTeam)
{
	auto original = [chain](int _pthis, int _iPlayerIndex, bool _bSameTeam)
	{
		return indexOfPDataAmx(chain->callNext(getPrivate<CBasePlayer>(_pthis), _iPlayerIndex, _bSameTeam));
	};

	return getPrivate<CBasePlayer>(callForward<size_t>(RG_CBasePlayer_Observer_IsValidTarget, original, indexOfEdict(pthis->pev), iPlayerIndex, bSameTeam));
}

void CBasePlayer_SetAnimation(IReGameHook_CBasePlayer_SetAnimation *chain, CBasePlayer *pthis, PLAYER_ANIM playerAnim)
{
	auto original = [chain](int _pthis, PLAYER_ANIM _playerAnim)
	{
		chain->callNext(getPrivate<CBasePlayer>(_pthis), _playerAnim);
	};

	callVoidForward(RG_CBasePlayer_SetAnimation, original, indexOfEdict(pthis->pev), playerAnim);
}

void CBasePlayer_GiveDefaultItems(IReGameHook_CBasePlayer_GiveDefaultItems *chain, CBasePlayer *pthis)
{
	auto original = [chain](int _pthis)
	{
		chain->callNext(getPrivate<CBasePlayer>(_pthis));
	};

	callVoidForward(RG_CBasePlayer_GiveDefaultItems, original, indexOfEdict(pthis->pev));
}

CBaseEntity *CBasePlayer_GiveNamedItem(IReGameHook_CBasePlayer_GiveNamedItem *chain, CBasePlayer *pthis, const char *pszName)
{
	auto original = [chain](int _pthis, const char *_pszName)
	{
		return indexOfPDataAmx(chain->callNext(getPrivate<CBasePlayer>(_pthis), _pszName));
	};

	return getPrivate<CBaseEntity>(callForward<size_t>(RG_CBasePlayer_GiveNamedItem, original, indexOfEdict(pthis->pev), pszName));
}

void CBasePlayer_AddAccount(IReGameHook_CBasePlayer_AddAccount *chain, CBasePlayer *pthis, int amount, RewardType type, bool bTrackChange)
{
	auto original = [chain](int _pthis, int _amount, RewardType _type, bool _bTrackChange)
	{
		chain->callNext(getPrivate<CBasePlayer>(_pthis), _amount, _type, _bTrackChange);
	};

	callVoidForward(RG_CBasePlayer_AddAccount, original, indexOfEdict(pthis->pev), amount, type, bTrackChange);
}

void CBasePlayer_GiveShield(IReGameHook_CBasePlayer_GiveShield *chain, CBasePlayer *pthis, bool bDeploy)
{
	auto original = [chain](int _pthis, bool _bDeploy)
	{
		chain->callNext(getPrivate<CBasePlayer>(_pthis), _bDeploy);
	};

	callVoidForward(RG_CBasePlayer_GiveShield, original, indexOfEdict(pthis->pev), bDeploy);
}

void CBasePlayer_SetClientUserInfoModel(IReGameHook_CBasePlayer_SetClientUserInfoModel *chain, CBasePlayer *pthis, char *infobuffer, char *szNewModel)
{
	auto original = [chain](int _pthis, char *_infobuffer, char *_szNewModel)
	{
		chain->callNext(getPrivate<CBasePlayer>(_pthis), _infobuffer, _szNewModel);
	};

	callVoidForward(RG_CBasePlayer_SetClientUserInfoModel, original, indexOfEdict(pthis->pev), infobuffer, szNewModel);
}

bool CBasePlayer_SetClientUserInfoName(IReGameHook_CBasePlayer_SetClientUserInfoName *chain, CBasePlayer *pthis, char *infobuffer, char *szNewName)
{
	auto original = [chain](int _pthis, char *_infobuffer, char *_szNewName)
	{
		return chain->callNext(getPrivate<CBasePlayer>(_pthis), _infobuffer, _szNewName);
	};

	return callForward<bool>(RG_CBasePlayer_SetClientUserInfoName, original, indexOfEdict(pthis->pev), infobuffer, szNewName);
}

bool CBasePlayer_HasRestrictItem(IReGameHook_CBasePlayer_HasRestrictItem *chain, CBasePlayer *pthis, ItemID item, ItemRestType type)
{
	auto original = [chain](int _pthis, ItemID _item, ItemRestType _type)
	{
		return chain->callNext(getPrivate<CBasePlayer>(_pthis), _item, _type);
	};

	return callForward<bool>(RG_CBasePlayer_HasRestrictItem, original, indexOfEdict(pthis->pev), item, type);
}

CBaseEntity *CBasePlayer_DropPlayerItem(IReGameHook_CBasePlayer_DropPlayerItem *chain, CBasePlayer *pthis, const char *pszItemName)
{
	auto original = [chain](int _pthis, const char *_pszItemName)
	{
		return indexOfPDataAmx(chain->callNext(getPrivate<CBasePlayer>(_pthis), _pszItemName));
	};

	return getPrivate<CBaseEntity>(callForward<size_t>(RG_CBasePlayer_DropPlayerItem, original, indexOfEdict(pthis->pev), pszItemName));
}

CBaseEntity *CBasePlayer_DropShield(IReGameHook_CBasePlayer_DropShield *chain, CBasePlayer *pthis, bool bDeploy)
{
	auto original = [chain](int _pthis, bool _bDeploy)
	{
		return indexOfPDataAmx(chain->callNext(getPrivate<CBasePlayer>(_pthis), _bDeploy));
	};

	return getPrivate<CBaseEntity>(callForward<size_t>(RG_CBasePlayer_DropShield, original, indexOfEdict(pthis->pev), bDeploy));
}

void CBasePlayer_OnSpawnEquip(IReGameHook_CBasePlayer_OnSpawnEquip *chain, CBasePlayer *pthis, bool addDefault, bool equipGame)
{
	auto original = [chain](int _pthis, bool _addDefault, bool _equipGame)
	{
		chain->callNext(getPrivate<CBasePlayer>(_pthis), _addDefault, _equipGame);
	};

	callVoidForward(RG_CBasePlayer_OnSpawnEquip, original, indexOfEdict(pthis->pev), addDefault, equipGame);
}

void CBasePlayer_Radio(IReGameHook_CBasePlayer_Radio *chain, CBasePlayer *pthis, const char *msg_id, const char *msg_verbose, short pitch, bool showIcon)
{
	auto original = [chain](int _pthis, const char *_msg_id, const char *_msg_verbose, short _pitch, bool _showIcon)
	{
		chain->callNext(getPrivate<CBasePlayer>(_pthis), _msg_id, _msg_verbose, _pitch, _showIcon);
	};

	callVoidForward(RG_CBasePlayer_Radio, original, indexOfEdict(pthis->pev), msg_id, msg_verbose, pitch, showIcon);
}

void CBasePlayer_Disappear(IReGameHook_CBasePlayer_Disappear *chain, CBasePlayer *pthis)
{
	auto original = [chain](int _pthis)
	{
		chain->callNext(getPrivate<CBasePlayer>(_pthis));
	};

	callVoidForward(RG_CBasePlayer_Disappear, original, indexOfEdict(pthis->pev));
}

void CBasePlayer_MakeVIP(IReGameHook_CBasePlayer_MakeVIP *chain, CBasePlayer *pthis)
{
	auto original = [chain](int _pthis)
	{
		chain->callNext(getPrivate<CBasePlayer>(_pthis));
	};

	callVoidForward(RG_CBasePlayer_MakeVIP, original, indexOfEdict(pthis->pev));
}

bool CBasePlayer_MakeBomber(IReGameHook_CBasePlayer_MakeBomber *chain, CBasePlayer *pthis)
{
	auto original = [chain](int _pthis)
	{
		return chain->callNext(getPrivate<CBasePlayer>(_pthis));
	};

	return callForward<bool>(RG_CBasePlayer_MakeBomber, original, indexOfEdict(pthis->pev));
}

void CBasePlayer_StartObserver(IReGameHook_CBasePlayer_StartObserver *chain, CBasePlayer *pthis, Vector &vecPosition, Vector &vecViewAngle)
{
	Vector vecPositionCopy(vecPosition), vecViewAngleCopy(vecViewAngle);

	auto original = [chain, &vecPositionCopy, &vecViewAngleCopy](int _pthis, cell _vecPosition, cell _vecViewAngle)
	{
		chain->callNext(getPrivate<CBasePlayer>(_pthis), vecPositionCopy, vecViewAngleCopy);
	};

	callVoidForward(RG_CBasePlayer_StartObserver, original, indexOfEdict(pthis->pev), getAmxVector(vecPosition), getAmxVector(vecViewAngle));
}

bool CBasePlayer_GetIntoGame(IReGameHook_CBasePlayer_GetIntoGame *chain, CBasePlayer *pthis)
{
	auto original = [chain](int _pthis)
	{
		return chain->callNext(getPrivate<CBasePlayer>(_pthis));
	};

	return callForward<bool>(RG_CBasePlayer_GetIntoGame, original, indexOfEdict(pthis->pev));
}

void CBaseAnimating_ResetSequenceInfo(IReGameHook_CBaseAnimating_ResetSequenceInfo *chain, CBaseAnimating *pthis)
{
	auto original = [chain](int _pthis)
	{
		chain->callNext(getPrivate<CBasePlayer>(_pthis));
	};

	callVoidForward(RG_CBaseAnimating_ResetSequenceInfo, original, indexOfEdict(pthis->pev));
}

int GetForceCamera(IReGameHook_GetForceCamera *chain, CBasePlayer *pObserver)
{
	auto original = [chain](int _pObserver)
	{
		return chain->callNext(getPrivate<CBasePlayer>(_pObserver));
	};

	return callForward<int>(RG_GetForceCamera, original, indexOfEdict(pObserver->pev));
}

void PlayerBlind(IReGameHook_PlayerBlind *chain, CBasePlayer *pPlayer, entvars_t *pevInflictor, entvars_t *pevAttacker, float fadeTime, float fadeHold, int alpha, Vector& color)
{
	Vector colorCopy(color);

	auto original = [chain, &colorCopy](int _pPlayer, int _pevInflictor, int _pevAttacker, float _fadeTime, float _fadeHold, int _alpha, cell _color)
	{
		chain->callNext(getPrivate<CBasePlayer>(_pPlayer), PEV(_pevInflictor), PEV(_pevAttacker), _fadeTime, _fadeHold, _alpha, colorCopy);
	};

	callVoidForward(RG_PlayerBlind, original, indexOfEdict(pPlayer->pev), indexOfEdict(pevInflictor), indexOfEdict(pevAttacker), fadeTime, fadeHold, alpha, getAmxVector(colorCopy));
}

void RadiusFlash_TraceLine(IReGameHook_RadiusFlash_TraceLine *chain, CBasePlayer *pPlayer, entvars_t *pevInflictor, entvars_t *pevAttacker, Vector& vecSrc, Vector& vecSpot, TraceResult *ptr)
{
	Vector vecSrcCopy(vecSrc), vecSpotCopy(vecSpot);

	auto original = [chain, &vecSrcCopy, &vecSpotCopy](int _pPlayer, int _pevInflictor, int _pevAttacker, cell _vecSrc, cell _vecSpot, TraceResult *_ptr)
	{
		chain->callNext(getPrivate<CBasePlayer>(_pPlayer), PEV(_pevInflictor), PEV(_pevAttacker), vecSrcCopy, vecSpotCopy, _ptr);
	};

	callVoidForward(RG_RadiusFlash_TraceLine, original, indexOfEdict(pPlayer->pev), indexOfEdict(pevInflictor), indexOfEdict(pevAttacker), getAmxVector(vecSrcCopy), getAmxVector(vecSpotCopy), ptr);
}

bool RoundEnd(IReGameHook_RoundEnd *chain, int winStatus, ScenarioEventEndRound event, float tmDelay)
{
	auto original = [chain](int _winStatus, ScenarioEventEndRound _event, float _tmDelay)
	{
		return chain->callNext(_winStatus, _event, _tmDelay);
	};

	return callForward<bool>(RG_RoundEnd, original, winStatus, event, tmDelay);
}

void PM_Move_AMXX(Move_t *data, int playerIndex)
{
	auto original = [data](int _playerIndex)
	{
		data->m_chain->callNext(data->m_args.ppmove, data->m_args.server);
	};

	callVoidForward(RG_PM_Move, original, playerIndex);
}

void PM_Move(IReGameHook_PM_Move *chain, playermove_t *ppmove, int server)
{
	g_pMove = ppmove;

	Move_args_t args(ppmove, server);
	Move_t data(chain, args);
	PM_Move_AMXX(&data, ppmove->player_index + 1);
}

void PM_AirMove(IReGameHook_PM_AirMove *chain, int playerIndex)
{
	auto original = [chain](int _playerIndex)
	{
		chain->callNext(_playerIndex);
	};

	callVoidForward(RG_PM_AirMove, original, playerIndex);
}

void HandleMenu_ChooseAppearance(IReGameHook_HandleMenu_ChooseAppearance *chain, CBasePlayer *pPlayer, int slot)
{
	auto original = [chain](int _pPlayer, int _slot)
	{
		chain->callNext(getPrivate<CBasePlayer>(_pPlayer), _slot);
	};

	callVoidForward(RG_HandleMenu_ChooseAppearance, original, indexOfEdict(pPlayer->pev), slot);
}

BOOL HandleMenu_ChooseTeam(IReGameHook_HandleMenu_ChooseTeam *chain, CBasePlayer *pPlayer, int slot)
{
	auto original = [chain](int _pPlayer, int _slot)
	{
		return chain->callNext(getPrivate<CBasePlayer>(_pPlayer), _slot);
	};

	return callForward<BOOL>(RG_HandleMenu_ChooseTeam, original, indexOfEdict(pPlayer->pev), slot);
}

void ShowMenu(IReGameHook_ShowMenu *chain, CBasePlayer *pPlayer, int bitsValidSlots, int nDisplayTime, BOOL fNeedMore, char *pszText)
{
	auto original = [chain](int _pPlayer, int _bitsValidSlots, int _nDisplayTime, BOOL _fNeedMore, char *_pszText)
	{
		chain->callNext(getPrivate<CBasePlayer>(_pPlayer), _bitsValidSlots, _nDisplayTime, _fNeedMore, _pszText);
	};

	callVoidForward(RG_ShowMenu, original, indexOfEdict(pPlayer->pev), bitsValidSlots, nDisplayTime, fNeedMore, pszText);
}

void ShowVGUIMenu(IReGameHook_ShowVGUIMenu *chain, CBasePlayer *pPlayer, int MenuType, int BitMask, char *szOldMenu)
{
	auto original = [chain](int _pPlayer, int _MenuType, int _BitMask, char *_szOldMenu)
	{
		chain->callNext(getPrivate<CBasePlayer>(_pPlayer), _MenuType, _BitMask, _szOldMenu);
	};

	callVoidForward(RG_ShowVGUIMenu, original, indexOfEdict(pPlayer->pev), MenuType, BitMask, szOldMenu);
}

bool BuyGunAmmo(IReGameHook_BuyGunAmmo *chain, CBasePlayer *player, CBasePlayerItem *weapon, bool bBlinkMoney)
{
	auto original = [chain](int _player, int _weapon, bool _bBlinkMoney)
	{
		return chain->callNext(getPrivate<CBasePlayer>(_player), getPrivate<CBasePlayerItem>(_weapon), _bBlinkMoney);
	};

	return callForward<bool>(RG_BuyGunAmmo, original, indexOfEdict(player->pev), indexOfEdict(weapon->pev), bBlinkMoney);
}

CBaseEntity *BuyWeaponByWeaponID(IReGameHook_BuyWeaponByWeaponID *chain, CBasePlayer *pPlayer, WeaponIdType weaponID)
{
	auto original = [chain](int _pPlayer, WeaponIdType _weaponID)
	{
		return indexOfPDataAmx(chain->callNext(getPrivate<CBasePlayer>(_pPlayer), _weaponID));
	};

	return getPrivate<CBaseEntity>(callForward<size_t>(RG_BuyWeaponByWeaponID, original, indexOfEdict(pPlayer->pev), weaponID));
}

BOOL CSGameRules_FShouldSwitchWeapon(IReGameHook_CSGameRules_FShouldSwitchWeapon *chain, CBasePlayer *pPlayer, CBasePlayerItem *pWeapon)
{
	auto original = [chain](int _pPlayer, int _pWeapon)
	{
		return chain->callNext(getPrivate<CBasePlayer>(_pPlayer), getPrivate<CBasePlayerItem>(_pWeapon));
	};

	return callForward<BOOL>(RG_CSGameRules_FShouldSwitchWeapon, original, indexOfEdict(pPlayer->pev), indexOfEdict(pWeapon->pev));
}

BOOL CSGameRules_GetNextBestWeapon(IReGameHook_CSGameRules_GetNextBestWeapon *chain, CBasePlayer *pPlayer, CBasePlayerItem *pCurrentWeapon)
{
	auto original = [chain](int _pPlayer, int _pCurrentWeapon)
	{
		return chain->callNext(getPrivate<CBasePlayer>(_pPlayer), getPrivate<CBasePlayerItem>(_pCurrentWeapon));
	};

	return callForward<BOOL>(RG_CSGameRules_GetNextBestWeapon, original, indexOfEdict(pPlayer->pev), indexOfEdict(pCurrentWeapon->pev));
}

float CSGameRules_FlPlayerFallDamage(IReGameHook_CSGameRules_FlPlayerFallDamage *chain, CBasePlayer *pPlayer)
{
	auto original = [chain](int _pPlayer)
	{
		return chain->callNext(getPrivate<CBasePlayer>(_pPlayer));
	};

	return callForward<float>(RG_CSGameRules_FlPlayerFallDamage, original, indexOfEdict(pPlayer->pev));
}

BOOL CSGameRules_FPlayerCanTakeDamage(IReGameHook_CSGameRules_FPlayerCanTakeDamage *chain, CBasePlayer *pPlayer, CBaseEntity *pAttacker)
{
	auto original = [chain](int _pPlayer, int _pAttacker)
	{
		return chain->callNext(getPrivate<CBasePlayer>(_pPlayer), getPrivate<CBaseEntity>(_pAttacker));
	};

	return callForward<BOOL>(RG_CSGameRules_FPlayerCanTakeDamage, original, indexOfEdict(pPlayer->pev), indexOfEdict(pAttacker->pev));
}

void CSGameRules_PlayerSpawn(IReGameHook_CSGameRules_PlayerSpawn *chain, CBasePlayer *pPlayer)
{
	auto original = [chain](int _pPlayer)
	{
		chain->callNext(getPrivate<CBasePlayer>(_pPlayer));
	};

	callVoidForward(RG_CSGameRules_PlayerSpawn, original, indexOfEdict(pPlayer->pev));
}

BOOL CSGameRules_FPlayerCanRespawn(IReGameHook_CSGameRules_FPlayerCanRespawn *chain, CBasePlayer *pPlayer)
{
	auto original = [chain](int _pPlayer)
	{
		return chain->callNext(getPrivate<CBasePlayer>(_pPlayer));
	};

	return callForward<BOOL>(RG_CSGameRules_FPlayerCanRespawn, original, indexOfEdict(pPlayer->pev));
}

edict_t *CSGameRules_GetPlayerSpawnSpot(IReGameHook_CSGameRules_GetPlayerSpawnSpot *chain, CBasePlayer *pPlayer)
{
	auto original = [chain](int _pPlayer)
	{
		return indexOfEdict(chain->callNext(getPrivate<CBasePlayer>(_pPlayer)));
	};

	return edictByIndexAmx(callForward<size_t>(RG_CSGameRules_GetPlayerSpawnSpot, original, indexOfEdict(pPlayer->pev)));
}

void CSGameRules_ClientUserInfoChanged(IReGameHook_CSGameRules_ClientUserInfoChanged *chain, CBasePlayer *pPlayer, char *infobuffer)
{
	auto original = [chain](int _pPlayer, char *_infobuffer)
	{
		chain->callNext(getPrivate<CBasePlayer>(_pPlayer), _infobuffer);
	};

	callVoidForward(RG_CSGameRules_ClientUserInfoChanged, original, indexOfEdict(pPlayer->pev), infobuffer);
}

void CSGameRules_PlayerKilled(IReGameHook_CSGameRules_PlayerKilled *chain, CBasePlayer *pVictim, entvars_t *pevKiller, entvars_t *pevInflictor)
{
	auto original = [chain](int _pVictim, int _pevKiller, int _pevInflictor)
	{
		chain->callNext(getPrivate<CBasePlayer>(_pVictim), PEV(_pevKiller), PEV(_pevInflictor));
	};

	callVoidForward(RG_CSGameRules_PlayerKilled, original, indexOfEdict(pVictim->pev), indexOfEdict(pevKiller), indexOfEdictAmx(pevInflictor));
}

void CSGameRules_DeathNotice(IReGameHook_CSGameRules_DeathNotice *chain, CBasePlayer *pVictim, entvars_t *pevKiller, entvars_t *pevInflictor)
{
	auto original = [chain](int _pVictim, int _pevKiller, int _pevInflictor)
	{
		chain->callNext(getPrivate<CBasePlayer>(_pVictim), PEV(_pevKiller), PEV(_pevInflictor));
	};

	callVoidForward(RG_CSGameRules_DeathNotice, original, indexOfEdict(pVictim->pev), indexOfEdict(pevKiller), indexOfEdictAmx(pevInflictor));
}

int CSGameRules_CanHavePlayerItem(IReGameHook_CSGameRules_CanHavePlayerItem *chain, CBasePlayer *pPlayer, CBasePlayerItem *pItem)
{
	auto original = [chain](int _pPlayer, int _pItem)
	{
		return chain->callNext(getPrivate<CBasePlayer>(_pPlayer), getPrivate<CBasePlayerItem>(_pItem));
	};

	return callForward<int>(RG_CSGameRules_CanHavePlayerItem, original, indexOfEdict(pPlayer->pev), indexOfEdict(pItem->pev));
}

int CSGameRules_DeadPlayerWeapons(IReGameHook_CSGameRules_DeadPlayerWeapons *chain, CBasePlayer *pPlayer)
{
	auto original = [chain](int _pPlayer)
	{
		return chain->callNext(getPrivate<CBasePlayer>(_pPlayer));
	};

	return callForward<int>(RG_CSGameRules_DeadPlayerWeapons, original, indexOfEdict(pPlayer->pev));
}

void CSGameRules_ServerDeactivate(IReGameHook_CSGameRules_ServerDeactivate *chain)
{
	auto original = [chain]()
	{
		chain->callNext();
	};

	callVoidForward(RG_CSGameRules_ServerDeactivate, original);
}

void CSGameRules_CheckMapConditions(IReGameHook_CSGameRules_CheckMapConditions *chain)
{
	auto original = [chain]()
	{
		chain->callNext();
	};

	callVoidForward(RG_CSGameRules_CheckMapConditions, original);
}

void CSGameRules_CleanUpMap(IReGameHook_CSGameRules_CleanUpMap *chain)
{
	auto original = [chain]()
	{
		chain->callNext();
	};

	callVoidForward(RG_CSGameRules_CleanUpMap, original);
}

void CSGameRules_RestartRound(IReGameHook_CSGameRules_RestartRound *chain)
{
	auto original = [chain]()
	{
		chain->callNext();
	};

	callVoidForward(RG_CSGameRules_RestartRound, original);
}

void CSGameRules_CheckWinConditions(IReGameHook_CSGameRules_CheckWinConditions *chain)
{
	auto original = [chain]()
	{
		chain->callNext();
	};

	callVoidForward(RG_CSGameRules_CheckWinConditions, original);
}

void CSGameRules_RemoveGuns(IReGameHook_CSGameRules_RemoveGuns *chain)
{
	auto original = [chain]()
	{
		chain->callNext();
	};

	callVoidForward(RG_CSGameRules_RemoveGuns, original);
}

CBasePlayer *CSGameRules_GiveC4(IReGameHook_CSGameRules_GiveC4 *chain)
{
	auto original = [chain]()
	{
		return indexOfPDataAmx(chain->callNext());
	};

	return getPrivate<CBasePlayer>(callForward<size_t>(RG_CSGameRules_GiveC4, original));
}

void CSGameRules_ChangeLevel(IReGameHook_CSGameRules_ChangeLevel *chain)
{
	auto original = [chain]()
	{
		chain->callNext();
	};

	callVoidForward(RG_CSGameRules_ChangeLevel, original);
}

void CSGameRules_GoToIntermission(IReGameHook_CSGameRules_GoToIntermission *chain)
{
	auto original = [chain]()
	{
		chain->callNext();
	};

	callVoidForward(RG_CSGameRules_GoToIntermission, original);
}

void CSGameRules_BalanceTeams(IReGameHook_CSGameRules_BalanceTeams *chain)
{
	auto original = [chain]()
	{
		chain->callNext();
	};

	callVoidForward(RG_CSGameRules_BalanceTeams, original);
}

void CSGameRules_OnRoundFreezeEnd(IReGameHook_CSGameRules_OnRoundFreezeEnd *chain)
{
	auto original = [chain]()
	{
		chain->callNext();
	};

	callVoidForward(RG_CSGameRules_OnRoundFreezeEnd, original);
}

void CBasePlayer_StartDeathCam(IReGameHook_CBasePlayer_StartDeathCam *chain, CBasePlayer *pthis)
{
	auto original = [chain](int _pthis)
	{
		return chain->callNext(getPrivate<CBasePlayer>(_pthis));
	};

	callVoidForward(RG_CBasePlayer_StartDeathCam, original, indexOfEdict(pthis->pev));
}

void CBasePlayer_SwitchTeam(IReGameHook_CBasePlayer_SwitchTeam *chain, CBasePlayer *pthis)
{
	auto original = [chain](int _pthis)
	{
		chain->callNext(getPrivate<CBasePlayer>(_pthis));
	};

	callVoidForward(RG_CBasePlayer_SwitchTeam, original, indexOfEdict(pthis->pev));
}

bool CBasePlayer_CanSwitchTeam(IReGameHook_CBasePlayer_CanSwitchTeam *chain, CBasePlayer *pthis, TeamName teamToSwap)
{
	auto original = [chain](int _pthis, TeamName _teamToSwap)
	{
		return chain->callNext(getPrivate<CBasePlayer>(_pthis), _teamToSwap);
	};

	return callForward<bool>(RG_CBasePlayer_CanSwitchTeam, original, indexOfEdict(pthis->pev), teamToSwap);
}

CGrenade *CBasePlayer_ThrowGrenade(IReGameHook_CBasePlayer_ThrowGrenade *chain, CBasePlayer *pthis, CBasePlayerWeapon *pWeapon, Vector &vecSrc, Vector &vecThrow, float time, unsigned short usEvent)
{
	Vector vecSrcCopy(vecSrc), vecThrowCopy(vecThrow);

	auto original = [chain, &vecSrcCopy, &vecThrowCopy](int _pthis, int _pWeapon, cell _vecSrc, cell _vecThrow, float _time, unsigned short _usEvent)
	{
		return indexOfPDataAmx(chain->callNext(getPrivate<CBasePlayer>(_pthis), getPrivate<CBasePlayerWeapon>(_pWeapon), vecSrcCopy, vecThrowCopy, _time, _usEvent));
	};

	return getPrivate<CGrenade>(callForward<size_t>(RG_CBasePlayer_ThrowGrenade, original, indexOfEdict(pthis->pev), indexOfEdict(pWeapon->pev), getAmxVector(vecSrcCopy), getAmxVector(vecThrowCopy), time, usEvent));
}

bool CSGameRules_CanPlayerHearPlayer(IReGameHook_CSGameRules_CanPlayerHearPlayer *chain, CBasePlayer *pListener, CBasePlayer *pSender)
{
	auto original = [chain](int _pListener, int _pSender)
	{
		return chain->callNext(getPrivate<CBasePlayer>(_pListener), getPrivate<CBasePlayer>(_pSender));
	};

	return callForward<bool>(RG_CSGameRules_CanPlayerHearPlayer, original, indexOfEdict(pListener->pev), indexOfEdict(pSender->pev));
}

void CWeaponBox_SetModel(IReGameHook_CWeaponBox_SetModel *chain, CWeaponBox *pthis, const char *pszModelName)
{
	auto original = [chain](int _pthis, const char *_pszModelName)
	{
		chain->callNext(getPrivate<CWeaponBox>(_pthis), _pszModelName);
	};

	callVoidForward(RG_CWeaponBox_SetModel, original, indexOfEdict(pthis->pev), pszModelName);
}

void CGrenade_DefuseBombStart(IReGameHook_CGrenade_DefuseBombStart *chain, CGrenade *pthis, CBasePlayer *pPlayer)
{
	auto original = [chain](int _pthis, int _pPlayer)
	{
		chain->callNext(getPrivate<CGrenade>(_pthis), getPrivate<CBasePlayer>(_pPlayer));
	};

	callVoidForward(RG_CGrenade_DefuseBombStart, original, indexOfEdict(pthis->pev), indexOfEdict(pPlayer->pev));
}

void CGrenade_DefuseBombEnd(IReGameHook_CGrenade_DefuseBombEnd *chain, CGrenade *pthis, CBasePlayer *pPlayer, bool bDefused)
{
	auto original = [chain](int _pthis, int _pPlayer, bool _bDefused)
	{
		chain->callNext(getPrivate<CGrenade>(_pthis), getPrivate<CBasePlayer>(_pPlayer), _bDefused);
	};

	callVoidForward(RG_CGrenade_DefuseBombEnd, original, indexOfEdict(pthis->pev), indexOfEdict(pPlayer->pev), bDefused);
}

void CGrenade_ExplodeHeGrenade(IReGameHook_CGrenade_ExplodeHeGrenade *chain, CGrenade *pthis, TraceResult *ptr, int bitsDamageType)
{
	auto original = [chain](int _pthis, TraceResult *_ptr, int _bitsDamageType)
	{
		chain->callNext(getPrivate<CGrenade>(_pthis), _ptr, _bitsDamageType);
	};

	callVoidForward(RG_CGrenade_ExplodeHeGrenade, original, indexOfEdict(pthis->pev), ptr, bitsDamageType);
}

void CGrenade_ExplodeFlashbang(IReGameHook_CGrenade_ExplodeFlashbang *chain, CGrenade *pthis, TraceResult *ptr, int bitsDamageType)
{
	auto original = [chain](int _pthis, TraceResult *_ptr, int _bitsDamageType)
	{
		chain->callNext(getPrivate<CGrenade>(_pthis), _ptr, _bitsDamageType);
	};

	callVoidForward(RG_CGrenade_ExplodeFlashbang, original, indexOfEdict(pthis->pev), ptr, bitsDamageType);
}

void CGrenade_ExplodeSmokeGrenade(IReGameHook_CGrenade_ExplodeSmokeGrenade *chain, CGrenade *pthis)
{
	auto original = [chain](int _pthis)
	{
		chain->callNext(getPrivate<CGrenade>(_pthis));
	};

	callVoidForward(RG_CGrenade_ExplodeSmokeGrenade, original, indexOfEdict(pthis->pev));
}

void CGrenade_ExplodeBomb(IReGameHook_CGrenade_ExplodeBomb *chain, CGrenade *pthis, TraceResult *ptr, int bitsDamageType)
{
	auto original = [chain](int _pthis, TraceResult *_ptr, int _bitsDamageType)
	{
		chain->callNext(getPrivate<CGrenade>(_pthis), _ptr, _bitsDamageType);
	};

	callVoidForward(RG_CGrenade_ExplodeBomb, original, indexOfEdict(pthis->pev), ptr, bitsDamageType);
}

CGrenade *ThrowHeGrenade(IReGameHook_ThrowHeGrenade *chain, entvars_t *pevOwner, Vector &vecStart, Vector &vecVelocity, float time, int iTeam, unsigned short usEvent)
{
	Vector vecStartCopy(vecStart), vecVelocityCopy(vecVelocity);

	auto original = [chain, &vecStartCopy, &vecVelocityCopy](int _pevOwner, cell _vecStart, cell _vecVelocity, float _time, int _iTeam, unsigned short _usEvent)
	{
		return indexOfPDataAmx(chain->callNext(PEV(_pevOwner), vecStartCopy, vecVelocityCopy, _time, _iTeam, _usEvent));
	};

	return getPrivate<CGrenade>(callForward<size_t>(RG_ThrowHeGrenade, original, indexOfEdict(pevOwner), getAmxVector(vecStartCopy), getAmxVector(vecVelocityCopy), time, iTeam, usEvent));
}

CGrenade *ThrowFlashbang(IReGameHook_ThrowFlashbang *chain, entvars_t *pevOwner, Vector &vecStart, Vector &vecVelocity, float time)
{
	Vector vecStartCopy(vecStart), vecVelocityCopy(vecVelocity);

	auto original = [chain, &vecStartCopy, &vecVelocityCopy](int _pevOwner, cell _vecStart, cell _vecVelocity, float _time)
	{
		return indexOfPDataAmx(chain->callNext(PEV(_pevOwner), vecStartCopy, vecVelocityCopy, _time));
	};

	return getPrivate<CGrenade>(callForward<size_t>(RG_ThrowFlashbang, original, indexOfEdict(pevOwner), getAmxVector(vecStartCopy), getAmxVector(vecVelocityCopy), time));
}

CGrenade *ThrowSmokeGrenade(IReGameHook_ThrowSmokeGrenade *chain, entvars_t *pevOwner, Vector &vecStart, Vector &vecVelocity, float time, unsigned short usEvent)
{
	Vector vecStartCopy(vecStart), vecVelocityCopy(vecVelocity);

	auto original = [chain, &vecStartCopy, &vecVelocityCopy](int _pevOwner, cell _vecStart, cell _vecVelocity, float _time, unsigned short _usEvent)
	{
		return indexOfPDataAmx(chain->callNext(PEV(_pevOwner), vecStartCopy, vecVelocityCopy, _time, _usEvent));
	};

	return getPrivate<CGrenade>(callForward<size_t>(RG_ThrowSmokeGrenade, original, indexOfEdict(pevOwner), getAmxVector(vecStartCopy), getAmxVector(vecVelocityCopy), time, usEvent));
}

CGrenade *PlantBomb(IReGameHook_PlantBomb *chain, entvars_t *pevOwner, Vector &vecStart, Vector &vecVelocity)
{
	Vector vecStartCopy(vecStart), vecVelocityCopy(vecVelocity);

	auto original = [chain, &vecStartCopy, &vecVelocityCopy](int _pevOwner, cell _vecStart, cell _vecVelocity)
	{
		return indexOfPDataAmx(chain->callNext(PEV(_pevOwner), vecStartCopy, vecVelocityCopy));
	};

	return getPrivate<CGrenade>(callForward<size_t>(RG_PlantBomb, original, indexOfEdictAmx(pevOwner), getAmxVector(vecStartCopy), getAmxVector(vecVelocityCopy)));
}

void CBasePlayer_RemoveSpawnProtection(IReGameHook_CBasePlayer_RemoveSpawnProtection *chain, CBasePlayer *pthis)
{
	auto original = [chain](int _pthis)
	{
		return chain->callNext(getPrivate<CBasePlayer>(_pthis));
	};

	callVoidForward(RG_CBasePlayer_RemoveSpawnProtection, original, indexOfEdict(pthis->pev));
}

void CBasePlayer_SetSpawnProtection(IReGameHook_CBasePlayer_SetSpawnProtection *chain, CBasePlayer *pthis, float flProtectionTime)
{
	auto original = [chain](int _pthis, float _flProtectionTime)
	{
		return chain->callNext(getPrivate<CBasePlayer>(_pthis), _flProtectionTime);
	};

	callVoidForward(RG_CBasePlayer_SetSpawnProtection, original, indexOfEdict(pthis->pev), flProtectionTime);
}

bool IsPenetrableEntity(IReGameHook_IsPenetrableEntity *chain, Vector &vecSrc, Vector &vecEnd, entvars_t *pevAttacker, edict_t *pHit)
{
	Vector vecSrcCopy(vecSrc), vecEndCopy(vecEnd);

	auto original = [chain, &vecSrcCopy, &vecEndCopy](cell _vecSrc, cell _vecEnd, int _pevAttacker, int _pHit)
	{
		return chain->callNext(vecSrcCopy, vecEndCopy, PEV(_pevAttacker), edictByIndexAmx(_pHit));
	};

	return callForward<bool>(RG_IsPenetrableEntity, original, getAmxVector(vecSrcCopy), getAmxVector(vecEndCopy), indexOfEdict(pevAttacker), indexOfEdict(pHit));
}

bool CBasePlayer_HintMessageEx(IReGameHook_CBasePlayer_HintMessageEx *chain, CBasePlayer *pthis, const char *pMessage, float duration, bool bDisplayIfPlayerDead, bool bOverride)
{
	auto original = [chain](int _pthis, const char *_pMessage, float _duration, bool _bDisplayIfPlayerDead, bool _bOverride)
	{
		return chain->callNext(getPrivate<CBasePlayer>(_pthis), _pMessage, _duration, _bDisplayIfPlayerDead, _bOverride);
	};

	return callForward<bool>(RG_CBasePlayer_HintMessageEx, original, indexOfEdict(pthis->pev), pMessage, duration, bDisplayIfPlayerDead, bOverride);
}

void CBasePlayer_UseEmpty(IReGameHook_CBasePlayer_UseEmpty *chain, CBasePlayer *pthis)
{
	auto original = [chain](int _pthis)
	{
		return chain->callNext(getPrivate<CBasePlayer>(_pthis));
	};

	callVoidForward(RG_CBasePlayer_UseEmpty, original, indexOfEdict(pthis->pev));
}

BOOL CBasePlayerWeapon_CanDeploy(IReGameHook_CBasePlayerWeapon_CanDeploy *chain, CBasePlayerWeapon *pthis)
{
	auto original = [chain](int _pthis)
	{
		return chain->callNext(getPrivate<CBasePlayerWeapon>(_pthis));
	};

	return callForward<BOOL>(RG_CBasePlayerWeapon_CanDeploy, original, indexOfEdict(pthis->pev));
}

BOOL CBasePlayerWeapon_DefaultDeploy(IReGameHook_CBasePlayerWeapon_DefaultDeploy *chain, CBasePlayerWeapon *pthis, char *szViewModel, char *szWeaponModel, int iAnim, char *szAnimExt, int skiplocal)
{
	auto original = [chain](int _pthis, char *_szViewModel, char *_szWeaponModel, int _iAnim, char *_szAnimExt, int _skiplocal)
	{
		return chain->callNext(getPrivate<CBasePlayerWeapon>(_pthis), _szViewModel, _szWeaponModel, _iAnim, _szAnimExt, _skiplocal);
	};

	return callForward<BOOL>(RG_CBasePlayerWeapon_DefaultDeploy, original, indexOfEdict(pthis->pev), szViewModel, szWeaponModel, iAnim, szAnimExt, skiplocal);
}

int CBasePlayerWeapon_DefaultReload(IReGameHook_CBasePlayerWeapon_DefaultReload *chain, CBasePlayerWeapon *pthis, int iClipSize, int iAnim, float fDelay)
{
	auto original = [chain](int _pthis, int _iClipSize, int _iAnim, float _fDelay)
	{
		return chain->callNext(getPrivate<CBasePlayerWeapon>(_pthis), _iClipSize, _iAnim, _fDelay);
	};

	return callForward<int>(RG_CBasePlayerWeapon_DefaultReload, original, indexOfEdict(pthis->pev), iClipSize, iAnim, fDelay);
}

bool CBasePlayerWeapon_DefaultShotgunReload(IReGameHook_CBasePlayerWeapon_DefaultShotgunReload *chain, CBasePlayerWeapon *pthis, int iAnim, int iStartAnim, float fDelay, float fStartDelay, const char *pszReloadSound1, const char *pszReloadSound2)
{
	auto original = [chain](int _pthis, int _iAnim, int _iStartAnim, float _fDelay, float _fStartDelay, const char *_pszReloadSound1, const char *_pszReloadSound2)
	{
		return chain->callNext(getPrivate<CBasePlayerWeapon>(_pthis), _iAnim, _iStartAnim, _fDelay, _fStartDelay, _pszReloadSound1, _pszReloadSound2);
	};

	return callForward<bool>(RG_CBasePlayerWeapon_DefaultShotgunReload, original, indexOfEdict(pthis->pev), iAnim, iStartAnim, fDelay, fStartDelay, pszReloadSound1, pszReloadSound2);
}

void CBasePlayer_DropIdlePlayer(IReGameHook_CBasePlayer_DropIdlePlayer *chain, CBasePlayer *pthis, const char *reason)
{
	auto original = [chain](int _pthis, const char *_reason)
	{
		return chain->callNext(getPrivate<CBasePlayer>(_pthis), _reason);
	};

	callVoidForward(RG_CBasePlayer_DropIdlePlayer, original, indexOfEdict(pthis->pev), reason);
}

CWeaponBox *CreateWeaponBox(IReGameHook_CreateWeaponBox *chain, CBasePlayerItem *pItem, CBasePlayer *pPlayerOwner, const char *modelName, Vector &origin, Vector &angles, Vector &velocity, float lifeTime, bool packAmmo)
{
	Vector vecOriginCopy(origin), vecAnglesCopy(angles), vecVelocityCopy(velocity);

	auto original = [chain, &vecOriginCopy, &vecAnglesCopy, &vecVelocityCopy](int _pItem, int _pPlayerOwner, const char *_modelName, cell _origin, cell _angles, cell _velocity, float _lifeTime, bool _packAmmo)
	{
		return indexOfPDataAmx(chain->callNext(getPrivate<CBasePlayerItem>(_pItem), getPrivate<CBasePlayer>(_pPlayerOwner), _modelName, vecOriginCopy, vecAnglesCopy, vecVelocityCopy, _lifeTime, _packAmmo));
	};

	return getPrivate<CWeaponBox>(callForward<size_t>(RG_CreateWeaponBox, original, indexOfEdictAmx(pItem->pev), indexOfEdictAmx(pPlayerOwner->pev), modelName, getAmxVector(vecOriginCopy), getAmxVector(vecAnglesCopy), getAmxVector(vecVelocityCopy), lifeTime, packAmmo));
}

CGib *SpawnHeadGib(IReGameHook_SpawnHeadGib *chain, entvars_t *pevVictim)
{
	auto original = [chain](int _pevVictim)
	{
		return indexOfPDataAmx(chain->callNext(PEV(_pevVictim)));
	};

	return getPrivate<CGib>(callForward<size_t>(RG_SpawnHeadGib, original, indexOfEdict(pevVictim)));
}

void SpawnRandomGibs(IReGameHook_SpawnRandomGibs *chain, entvars_t *pevVictim, int cGibs, int human)
{
	auto original = [chain](int _pevVictim, int _cGibs, int _human)
	{
		chain->callNext(PEV(_pevVictim), _cGibs, _human);
	};

	callVoidForward(RG_SpawnRandomGibs, original, indexOfEdict(pevVictim), cGibs, human);
}

void CGib_Spawn(IReGameHook_CGib_Spawn *chain, CGib *pthis, const char *szGibModel)
{
	auto original = [chain](int _pthis, const char *_szGibModel)
	{
		chain->callNext(getPrivate<CGib>(_pthis), _szGibModel);
	};

	callVoidForward(RG_CGib_Spawn, original, indexOfEdict(pthis->pev), szGibModel);
}

void CGib_BounceGibTouch(IReGameHook_CGib_BounceGibTouch *chain, CGib *pthis, CBaseEntity *pOther)
{
	auto original = [chain](int _pthis, int _pOther)
	{
		chain->callNext(getPrivate<CGib>(_pthis), getPrivate<CBaseEntity>(_pOther));
	};

	callVoidForward(RG_CGib_BounceGibTouch, original, indexOfEdict(pthis->pev), indexOfEdict(pOther->pev));
}

void CGib_WaitTillLand(IReGameHook_CGib_WaitTillLand *chain, CGib *pthis)
{
	auto original = [chain](int _pthis)
	{
		chain->callNext(getPrivate<CGib>(_pthis));
	};

	callVoidForward(RG_CGib_WaitTillLand, original, indexOfEdict(pthis->pev));
}

void CBaseEntity_FireBullets(IReGameHook_CBaseEntity_FireBullets *chain, CBaseEntity *pEntity, ULONG cShots, Vector &vecSrc, Vector &vecDirShooting, Vector &vecSpread, float flDistance, int iBulletType, int iTracerFreq, int iDamage, entvars_t *pevAttacker)
{
	Vector vecSrcCopy(vecSrc), vecDirShootingCopy(vecDirShooting), vecSpreadCopy(vecSpread);

	auto original = [chain, &vecSrcCopy, &vecDirShootingCopy, &vecSpreadCopy](int _pEntity, ULONG _cShots, cell _vecSrc, cell _vecDirShooting, cell _vecSpread, float _flDistance, int _iBulletType, int _iTracerFreq, int _iDamage, int _pevAttacker)
	{
		chain->callNext(getPrivate<CBaseEntity>(_pEntity), _cShots, vecSrcCopy, vecDirShootingCopy, vecSpreadCopy, _flDistance, _iBulletType, _iTracerFreq, _iDamage, PEV(_pevAttacker));
	};

	callVoidForward(RG_CBaseEntity_FireBullets, original, indexOfEdict(pEntity->pev), cShots, getAmxVector(vecSrcCopy), getAmxVector(vecDirShootingCopy), getAmxVector(vecSpreadCopy), flDistance, iBulletType, iTracerFreq, iDamage, indexOfEdict(pevAttacker));
}

void CBaseEntity_FireBuckshots(IReGameHook_CBaseEntity_FireBuckshots *chain, CBaseEntity *pEntity, ULONG cShots, Vector &vecSrc, Vector &vecDirShooting, Vector &vecSpread, float flDistance, int iTracerFreq, int iDamage, entvars_t *pevAttacker)
{
	Vector vecSrcCopy(vecSrc), vecDirShootingCopy(vecDirShooting), vecSpreadCopy(vecSpread);

	auto original = [chain, &vecSrcCopy, &vecDirShootingCopy, &vecSpreadCopy](int _pEntity, ULONG _cShots, cell _vecSrc, cell _vecDirShooting, cell _vecSpread, float _flDistance, int _iTracerFreq, int _iDamage, int _pevAttacker)
	{
		chain->callNext(getPrivate<CBaseEntity>(_pEntity), _cShots, vecSrcCopy, vecDirShootingCopy, vecSpreadCopy, _flDistance, _iTracerFreq, _iDamage, PEV(_pevAttacker));
	};

	callVoidForward(RG_CBaseEntity_FireBuckshots, original, indexOfEdict(pEntity->pev), cShots, getAmxVector(vecSrcCopy), getAmxVector(vecDirShootingCopy), getAmxVector(vecSpreadCopy), flDistance, iTracerFreq, iDamage, indexOfEdict(pevAttacker));
}

Vector &CBaseEntity_FireBullets3(IReGameHook_CBaseEntity_FireBullets3 *chain, CBaseEntity *pEntity, Vector &vecSrc, Vector &vecDirShooting, float vecSpread, float flDistance, int iPenetration, int iBulletType, int iDamage, float flRangeModifier, entvars_t *pevAttacker, bool bPistol, int shared_rand)
{
	Vector vecSrcCopy(vecSrc), vecDirShootingCopy(vecDirShooting);

	auto original = [chain, &vecSrcCopy, &vecDirShootingCopy](int _pEntity, cell _vecSrc, cell _vecDirShooting, float _vecSpread, float _flDistance, int _iPenetration, int _iBulletType, int _iDamage, float _flRangeModifier, int _pevAttacker, bool _bPistol, int _shared_rand) -> Vector&
	{
		return chain->callNext(getPrivate<CBaseEntity>(_pEntity), vecSrcCopy, vecDirShootingCopy, _vecSpread, _flDistance, _iPenetration, _iBulletType, _iDamage, _flRangeModifier, PEV(_pevAttacker), _bPistol, _shared_rand);
	};

	return callForward<Vector &>(RG_CBaseEntity_FireBullets3, original,
		indexOfEdict(pEntity->pev),
		getAmxVector(vecSrcCopy), getAmxVector(vecDirShootingCopy),
		vecSpread, flDistance, iPenetration, iBulletType, iDamage, flRangeModifier,
		indexOfEdict(pevAttacker),
		bPistol,
		shared_rand);
}

void CBasePlayer_Observer_SetMode(IReGameHook_CBasePlayer_Observer_SetMode *chain, CBasePlayer *pthis, int iMode)
{
	auto original = [chain](int _pthis, int _iMode)
	{
		chain->callNext(getPrivate<CBasePlayer>(_pthis), _iMode);
	};

	callVoidForward(RG_CBasePlayer_Observer_SetMode, original, indexOfEdict(pthis->pev), iMode);
}

void CBasePlayer_Observer_FindNextPlayer(IReGameHook_CBasePlayer_Observer_FindNextPlayer *chain, CBasePlayer *pthis, bool bReverse, const char *name)
{
	auto original = [chain](int _pthis, bool _bReverse, const char *_name)
	{
		chain->callNext(getPrivate<CBasePlayer>(_pthis), _bReverse, _name);
	};

	callVoidForward(RG_CBasePlayer_Observer_FindNextPlayer, original, indexOfEdict(pthis->pev), bReverse, name);
}

void CBasePlayer_Pain(IReGameHook_CBasePlayer_Pain *chain, CBasePlayer *pthis, int iLastHitGroup, bool bHasArmour)
{
	auto original = [chain](int _pthis, int _iLastHitGroup, bool _bHasArmour)
	{
		return chain->callNext(getPrivate<CBasePlayer>(_pthis), _iLastHitGroup, _bHasArmour);
	};

	callVoidForward(RG_CBasePlayer_Pain, original, indexOfEdict(pthis->pev), iLastHitGroup, bHasArmour);
}

void CBasePlayer_DeathSound(IReGameHook_CBasePlayer_DeathSound *chain, CBasePlayer *pthis)
{
	auto original = [chain](int _pthis)
	{
		return chain->callNext(getPrivate<CBasePlayer>(_pthis));
	};

	callVoidForward(RG_CBasePlayer_DeathSound, original, indexOfEdict(pthis->pev));
}

void CBasePlayer_JoiningThink(IReGameHook_CBasePlayer_JoiningThink *chain, CBasePlayer *pthis)
{
	auto original = [chain](int _pthis)
	{
		return chain->callNext(getPrivate<CBasePlayer>(_pthis));
	};

	callVoidForward(RG_CBasePlayer_JoiningThink, original, indexOfEdict(pthis->pev));
}

void PM_LadderMove_AMXX(IReGameHook_PM_LadderMove *chain, physent_t *pLadder, int playerIndex)
{
	auto original = [chain](physent_t *_pLadder, int _playerIndex)
	{
		chain->callNext(_pLadder);
	};

	callVoidForward(RG_PM_LadderMove, original, pLadder, playerIndex);
}

void PM_LadderMove(IReGameHook_PM_LadderMove *chain, physent_t *pLadder)
{
	PM_LadderMove_AMXX(chain, pLadder, g_pMove->player_index + 1);
}

void PM_WaterJump_AMXX(IReGameHook_PM_WaterJump *chain, int playerIndex)
{
	auto original = [chain](int _playerIndex)
	{
		chain->callNext();
	};

	callVoidForward(RG_PM_WaterJump, original, playerIndex);
}

void PM_WaterJump(IReGameHook_PM_WaterJump *chain)
{
	PM_WaterJump_AMXX(chain, g_pMove->player_index + 1);
}

void PM_CheckWaterJump_AMXX(IReGameHook_PM_CheckWaterJump *chain, int playerIndex)
{
	auto original = [chain](int _playerIndex)
	{
		chain->callNext();
	};

	callVoidForward(RG_PM_CheckWaterJump, original, playerIndex);
}

void PM_CheckWaterJump(IReGameHook_PM_CheckWaterJump *chain)
{
	PM_CheckWaterJump_AMXX(chain, g_pMove->player_index + 1);
}

void PM_Jump_AMXX(IReGameHook_PM_Jump *chain, int playerIndex)
{
	auto original = [chain](int _playerIndex)
	{
		chain->callNext();
	};

	callVoidForward(RG_PM_Jump, original, playerIndex);
}

void PM_Jump(IReGameHook_PM_Jump *chain)
{
	PM_Jump_AMXX(chain, g_pMove->player_index + 1);
}

void PM_Duck_AMXX(IReGameHook_PM_Duck *chain, int playerIndex)
{
	auto original = [chain](int _playerIndex)
	{
		chain->callNext();
	};

	callVoidForward(RG_PM_Duck, original, playerIndex);
}

void PM_Duck(IReGameHook_PM_Duck *chain)
{
	PM_Duck_AMXX(chain, g_pMove->player_index + 1);
}

void PM_UnDuck_AMXX(IReGameHook_PM_UnDuck *chain, int playerIndex)
{
	auto original = [chain](int _playerIndex)
	{
		chain->callNext();
	};

	callVoidForward(RG_PM_UnDuck, original, playerIndex);
}

void PM_UnDuck(IReGameHook_PM_UnDuck *chain)
{
	PM_UnDuck_AMXX(chain, g_pMove->player_index + 1);
}

void PM_PlayStepSound_AMXX(IReGameHook_PM_PlayStepSound *chain, int step, float fvol, int playerIndex)
{
	auto original = [chain](int _step, float _fvol, int _playerIndex)
	{
		chain->callNext(_step, _fvol);
	};

	callVoidForward(RG_PM_PlayStepSound, original, step, fvol, playerIndex);
}

void PM_PlayStepSound(IReGameHook_PM_PlayStepSound *chain, int step, float fvol)
{
	PM_PlayStepSound_AMXX(chain, step, fvol, g_pMove->player_index + 1);
}

void PM_AirAccelerate_AMXX(IReGameHook_PM_AirAccelerate *chain, vec_t *wishdir, float wishspeed, float accel, int playerIndex)
{
	Vector wishdirCopy(wishdir);

	auto original = [chain, &wishdirCopy](cell _wishdir, float _wishspeed, float _accel, int _playerIndex)
	{
		chain->callNext(wishdirCopy, _wishspeed, _accel);
	};

	callVoidForward(RG_PM_AirAccelerate, original, getAmxVector(wishdirCopy), wishspeed, accel, playerIndex);
}

void PM_AirAccelerate(IReGameHook_PM_AirAccelerate *chain, vec_t *wishdir, float wishspeed, float accel)
{
	PM_AirAccelerate_AMXX(chain, wishdir, wishspeed, accel, g_pMove->player_index + 1);
}

void ClearMultiDamage(IReGameHook_ClearMultiDamage *chain)
{
	auto original = [chain]()
	{
		chain->callNext();
	};

	callVoidForward(RG_ClearMultiDamage, original);
}

void AddMultiDamage(IReGameHook_AddMultiDamage *chain, entvars_t *pevInflictor, CBaseEntity *pEntity, float flDamage, int bitsDamageType)
{
	auto original = [chain](int _pevInflictor, int _pEntity, float _flDamage, int _bitsDamageType)
	{
		chain->callNext(PEV(_pevInflictor), getPrivate<CBaseEntity>(_pEntity), _flDamage, _bitsDamageType);
	};

	callVoidForward(RG_AddMultiDamage, original, indexOfEdict(pevInflictor), indexOfEdict(pEntity->pev), flDamage, bitsDamageType);
}

void ApplyMultiDamage(IReGameHook_ApplyMultiDamage *chain, entvars_t *pevInflictor, entvars_t *pevAttacker)
{
	auto original = [chain](int _pevInflictor, int _pevAttacker)
	{
		chain->callNext(PEV(_pevInflictor), PEV(_pevAttacker));
	};

	callVoidForward(RG_ApplyMultiDamage, original, indexOfEdict(pevInflictor), indexOfEdict(pevAttacker));
}

void BuyItem(IReGameHook_BuyItem *chain, CBasePlayer *pPlayer, int iSlot)
{
	auto original = [chain](int _pPlayer, int _iSlot)
	{
		chain->callNext(getPrivate<CBasePlayer>(_pPlayer), _iSlot);
	};

	callVoidForward(RG_BuyItem, original, indexOfEdict(pPlayer->pev), iSlot);
}

void CSGameRules_Think(IReGameHook_CSGameRules_Think *chain)
{
	auto original = [chain]()
	{
		chain->callNext();
	};

	callVoidForward(RG_CSGameRules_Think, original);
}

BOOL CSGameRules_TeamFull(IReGameHook_CSGameRules_TeamFull *chain, int team_id)
{
	auto original = [chain](int _team_id)
	{
		return chain->callNext(_team_id);
	};

	return callForward<BOOL>(RG_CSGameRules_TeamFull, original, team_id);
}

BOOL CSGameRules_TeamStacked(IReGameHook_CSGameRules_TeamStacked *chain, int newTeam_id, int curTeam_id)
{
	auto original = [chain](int _newTeam_id, int _curTeam_id)
	{
		return chain->callNext(_newTeam_id, _curTeam_id);
	};

	return callForward<BOOL>(RG_CSGameRules_TeamStacked, original, newTeam_id, curTeam_id);
}

void CSGameRules_PlayerGotWeapon(IReGameHook_CSGameRules_PlayerGotWeapon *chain, CBasePlayer *pPlayer, CBasePlayerItem *pWeapon)
{
	auto original = [chain](int _pPlayer, int _pWeapon)
	{
		chain->callNext(getPrivate<CBasePlayer>(_pPlayer), getPrivate<CBasePlayerItem>(_pWeapon));
	};

	callVoidForward(RG_CSGameRules_PlayerGotWeapon, original, indexOfEdict(pPlayer->pev), indexOfEdict(pWeapon->pev));
}

void CBotManager_OnEvent(IReGameHook_CBotManager_OnEvent *chain, GameEventType event, CBaseEntity* pEntity, CBaseEntity* pOther)
{
	auto original = [chain](GameEventType _event, int _pEntity, int _pOther)
	{
		chain->callNext(_event, getPrivate<CBaseEntity>(_pEntity), getPrivate<CBaseEntity>(_pOther));
	};

	callVoidForward(RG_CBotManager_OnEvent, original, event, indexOfEdict(pEntity->pev), indexOfEdict(pOther->pev));
}

void CBasePlayer_CheckTimeBasedDamage(IReGameHook_CBasePlayer_CheckTimeBasedDamage *chain, CBasePlayer *pthis)
{
	auto original = [chain](int _pthis)
	{
		chain->callNext(getPrivate<CBasePlayer>(_pthis));
	};

	callVoidForward(RG_CBasePlayer_CheckTimeBasedDamage, original, indexOfEdict(pthis->pev));
}

edict_t *CBasePlayer_EntSelectSpawnPoint(IReGameHook_CBasePlayer_EntSelectSpawnPoint *chain, CBasePlayer *pthis)
{
	auto original = [chain](int _pthis)
	{
		return indexOfEdict(chain->callNext(getPrivate<CBasePlayer>(_pthis)));
	};

	return edictByIndexAmx(callForward<size_t>(RG_CBasePlayer_EntSelectSpawnPoint, original, indexOfEdict(pthis->pev)));
}

void CBasePlayerWeapon_ItemPostFrame(IReGameHook_CBasePlayerWeapon_ItemPostFrame *chain, CBasePlayerWeapon *pthis)
{
	auto original = [chain](int _pthis)
	{
		chain->callNext(getPrivate<CBasePlayerWeapon>(_pthis));
	};

	callVoidForward(RG_CBasePlayerWeapon_ItemPostFrame, original, indexOfEdict(pthis->pev));
}

void CBasePlayerWeapon_KickBack(IReGameHook_CBasePlayerWeapon_KickBack *chain, CBasePlayerWeapon *pthis, float up_base, float lateral_base, float up_modifier, float lateral_modifier, float up_max, float lateral_max, int direction_change)
{
	auto original = [chain](int _pthis, float _up_base, float _lateral_base, float _up_modifier, float _lateral_modifier, float _up_max, float _lateral_max, int _direction_change)
	{
		chain->callNext(getPrivate<CBasePlayerWeapon>(_pthis), _up_base, _lateral_base, _up_modifier, _lateral_modifier, _up_max, _lateral_max, _direction_change);
	};

	callVoidForward(RG_CBasePlayerWeapon_KickBack, original, indexOfEdict(pthis->pev), up_base, lateral_base, up_modifier, lateral_modifier, up_max, lateral_max, direction_change);
}

void CBasePlayerWeapon_SendWeaponAnim(IReGameHook_CBasePlayerWeapon_SendWeaponAnim *chain, CBasePlayerWeapon *pthis, int iAnim, int skiplocal)
{
	auto original = [chain](int _pthis, int _iAnim, int _skiplocal)
	{
		chain->callNext(getPrivate<CBasePlayerWeapon>(_pthis), _iAnim, _skiplocal);
	};

	callVoidForward(RG_CBasePlayerWeapon_SendWeaponAnim, original, indexOfEdict(pthis->pev), iAnim, skiplocal);
}

void CBasePlayer_PlayerDeathThink(IReGameHook_CBasePlayer_PlayerDeathThink *chain, CBasePlayer *pthis)
{
	auto original = [chain](int _pthis)
	{
		chain->callNext(getPrivate<CBasePlayer>(_pthis));
	};

	callVoidForward(RG_CBasePlayer_PlayerDeathThink, original, indexOfEdict(pthis->pev));
}

void CBasePlayer_Observer_Think(IReGameHook_CBasePlayer_Observer_Think *chain, CBasePlayer *pthis)
{
	auto original = [chain](int _pthis)
	{
		chain->callNext(getPrivate<CBasePlayer>(_pthis));
	};

	callVoidForward(RG_CBasePlayer_Observer_Think, original, indexOfEdict(pthis->pev));
}

void CBasePlayer_RemoveAllItems(IReGameHook_CBasePlayer_RemoveAllItems *chain, CBasePlayer *pthis, BOOL removeSuit)
{
	auto original = [chain](int _pthis, BOOL _removeSuit)
	{
		chain->callNext(getPrivate<CBasePlayer>(_pthis), _removeSuit);
	};

	callVoidForward(RG_CBasePlayer_RemoveAllItems, original, indexOfEdict(pthis->pev), removeSuit);
}

void CSGameRules_SendDeathMessage(IReGameHook_CSGameRules_SendDeathMessage *chain, CBaseEntity *pKiller, CBasePlayer *pVictim, CBasePlayer *pAssister, entvars_t *pevInflictor, const char *killerWeaponName, int iDeathMessageFlags, int iRarityOfKill)
{
	auto original = [chain](int _pKiller, int _pVictim, int _pAssister, int _pevInflictor, const char *_killerWeaponName, int _iDeathMessageFlags, int _iRarityOfKill)
	{
		chain->callNext(getPrivate<CBaseEntity>(_pKiller), getPrivate<CBasePlayer>(_pVictim), getPrivate<CBasePlayer>(_pAssister), PEV(_pevInflictor), _killerWeaponName, _iDeathMessageFlags, _iRarityOfKill);
	};

	callVoidForward(RG_CSGameRules_SendDeathMessage, original, indexOfPDataAmx(pKiller), indexOfEdict(pVictim->pev), indexOfPDataAmx(pAssister), indexOfEdictAmx(pevInflictor), killerWeaponName, iDeathMessageFlags, iRarityOfKill);
}

void CBasePlayer_UpdateStatusBar(IReGameHook_CBasePlayer_UpdateStatusBar *chain, CBasePlayer *pthis)
{
	auto original = [chain](int _pthis)
	{
		chain->callNext(getPrivate<CBasePlayer>(_pthis));
	};

	callVoidForward(RG_CBasePlayer_UpdateStatusBar, original, indexOfEdict(pthis->pev));
}

void CBasePlayer_TakeDamageImpulse(IReGameHook_CBasePlayer_TakeDamageImpulse *chain, CBasePlayer *pthis, CBasePlayer *pAttacker, float flKnockbackForce, float flVelModifier)
{
	auto original = [chain](int _pthis, int _pAttacker, float _flKnockbackForce, float _flVelModifier)
	{
		chain->callNext(getPrivate<CBasePlayer>(_pthis), getPrivate<CBasePlayer>(_pAttacker), _flKnockbackForce, _flVelModifier);
	};

	callVoidForward(RG_CBasePlayer_TakeDamageImpulse, original, indexOfEdict(pthis->pev), indexOfEdict(pAttacker->pev), flKnockbackForce, flVelModifier);
}

/*
* VTC functions
*/
int g_iClientStartSpeak, g_iClientStopSpeak;

void OnClientStartSpeak(size_t clientIndex)
{
	g_amxxapi.ExecuteForward(g_iClientStartSpeak, clientIndex);
}

void OnClientStopSpeak(size_t clientIndex)
{
	g_amxxapi.ExecuteForward(g_iClientStopSpeak, clientIndex);
}

/*
* ReChecker functions
*/
void FileConsistencyProcess_AMXX(FileConsistencyProcess_t *data, IGameClient *cl, const char *filename, const char *cmd, ResourceType_e type, uint32 responseHash, bool isBreak)
{
	int hashCopy = responseHash;
	auto original = [data, hashCopy](int _cl, const char *_filename, const char *_cmd, ResourceType_e _type, uint32 _hash, bool _isBreak)
	{
		data->m_chain->callNext(clientByIndex(_cl), data->m_args, _type, hashCopy);
	};

	if (g_RecheckerFuncs->GetResource()->GetPrevHash() == responseHash) {
		responseHash = 0;
	}

	callVoidForward(RC_FileConsistencyProcess, original, cl->GetId() + 1, filename, cmd, type, responseHash, isBreak);
}

void FileConsistencyProcess(IRecheckerHook_FileConsistencyProcess *chain, IGameClient *cl, IResourceBuffer *res, ResourceType_e typeFind, uint32 responseHash)
{
	FileConsistencyProcess_t data(chain, res);
	FileConsistencyProcess_AMXX(&data, cl, res->GetFileName(), res->GetCmdExec(), typeFind, responseHash, res->IsBreak());
}

void FileConsistencyFinal(IRecheckerHook_FileConsistencyFinal *chain, IGameClient *cl)
{
	auto original = [chain](int _cl)
	{
		chain->callNext(clientByIndex(_cl));
	};

	callVoidForward(RC_FileConsistencyFinal, original, cl->GetId() + 1);
}

void CmdExec_AMXX(CmdExec_t *data, IGameClient *cl, const char *filename, char *cmd, uint32 responseHash)
{
	int hashCopy = responseHash;
	auto original = [data, hashCopy](int _cl, const char *_filename, char *_cmd, uint32 _responseHash)
	{
		data->m_chain->callNext(clientByIndex(_cl), data->m_args, _cmd, hashCopy);
	};

	if (g_RecheckerFuncs->GetResource()->GetPrevHash() == responseHash) {
		responseHash = 0;
	}

	callVoidForward(RC_CmdExec, original, cl->GetId() + 1, filename, cmd, responseHash);
}

void CmdExec(IRecheckerHook_CmdExec *chain, IGameClient *cl, IResourceBuffer *res, char *cmdExec, uint32 responseHash)
{
	CmdExec_t data(chain, res);
	CmdExec_AMXX(&data, cl, res->GetFileName(), cmdExec, responseHash);
}

