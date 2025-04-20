/*
*
*    This program is free software; you can redistribute it and/or modify it
*    under the terms of the GNU General Public License as published by the
*    Free Software Foundation; either version 2 of the License, or (at
*    your option) any later version.
*
*    This program is distributed in the hope that it will be useful, but
*    WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*    General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program; if not, write to the Free Software Foundation,
*    Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*
*    In addition, as a special exception, the author gives permission to
*    link the code of this program with the Half-Life Game Engine ("HL
*    Engine") and Modified Game Libraries ("MODs") developed by Valve,
*    L.L.C ("Valve").  You must obey the GNU General Public License in all
*    respects for all of the code used other than the HL Engine and MODs
*    from Valve.  If you modify this file, you may extend this exception
*    to your version of the file, but you are not obligated to do so.  If
*    you do not wish to do so, delete this exception statement from your
*    version.
*
*/

#pragma once

#include <API/CSPlayerItem.h>
#include <API/CSPlayerWeapon.h>
#include <utlarray.h>

enum WeaponInfiniteAmmoMode
{
	WPNMODE_INFINITE_CLIP = 1,
	WPNMODE_INFINITE_BPAMMO
};

class CCSPlayer: public CCSMonster
{
	DECLARE_CLASS_TYPES(CCSPlayer, CCSMonster);
public:
	CCSPlayer() :
		m_bForceShowMenu(false),
		m_flRespawnPending(0),
		m_flSpawnProtectionEndTime(0),
		m_iWeaponInfiniteAmmo(0),
		m_iWeaponInfiniteIds(0),
		m_bCanShootOverride(false),
		m_bGameForcingRespawn(false),
		m_bAutoBunnyHopping(false),
		m_bMegaBunnyJumping(false),
		m_bPlantC4Anywhere(false),
		m_bSpawnProtectionEffects(false),
		m_flJumpHeight(0),
		m_flLongJumpHeight(0),
		m_flLongJumpForce(0),
		m_flDuckSpeedMultiplier(0),
		m_iUserID(-1)
	{
		m_szModel[0] = '\0';

		// Resets the kill history for this player
		for (int i = 0; i < MAX_CLIENTS; i++)
		{
			m_iNumKilledByUnanswered[i] = 0;
			m_bPlayerDominated[i]       = false;
		}
	}

	virtual bool IsConnected() const = 0;
	virtual void SetAnimation(PLAYER_ANIM playerAnim) = 0;
	virtual void AddAccount(int amount, RewardType type = RT_NONE, bool bTrackChange = true) = 0;
	virtual CBaseEntity *GiveNamedItem(const char *pszName) = 0;
	virtual CBaseEntity *GiveNamedItemEx(const char *pszName) = 0;
	virtual void GiveDefaultItems() = 0;
	virtual void GiveShield(bool bDeploy = true) = 0;
	virtual CBaseEntity *DropShield(bool bDeploy = true) = 0;
	virtual CBaseEntity *DropPlayerItem(const char *pszItemName) = 0;
	virtual bool RemoveShield() = 0;
	virtual void RemoveAllItems(bool bRemoveSuit) = 0;
	virtual bool RemovePlayerItem(const char* pszItemName) = 0;
	virtual void SetPlayerModel(bool bHasC4) = 0;
	virtual void SetPlayerModelEx(const char *modelName) = 0;
	virtual void SetNewPlayerModel(const char *modelName) = 0;
	virtual void ClientCommand(const char *cmd, const char *arg1 = nullptr, const char *arg2 = nullptr, const char *arg3 = nullptr) = 0;
	virtual void SetProgressBarTime(int time) = 0;
	virtual void SetProgressBarTime2(int time, float timeElapsed) = 0;
	virtual struct edict_s *EntSelectSpawnPoint() = 0;
	virtual void SetBombIcon(bool bFlash = false) = 0;
	virtual void SetScoreAttrib(CBasePlayer *dest) = 0;
	virtual void SendItemStatus() = 0;
	virtual void ReloadWeapons(CBasePlayerItem *pWeapon = nullptr, bool bForceReload = false, bool bForceRefill = false) = 0;
	virtual void Observer_SetMode(int iMode) = 0;
	virtual bool SelectSpawnSpot(const char *pEntClassName, CBaseEntity* &pSpot) = 0;
	virtual bool SwitchWeapon(CBasePlayerItem *pWeapon) = 0;
	virtual void SwitchTeam() = 0;
	virtual bool JoinTeam(TeamName team) = 0;
	virtual void StartObserver(Vector& vecPosition, Vector& vecViewAngle) = 0;
	virtual void TeamChangeUpdate() = 0;
	virtual void DropSecondary() = 0;
	virtual void DropPrimary() = 0;
	virtual bool HasPlayerItem(CBasePlayerItem *pCheckItem) = 0;
	virtual bool HasNamedPlayerItem(const char *pszItemName) = 0;
	virtual CBasePlayerItem *GetItemById(WeaponIdType weaponID) = 0;
	virtual CBasePlayerItem *GetItemByName(const char *itemName) = 0;
	virtual void Disappear() = 0;
	virtual void MakeVIP() = 0;
	virtual bool MakeBomber() = 0;
	virtual void ResetSequenceInfo() = 0;
	virtual void StartDeathCam() = 0;
	virtual bool RemovePlayerItemEx(const char* pszItemName, bool bRemoveAmmo) = 0;
	virtual void SetSpawnProtection(float flProtectionTime) = 0;
	virtual void RemoveSpawnProtection() = 0;
	virtual bool HintMessageEx(const char *pMessage, float duration = 6.0f, bool bDisplayIfPlayerDead = false, bool bOverride = false) = 0;
	virtual void Reset() = 0;
	virtual void OnSpawnEquip(bool addDefault = true, bool equipGame = true) = 0;
	virtual void SetScoreboardAttributes(CBasePlayer *destination = nullptr) = 0;
	virtual void Observer_FindNextPlayer(bool bReverse, const char *name = nullptr) = 0;
	virtual void TakeDamageImpulse(CBasePlayer *pAttacker, float flKnockbackForce, float flVelModifier) = 0;

	CBasePlayer *BasePlayer() const;

public:
	enum EProtectionState
	{
		ProtectionSt_NoSet,
		ProtectionSt_Active,
		ProtectionSt_Expired,
	};

	EProtectionState GetProtectionState() const;
	bool CheckActivityInGame();

public:
	char m_szModel[32];
	bool m_bForceShowMenu;
	float m_flRespawnPending;
	float m_flSpawnProtectionEndTime;
	Vector m_vecOldvAngle;
	int m_iWeaponInfiniteAmmo;
	int m_iWeaponInfiniteIds;
	bool m_bCanShootOverride;
	bool m_bGameForcingRespawn;
	bool m_bAutoBunnyHopping;
	bool m_bMegaBunnyJumping;
	bool m_bPlantC4Anywhere;
	bool m_bSpawnProtectionEffects;
	double m_flJumpHeight;
	double m_flLongJumpHeight;
	double m_flLongJumpForce;
	double m_flDuckSpeedMultiplier;

	int m_iUserID;
	struct CDamageRecord_t
	{
		float flDamage            = 0.0f;
		float flFlashDurationTime = 0.0f;
		int userId                = -1;
	};
	using DamageList_t = CUtlArray<CDamageRecord_t, MAX_CLIENTS>;
	DamageList_t m_DamageList; // A unified array of recorded damage that includes giver and taker in each entry
	DamageList_t &GetDamageList() { return m_DamageList; }
	int m_iNumKilledByUnanswered[MAX_CLIENTS]; // [0-31] how many unanswered kills this player has been dealt by each other player
	bool m_bPlayerDominated[MAX_CLIENTS]; // [0-31] array of state per other player whether player is dominating other players
};

// Inlines
inline CBasePlayer *CCSPlayer::BasePlayer() const
{
	return reinterpret_cast<CBasePlayer *>(this->m_pContainingEntity);
}

inline CCSPlayer::EProtectionState CCSPlayer::GetProtectionState() const
{
	// no protection set
	if (m_flSpawnProtectionEndTime <= 0.0f)
		return ProtectionSt_NoSet;

	// check if end time of protection isn't expired yet
	if (m_flSpawnProtectionEndTime >= gpGlobals->time)
		return ProtectionSt_Active;

	// has expired
	return ProtectionSt_Expired;
}
