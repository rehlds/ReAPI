#include "precompiled.h"

void Broadcast(const char *sentence)
{
	char text[128];
	snprintf(text, sizeof text, "%%!MRAD_%s", sentence);

	EMESSAGE_BEGIN(MSG_BROADCAST, gmsgSendAudio);
		EWRITE_BYTE(0);
		EWRITE_STRING(text);
		EWRITE_SHORT(100);
	EMESSAGE_END();
}

struct {
	TeamName team;
	ModelName model;

} modelStruct[] = {
	{ TERRORIST, MODEL_TERROR },
	{ TERRORIST, MODEL_LEET },
	{ TERRORIST, MODEL_ARCTIC },
	{ TERRORIST, MODEL_GUERILLA },
	{ CT, MODEL_URBAN },
	{ CT, MODEL_GSG9 },
	{ CT, MODEL_GIGN },
	{ CT, MODEL_SAS },
};

ModelName GetModelAuto(TeamName team)
{
	int cur = 0;
	const int numSkin = 4;
	for (auto& it : modelStruct) {
		if (it.team != team) {
			++cur;
			continue;
		}

		return modelStruct[RANDOM_LONG(cur, cur + numSkin - 1)].model;
	}

	return MODEL_UNASSIGNED;
}

CTempStrings::CTempStrings()
{
	m_current = 0;
}


char* CTempStrings::push(AMX* amx)
{
	if (m_current == STRINGS_MAX) {
		AMXX_LogError(amx, AMX_ERR_NATIVE, "temp strings limit exceeded, contact reapi authors");
		return nullptr;
	}

	return m_strings[m_current++];
}

void CTempStrings::pop(size_t count)
{
	m_current -= count;
}

CBaseEntity *GiveNamedItemInternal(AMX *amx, CBasePlayer *pPlayer, const char *pszItemName, const size_t uid)
{
	edict_t *pEdict = CREATE_NAMED_ENTITY(ALLOC_STRING(pszItemName));
	if (FNullEnt(pEdict))
	{
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: Item \"%s\" failed to create!\n", __FUNCTION__, pszItemName);
		return nullptr;
	}

	pEdict->v.origin = pPlayer->pev->origin;
	pEdict->v.spawnflags |= SF_NORESPAWN;

	// In some cases, we must to sets unique id
	// for the entity before it will triggered a spawn.
	pEdict->v.impulse = uid;

	MDLL_Spawn(pEdict);
	MDLL_Touch(pEdict, ENT(pPlayer->pev));

	CBaseEntity *pEntity = getPrivate<CBaseEntity>(pEdict);

	// not allow the item to fall to the ground.
	if (FNullEnt(pEntity->pev->owner) || pEntity->pev->owner != pPlayer->edict())
	{
		pEntity->pev->targetname = iStringNull;
		pEntity->pev->flags |= FL_KILLME;

		return nullptr;
	}

	return pEntity;
}

void StudioFrameAdvanceEnt(studiohdr_t *pstudiohdr, edict_t *pEdict)
{
	float flInterval = gpGlobals->time - pEdict->v.animtime;
	if (flInterval <= 0.001f) {
		pEdict->v.animtime = gpGlobals->time;
		return;
	}

	if (pEdict->v.animtime == 0.0f) {
		flInterval = 0.0f;
	}

	if (!pstudiohdr)
		return;

	if (pEdict->v.sequence >= pstudiohdr->numseq || pEdict->v.sequence < 0) {
		return;
	}

	float flFrameRate = 256.0f;
	mstudioseqdesc_t *pseqdesc = (mstudioseqdesc_t *)((byte *)pstudiohdr + pstudiohdr->seqindex) + int(pEdict->v.sequence);
	if (pseqdesc->numframes > 1)
	{
		flFrameRate = pseqdesc->fps * 256.0f / (pseqdesc->numframes - 1);
	}

	pEdict->v.frame += flInterval * flFrameRate * pEdict->v.framerate;
	pEdict->v.animtime = gpGlobals->time;

	if (pEdict->v.frame < 0.0f || pEdict->v.frame >= 256.0f)
	{
		// true if the sequence loops
		if (pseqdesc->flags & STUDIO_LOOPING)
			pEdict->v.frame -= int(pEdict->v.frame / 256.0f) * 256.0f;
		else
			pEdict->v.frame = (pEdict->v.frame < 0.0f) ? 0.0f : 255.0f;
	}
}

void FixupAngles(edict_t *pEdict, Vector &vecSrc)
{
	vecSrc.x -= pEdict->v.origin.x;
	vecSrc.y -= pEdict->v.origin.y;

	float x = vecSrc.x;
	float y = vecSrc.y;
	float c = cos((pEdict->v.angles.y * M_PI / 180.0));
	float s = sin((pEdict->v.angles.y * M_PI / 180.0));

	vecSrc.x = x * c - y * s;
	vecSrc.y = y * c + x * s;

	vecSrc.x += pEdict->v.origin.x;
	vecSrc.y += pEdict->v.origin.y;
}

void GetBonePosition(CBaseEntity *pEntity, int iBone, Vector *pVecOrigin, Vector *pVecAngles)
{
	Vector vecOrigin, vecAngles;
	edict_t *pEdict = pEntity->edict();

	if (pVecOrigin) *pVecOrigin = Vector(0, 0, 0);
	if (pVecAngles) *pVecAngles = Vector(0, 0, 0);

	studiohdr_t *pstudiohdr = static_cast<studiohdr_t *>(GET_MODEL_PTR(pEdict));
	if (!pstudiohdr)
		return;

	if (iBone < 0 || iBone >= pstudiohdr->numbones)
		return; // invalid bone

	float flFrame = pEdict->v.frame;
	float flAnimTime = pEdict->v.animtime;

	// force to update frame
	StudioFrameAdvanceEnt(pstudiohdr, pEdict);

	pEntity->pev->angles.x = -pEntity->pev->angles.x;
	GET_BONE_POSITION(pEdict, iBone, vecOrigin, vecAngles);
	pEntity->pev->angles.x = -pEntity->pev->angles.x;

	// restore the original state for the entity
	pEdict->v.frame = flFrame;
	pEdict->v.animtime = flAnimTime;

	// ReGameDLL already have fixes angles for non-players entities
	if (!g_ReGameApi && !pEntity->IsPlayer()) {
		FixupAngles(pEdict, vecOrigin);
	}

	if (pVecOrigin) {
		*pVecOrigin = vecOrigin;
	}

	if (pVecAngles) {
		*pVecAngles = vecAngles;
	}
}

void GetAttachment(CBaseEntity *pEntity, int iAttachment, Vector *pVecOrigin, Vector *pVecAngles)
{
	Vector vecOrigin, vecAngles;
	edict_t *pEdict = pEntity->edict();

	if (pVecOrigin) *pVecOrigin = Vector(0, 0, 0);
	if (pVecAngles) *pVecAngles = Vector(0, 0, 0);

	studiohdr_t *pstudiohdr = static_cast<studiohdr_t *>(GET_MODEL_PTR(pEdict));
	if (!pstudiohdr)
		return;

	if (iAttachment < 0 || iAttachment >= pstudiohdr->numattachments)
		return; // invalid attachment

	float flFrame = pEdict->v.frame;
	float flAnimTime = pEdict->v.animtime;

	// force to update frame
	StudioFrameAdvanceEnt(pstudiohdr, pEdict);

	GET_ATTACHMENT(pEdict, iAttachment, vecOrigin, vecAngles);

	// restore the original state for the entity
	pEdict->v.frame = flFrame;
	pEdict->v.animtime = flAnimTime;

	// ReGameDLL already have fixes angles for non-players entities
	if (!g_ReGameApi && !pEntity->IsPlayer()) {
		FixupAngles(pEdict, vecOrigin);
	}

	if (pVecOrigin) {
		*pVecOrigin = vecOrigin;
	}

	if (pVecAngles) {
		*pVecAngles = vecAngles;
	}
}

void SetBodygroup(CBaseEntity *pEntity, int iGroup, int iValue)
{
	studiohdr_t *pstudiohdr = static_cast<studiohdr_t *>(GET_MODEL_PTR(pEntity->edict()));
	if (!pstudiohdr || iGroup > pstudiohdr->numbodyparts)
	{
		return;
	}

	mstudiobodyparts_t *pbodypart = (mstudiobodyparts_t *)((byte *)pstudiohdr + pstudiohdr->bodypartindex) + iGroup;

	if (iValue >= pbodypart->nummodels)
	{
		return;
	}

	int iCurrent = (pEntity->pev->body / pbodypart->base) % pbodypart->nummodels;
	pEntity->pev->body += (iValue - iCurrent) * pbodypart->base;
}

int GetBodygroup(CBaseEntity *pEntity, int iGroup)
{
	studiohdr_t *pstudiohdr = static_cast<studiohdr_t *>(GET_MODEL_PTR(pEntity->edict()));

	if (!pstudiohdr || iGroup > pstudiohdr->numbodyparts)
	{
		return 0;
	}

	mstudiobodyparts_t *pbodypart = (mstudiobodyparts_t *)((byte *)pstudiohdr + pstudiohdr->bodypartindex) + iGroup;

	if (pbodypart->nummodels <= 1)
	{
		return 0;
	}

	int iCurrent = (pEntity->pev->body / pbodypart->base) % pbodypart->nummodels;
	return iCurrent;
}

bool GetSequenceInfo2(CBaseEntity *pEntity, int *piFlags, float *pflFrameRate, float *pflGroundSpeed)
{
	studiohdr_t *pstudiohdr = static_cast<studiohdr_t *>(GET_MODEL_PTR(pEntity->edict()));

	if (!pstudiohdr || pEntity->pev->sequence >= pstudiohdr->numseq)
	{
		*piFlags = 0;
		*pflFrameRate = 0;
		*pflGroundSpeed = 0;
		return false;
	}

	mstudioseqdesc_t *pseqdesc = (mstudioseqdesc_t *)((byte *)pstudiohdr + pstudiohdr->seqindex) + int(pEntity->pev->sequence);
	*piFlags = pseqdesc->flags;
	if (pseqdesc->numframes <= 1)
	{
		*pflFrameRate = 256.0f;
		*pflGroundSpeed = 0.0f;
	}
	else
	{
		*pflFrameRate = pseqdesc->fps * 256.0f / (pseqdesc->numframes - 1);
		*pflGroundSpeed = Q_sqrt(pseqdesc->linearmovement[0] * pseqdesc->linearmovement[0] + pseqdesc->linearmovement[1] * pseqdesc->linearmovement[1] + pseqdesc->linearmovement[2] * pseqdesc->linearmovement[2]);
		*pflGroundSpeed = *pflGroundSpeed * pseqdesc->fps / (pseqdesc->numframes - 1);
	}

	return true;
}

void RemoveOrDropItem(CBasePlayer *pPlayer, CBasePlayerItem *pItem, GiveType type)
{
	switch (type)
	{
	case GT_DROP_AND_REPLACE:
		pPlayer->CSPlayer()->DropPlayerItem(STRING(pItem->pev->classname));
		break;
	case GT_REPLACE:
		pPlayer->pev->weapons &= ~(1 << pItem->m_iId);
		pPlayer->RemovePlayerItem(pItem);
		pItem->Kill();
		break;
	case GT_APPEND:
	default:
		break;
	}
}

const char *getATypeStr(AType type)
{
	static constexpr const char *s_ATypes[]
	{
		"ATYPE_INTEGER",
		"ATYPE_FLOAT",
		"ATYPE_STRING",
		"ATYPE_CLASSPTR",
		"ATYPE_EDICT",
		"ATYPE_EVARS",
		"ATYPE_BOOL",
		"ATYPE_VECTOR",
		"ATYPE_TRACE"
	};

	if (type >= arraysize(s_ATypes))
		return "";

	return s_ATypes[type];
}

const char *NET_AdrToString(const netadr_t &a, bool onlyBase)
{
	static char s[64];

	Q_memset(s, 0, sizeof(s));

	if (a.type == NA_LOOPBACK)
		Q_snprintf(s, sizeof(s), "loopback");

	else if (a.type == NA_IP)
	{
		if (onlyBase)
			Q_snprintf(s, sizeof(s), "%i.%i.%i.%i", a.ip[0], a.ip[1], a.ip[2], a.ip[3]);
		else
			Q_snprintf(s, sizeof(s), "%i.%i.%i.%i:%i", a.ip[0], a.ip[1], a.ip[2], a.ip[3], ntohs(a.port));
	}
#ifdef _WIN32
	else // NA_IPX
		Q_snprintf(s, sizeof(s), "%02x%02x%02x%02x:%02x%02x%02x%02x%02x%02x:%i", a.ipx[0], a.ipx[1], a.ipx[2], a.ipx[3], a.ipx[4], a.ipx[5], a.ipx[6], a.ipx[7], a.ipx[8], a.ipx[9], ntohs(a.port));
#endif // _WIN32

	return s;
}

