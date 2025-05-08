#if defined _cssdk_const_included
	#endinput
#endif
#define _cssdk_const_included

/**
* Basic constants
*/
#define BIT(%0)                         (1<<(%0))

#define NULLENT                         -1

#define MAX_WEAPONS                     32
#define MAX_CLIENTS                     32                  // Max # of clients allowed in a server.
#define MAX_ITEM_TYPES                  6                   // hud item selection slots

#define MAX_EDICT_BITS                  11                  // How many bits to use to encode an edict. # of bits needed to represent max edicts
#define MAX_EDICTS                      (1<<MAX_EDICT_BITS) // Max # of edicts in a level (2048)

/**
* get_entvar(entity, var_button) or get_entvar(entity, var_oldbuttons) values
*/
#define IN_ATTACK                       (1<<0)
#define IN_JUMP                         (1<<1)
#define IN_DUCK                         (1<<2)
#define IN_FORWARD                      (1<<3)
#define IN_BACK                         (1<<4)
#define IN_USE                          (1<<5)
#define IN_CANCEL                       (1<<6)
#define IN_LEFT                         (1<<7)
#define IN_RIGHT                        (1<<8)
#define IN_MOVELEFT                     (1<<9)
#define IN_MOVERIGHT                    (1<<10)
#define IN_ATTACK2                      (1<<11)
#define IN_RUN                          (1<<12)
#define IN_RELOAD                       (1<<13)
#define IN_ALT1                         (1<<14)
#define IN_SCORE                        (1<<15) // Used by client.dll for when scoreboard is held down

/**
* get_entvar(entity, var_flags) values
*/
#define FL_FLY                          (1<<0)  // Changes the SV_Movestep() behavior to not need to be on ground
#define FL_SWIM                         (1<<1)  // Changes the SV_Movestep() behavior to not need to be on ground (but stay in water)
#define FL_CONVEYOR                     (1<<2)
#define FL_CLIENT                       (1<<3)
#define FL_INWATER                      (1<<4)
#define FL_MONSTER                      (1<<5)
#define FL_GODMODE                      (1<<6)
#define FL_NOTARGET                     (1<<7)
#define FL_SKIPLOCALHOST                (1<<8)  // Don't send entity to local host, it's predicting this entity itself
#define FL_ONGROUND                     (1<<9)  // At rest / on the ground
#define FL_PARTIALGROUND                (1<<10) // Not all corners are valid
#define FL_WATERJUMP                    (1<<11) // Player jumping out of water
#define FL_FROZEN                       (1<<12) // Player is frozen for 3rd person camera
#define FL_FAKECLIENT                   (1<<13) // JAC: fake client, simulated server side; don't send network messages to them
#define FL_DUCKING                      (1<<14) // Player flag -- Player is fully crouched
#define FL_FLOAT                        (1<<15) // Apply floating force to this entity when in water
#define FL_GRAPHED                      (1<<16) // Worldgraph has this ent listed as something that blocks a connection
#define FL_IMMUNE_WATER                 (1<<17)
#define FL_IMMUNE_SLIME                 (1<<18)
#define FL_IMMUNE_LAVA                  (1<<19)
#define FL_PROXY                        (1<<20) // This is a spectator proxy
#define FL_ALWAYSTHINK                  (1<<21) // Brush model flag -- call think every frame regardless of nextthink - ltime (for constantly changing velocity/path)
#define FL_BASEVELOCITY                 (1<<22) // Base velocity has been applied this frame (used to convert base velocity into momentum)
#define FL_MONSTERCLIP                  (1<<23) // Only collide in with monsters who have FL_MONSTERCLIP set
#define FL_ONTRAIN                      (1<<24) // Player is _controlling_ a train, so movement commands should be ignored on client during prediction.
#define FL_WORLDBRUSH                   (1<<25) // Not moveable/removeable brush entity (really part of the world, but represented as an entity for transparency or something)
#define FL_SPECTATOR                    (1<<26) // This client is a spectator, don't run touch functions, etc.
#define FL_CUSTOMENTITY                 (1<<29) // This is a custom entity
#define FL_KILLME                       (1<<30) // This entity is marked for death -- This allows the engine to kill ents at the appropriate time
#define FL_DORMANT                      (1<<31) // Entity is dormant, no updates to client

/**
* global_get(glb_trace_flags) values
* @note Custom flags that we can retrieve in pfnShouldCollide/TraceLine/TraceHull
*/
#define FTRACE_BULLET                   (1<<16)
#define FTRACE_FLASH                    (1<<17)
#define FTRACE_KNIFE                    (1<<18)

/**
* get_entvar(entity, var_movetype) values
*/
#define MOVETYPE_NONE                   0  // Never moves
#define MOVETYPE_WALK                   3  // Player only - moving on the ground
#define MOVETYPE_STEP                   4  // Gravity, special edge handling -- monsters use this
#define MOVETYPE_FLY                    5  // No gravity, but still collides with stuff
#define MOVETYPE_TOSS                   6  // Gravity/Collisions
#define MOVETYPE_PUSH                   7  // No clip to world, push and crush
#define MOVETYPE_NOCLIP                 8  // No gravity, no collisions, still do velocity/avelocity
#define MOVETYPE_FLYMISSILE             9  // Extra size to monsters
#define MOVETYPE_BOUNCE                 10 // Just like Toss, but reflect velocity when contacting surfaces
#define MOVETYPE_BOUNCEMISSILE          11 // Bounce w/o gravity
#define MOVETYPE_FOLLOW                 12 // Track movement of aiment
#define MOVETYPE_PUSHSTEP               13 // BSP model that needs physics/world collisions (uses nearest hull for world collision)

/**
* get_entvar(entity, var_solid) values
*
* @note Some movetypes will cause collisions independent of SOLID_NOT and
* SOLID_TRIGGER when the entity moves. SOLID only effects OTHER entities
* colliding with this one when they move - UGH!
*/
#define SOLID_NOT                       0 // No interaction with other objects
#define SOLID_TRIGGER                   1 // Touch on edge, but not blocking
#define SOLID_BBOX                      2 // Touch on edge, block
#define SOLID_SLIDEBOX                  3 // Touch on edge, but not an onground
#define SOLID_BSP                       4 // BSP clip, touch on edge, block

/**
* get_entvar(entity, var_deadflag) values
*/
#define DEAD_NO                         0 // Alive
#define DEAD_DYING                      1 // Playing death animation or still falling off of a ledge waiting to hit ground
#define DEAD_DEAD                       2 // Dead, lying still
#define DEAD_RESPAWNABLE                3
#define DEAD_DISCARDBODY                4

/**
* new Float:takedamage;
* get_entvar(entity, var_takedamage, takedamage) values
* or
* new Float:takedamage = Float:get_entvar(entity, var_takedamage) values
*/
#define DAMAGE_NO                       0.0
#define DAMAGE_YES                      1.0
#define DAMAGE_AIM                      2.0

/**
* get_entvar(entity, var_effects) values
*/
#define EF_BRIGHTFIELD                  1    // Swirling cloud of particles
#define EF_MUZZLEFLASH                  2    // Single frame ELIGHT on entity attachment 0
#define EF_BRIGHTLIGHT                  4    // DLIGHT centered at entity origin
#define EF_DIMLIGHT                     8    // Player flashlight
#define EF_INVLIGHT                     16   // Get lighting from ceiling
#define EF_NOINTERP                     32   // Don't interpolate the next frame
#define EF_LIGHT                        64   // Rocket flare glow sprite
#define EF_NODRAW                       128  // Don't draw entity
#define EF_FORCEVISIBILITY              2048 // force visibility
#define EF_OWNER_VISIBILITY             4096 // visibility for owner
#define EF_OWNER_NO_VISIBILITY          8192 // no visibility for owner
#define EF_NOSLERP                      16384 // no slerp flag for this entity (addtofullpack)
#define EF_FOLLOWKEEPRENDER             32768 // the entity following will not copy the render (like it follows nothing)

/**
* Break Model Defines
*/
#define BREAK_TYPEMASK                  0x4F
#define BREAK_GLASS                     0x01
#define BREAK_METAL                     0x02
#define BREAK_FLESH                     0x04
#define BREAK_WOOD                      0x08

#define BREAK_SMOKE                     0x10
#define BREAK_TRANS                     0x20
#define BREAK_CONCRETE                  0x40
#define BREAK_2                         0x80

/**
* Colliding temp entity sounds
*/
#define BOUNCE_GLASS                    BREAK_GLASS
#define BOUNCE_METAL                    BREAK_METAL
#define BOUNCE_FLESH                    BREAK_FLESH
#define BOUNCE_WOOD                     BREAK_WOOD
#define BOUNCE_SHRAP                    0x10
#define BOUNCE_SHELL                    0x20
#define BOUNCE_CONCRETE                 BREAK_CONCRETE
#define BOUNCE_SHOTSHELL                0x80

/**
* Temp entity bounce sound types
*/
#define TE_BOUNCE_NULL                  0
#define TE_BOUNCE_SHELL                 1
#define TE_BOUNCE_SHOTSHELL             2

/**
* Spectating camera mode constants
*
* @note These constants are linked to different camera modes available when you
*   are spectating (either dead or when in spectator team). Usually this is
*   stored in the var_iuser1 field in Counter-Strike game.
*/
#define OBS_NONE                        0
#define OBS_CHASE_LOCKED                1 // Locked Chase Cam
#define OBS_CHASE_FREE                  2 // Free Chase Cam
#define OBS_ROAMING                     3 // Free Look
#define OBS_IN_EYE                      4 // First Person
#define OBS_MAP_FREE                    5 // Free Overview
#define OBS_MAP_CHASE                   6 // Chase Overview

/**
* Custom constants iuser3
*
*/
#define PLAYER_CAN_SHOOT                (1<<0)
#define PLAYER_FREEZE_TIME_OVER         (1<<1)
#define PLAYER_IN_BOMB_ZONE             (1<<2)
#define PLAYER_HOLDING_SHIELD           (1<<3)
#define PLAYER_PREVENT_DUCK             (1<<4)
#define PLAYER_PREVENT_CLIMB            (1<<5) // The player can't climb ladder
#define PLAYER_PREVENT_JUMP             (1<<6)

/**
* Instant damage values for use with the 3rd parameter of the "Damage" client
* message.
*/
#define DMG_GENERIC                     0           // Generic damage was done
#define DMG_CRUSH                       (1<<0)      // Crushed by falling or moving object
#define DMG_BULLET                      (1<<1)      // Shot
#define DMG_SLASH                       (1<<2)      // Cut, clawed, stabbed
#define DMG_BURN                        (1<<3)      // Heat burned
#define DMG_FREEZE                      (1<<4)      // Frozen
#define DMG_FALL                        (1<<5)      // Fell too far
#define DMG_BLAST                       (1<<6)      // Explosive blast damage
#define DMG_CLUB                        (1<<7)      // Crowbar, punch, headbutt
#define DMG_SHOCK                       (1<<8)      // Electric shock
#define DMG_SONIC                       (1<<9)      // Sound pulse shockwave
#define DMG_ENERGYBEAM                  (1<<10)     // Laser or other high energy beam
#define DMG_NEVERGIB                    (1<<12)     // With this bit OR'd in, no damage type will be able to gib victims upon death
#define DMG_ALWAYSGIB                   (1<<13)     // With this bit OR'd in, any damage type can be made to gib victims upon death.
#define DMG_DROWN                       (1<<14)     // Drowning
#define DMG_PARALYZE                    (1<<15)     // Slows affected creature down
#define DMG_NERVEGAS                    (1<<16)     // Nerve toxins, very bad
#define DMG_POISON                      (1<<17)     // Blood poisioning
#define DMG_RADIATION                   (1<<18)     // Radiation exposure
#define DMG_DROWNRECOVER                (1<<19)     // Drowning recovery
#define DMG_ACID                        (1<<20)     // Toxic chemicals or acid burns
#define DMG_SLOWBURN                    (1<<21)     // In an oven
#define DMG_SLOWFREEZE                  (1<<22)     // In a subzero freezer
#define DMG_MORTAR                      (1<<23)     // Hit by air raid (done to distinguish grenade from mortar)
#define DMG_GRENADE                     (1<<24)     // Counter-Strike only - Hit by HE grenade
#define DMG_TIMEBASED                   (~(0x3fff)) // Mask for time-based damage

// These are the damage types that are allowed to gib corpses
#define DMG_GIB_CORPSE                  (DMG_CRUSH | DMG_FALL | DMG_BLAST | DMG_SONIC | DMG_CLUB)

// These are the damage types that have client hud art
#define DMG_SHOWNHUD                    (DMG_POISON | DMG_ACID | DMG_FREEZE | DMG_SLOWFREEZE | DMG_DROWN | DMG_BURN | DMG_SLOWBURN | DMG_NERVEGAS | DMG_RADIATION | DMG_SHOCK)

/**
* When calling KILLED(), a value that governs gib behavior is expected to be
* one of these three values
*/
#define GIB_NORMAL                      0 // Gib if entity was overkilled
#define GIB_NEVER                       1 // Never gib, no matter how much death damage is done ( freezing, etc )
#define GIB_ALWAYS                      2 // Always gib ( Houndeye Shock, Barnacle Bite )

/**
* Player physics flags bits
* CBasePlayer::m_afPhysicsFlags
*/
#define PFLAG_ONLADDER                  (1<<0)
#define PFLAG_ONSWING                   (1<<0)
#define PFLAG_ONTRAIN                   (1<<1)
#define PFLAG_ONBARNACLE                (1<<2)
#define PFLAG_DUCKING                   (1<<3) // In the process of ducking, but totally squatted yet
#define PFLAG_USING                     (1<<4) // Using a continuous entity
#define PFLAG_OBSERVER                  (1<<5) // Player is locked in stationary cam mode. Spectators can move, observers can't.

/**
* Player HUD
* CBasePlayer::m_iHideHUD
*/
#define HIDEHUD_WEAPONS                 (1<<0)
#define HIDEHUD_FLASHLIGHT              (1<<1)
#define HIDEHUD_ALL                     (1<<2)
#define HIDEHUD_HEALTH                  (1<<3)
#define HIDEHUD_TIMER                   (1<<4)
#define HIDEHUD_MONEY                   (1<<5)
#define HIDEHUD_CROSSHAIR               (1<<6)
#define HIDEHUD_OBSERVER_CROSSHAIR      (1<<7)

/**
* Item's flags
*/
#define ITEM_FLAG_SELECTONEMPTY         (1<<0)
#define ITEM_FLAG_NOAUTORELOAD          (1<<1)
#define ITEM_FLAG_NOAUTOSWITCHEMPTY     (1<<2)
#define ITEM_FLAG_LIMITINWORLD          (1<<3)
#define ITEM_FLAG_EXHAUSTIBLE           (1<<4)
#define ITEM_FLAG_NOFIREUNDERWATER      (1<<5)

/**
* @section get_entvar(entity, var_spawnflags) values
*/

/**
* func_train
*/
#define SF_TRAIN_WAIT_RETRIGGER         1
#define SF_TRAIN_START_ON               4 // Train is initially moving
#define SF_TRAIN_PASSABLE               8 // Train is not solid -- used to make water trains

/**
* func_wall_toggle
*/
#define SF_WALL_TOOGLE_START_OFF        0x0001
#define SF_WALL_TOOGLE_NOTSOLID         0x0008

/**
* func_conveyor
*/
#define SF_CONVEYOR_VISUAL              0x0001
#define SF_CONVEYOR_NOTSOLID            0x0002

/**
* func_button
*/
#define SF_BUTTON_DONTMOVE              1
#define SF_BUTTON_TOGGLE                32  // Button stays pushed until reactivated
#define SF_BUTTON_SPARK_IF_OFF          64  // Button sparks in OFF state
#define SF_BUTTON_TOUCH_ONLY            256 // Button only fires as a result of USE key.

/**
* func_rot_button
*/
#define SF_ROTBUTTON_NOTSOLID           1
#define SF_ROTBUTTON_BACKWARDS          2

/**
* env_global
*/
#define SF_GLOBAL_SET                   1  // Set global state to initial state on spawn

/**
* multisource
*/
#define MAX_MS_TARGETS                  32 // maximum number of targets a single multisource entity may be assigned.
#define SF_MULTI_INIT                   1

/**
* momentary_rot_button
*/
#define SF_MOMENTARY_DOOR               0x0001 // Make this button behave like a door (HACKHACK)
                                               // This will disable use and make the button solid
                                               // rotating buttons were made SOLID_NOT by default since their were some
                                               // collision problems with them...

/**
* button_target
*/
#define SF_BTARGET_USE                  0x0001
#define SF_BTARGET_ON                   0x0002

/**
* func_door, func_water, func_door_rotating, momementary_door
*/
#define SF_DOOR_START_OPEN              1
#define SF_DOOR_PASSABLE                8
#define SF_DOOR_NO_AUTO_RETURN          32
#define SF_DOOR_USE_ONLY                256        // door must be opened by player's use button.
#define SF_DOOR_TOUCH_ONLY_CLIENTS      1024       // Only clients can touch
#define SF_DOOR_ACTUALLY_WATER          0x80000000 // This bit marks that func_door are actually func_water

/**
* gibshooter
*/
#define SF_GIBSHOOTER_REPEATABLE        1 // Allows a gibshooter to be refired

/**
* env_funnel
*/
#define SF_FUNNEL_REVERSE               1 // Funnel effect repels particles instead of attracting them

/**
* env_bubbles
*/
#define SF_BUBBLES_STARTOFF             0x0001

/**
* env_blood
*/
#define SF_BLOOD_RANDOM                 0x0001
#define SF_BLOOD_STREAM                 0x0002
#define SF_BLOOD_PLAYER                 0x0004
#define SF_BLOOD_DECAL                  0x0008

/**
* env_shake
*/
#define SF_SHAKE_EVERYONE               0x0001 // Don't check radius
#define SF_SHAKE_DISRUPT                0x0002 // Disrupt controls
#define SF_SHAKE_INAIR                  0x0004 // Shake players in air

/**
* env_fade
*/
#define SF_FADE_IN                      0x0001 // Fade in, not out
#define SF_FADE_MODULATE                0x0002 // Modulate, don't blend
#define SF_FADE_ONLYONE                 0x0004

/**
* env_beam, env_lightning
*/
#define SF_BEAM_STARTON                 0x0001
#define SF_BEAM_TOGGLE                  0x0002
#define SF_BEAM_RANDOM                  0x0004
#define SF_BEAM_RING                    0x0008
#define SF_BEAM_SPARKSTART              0x0010
#define SF_BEAM_SPARKEND                0x0020
#define SF_BEAM_DECALS                  0x0040
#define SF_BEAM_SHADEIN                 0x0080
#define SF_BEAM_SHADEOUT                0x0100
#define SF_BEAM_TEMPORARY               0x8000

/**
* env_sprite
*/
#define SF_SPRITE_STARTON               0x0001
#define SF_SPRITE_ONCE                  0x0002
#define SF_SPRITE_TEMPORARY             0x8000

/**
* env_message
*/
#define SF_MESSAGE_ONCE                 0x0001 // Fade in, not out
#define SF_MESSAGE_ALL                  0x0002 // Send to all clients

/**
* env_explosion
*/
#define SF_ENVEXPLOSION_NODAMAGE        (1<<0) // When set, ENV_EXPLOSION will not actually inflict damage
#define SF_ENVEXPLOSION_REPEATABLE      (1<<1) // Can this entity be refired?
#define SF_ENVEXPLOSION_NOFIREBALL      (1<<2) // Don't draw the fireball
#define SF_ENVEXPLOSION_NOSMOKE         (1<<3) // Don't draw the smoke
#define SF_ENVEXPLOSION_NODECAL         (1<<4) // Don't make a scorch mark
#define SF_ENVEXPLOSION_NOSPARKS        (1<<5) // Don't make a scorch mark

/**
* func_tank
*/
#define SF_TANK_ACTIVE                  0x0001
#define SF_TANK_PLAYER                  0x0002
#define SF_TANK_HUMANS                  0x0004
#define SF_TANK_ALIENS                  0x0008
#define SF_TANK_LINEOFSIGHT             0x0010
#define SF_TANK_CANCONTROL              0x0020
#define SF_TANK_SOUNDON                 0x8000

/**
* grenade
*/
#define SF_DETONATE                     0x0001 // Grenades flagged with this will be triggered when the owner calls detonateSatchelCharges

/**
* game_score
*/
#define SF_SCORE_NEGATIVE               0x0001 // Allow negative scores
#define SF_SCORE_TEAM                   0x0002 // Award points to team in teamplay

/**
* game_text
*/
#define SF_ENVTEXT_ALLPLAYERS           0x0001 // Message will be displayed to all players instead of just the activator.

/**
* game_team_master
*/
#define SF_TEAMMASTER_FIREONCE          0x0001 // Remove on Fire
#define SF_TEAMMASTER_ANYTEAM           0x0002 // Any team until set? -- Any team can use this until the team is set (otherwise no teams can use it)

/**
* game_team_set
*/
#define SF_TEAMSET_FIREONCE             0x0001 // Remove entity after firing.
#define SF_TEAMSET_CLEARTEAM            0x0002 // Clear team -- Sets the team to "NONE" instead of activator

/**
* game_player_hurt
*/
#define SF_PKILL_FIREONCE               0x0001 // Remove entity after firing.

/**
* game_counter
*/
#define SF_GAMECOUNT_FIREONCE           0x0001 // Remove entity after firing.
#define SF_GAMECOUNT_RESET              0x0002 // Reset entity Initial value after fired.
#define SF_GAMECOUNT_OVER_LIMIT         0x0004 // Fire a target when initial value is higher than limit value.

/**
* game_player_equip
*/
#define MAX_EQUIP                       32
#define SF_PLAYEREQUIP_USEONLY          0x0001 // If set, the game_player_equip entity will not equip respawning players,
                                               // but only react to direct triggering, equipping its activator. This makes its master obsolete.

/**
* game_player_team
*/
#define SF_PTEAM_FIREONCE               0x0001 // Remove entity after firing.
#define SF_PTEAM_KILL                   0x0002 // Kill Player.
#define SF_PTEAM_GIB                    0x0004 // Gib Player.

/**
* func_plat
*/
#define SF_PLAT_TOGGLE                  0x0001 // The lift is no more automatically called from top and activated by stepping on it.
                                               // It required trigger to do so.

/**
* func_trackchange
*/
#define SF_TRACK_ACTIVATETRAIN          0x00000001
#define SF_TRACK_RELINK                 0x00000002
#define SF_TRACK_ROTMOVE                0x00000004
#define SF_TRACK_STARTBOTTOM            0x00000008
#define SF_TRACK_DONT_MOVE              0x00000010

/**
* func_tracktrain
*/
#define SF_TRACKTRAIN_NOPITCH           0x0001
#define SF_TRACKTRAIN_NOCONTROL         0x0002
#define SF_TRACKTRAIN_FORWARDONLY       0x0004
#define SF_TRACKTRAIN_PASSABLE          0x0008

/**
* path_track
*/
#define SF_PATH_DISABLED                0x00000001
#define SF_PATH_FIREONCE                0x00000002
#define SF_PATH_ALTREVERSE              0x00000004
#define SF_PATH_DISABLE_TRAIN           0x00000008
#define SF_PATH_ALTERNATE               0x00008000

/**
* path_corner
*/
#define SF_CORNER_WAITFORTRIG           0x001
#define SF_CORNER_TELEPORT              0x002
#define SF_CORNER_FIREONCE              0x004

/**
* trigger_push
*/
#define SF_TRIGGER_PUSH_ONCE            1
#define SF_TRIGGER_PUSH_START_OFF       2 // Spawnflag that makes trigger_push spawn turned OFF

/**
* trigger_hurt
*/
#define SF_TRIGGER_HURT_TARGETONCE      1  // Only fire hurt target once
#define SF_TRIGGER_HURT_START_OFF       2  // Spawnflag that makes trigger_push spawn turned OFF
#define SF_TRIGGER_HURT_NO_CLIENTS      8  // Spawnflag that makes trigger_push spawn turned OFF
#define SF_TRIGGER_HURT_CLIENTONLYFIRE  16 // Trigger hurt will only fire its target if it is hurting a client
#define SF_TRIGGER_HURT_CLIENTONLYTOUCH 32 // Only clients may touch this trigger.

/**
* trigger_auto
*/
#define SF_AUTO_FIREONCE                0x0001
#define SF_AUTO_NORESET                 0x0002

/**
* trigger_relay
*/
#define SF_RELAY_FIREONCE               0x0001

/**
* multi_manager
*/
#define SF_MULTIMAN_THREAD              0x00000001
#define SF_MULTIMAN_CLONE               0x80000000

/**
* env_render
* @note These are flags to indicate masking off various render parameters that
* are usually copied to the targets
*/
#define SF_RENDER_MASKFX                (1<<0)
#define SF_RENDER_MASKAMT               (1<<1)
#define SF_RENDER_MASKMODE              (1<<2)
#define SF_RENDER_MASKCOLOR             (1<<3)

/**
* trigger_changelevel
*/
#define SF_CHANGELEVEL_USEONLY          0x0002

/**
* trigger_endsection
*/
#define SF_ENDSECTION_USEONLY           0x0001

/**
* trigger_camera
*/
#define SF_CAMERA_PLAYER_POSITION       1
#define SF_CAMERA_PLAYER_TARGET         2
#define SF_CAMERA_PLAYER_TAKECONTROL    4

/**
* func_rotating
*/
#define SF_BRUSH_ROTATE_START_ON        1
#define SF_BRUSH_ROTATE_BACKWARDS       2
#define SF_BRUSH_ROTATE_Z_AXIS          4
#define SF_BRUSH_ROTATE_X_AXIS          8
#define SF_BRUSH_ACCDCC                 16   // Brush should accelerate and decelerate when toggled
#define SF_BRUSH_HURT                   32   // Rotating brush that inflicts pain based on rotation speed
#define SF_BRUSH_ROTATE_NOT_SOLID       64   // Some special rotating objects are not solid.
#define SF_BRUSH_ROTATE_SMALLRADIUS     128
#define SF_BRUSH_ROTATE_MEDIUMRADIUS    256
#define SF_BRUSH_ROTATE_LARGERADIUS     512

/**
* triggers
*/
#define SF_TRIGGER_ALLOWMONSTERS        1  // Monsters allowed to fire this trigger
#define SF_TRIGGER_NOCLIENTS            2  // Players not allowed to fire this trigger
#define SF_TRIGGER_PUSHABLES            4  // Only pushables can fire this trigger
#define SF_TRIGGER_NORESET              64 // It is not allowed to be resetting on a new round

/**
* trigger_multiple
*/
#define SF_TRIGGER_MULTIPLE_NOTOUCH     0x0001

/**
* trigger_counter
*/
#define SF_TRIGGER_COUNTER_NOMESSAGE    0x0001

/**
* func_breakable
*/
#define SF_BREAK_TRIGGER_ONLY           1   // May only be broken by trigger
#define SF_BREAK_TOUCH                  2   // Can be 'crashed through' by running player (plate glass)
#define SF_BREAK_PRESSURE               4   // Can be broken by a player standing on it
#define SF_BREAK_CROWBAR                256 // Instant break if hit with crowbar

/**
* func_pushable (also func_breakable, so don't collide with those flags)
*/
#define SF_PUSH_BREAKABLE               128

/**
* light_spawn
*/
#define SF_LIGHT_START_OFF              1

/**
* info_decal
*/
#define SF_DECAL_NOTINDEATHMATCH        2048

/**
* worldspawn
*/
#define SF_WORLD_DARK                   0x0001 // Fade from black at startup
#define SF_WORLD_TITLE                  0x0002 // Display game title at startup
#define SF_WORLD_FORCETEAM              0x0004 // Force teams

/**
* Set this bit on guns and stuff that should never respawn
*/
#define SF_NORESPAWN                    (1<<30)

/**
* @endsection
*/

/**
* Train status values
*/
#define TRAIN_ACTIVE                    0x80
#define TRAIN_NEW                       0xc0

#define TRAIN_OFF                       0x00
#define TRAIN_NEUTRAL                   0x01
#define TRAIN_SLOW                      0x02
#define TRAIN_MEDIUM                    0x03
#define TRAIN_FAST                      0x04
#define TRAIN_BACK                      0x05

/**
* PlaybackEvent flags
*/
#define FEV_NOTHOST                     (1<<0) // Skip local host for event send.
#define FEV_RELIABLE                    (1<<1) // Send the event reliably. You must specify the origin and angles
                                               // for this to work correctly on the server for anything
                                               // that depends on the event origin/angles. I.e., the origin/angles are not
                                               // taken from the invoking edict for reliable events.

#define FEV_GLOBAL                      (1<<2) // Don't restrict to PAS/PVS, send this event to _everybody_ on the server ( useful for stopping CHAN_STATIC
                                               // sounds started by client event when client is not in PVS anymore ( hwguy in TFC e.g. ).

#define FEV_UPDATE                      (1<<3) // If this client already has one of these events in its queue, just update the event instead of sending it as a duplicate
#define FEV_HOSTONLY                    (1<<4) // Only send to entity specified as the invoker
#define FEV_SERVER                      (1<<5) // Only send if the event was created on the server.
#define FEV_CLIENT                      (1<<6) // Only issue event client side ( from shared code )

/**
* Cap bits to indicate what an object's capabilities are, currently used for
* save/restore and level transitions.
*/
#define FCAP_CUSTOMSAVE                 0x00000001
#define FCAP_ACROSS_TRANSITION          0x00000002 // Should transfer between transitions
#define FCAP_MUST_SPAWN                 0x00000004 // Spawn after restore
#define FCAP_IMPULSE_USE                0x00000008 // Can be used by the player
#define FCAP_CONTINUOUS_USE             0x00000010 // Can be used by the player
#define FCAP_ONOFF_USE                  0x00000020 // Can be used by the player
#define FCAP_DIRECTIONAL_USE            0x00000040 // Player sends +/- 1 when using (currently only tracktrains)
#define FCAP_MASTER                     0x00000080 // Can be used to "master" other entities (like multisource)
#define FCAP_MUST_RESET                 0x00000100 // Should reset on the new round
#define FCAP_MUST_RELEASE               0x00000200 // Should release on the new round
#define FCAP_DONT_SAVE                  0x80000000 // Don't save this

// UNDONE: This will ignore transition volumes (trigger_transition), but not the PVS!!!
#define FCAP_FORCE_TRANSITION           0x00000080 // ALWAYS goes across transitions

/**
* All monsters need this data
*/
#define DONT_BLEED                      -1
#define BLOOD_COLOR_RED                 247
#define BLOOD_COLOR_YELLOW              195
#define BLOOD_COLOR_GREEN               BLOOD_COLOR_YELLOW

/**
* CBasePlayer::m_iIgnoreGlobalChat
*/
enum
{
	IGNOREMSG_NONE,  // Nothing to do
	IGNOREMSG_ENEMY, // To ignore any chat messages from the enemy
	IGNOREMSG_TEAM   // Same as IGNOREMSG_ENEMY but ignore teammates
}

/**
* Hitgroups
*/
enum HitBoxGroup
{
	HITGROUP_GENERIC = 0,
	HITGROUP_HEAD,
	HITGROUP_CHEST,
	HITGROUP_STOMACH,
	HITGROUP_LEFTARM,
	HITGROUP_RIGHTARM,
	HITGROUP_LEFTLEG,
	HITGROUP_RIGHTLEG,
	HITGROUP_SHIELD
};

/**
* Armor types for m_iKevlar
*/
enum ArmorType
{
	ARMOR_NONE,     // No armor
	ARMOR_KEVLAR,   // Body vest only
	ARMOR_VESTHELM, // Vest and helmet
};

/**
* For CArmoury::m_Armoury_iItem
*/
enum ArmouryItemPack
{
	ARMOURY_MP5NAVY,
	ARMOURY_TMP,
	ARMOURY_P90,
	ARMOURY_MAC10,
	ARMOURY_AK47,
	ARMOURY_SG552,
	ARMOURY_M4A1,
	ARMOURY_AUG,
	ARMOURY_SCOUT,
	ARMOURY_G3SG1,
	ARMOURY_AWP,
	ARMOURY_M3,
	ARMOURY_XM1014,
	ARMOURY_M249,
	ARMOURY_FLASHBANG,
	ARMOURY_HEGRENADE,
	ARMOURY_KEVLAR,
	ARMOURY_ASSAULT,
	ARMOURY_SMOKEGRENADE,
	ARMOURY_SHIELD,
	ARMOURY_FAMAS,
	ARMOURY_SG550,
	ARMOURY_GALIL,
	ARMOURY_UMP45,
	ARMOURY_GLOCK18,
	ARMOURY_USP,
	ARMOURY_ELITE,
	ARMOURY_FIVESEVEN,
	ARMOURY_P228,
	ARMOURY_DEAGLE,
};

/**
* Reward types for RG_CBasePlayer_AddAccount
*/
enum RewardType
{
	RT_NONE,
	RT_ROUND_BONUS,
	RT_PLAYER_RESET,
	RT_PLAYER_JOIN,
	RT_PLAYER_SPEC_JOIN,
	RT_PLAYER_BOUGHT_SOMETHING,
	RT_HOSTAGE_TOOK,
	RT_HOSTAGE_RESCUED,
	RT_HOSTAGE_DAMAGED,
	RT_HOSTAGE_KILLED,
	RT_TEAMMATES_KILLED,
	RT_ENEMY_KILLED,
	RT_INTO_GAME,
	RT_VIP_KILLED,
	RT_VIP_RESCUED_MYSELF
};

/**
* Team IDs for m_iTeam
*/
enum TeamName
{
	TEAM_UNASSIGNED,
	TEAM_TERRORIST,
	TEAM_CT,
	TEAM_SPECTATOR
};

/**
* For m_iModelName
*/
enum ModelName
{
	MODEL_UNASSIGNED,
	MODEL_CT_URBAN,
	MODEL_T_TERROR,
	MODEL_T_LEET,
	MODEL_T_ARCTIC,
	MODEL_CT_GSG9,
	MODEL_CT_GIGN,
	MODEL_CT_SAS,
	MODEL_T_GUERILLA,
	MODEL_CT_VIP,
	MODEL_T_MILITIA,
	MODEL_CT_SPETSNAZ,
	MODEL_AUTO
};

/**
* weapon respawning return codes
*/
enum
{
	GR_NONE = 0,
	GR_WEAPON_RESPAWN_YES,
	GR_WEAPON_RESPAWN_NO,
	GR_AMMO_RESPAWN_YES,
	GR_AMMO_RESPAWN_NO,
	GR_ITEM_RESPAWN_YES,
	GR_ITEM_RESPAWN_NO,
	GR_PLR_DROP_GUN_ALL,
	GR_PLR_DROP_GUN_ACTIVE,
	GR_PLR_DROP_GUN_NO,
	GR_PLR_DROP_AMMO_ALL,
	GR_PLR_DROP_AMMO_ACTIVE,
	GR_PLR_DROP_AMMO_NO
};

/**
* Weapon IDs for rg_get_weaponbox_id, GetGrenadeType, rg_get_weapon_info(const weapon_name[], WI_ID)
*/
enum WeaponIdType
{
	WEAPON_NONE,
	WEAPON_P228,
	WEAPON_GLOCK,
	WEAPON_SCOUT,
	WEAPON_HEGRENADE,
	WEAPON_XM1014,
	WEAPON_C4,
	WEAPON_MAC10,
	WEAPON_AUG,
	WEAPON_SMOKEGRENADE,
	WEAPON_ELITE,
	WEAPON_FIVESEVEN,
	WEAPON_UMP45,
	WEAPON_SG550,
	WEAPON_GALIL,
	WEAPON_FAMAS,
	WEAPON_USP,
	WEAPON_GLOCK18,
	WEAPON_AWP,
	WEAPON_MP5N,
	WEAPON_M249,
	WEAPON_M3,
	WEAPON_M4A1,
	WEAPON_TMP,
	WEAPON_G3SG1,
	WEAPON_FLASHBANG,
	WEAPON_DEAGLE,
	WEAPON_SG552,
	WEAPON_AK47,
	WEAPON_KNIFE,
	WEAPON_P90,
	WEAPON_SHIELDGUN = 99
};

/**
* For CBaseMonster::m_rgbTimeBasedDamage
* prefix should be ITBD_
*/
enum TimeBasedDamage
{
	ITDB_PARALYZE,
	ITDB_NERVEGAS,
	ITDB_POISON,
	ITDB_RADIATION,
	ITDB_DROWNRECOVER,
	ITDB_ACID,
	ITDB_SLOWBURN,
	ITDB_SLOWFREEZE
};

/**
* m_Activity
*/
enum Activity
{
	ACT_INVALID = -1,
	ACT_RESET, // Set m_Activity to this invalid value to force a reset to m_IdealActivity
	ACT_IDLE,
	ACT_GUARD,
	ACT_WALK,
	ACT_RUN,
	ACT_FLY,
	ACT_SWIM,
	ACT_HOP,
	ACT_LEAP,
	ACT_FALL,
	ACT_LAND,
	ACT_STRAFE_LEFT,
	ACT_STRAFE_RIGHT,
	ACT_ROLL_LEFT,
	ACT_ROLL_RIGHT,
	ACT_TURN_LEFT,
	ACT_TURN_RIGHT,
	ACT_CROUCH,
	ACT_CROUCHIDLE,
	ACT_STAND,
	ACT_USE,
	ACT_SIGNAL1,
	ACT_SIGNAL2,
	ACT_SIGNAL3,
	ACT_TWITCH,
	ACT_COWER,
	ACT_SMALL_FLINCH,
	ACT_BIG_FLINCH,
	ACT_RANGE_ATTACK1,
	ACT_RANGE_ATTACK2,
	ACT_MELEE_ATTACK1,
	ACT_MELEE_ATTACK2,
	ACT_RELOAD,
	ACT_ARM,
	ACT_DISARM,
	ACT_EAT,
	ACT_DIESIMPLE,
	ACT_DIEBACKWARD,
	ACT_DIEFORWARD,
	ACT_DIEVIOLENT,
	ACT_BARNACLE_HIT,
	ACT_BARNACLE_PULL,
	ACT_BARNACLE_CHOMP,
	ACT_BARNACLE_CHEW,
	ACT_SLEEP,
	ACT_INSPECT_FLOOR,
	ACT_INSPECT_WALL,
	ACT_IDLE_ANGRY,
	ACT_WALK_HURT,
	ACT_RUN_HURT,
	ACT_HOVER,
	ACT_GLIDE,
	ACT_FLY_LEFT,
	ACT_FLY_RIGHT,
	ACT_DETECT_SCENT,
	ACT_SNIFF,
	ACT_BITE,
	ACT_THREAT_DISPLAY,
	ACT_FEAR_DISPLAY,
	ACT_EXCITED,
	ACT_SPECIAL_ATTACK1,
	ACT_SPECIAL_ATTACK2,
	ACT_COMBAT_IDLE,
	ACT_WALK_SCARED,
	ACT_RUN_SCARED,
	ACT_VICTORY_DANCE,
	ACT_DIE_HEADSHOT,
	ACT_DIE_CHESTSHOT,
	ACT_DIE_GUTSHOT,
	ACT_DIE_BACKSHOT,
	ACT_FLINCH_HEAD,
	ACT_FLINCH_CHEST,
	ACT_FLINCH_STOMACH,
	ACT_FLINCH_LEFTARM,
	ACT_FLINCH_RIGHTARM,
	ACT_FLINCH_LEFTLEG,
	ACT_FLINCH_RIGHTLEG,
	ACT_FLINCH,
	ACT_LARGE_FLINCH,
	ACT_HOLDBOMB,
	ACT_IDLE_FIDGET,
	ACT_IDLE_SCARED,
	ACT_IDLE_SCARED_FIDGET,
	ACT_FOLLOW_IDLE,
	ACT_FOLLOW_IDLE_FIDGET,
	ACT_FOLLOW_IDLE_SCARED,
	ACT_FOLLOW_IDLE_SCARED_FIDGET,
	ACT_CROUCH_IDLE,
	ACT_CROUCH_IDLE_FIDGET,
	ACT_CROUCH_IDLE_SCARED,
	ACT_CROUCH_IDLE_SCARED_FIDGET,
	ACT_CROUCH_WALK,
	ACT_CROUCH_WALK_SCARED,
	ACT_CROUCH_DIE,
	ACT_WALK_BACK,
	ACT_IDLE_SNEAKY,
	ACT_IDLE_SNEAKY_FIDGET,
	ACT_WALK_SNEAKY,
	ACT_WAVE,
	ACT_YES,
	ACT_NO
};

/**
* WinStatus for RG_RoundEnd, rg_round_end
*/
enum WinStatus
{
	WINSTATUS_NONE = 0,
	WINSTATUS_CTS,
	WINSTATUS_TERRORISTS,
	WINSTATUS_DRAW,
};

// Bypass warning: 200 on amxmodx 1.8.2
#if AMXX_VERSION_NUM < 183
	#define ROUND_ESCAPING_TERRORISTS_NEUTRALIZED ROUND_ESCAPING_TERRORISTS_NEUTR
	#define RR_ESCAPING_TERRORISTS_NEUTRALIZED RR_ESCAPING_TERRORISTS_NEUTR
#endif

/**
* Used for EndRoundMessage() logged messages
*/
enum ScenarioEventEndRound
{
	ROUND_NONE,
	ROUND_TARGET_BOMB,
	ROUND_VIP_ESCAPED,
	ROUND_VIP_ASSASSINATED,
	ROUND_TERRORISTS_ESCAPED,
	ROUND_CTS_PREVENT_ESCAPE,
	ROUND_ESCAPING_TERRORISTS_NEUTRALIZED,
	ROUND_BOMB_DEFUSED,
	ROUND_CTS_WIN,
	ROUND_TERRORISTS_WIN,
	ROUND_END_DRAW,
	ROUND_ALL_HOSTAGES_RESCUED,
	ROUND_TARGET_SAVED,
	ROUND_HOSTAGE_NOT_RESCUED,
	ROUND_TERRORISTS_NOT_ESCAPED,
	ROUND_VIP_NOT_ESCAPED,
	ROUND_GAME_COMMENCE,
	ROUND_GAME_RESTART,
	ROUND_GAME_OVER
};

/**
* Animation IDs for RG_CBasePlayer_SetAnimation, rg_set_animation
*/
enum PLAYER_ANIM
{
	PLAYER_IDLE,
	PLAYER_WALK,
	PLAYER_JUMP,
	PLAYER_SUPERJUMP,
	PLAYER_DIE,
	PLAYER_ATTACK1,
	PLAYER_ATTACK2,
	PLAYER_FLINCH,
	PLAYER_LARGE_FLINCH,
	PLAYER_RELOAD,
	PLAYER_HOLDBOMB
};

/**
* For CBasePlayer::m_iJoiningState
*/
enum JoinState
{
	JOINED,
	SHOWLTEXT,
	READINGLTEXT,
	SHOWTEAMSELECT,
	PICKINGTEAM,
	GETINTOGAME
};

/**
* For CBasePlayer::m_iMenu
*/
enum _Menu
{
	Menu_OFF,
	Menu_ChooseTeam,
	Menu_IGChooseTeam,
	Menu_ChooseAppearance,
	Menu_Buy,
	Menu_BuyPistol,
	Menu_BuyRifle,
	Menu_BuyMachineGun,
	Menu_BuyShotgun,
	Menu_BuySubMachineGun,
	Menu_BuyItem,
	Menu_Radio1,
	Menu_Radio2,
	Menu_Radio3,
	Menu_ClientBuy
};

/**
* For CBasePlayer::m_flLastCommandTime[8]
*/
enum TrackCommands
{
	CMD_SAY,
	CMD_SAYTEAM,
	CMD_FULLUPDATE,
	CMD_VOTE,
	CMD_VOTEMAP,
	CMD_LISTMAPS,
	CMD_LISTPLAYERS,
	CMD_NIGHTVISION,
	COMMANDS_TO_TRACK,
};

/**
* For CBasePlayer::m_rgpPlayerItems[6]
*/
enum InventorySlotType
{
	NONE_SLOT,
	PRIMARY_WEAPON_SLOT,
	PISTOL_SLOT,
	KNIFE_SLOT,
	GRENADE_SLOT,
	C4_SLOT,
};

/**
* Sets animation for weapon_shield of SendWeaponAnim
*/
enum shieldgren_e
{
	SHIELDREN_IDLE = 4,
	SHIELDREN_UP,
	SHIELDREN_DOWN
};

enum shieldgun_e
{
	SHIELDGUN_IDLE,
	SHIELDGUN_SHOOT1,
	SHIELDGUN_SHOOT2,
	SHIELDGUN_SHOOT_EMPTY,
	SHIELDGUN_RELOAD,
	SHIELDGUN_DRAW,
	SHIELDGUN_DRAWN_IDLE,
	SHIELDGUN_UP,
	SHIELDGUN_DOWN,
};

/**
* Flags for CUnifiedSignals m_signals
*/
enum SignalState
{
	SIGNAL_BUY       = BIT(0),
	SIGNAL_BOMB      = BIT(1),
	SIGNAL_RESCUE    = BIT(2),
	SIGNAL_ESCAPE    = BIT(3),
	SIGNAL_VIPSAFETY = BIT(4),
};

/**
* Weapon states of the following weapons: usp, elite, glock18, famas and m4a1.
*/
enum WeaponState
{
	WPNSTATE_USP_SILENCED       = BIT(0),
	WPNSTATE_GLOCK18_BURST_MODE = BIT(1),
	WPNSTATE_M4A1_SILENCED      = BIT(2),
	WPNSTATE_ELITE_LEFT         = BIT(3),
	WPNSTATE_FAMAS_BURST_MODE   = BIT(4),
	WPNSTATE_SHIELD_DRAWN       = BIT(5),
};

/**
* Bullet types for rg_fire_bullets, rg_fire_bullets3
*/
enum Bullet
{
	BULLET_NONE,
	BULLET_PLAYER_9MM,
	BULLET_PLAYER_MP5,
	BULLET_PLAYER_357,
	BULLET_PLAYER_BUCKSHOT,
	BULLET_PLAYER_CROWBAR,
	BULLET_MONSTER_9MM,
	BULLET_MONSTER_MP5,
	BULLET_MONSTER_12MM,
	BULLET_PLAYER_45ACP,
	BULLET_PLAYER_338MAG,
	BULLET_PLAYER_762MM,
	BULLET_PLAYER_556MM,
	BULLET_PLAYER_50AE,
	BULLET_PLAYER_57MM,
	BULLET_PLAYER_357SIG,
};

/**
* Reward rules for rg_get_account_rules, rg_set_account_rules
*/
enum RewardRules
{
	RR_CTS_WIN,
	RR_TERRORISTS_WIN,
	RR_TARGET_BOMB,
	RR_VIP_ESCAPED,
	RR_VIP_ASSASSINATED,
	RR_TERRORISTS_ESCAPED,
	RR_CTS_PREVENT_ESCAPE,
	RR_ESCAPING_TERRORISTS_NEUTRALIZED,
	RR_BOMB_DEFUSED,
	RR_BOMB_PLANTED,
	RR_BOMB_EXPLODED,
	RR_ALL_HOSTAGES_RESCUED,
	RR_TARGET_BOMB_SAVED,
	RR_HOSTAGE_NOT_RESCUED,
	RR_VIP_NOT_ESCAPED,
	RR_LOSER_BONUS_DEFAULT,
	RR_LOSER_BONUS_MIN,
	RR_LOSER_BONUS_MAX,
	RR_LOSER_BONUS_ADD,
	RR_RESCUED_HOSTAGE,
	RR_TOOK_HOSTAGE_ACC,
	RR_TOOK_HOSTAGE,
	RR_END
};

/**
* For RG_CBasePlayer_HasRestrictItem
*/
enum ItemRestType
{
	ITEM_TYPE_BUYING,  // When a player is buying items
	ITEM_TYPE_TOUCHED, // When the player touches a weaponbox or armoury_entity
	ITEM_TYPE_EQUIPPED // When an entity game_player_equip gives item to player or default items on player spawn
};

/**
* Constant items
*/
enum ItemID
{
	ITEM_NONE = -1,
	ITEM_SHIELDGUN,
	ITEM_P228,
	ITEM_GLOCK,
	ITEM_SCOUT,
	ITEM_HEGRENADE,
	ITEM_XM1014,
	ITEM_C4,
	ITEM_MAC10,
	ITEM_AUG,
	ITEM_SMOKEGRENADE,
	ITEM_ELITE,
	ITEM_FIVESEVEN,
	ITEM_UMP45,
	ITEM_SG550,
	ITEM_GALIL,
	ITEM_FAMAS,
	ITEM_USP,
	ITEM_GLOCK18,
	ITEM_AWP,
	ITEM_MP5N,
	ITEM_M249,
	ITEM_M3,
	ITEM_M4A1,
	ITEM_TMP,
	ITEM_G3SG1,
	ITEM_FLASHBANG,
	ITEM_DEAGLE,
	ITEM_SG552,
	ITEM_AK47,
	ITEM_KNIFE,
	ITEM_P90,
	ITEM_NVG,
	ITEM_DEFUSEKIT,
	ITEM_KEVLAR,
	ITEM_ASSAULT,
	ITEM_LONGJUMP,
	ITEM_SODACAN,
	ITEM_HEALTHKIT,
	ITEM_ANTIDOTE,
	ITEM_BATTERY
};

/**
* For CMapInfo:m_iBuyingStatus
*/
enum InfoMapBuyParam
{
	BUYING_EVERYONE = 0,
	BUYING_ONLY_CTS,
	BUYING_ONLY_TERRORISTS,
	BUYING_NO_ONE,
};

/**
* For SetUse
*/
#define USE_OFF                         0
#define USE_ON                          1
#define USE_SET                         2
#define USE_TOGGLE                      3

/**
* For CBasePlayer:m_iWeaponVolume
*/
#define LOUD_GUN_VOLUME                 1000
#define NORMAL_GUN_VOLUME               600
#define QUIET_GUN_VOLUME                200
#define BIG_EXPLOSION_VOLUME            2048
#define NORMAL_EXPLOSION_VOLUME         1024
#define SMALL_EXPLOSION_VOLUME          512

/**
* For CBasePlayer:m_iWeaponFlash
*/
#define BRIGHT_GUN_FLASH                512
#define NORMAL_GUN_FLASH                256
#define DIM_GUN_FLASH                   128

/**
* Player zoom constants
* For m_iFOV
*/
#define DEFAULT_LARGE_AWP_ZOOM            10
#define DEFAULT_LARGE_OTHER_SNIPERS_ZOOM  15
#define DEFAULT_SMALL_SNIPERS_ZOOM        40
#define DEFAULT_AUG_SG552_ZOOM            55
#define DEFAULT_NO_ZOOM                   90

/**
* Scoreboard attribute constants
* For "ScoreAttrib" game event
*/
#define SCORE_STATUS_NONE                 0
#define SCORE_STATUS_DEAD                 (1<<0)
#define SCORE_STATUS_BOMB                 (1<<1)
#define SCORE_STATUS_VIP                  (1<<2)
#define SCORE_STATUS_DEFKIT               (1<<3)

/**
* For RG_CBotManager_OnEvent
*/
enum GameEventType
{
	EVENT_INVALID = 0,
	EVENT_WEAPON_FIRED,                 // tell bots the player is attack (argumens: 1 = attacker, 2 = NULL)
	EVENT_WEAPON_FIRED_ON_EMPTY,        // tell bots the player is attack without clip ammo (argumens: 1 = attacker, 2 = NULL)
	EVENT_WEAPON_RELOADED,              // tell bots the player is reloading his weapon (argumens: 1 = reloader, 2 = NULL)

	EVENT_HE_GRENADE_EXPLODED,          // tell bots the HE grenade is exploded (argumens: 1 = grenade thrower, 2 = NULL)
	EVENT_FLASHBANG_GRENADE_EXPLODED,   // tell bots the flashbang grenade is exploded (argumens: 1 = grenade thrower, 2 = explosion origin)
	EVENT_SMOKE_GRENADE_EXPLODED,       // tell bots the smoke grenade is exploded (argumens: 1 = grenade thrower, 2 = NULL)
	EVENT_GRENADE_BOUNCED,

	EVENT_BEING_SHOT_AT,
	EVENT_PLAYER_BLINDED_BY_FLASHBANG,  // tell bots the player is flashed (argumens: 1 = flashed player, 2 = NULL)
	EVENT_PLAYER_FOOTSTEP,              // tell bots the player is running (argumens: 1 = runner, 2 = NULL)
	EVENT_PLAYER_JUMPED,                // tell bots the player is jumped (argumens: 1 = jumper, 2 = NULL)
	EVENT_PLAYER_DIED,                  // tell bots the player is killed (argumens: 1 = victim, 2 = killer)
	EVENT_PLAYER_LANDED_FROM_HEIGHT,    // tell bots the player is fell with some damage (argumens: 1 = felled player, 2 = NULL)
	EVENT_PLAYER_TOOK_DAMAGE,           // tell bots the player is take damage (argumens: 1 = victim, 2 = attacker)
	EVENT_HOSTAGE_DAMAGED,              // tell bots the player has injured a hostage (argumens: 1 = hostage, 2 = injurer)
	EVENT_HOSTAGE_KILLED,               // tell bots the player has killed a hostage (argumens: 1 = hostage, 2 = killer)

	EVENT_DOOR,                         // tell bots the door is moving (argumens: 1 = door, 2 = NULL)
	EVENT_BREAK_GLASS,                  // tell bots the glass has break (argumens: 1 = glass, 2 = NULL)
	EVENT_BREAK_WOOD,                   // tell bots the wood has break (argumens: 1 = wood, 2 = NULL)
	EVENT_BREAK_METAL,                  // tell bots the metal/computer has break (argumens: 1 = metal/computer, 2 = NULL)
	EVENT_BREAK_FLESH,                  // tell bots the flesh has break (argumens: 1 = flesh, 2 = NULL)
	EVENT_BREAK_CONCRETE,               // tell bots the concrete has break (argumens: 1 = concrete, 2 = NULL)

	EVENT_BOMB_PLANTED,                 // tell bots the bomb has been planted (argumens: 1 = planter, 2 = NULL)
	EVENT_BOMB_DROPPED,                 // tell bots the bomb has been dropped (argumens: 1 = NULL, 2 = NULL)
	EVENT_BOMB_PICKED_UP,               // let the bots hear the bomb pickup (argumens: 1 = player that pickup c4, 2 = NULL)
	EVENT_BOMB_BEEP,                    // let the bots hear the bomb beeping (argumens: 1 = c4, 2 = NULL)
	EVENT_BOMB_DEFUSING,                // tell the bots someone has started defusing (argumens: 1 = defuser, 2 = NULL)
	EVENT_BOMB_DEFUSE_ABORTED,          // tell the bots someone has aborted defusing (argumens: 1 = NULL, 2 = NULL)
	EVENT_BOMB_DEFUSED,                 // tell the bots the bomb is defused (argumens: 1 = defuser, 2 = NULL)
	EVENT_BOMB_EXPLODED,                // let the bots hear the bomb exploding (argumens: 1 = NULL, 2 = NULL)

	EVENT_HOSTAGE_USED,                 // tell bots the hostage is used (argumens: 1 = user, 2 = NULL)
	EVENT_HOSTAGE_RESCUED,              // tell bots the hostage is rescued (argumens: 1 = rescuer (CBasePlayer *), 2 = hostage (CHostage *))
	EVENT_ALL_HOSTAGES_RESCUED,         // tell bots the all hostages are rescued (argumens: 1 = NULL, 2 = NULL)

	EVENT_VIP_ESCAPED,                  // tell bots the VIP is escaped (argumens: 1 = NULL, 2 = NULL)
	EVENT_VIP_ASSASSINATED,             // tell bots the VIP is assassinated (argumens: 1 = NULL, 2 = NULL)
	EVENT_TERRORISTS_WIN,               // tell bots the terrorists won the round (argumens: 1 = NULL, 2 = NULL)
	EVENT_CTS_WIN,                      // tell bots the CTs won the round (argumens: 1 = NULL, 2 = NULL)
	EVENT_ROUND_DRAW,                   // tell bots the round was a draw (argumens: 1 = NULL, 2 = NULL)
	EVENT_ROUND_WIN,                    // tell carreer the round was a win (argumens: 1 = NULL, 2 = NULL)
	EVENT_ROUND_LOSS,                   // tell carreer the round was a loss (argumens: 1 = NULL, 2 = NULL)
	EVENT_ROUND_START,                  // tell bots the round was started (when freeze period is expired) (argumens: 1 = NULL, 2 = NULL)
	EVENT_PLAYER_SPAWNED,               // tell bots the player is spawned (argumens: 1 = spawned player, 2 = NULL)
	EVENT_CLIENT_CORPSE_SPAWNED,
	EVENT_BUY_TIME_START,
	EVENT_PLAYER_LEFT_BUY_ZONE,
	EVENT_DEATH_CAMERA_START,
	EVENT_KILL_ALL,
	EVENT_ROUND_TIME,
	EVENT_DIE,
	EVENT_KILL,
	EVENT_HEADSHOT,
	EVENT_KILL_FLASHBANGED,
	EVENT_TUTOR_BUY_MENU_OPENNED,
	EVENT_TUTOR_AUTOBUY,
	EVENT_PLAYER_BOUGHT_SOMETHING,
	EVENT_TUTOR_NOT_BUYING_ANYTHING,
	EVENT_TUTOR_NEED_TO_BUY_PRIMARY_WEAPON,
	EVENT_TUTOR_NEED_TO_BUY_PRIMARY_AMMO,
	EVENT_TUTOR_NEED_TO_BUY_SECONDARY_AMMO,
	EVENT_TUTOR_NEED_TO_BUY_ARMOR,
	EVENT_TUTOR_NEED_TO_BUY_DEFUSE_KIT,
	EVENT_TUTOR_NEED_TO_BUY_GRENADE,
	EVENT_CAREER_TASK_DONE,

	EVENT_START_RADIO_1,
	EVENT_RADIO_COVER_ME,
	EVENT_RADIO_YOU_TAKE_THE_POINT,
	EVENT_RADIO_HOLD_THIS_POSITION,
	EVENT_RADIO_REGROUP_TEAM,
	EVENT_RADIO_FOLLOW_ME,
	EVENT_RADIO_TAKING_FIRE,
	EVENT_START_RADIO_2,
	EVENT_RADIO_GO_GO_GO,
	EVENT_RADIO_TEAM_FALL_BACK,
	EVENT_RADIO_STICK_TOGETHER_TEAM,
	EVENT_RADIO_GET_IN_POSITION_AND_WAIT,
	EVENT_RADIO_STORM_THE_FRONT,
	EVENT_RADIO_REPORT_IN_TEAM,
	EVENT_START_RADIO_3,
	EVENT_RADIO_AFFIRMATIVE,
	EVENT_RADIO_ENEMY_SPOTTED,
	EVENT_RADIO_NEED_BACKUP,
	EVENT_RADIO_SECTOR_CLEAR,
	EVENT_RADIO_IN_POSITION,
	EVENT_RADIO_REPORTING_IN,
	EVENT_RADIO_GET_OUT_OF_THERE,
	EVENT_RADIO_NEGATIVE,
	EVENT_RADIO_ENEMY_DOWN,
	EVENT_END_RADIO,

	EVENT_NEW_MATCH,                    // tell bots the game is new (argumens: 1 = NULL, 2 = NULL)
	EVENT_PLAYER_CHANGED_TEAM,          // tell bots the player is switch his team (also called from ClientPutInServer()) (argumens: 1 = switcher, 2 = NULL)
	EVENT_BULLET_IMPACT,                // tell bots the player is shoot at wall (argumens: 1 = shooter, 2 = shoot trace end position)
	EVENT_GAME_COMMENCE,                // tell bots the game is commencing (argumens: 1 = NULL, 2 = NULL)
	EVENT_WEAPON_ZOOMED,                // tell bots the player is switch weapon zoom (argumens: 1 = zoom switcher, 2 = NULL)
	EVENT_HOSTAGE_CALLED_FOR_HELP,      // tell bots the hostage is talking (argumens: 1 = listener, 2 = NULL)
	NUM_GAME_EVENTS,
};

/**
* Weapon secondary attack states
* For CCSPlayerWeapon::m_iStateSecondaryAttack
*/
enum SecondaryAtkState
{
	WEAPON_SECONDARY_ATTACK_NONE = 0,
	WEAPON_SECONDARY_ATTACK_SET,
	WEAPON_SECONDARY_ATTACK_BLOCK
};

/**
* Decal list for rg_decal_trace
*/
enum Decal
{
	DECAL_GUNSHOT1 = 0,
	DECAL_GUNSHOT2,
	DECAL_GUNSHOT3,
	DECAL_GUNSHOT4,
	DECAL_GUNSHOT5,
	DECAL_LAMBDA1,
	DECAL_LAMBDA2,
	DECAL_LAMBDA3,
	DECAL_LAMBDA4,
	DECAL_LAMBDA5,
	DECAL_LAMBDA6,
	DECAL_SCORCH1,
	DECAL_SCORCH2,
	DECAL_BLOOD1,
	DECAL_BLOOD2,
	DECAL_BLOOD3,
	DECAL_BLOOD4,
	DECAL_BLOOD5,
	DECAL_BLOOD6,
	DECAL_YBLOOD1,
	DECAL_YBLOOD2,
	DECAL_YBLOOD3,
	DECAL_YBLOOD4,
	DECAL_YBLOOD5,
	DECAL_YBLOOD6,
	DECAL_GLASSBREAK1,
	DECAL_GLASSBREAK2,
	DECAL_GLASSBREAK3,
	DECAL_BIGSHOT1,
	DECAL_BIGSHOT2,
	DECAL_BIGSHOT3,
	DECAL_BIGSHOT4,
	DECAL_BIGSHOT5,
	DECAL_SPIT1,
	DECAL_SPIT2,
	DECAL_BPROOF1,           // Bulletproof glass decal
	DECAL_GARGSTOMP1,        // Gargantua stomp crack
	DECAL_SMALLSCORCH1,      // Small scorch mark
	DECAL_SMALLSCORCH2,      // Small scorch mark
	DECAL_SMALLSCORCH3,      // Small scorch mark
	DECAL_MOMMABIRTH,        // Big momma birth splatter
	DECAL_MOMMASPLAT,
};

/**
* Player relationship return codes
*/
enum
{
	GR_NOTTEAMMATE = 0,
	GR_TEAMMATE,
	GR_ENEMY,
	GR_ALLY,
	GR_NEUTRAL,
};

// Flags for specifying extra info about player death
enum DeathMessageFlags
{
	// float[3]
	// Position where the victim was killed by the enemy
	PLAYERDEATH_POSITION          = 0x001,

	// byte
	// Index of the assistant who helped the attacker kill the victim
	PLAYERDEATH_ASSISTANT         = 0x002,

	// short
	// Bitsum classification for the rarity of the kill
	// See enum KillRarity for details
	PLAYERDEATH_KILLRARITY        = 0x004
};

// Classifying various player kill methods in the game
enum KillRarity
{
	KILLRARITY_HEADSHOT         = 0x001, // Headshot
	KILLRARITY_KILLER_BLIND     = 0x002, // Killer was blind
	KILLRARITY_NOSCOPE          = 0x004, // No-scope sniper rifle kill
	KILLRARITY_PENETRATED       = 0x008, // Penetrated kill (through walls)
	KILLRARITY_THRUSMOKE        = 0x010, // Smoke grenade penetration kill (bullets went through smoke)
	KILLRARITY_ASSISTEDFLASH    = 0x020, // Assister helped with a flash
	KILLRARITY_DOMINATION_BEGAN = 0x040, // Killer player began dominating the victim (NOTE: this flag is set once)
	KILLRARITY_DOMINATION       = 0x080, // Continues domination by the killer
	KILLRARITY_REVENGE          = 0x100, // Revenge by the killer
	KILLRARITY_INAIR            = 0x200  // Killer was in the air (skill to deal with high inaccuracy)
};
