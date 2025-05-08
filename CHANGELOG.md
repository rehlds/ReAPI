# [ReAPI](https://github.com/rehlds/ReAPI) Changelog

---

## [`5.24.0.300`](https://github.com/rehlds/ReAPI/releases/tag/5.24.0.300) - 2023-12-12

> [!CAUTION]
> This version is compatible with [ReGameDLL](https://github.com/rehlds/ReGameDLL_CS/) version [5.26.0.668](https://github.com/rehlds/ReGameDLL_CS/releases/tag/5.26.0.668) and higher!
>
> `Older versions are not supported.`
>
> Update `only` with [ReGameDLL](https://github.com/rehlds/ReGameDLL_CS/).

### Added
* `API`:
  * Implemented `RG_CreateWeaponBox` hook by @dystopm in https://github.com/rehlds/ReAPI/pull/275
  * Implemented `RG_PM_LadderMove` hook by @ShadowsAdi in https://github.com/rehlds/ReAPI/pull/254
  * CSPlayer and CKnife additions + headers refactory by @dystopm in https://github.com/rehlds/ReAPI/pull/277
  * Added rg_set/get_global_iteminfo natives by @dystopm in https://github.com/rehlds/ReAPI/pull/279
  * Implement `RH_ExecuteServerStringCmd` hook by @ShadowsAdi in https://github.com/rehlds/ReAPI/pull/263
  * New `gamedll` hookchains by @dystopm in https://github.com/rehlds/ReAPI/pull/280
* Added ITEM_FLAG_NOFIREUNDERWATER to cssdk_const.inc by @fl0werD in https://github.com/rehlds/ReAPI/pull/267
* Added a new argument removeAmmo to the rg_remove_items_by_slot native by @Javekson in https://github.com/rehlds/ReAPI/pull/283
* Added  new trace flags to cssdk_const.inc by @justgo97 in https://github.com/rehlds/ReAPI/pull/278
* Added m_flEjectBrass description by @RauliTop in https://github.com/rehlds/ReAPI/pull/274
* `rg_give_defusekit()`: 
  * Added player team check by @Giferns in https://github.com/rehlds/ReAPI/pull/234
* Added GetBodygroup, SetBodygroup, GetSequenceInfo natives by @dystopm in https://github.com/rehlds/ReAPI/pull/294
* Implemented RH_SV_AllowPhysent hook by @justgo97 in https://github.com/rehlds/ReAPI/pull/265
* Implemented `rh_get_client_connect_time()` native by @FEDERICOMB96 in https://github.com/rehlds/ReAPI/pull/259
* Implemented CCSEntity members to export in AMXX headers by @dystopm in https://github.com/rehlds/ReAPI/pull/296

### Changed
* `API upgrade`: 15 new natives by @dystopm in https://github.com/rehlds/ReAPI/pull/297
  * rg_spawn_grenade: Spawns a grenade entity with specified parameters.
  * rg_create_weaponbox: Spawns a weaponbox entity with specified properties.
  * rg_remove_entity: Removes an entity using gamedll's UTIL_Remove function.
  * rg_decal_trace: Creates a decal in the world based on a traceresult.
  * rg_emit_texture_sound: Emits a sound based on a traceresult simulating a bullet hit.
  * rg_add_ammo_registry: Generates an ammo slot in the game's logic.
  * rg_weapon_deploy: Deploys a weapon attached to a player.
  * rg_weapon_reload: Reloads a weapon or a player's active weapon.
  * rg_weapon_shotgun_reload: Forces shotgun reload thinking on a weapon or a player's active weapon.
  * rg_weapon_send_animation: Sends a weapon animation to a player.
  * rg_weapon_kickback: Emits a recoil effect on a weapon's player.
  * rg_switch_best_weapon: Switches a player's current weapon to the best one on their inventory.
  * rg_disappear: Makes a player disappear from the world.
  * rg_set_observer_mode: Sets a player's current Observer mode.
  * rg_death_notice: Emits a death notice.
* Refactored `rg_remove_items_by_slot` and updated the return logic by @Javekson in https://github.com/rehlds/ReAPI/pull/288
* Updated the return logic of `rg_drop_item` and `rg_drop_items_by_slot` by @Javekson in https://github.com/rehlds/ReAPI/pull/289
* Headers update, rename `m_bHasSecondaryAttack`, `CSPlayer` member additions by @dystopm in https://github.com/rehlds/ReAPI/pull/293
* Improved include descriptions `rg_fire_bullets`, `rg_fire_buckshots` and `rg_fire_bullets3` by @RauliTop in https://github.com/rehlds/ReAPI/pull/245
* Improved description for `rg_get_weapon_info` by @Nord1cWarr1or in https://github.com/rehlds/ReAPI/pull/269

### Fixed
* Fixed `TimeBasedDamage enum` typo at cssdk_const.inc by @RauliTop in https://github.com/rehlds/ReAPI/pull/256
* Fixed error 029: invalid expression for IsRoundExpireEvent by @Javekson in https://github.com/rehlds/ReAPI/pull/286
* Fixed `rg_transfer_c4`: prevent C4 destruction on arg receiver = 0 by @Javekson in https://github.com/rehlds/ReAPI/pull/291
* Fixed GiveC4 hook callback return type by @dystopm in https://github.com/rehlds/ReAPI/pull/295

## New Contributors
* @justgo97 made their first contribution in https://github.com/rehlds/ReAPI/pull/265
* @RauliTop made their first contribution in https://github.com/rehlds/ReAPI/pull/245
* @Giferns made their first contribution in https://github.com/rehlds/ReAPI/pull/234
* @dystopm made their first contribution in https://github.com/rehlds/ReAPI/pull/275
* @Javekson made their first contribution in https://github.com/rehlds/ReAPI/pull/283

**Full Changelog**: [5.22.0.253...5.24.0.300](https://github.com/rehlds/ReAPI/compare/5.22.0.253...5.24.0.300)

## [`5.22.0.254`](https://github.com/rehlds/ReAPI/releases/tag/5.22.0.254) - 2022-09-19

### Added
* Implemented `RH_*_Precache_*`, `RH_SV_AddResource`, `RH_SV_ClientPrintf` & `RH_SV_CheckUserInfo` hooks by @ShadowsAdi in https://github.com/rehlds/ReAPI/pull/249

## New Contributors
* @ShadowsAdi made their first contribution in https://github.com/rehlds/ReAPI/pull/249

**Full Changelog**: [5.21.0.252...5.22.0.254](https://github.com/rehlds/ReAPI/compare/5.21.0.252...5.22.0.254)

## [`5.21.0.252`](https://github.com/rehlds/ReAPI/releases/tag/5.21.0.252) - 2022-02-04

### Added
* Added a new native `REU_GetAuthKey`
* Add `GitHub Action` for Remove old artifacts by @wopox1337 in https://github.com/rehlds/ReAPI/pull/230

### Changed
* Reverted "Add `GitHub Action` for Remove old artifacts" by @wopox1337 in https://github.com/rehlds/ReAPI/pull/231

### Fixed
* Fixed wrong native error description by @Nord1cWarr1or in https://github.com/rehlds/ReAPI/pull/236

**Full Changelog**: [5.21.0.248...5.21.0.252](https://github.com/rehlds/ReAPI/compare/5.21.0.248...5.21.0.252)

## [`5.21.0.248`](https://github.com/rehlds/ReAPI/releases/tag/5.21.0.248) - 2021-10-25

### Added
* Added `ClientConnected()` hook by @bionext03 in https://github.com/rehlds/ReAPI/pull/223
* Added `pr_dlls.h` & fix `GetEntityInit()` return type by @wopox1337 in https://github.com/rehlds/ReAPI/pull/228
* Implemented `GetEntityInit()` hook by @francoromaniello in https://github.com/rehlds/ReAPI/pull/215
* Implemented `SV_EmitPings()` hook by @francoromaniello in https://github.com/rehlds/ReAPI/pull/212
* Implemented `rh_get_net_from()` native by @francoromaniello in https://github.com/rehlds/ReAPI/pull/221
* Implemented `SV_ConnectClient()` hook by @francoromaniello in https://github.com/rehlds/ReAPI/pull/220
* Implemented `Con_Printf()` hook by @francoromaniello in https://github.com/rehlds/ReAPI/pull/222
* API members for new CVars `sv_enablebunnyhopping` and `sv_autobunnyhopping` by @aleeperezz16 in https://github.com/rehlds/ReAPI/pull/225
* cssdk_const.inc: Added `ScoreAttrib` constants by @Nord1cWarr1or in https://github.com/rehlds/ReAPI/pull/214
* API: Added hooks `ED_Alloc()` and `ED_Free()` by @DarthMan in https://github.com/rehlds/ReAPI/pull/227

## New Contributors
* @bionext03 made their first contribution in https://github.com/rehlds/ReAPI/pull/223
* @aleeperezz16 made their first contribution in https://github.com/rehlds/ReAPI/pull/225
* @DarthMan made their first contribution in https://github.com/rehlds/ReAPI/pull/227

**Full Changelog**: [5.20.0.236...5.21.0.248](https://github.com/rehlds/ReAPI/compare/5.20.0.236...5.21.0.248)

## [`5.20.0.236`](https://github.com/rehlds/ReAPI/releases/tag/5.20.0.236) - 2021-09-14

### Fixed
* `player.h`: fix `ForEachItem()` items iteration @wopox1337;
* `ReAPI_gamedll_const.inc`: fix: `m_rgbTimeBasedDamage` description @wopox1337;
* Fixed desc typo by @Vaqtincha in https://github.com/rehlds/ReAPI/pull/213
* Fixed desc typo in EntVars by @FEDERICOMB96 in https://github.com/rehlds/ReAPI/pull/216

**Full Changelog**: [5.20.0.231...5.20.0.236](https://github.com/rehlds/ReAPI/compare/5.20.0.231...5.20.0.236)

## [`5.20.0.231`](https://github.com/rehlds/ReAPI/releases/tag/5.20.0.231) - 2021-09-02

### Added
* Added defines for CBasePlayer members by @FEDERICOMB96 in https://github.com/rehlds/ReAPI/pull/195
* Implemented `CBaseEntity::Fire<Bullets[3]|Buckshots>` hooks. by @FEDERICOMB96 in https://github.com/rehlds/ReAPI/pull/202
* Implemented player `Pain`, `DeathSound` and `JoiningThink` hooks by @fl0werD in https://github.com/rehlds/ReAPI/pull/209
* Implemented  `RG_CBasePlayer_Observer_SetMode` hook by @Lopol2010 in https://github.com/rehlds/ReAPI/pull/208
* Implemented `RG_CBasePlayer_Observer_FindNextPlayer` hook  by @francoromaniello in https://github.com/rehlds/ReAPI/pull/210
* Added new natives `rg_spawn_head_gib` and `rg_spawn_random_gibs` by @FEDERICOMB96 in https://github.com/rehlds/ReAPI/pull/200
* Added new native: `rg_fire_buckshots` by @FEDERICOMB96 in https://github.com/rehlds/ReAPI/pull/204
* Added player zoom constants by @Nord1cWarr1or in https://github.com/rehlds/ReAPI/pull/190

### Fixed
* Fixed `rg_plant_bomb` by @francoromaniello in https://github.com/rehlds/ReAPI/pull/196
* Minor fixes by @Vaqtincha in https://github.com/rehlds/ReAPI/pull/193
  * fixed `rg_remove_items_by_slot()` c4 slot;
  * fixed `rg_remove_items_by_slot()` weapon hud;
  * added `RG_SpawnRandomGibs()` victim arg.
* Fixed `rg_round_end` WINSTATUS_NONE code by @francoromaniello in https://github.com/rehlds/ReAPI/pull/201
* Fixed conflict interfaces `vgui.dll` & `swds.dl`l (fix listenserver crash) by @Vaqtincha in https://github.com/rehlds/ReAPI/pull/206

## New Contributors
* @Lopol2010 made their first contribution in https://github.com/rehlds/ReAPI/pull/208

**Full Changelog**: [5.19.0.217...5.20.0.231](https://github.com/rehlds/ReAPI/compare/5.19.0.217...5.20.0.231)

## [`5.19.0.217`](https://github.com/rehlds/ReAPI/releases/tag/5.19.0.217) - 2021-06-14

### Changed
* hook_list.cpp: Reworked argument parser

### Fixed
* ReAPI_gamedll_const.inc: fixed AMXX compiler warning
* Fixed arg_amount not used in rg_get_user_ammo by @francoromaniello in https://github.com/rehlds/ReAPI/pull/192
* Fixed rg_round_end return type by @etojuice in https://github.com/rehlds/ReAPI/pull/194

## New Contributors
* @etojuice made their first contribution in https://github.com/rehlds/ReAPI/pull/194

**Full Changelog**: [5.19.0.211...5.19.0.217](https://github.com/rehlds/ReAPI/compare/5.19.0.211...5.19.0.217)

## [`5.19.0.211`](https://github.com/rehlds/ReAPI/releases/tag/5.19.0.211) - 2021-02-05

### Changed
* Removed unnecessary filters: `set_rebuy` fix tag mismatch

### Fixed
* Fixed `AMXX` include files

**Full Changelog**: [5.19.0.210...5.19.0.211](https://github.com/rehlds/ReAPI/compare/5.19.0.210...5.19.0.211)

## [`5.19.0.210`](https://github.com/rehlds/ReAPI/releases/tag/5.19.0.210) - 2021-01-04

### Added
* Added a new native "rg_plant_bomb" by @francoromaniello in https://github.com/rehlds/ReAPI/pull/178

### Fixed
* Fixed `get_ucmd`, `get_pmtrace` natives. https://github.com/rehlds/ReAPI/pull/182

**Full Changelog**: [5.18.0.205...5.19.0.210](https://github.com/rehlds/ReAPI/compare/5.18.0.205...5.19.0.210)

## [`5.18.0.205`](https://github.com/rehlds/ReAPI/releases/tag/5.18.0.205) - 2020-12-18

### Added
* CSSDK: 
  * Implemented DECLARE_CLASS_TYPES for all game classes

### Fixed
* set/get_member_s: fix missing check for members CCSPlayer/CCSPlayerWeapon

**Full Changelog**: [5.18.0.203...5.18.0.205](https://github.com/rehlds/ReAPI/compare/5.18.0.203...5.18.0.205)

## [`5.18.0.203`](https://github.com/rehlds/ReAPI/releases/tag/5.18.0.203) - 2020-12-17

### Changed
* API Interfaces:
  * make virtual functions pure to explicitly define tables of RTTI. GCC can't resolve them

### Fixed
* Fix load on Linux

**Full Changelog**: [5.18.0.202...5.18.0.203](https://github.com/rehlds/ReAPI/compare/5.18.0.202...5.18.0.203)

## [`5.18.0.202`](https://github.com/rehlds/ReAPI/releases/tag/5.18.0.202) - 2020-12-16

### Added
* Implemented g/set_member safe version

### Changed
* Enabled RTTI
* Adjusted version script

**Full Changelog**: [5.17.0.200...5.18.0.202](https://github.com/rehlds/ReAPI/compare/5.17.0.200...5.18.0.202)

## [`5.17.0.200`](https://github.com/rehlds/ReAPI/releases/tag/5.17.0.200) - 2020-12-01

### Added
* Implemented native to get the current hookchain handle in amxx callback by @afwn90cj93201nixr2e1re in https://github.com/rehlds/ReAPI/pull/173
* Added API check if is player can respawn by @fl0werD in https://github.com/rehlds/ReAPI/pull/174

**Full Changelog**: [5.16.0.198...5.17.0.200](https://github.com/rehlds/ReAPI/compare/5.16.0.198...5.17.0.200)

## [`5.16.0.198`](https://github.com/rehlds/ReAPI/releases/tag/5.16.0.198) - 2020-10-10

### Added
* Implemented IsReAPIHookOriginalWasCalled which determine whether original function was called or not by @afwn90cj93201nixr2e1re in https://github.com/rehlds/ReAPI/pull/172

**Full Changelog**: [5.15.0.197...5.16.0.198](https://github.com/rehlds/ReAPI/compare/5.15.0.197...5.16.0.198)

## [`5.15.0.197`](https://github.com/rehlds/ReAPI/releases/tag/5.15.0.197) - 2020-07-16

### Added
* Implemented members for knife weapon by @afwn90cj93201nixr2e1re in https://github.com/rehlds/ReAPI/pull/168

### Changed
* Some changes by @afwn90cj93201nixr2e1re in https://github.com/rehlds/ReAPI/pull/172

### Fixed
* Compile warn fix by @Vaqtincha in https://github.com/rehlds/ReAPI/pull/167

**Full Changelog**: [5.14.0.195...5.15.0.197](https://github.com/rehlds/ReAPI/compare/5.14.0.195...5.15.0.197)

## [`5.14.0.195`](https://github.com/rehlds/ReAPI/releases/tag/5.14.0.195) - 2020-05-27

### Added
* Implemented CGib by @fl0werD in https://github.com/rehlds/ReAPI/pull/164

### Changed
* backed compatibility

### Fixed
* new line fix

**Full Changelog**: [5.13.0.194...5.14.0.195](https://github.com/rehlds/ReAPI/compare/5.13.0.194...5.14.0.195)

## [`5.13.0.194`](https://github.com/rehlds/ReAPI/releases/tag/5.13.0.194) - 2020-05-14

### Added
* Added m_bCanShootOverride offset By @fl0werD in https://github.com/rehlds/ReAPI/pull/160

### Changed
* Updated SDK

### Fixed
* Fixed typo

**Full Changelog**: [5.12.0.192...5.13.0.194](https://github.com/rehlds/ReAPI/compare/5.12.0.192...5.13.0.194)

## [`5.12.0.192`](https://github.com/rehlds/ReAPI/releases/tag/5.12.0.192) - 2019-12-14

### Added
* Added API to set if player can hear another player by @fant1kua in https://github.com/rehlds/ReAPI/pull/158

**Full Changelog**: [5.11.0.191...5.12.0.192](https://github.com/rehlds/ReAPI/compare/5.11.0.191...5.12.0.192)

## [`5.11.0.191`](https://github.com/rehlds/ReAPI/releases/tag/5.11.0.191) - 2019-12-07

### Added
* added missed offsets by @fl0werD in https://github.com/rehlds/ReAPI/pull/156

**Full Changelog**: [5.11.0.190...5.11.0.191](https://github.com/rehlds/ReAPI/compare/5.11.0.190...5.11.0.191)

## [`5.11.0.190`](https://github.com/rehlds/ReAPI/releases/tag/5.11.0.190) - 2019-11-09

### Added
- Implemented RG_CBasePlayer_DropIdlePlayer hook by @d3m37r4 in [#148](https://github.com/rehlds/ReAPI/pull/148)

**Full Changelog**: [5.11.0.189...5.11.0.190](https://github.com/rehlds/ReAPI/compare/5.11.0.189...5.11.0.190)

## [`5.11.0.189`](https://github.com/rehlds/ReAPI/releases/tag/5.11.0.189) - 2019-10-30

### Added
- Implemented `RG_CBasePlayerWeapon_CanDeploy` & `CBasePlayerWeapon_DefaultDeploy` hooks by @fant1kua in [#150](https://github.com/rehlds/ReAPI/pull/150)
- Implemented `RG_CBasePlayerWeapon_DefaultReload` hook
- Implemented `RG_CBasePlayerWeapon_DefaultShotgunReload` hook

**Full Changelog**: [5.10.0.188...5.11.0.189](https://github.com/rehlds/ReAPI/compare/5.10.0.188...5.11.0.189)

## [`5.10.0.188`](https://github.com/rehlds/ReAPI/releases/tag/5.10.0.188) - 2019-09-06

### Changed
* Re-configured `publish.gradle`

**Full Changelog**: [5.10.0.187...5.10.0.188](https://github.com/rehlds/ReAPI/compare/5.10.0.187...5.10.0.188)

## [`5.10.0.187`](https://github.com/rehlds/ReAPI/releases/tag/5.10.0.187) - 2019-09-04

### Fixed
* Fixed `set_pmove`/`get_pmove` natives by @greymood09 in https://github.com/rehlds/ReAPI/issues/119

**Full Changelog**: [5.10.0.186...5.10.0.187](https://github.com/rehlds/ReAPI/compare/5.10.0.186...5.10.0.187)

## [`5.10.0.186`](https://github.com/rehlds/ReAPI/releases/tag/5.10.0.186) - 2019-08-30

### Added
* Implemented `RG_CBasePlayer_UseEmpty` hook by @fant1kua in https://github.com/rehlds/ReAPI/pull/140
* Added rg_initial`ize_player_counts native by @fant1kua in https://github.com/rehlds/ReAPI/pull/146
* Added native `rg_check_win_conditions` by @fant1kua in https://github.com/rehlds/ReAPI/pull/144
* Added `rg_restart_round` native by @fant1kua in https://github.com/rehlds/ReAPI/pull/145

### Fixed
* Fixed bug with change team if player is die #142 by @fant1kua in https://github.com/rehlds/ReAPI/pull/143

**Full Changelog**: [5.9.0.178...5.10.0.186](https://github.com/rehlds/ReAPI/compare/5.9.0.178...5.10.0.186)

## [`5.9.0.178`](https://github.com/rehlds/ReAPI/releases/tag/5.9.0.178) - 2019-08-02

### Changed
- Removed `debug` print 

**Full Changelog**: [5.9.0.177...5.9.0.178](https://github.com/rehlds/ReAPI/compare/5.9.0.177...5.9.0.178)

## [`5.9.0.177`](https://github.com/rehlds/ReAPI/releases/tag/5.9.0.177) - 2019-07-29

### Added
- Updated `ReAPI.inc` - added `ATYPE_BOOL` and `ATYPE_EVARS` types
- Added option flag `-static-libstdc++`
- Added native rh_client_drop by @d3m37r4 in https://github.com/rehlds/ReAPI/pull/135
- Added more information about error

### Fixed
- Fixed `SetHookChainArg` crash due to incorrect pointer to argument
  - `hookctx_t` gets the original function as the base address of the arguments, some compilers do UB and so for this reason `hookctx_t` has been reworked and now uses tuple.
- Fixed another crash due AMXX_Assert function.
- Fixed duplicate error and some another errors

**Full Changelog**: [5.9.0.171...5.9.0.177](https://github.com/rehlds/ReAPI/compare/5.9.0.171...5.9.0.177)

## [`5.9.0.171`](https://github.com/rehlds/ReAPI/releases/tag/5.9.0.171) - 2019-06-23

### Added
- `get_member_game`: support string

### Fixed
- `GCC` support and fixed warnings
- Fixes [#134](https://github.com/rehlds/ReAPI/issues/134) `rh_get_mapname` fixed incorrectly get length argument

### Changed
- Updated `README.md`

**Full Changelog**: [5.9.0.167...5.9.0.171](https://github.com/rehlds/ReAPI/compare/5.9.0.167...5.9.0.171)

## [`5.9.0.167`](https://github.com/rehlds/ReAPI/releases/tag/5.9.0.167) - 2019-06-06

### Added
- Implemented hook `CBasePlayer::HintMessageEx`.  Closes [#133](https://github.com/rehlds/ReAPI/issues/133)
  - Added native `rg_hint_message​`
  - Added news `CBasePlayer` members​
- Bump minor version `API​`

**Full Changelog**: [5.8.0.166...5.9.0.167](https://github.com/rehlds/ReAPI/compare/5.8.0.166...5.9.0.167)

## [`5.8.0.166`](https://github.com/rehlds/ReAPI/releases/tag/5.8.0.166) - 2019-04-17

### Fixed
- Fixed typo by @SmiteIsTrashBro in https://github.com/rehlds/ReAPI/pull/130
  - Little typo fix for `var_fuser*`, that are float instead of int.

### New Contributors
* @SmiteIsTrashBro made their first contribution in https://github.com/rehlds/ReAPI/pull/130

**Full Changelog**: [5.8.0.165...5.8.0.166](https://github.com/rehlds/ReAPI/compare/5.8.0.165...5.8.0.166)

## [`5.8.0.165`](https://github.com/rehlds/ReAPI/releases/tag/5.8.0.165) - 2019-02-13

### Fixed
- Merged pull request [#129](https://github.com/rehlds/ReAPI/pull/129) from `Vaqtincha/tmppatch-1` 
  - Fixed typo

**Full Changelog**: [5.8.0.163...5.8.0.165](https://github.com/rehlds/ReAPI/compare/5.8.0.163...5.8.0.165)

## [`5.8.0.163`](https://github.com/rehlds/ReAPI/releases/tag/5.8.0.163) - 2019-01-23

### Added
* Added `m_M4A1_flBaseDamageSil`, `m_USP_flBaseDamageSil` and `m_Famas_flBaseDamageBurst` members by @fant1kua in https://github.com/rehlds/ReAPI/pull/126

**Full Changelog**: [5.7.0.162...5.8.0.163](https://github.com/rehlds/ReAPI/compare/5.7.0.162...5.8.0.163)

## [`5.7.0.162`](https://github.com/rehlds/ReAPI/releases/tag/5.7.0.162) - 2019-01-07

### Added
- Add `m_flBaseDamage` member by @fant1kua in [#124](https://github.com/rehlds/ReAPI/pull/124)

**Full Changelog**: [5.6.0.161...5.7.0.162](https://github.com/rehlds/ReAPI/compare/5.6.0.161...5.7.0.162)

## [`5.6.0.161`](https://github.com/rehlds/ReAPI/releases/tag/5.6.0.161) - 2018-12-23

### Changed
- Minor update `cssdk`

**Full Changelog**: [5.6.0.160...5.6.0.161](https://github.com/rehlds/ReAPI/compare/5.6.0.160...5.6.0.161)

## [`5.6.0.160`](https://github.com/rehlds/ReAPI/releases/tag/5.6.0.160) - 2018-10-02

### Fixed
- Resolved [#121](https://github.com/rehlds/ReAPI/issues/121)
- Fixed conflict `cssdk_const.inc` due to `hlsdk_const.inc` update

> [!WARNING]
> Removed file `hlsdk_const.inc`, you need manually to take include from `AmxModX 1.9.0`

**Full Changelog**: [5.6.0.158...5.6.0.160](https://github.com/rehlds/ReAPI/compare/5.6.0.158...5.6.0.160)

## [`5.6.0.158`](https://github.com/rehlds/ReAPI/releases/tag/5.6.0.158) - 2018-07-09

### Fixed
- Fixed `pmove` pointer

**Full Changelog**: [5.6.0.157...5.6.0.158](https://github.com/rehlds/ReAPI/compare/5.6.0.157...5.6.0.158)

## [`5.6.0.157`](https://github.com/rehlds/ReAPI/releases/tag/5.6.0.157) - 2018-05-18

### Added
- `rg_round_end`: Add option trigger to all hooks

**Full Changelog**: [5.6.0.156...5.6.0.157](https://github.com/rehlds/ReAPI/compare/5.6.0.156...5.6.0.157)

## [`5.6.0.156`](https://github.com/rehlds/ReAPI/releases/tag/5.6.0.156) - 2018-05-18

### Fixed
- the bypass amxx warning 200 by @wopox1337 in https://github.com/rehlds/ReAPI/pull/113
  - warning 200: `symbol "RG_CBasePlayer_SetSpawnProtecti" is truncated to 31 characters`

**Full Changelog**: [5.6.0.155...5.6.0.156](https://github.com/rehlds/ReAPI/compare/5.6.0.155...5.6.0.156)

## [`5.6.0.155`](https://github.com/rehlds/ReAPI/releases/tag/5.6.0.155) - 2018-02-25

### Fixed
* Fixed copypaste errors by @In-line in https://github.com/rehlds/ReAPI/pull/103

### Changed
- Replaced `SetTouch` to `SetBlocked` in `CEntityCallback::SetBlocked` by @In-line in https://github.com/rehlds/ReAPI/pull/109
- Updated `regamedll api` version
- Added hookchain and getter/setter for spawn protection
  - Added hookchain for `IsPenetrableEntity`

**Full Changelog**: [5.5.0.150...5.6.0.155](https://github.com/rehlds/ReAPI/compare/5.5.0.150...5.6.0.155)

## [`5.5.0.150`](https://github.com/rehlds/ReAPI/releases/tag/5.5.0.150) - 2018-02-25

### Fixed
- Fixed `rg_set_iteminfo` allocate string via engine

### Changed
- Enhanced `rg_remove_item`: add new param remove ammunition

**Full Changelog**: [5.5.0.148...5.5.0.150](https://github.com/rehlds/ReAPI/compare/5.5.0.148...5.5.0.150)

## [`5.5.0.148`](https://github.com/rehlds/ReAPI/releases/tag/5.5.0.148) - 2018-02-09
 
### Added  
- Added `getAmxVector` to simplify vector converting code

### Fixed
- Fixed `RG_ThrowFlashbang` never be called

### Changed
- bump `minor` version

**Full Changelog**: [5.2.0.145...5.5.0.148](https://github.com/rehlds/ReAPI/compare/5.2.0.145...5.5.0.148)

## [`5.2.0.145`](https://github.com/rehlds/ReAPI/releases/tag/5.2.0.145) - 2018-01-28
 
### Added  
- `rg_round_end`: add safe checks to index of bounds

### Changed
- Updated `regamedll API` and implemented hookschain's:
  - `CSGameRules::CanPlayerHearPlayer`, 
  - `CBasePlayer::SwitchTeam`, 
  - `CBasePlayer::CanSwitchTeam`, 
  - `CBasePlayer::ThrowGrenade`, 
  - `CWeaponBox::SetModel`, 
  - `CGrenade::DefuseBombStart`, 
  - `CGrenade::DefuseBombEnd`, 
  - `CGrenade::ExplodeHeGrenade`, 
  - `CGrenade::ExplodeFlashbang`, 
  - `CGrenade::ExplodeSmokeGrenade`, 
  - `CGrenade::ExplodeBomb`, 
  - `ThrowHeGrenade`, 
  - `ThrowFlashbang`, 
  - `ThrowSmokeGrenade`

**Full Changelog**: [5.2.0.143...5.2.0.145](https://github.com/rehlds/ReAPI/compare/5.2.0.143...5.2.0.145)

## [`5.2.0.143`](https://github.com/rehlds/ReAPI/releases/tag/5.2.0.143) - 2017-12-11
 
### Fixed  
- Fixed crash `rg_get_iteminfo`

**Full Changelog**: [5.2.0.142...5.2.0.143](https://github.com/rehlds/ReAPI/compare/5.2.0.142...5.2.0.143)

## [`5.2.0.142`](https://github.com/rehlds/ReAPI/releases/tag/5.2.0.142) - 2017-11-27

### Added
* Natives misc by @s1lentq in https://github.com/rehlds/ReAPI/pull/6
* Added `rh_update_user_info` by @WPMGPRoSToTeMa in https://github.com/rehlds/ReAPI/pull/11
* Added `rg_reset_maxspeed` native by @ivanmaxlogiudice in https://github.com/rehlds/ReAPI/pull/28
* Added tag safety for `get`/`set_member` by @WPMGPRoSToTeMa in https://github.com/rehlds/ReAPI/pull/62

### Fixed
* Fixed return value to match api changes by @voed in https://github.com/rehlds/ReAPI/pull/5
* Fixed context data for nested hookchain forwards, added member natives by @theAsmodai in https://github.com/rehlds/ReAPI/pull/10
* Fixed `DeathNotice` hook crash (#12) by @WPMGPRoSToTeMa in https://github.com/rehlds/ReAPI/pull/13
* Fixed natives `rg_set_iteminfo`/`rg_get_iteminfo` in https://github.com/rehlds/ReAPI/pull/92

### Changed
* Refactoring by @theAsmodai in https://github.com/rehlds/ReAPI/pull/4
* Allow get knife info with `rg_get_weapon_info` by @ivanmaxlogiudice in https://github.com/rehlds/ReAPI/pull/38
* Allow get `WEAPON_C4` & `WEAPON_KNIFE` info by @ivanmaxlogiudice in https://github.com/rehlds/ReAPI/pull/52
* Grammar and spelling corrections in comments by @OciXCrom in https://github.com/rehlds/ReAPI/pull/74
* `StartDeathCam()` immediately when changing team to spectator via rg_set_user_team by @In-line in https://github.com/rehlds/ReAPI/pull/81
* Comments for parsing docgen by @s008nyx in https://github.com/rehlds/ReAPI/pull/90
* Comments & descriptions improvements by @voed in https://github.com/rehlds/ReAPI/pull/91

### New Contributors
* @theAsmodai made their first contribution in https://github.com/rehlds/ReAPI/pull/4
* @voed made their first contribution in https://github.com/rehlds/ReAPI/pull/5
* @s1lentq made their first contribution in https://github.com/rehlds/ReAPI/pull/6
* @WPMGPRoSToTeMa made their first contribution in https://github.com/rehlds/ReAPI/pull/11
* @ivanmaxlogiudice made their first contribution in https://github.com/rehlds/ReAPI/pull/28
* @OciXCrom made their first contribution in https://github.com/rehlds/ReAPI/pull/74
* @s008nyx made their first contribution in https://github.com/rehlds/ReAPI/pull/90

**Full Changelog**: [5.2.0.142](https://github.com/rehlds/ReAPI/commits/5.2.0.142)
