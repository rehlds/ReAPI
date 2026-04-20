#if defined _reapi_engine_const_included
	#endinput
#endif

#define _reapi_engine_const_included

/**
* For native rh_get_mapname
*/
enum MapNameType
{
	MNT_TRUE,   // return the original map name independant of the name set with via rh_set_mapname
	MNT_SET     // return the name of the current map
};

/**
* For native CheckVisibilityInOrigin
*/
enum CheckVisibilityType
{
	VisibilityInPVS = 0, // Check in Potentially Visible Set (PVS)
	VisibilityInPAS      // Check in Potentially Audible Set (PAS)
};

/**
* For RH_SV_AddResource hook
*/
enum ResourceType_t
{
	t_sound = 0,
	t_skin,
	t_model,
	t_decal,
	t_generic,
	t_eventscript,
	t_world,		// Fake type for world, is really t_model
	rt_unk,

	rt_max
};

/**
* rh_emit_sound2 flags
*/
#define SND_EMIT2_NOPAS   BIT(0) // Never to check PAS
#define SND_EMIT2_INVOKER BIT(1) // Do not send to the client invoker

/**
* enum EngineFunc
*/
enum EngineFunc
{
	/*
	* Description:  -
	* Params:       (const recipients, const entity, const channel, const sample[], const volume, Float:attenuation, const fFlags, const pitch)
	*/
	RH_SV_StartSound = BEGIN_FUNC_REGION(engine),

	/*
	* Description:  -
	* Params:       (const client, bool:crash, const fmt[])
	*/
	RH_SV_DropClient,

	/*
	* Description:  -
	* Params:       (const runPhysics)
	*/
	RH_SV_ActivateServer,

	/*
	* Description:  -
	* Params:       (pcvar, const value[])
	*/
	RH_Cvar_DirectSet,

	/*
	* Description:  Receiver is player index or 0 when update will be sended to all.
	* Params:       (const client, buffer, const receiver)
	*/
	RH_SV_WriteFullClientUpdate,

	/*
	* Description:  -
	* Params:       (const classname[])
	*/
	RH_GetEntityInit,

	/*
	* Description:  Called after processing a client connection request.
	* Params:       (const client)
	*/
	RH_ClientConnected,

	/*
	* Description:  Called when processing a 'connect' client connectionless packet.
	* Params:       ()
	*/
	RH_SV_ConnectClient,

	/*
	* Description:  Called when client it's in the scoreboard
	* Params:       (const client)
	*/
	RH_SV_EmitPings,

	/*
	* Description:  Called when an entity is created.
	* Return type:  Edict * (Entity index)
	* Params:       ()
	*/
	RH_ED_Alloc,

	/*
	* Description:  Called when an entity is removed (freed from server).
	* Params:       (const entity)
	*/
	RH_ED_Free,

	/*
	* Description:  Called when a message is being sent to the server's console.
	* Params:       (const string[])
	*/
	RH_Con_Printf,

	/*
	* Description:   Called when a player's userinfo is being checked.
	* Params:        (adr, buffer, bool:reconnect, reconnectSlot, name[])
	*
	* @note          Param adr is unused, guaranteed to return nothing also, don't send anything through it.
	* @note          In order for param name work, hook needs to be registered as Post.
	*/
	RH_SV_CheckUserInfo,

	/*
	* Description:  Called when a generic resource is being added to generic precache list.
	* Return type:  int
	* Params:       (const string[])
	*/
	RH_PF_precache_generic_I,

	/*
	* Description:  Called when a model is being added to model precache list.
	* Return type:  int
	* Params:       (const string[])
	*/
	RH_PF_precache_model_I,

	/*
	* Description:  Called when a sound is being added to sound precache list.
	* Return type:  int
	* Params:       (const string[])
	*/
	RH_PF_precache_sound_I,

	/*
	* Description:  Called when an event is being added to event precache list.
	* Return type:  int
	* Params:       (const string[])
	*/
	RH_EV_Precache,

	/*
	* Description:  Called when a resource is being added to resource list.
	* Params:       (ResourceType_t:type, const filename[], size, flags, index)
	*/
	RH_SV_AddResource,

	/*
	* Description:  Called when message is being printed to client console.
	* Params:       (const string[])
	*/
	RH_SV_ClientPrintf,

	/*
	* Description:  Called before adding an entity to the physents of a player.
	* Return type:  bool
	* Params:       (const entity, const client)
	*/
	RH_SV_AllowPhysent,

	/*
	* Description:  Called when a command is being sent to server.
	* Params:       (const cmd[], source, id)
	*/
	RH_ExecuteServerStringCmd,

	/*
	* Description:  Called when server sends resources list and location.
	* Params:       (const client)
	*/
	RH_SV_SendResources,

	/*
	* Description:  Called when an entity checks collision with another entity.
	* Return type:  int
	* Params:       (const entity)
	*/
	RH_SV_TestEntityPosition,

};

/**
* enum EntVars
*/
enum EntVars
{
	/*
	* Description:      -
	* Member type:      string_t
	* Get params:       get_entvar(index, EntVars:var, dest[], const lenght);
	* Set params:       set_entvar(index, EntVars:var, const source[]);
	*/
	var_classname = BEGIN_MEMBER_REGION(entvars),

	/*
	* Description:      -
	* Member type:      string_t
	* Get params:       get_entvar(index, EntVars:var, dest[], const lenght);
	* Set params:       set_entvar(index, EntVars:var, const source[]);
	*/
	var_globalname,

	/*
	* Description:      -
	* Member type:      class Vector
	* Get params:       get_entvar(index, EntVars:var, Float:output[3]);
	* Set params:       set_entvar(index, EntVars:var, Float:dest[3]);
	*/
	var_origin,

	/*
	* Description:      -
	* Member type:      class Vector
	* Get params:       get_entvar(index, EntVars:var, Float:output[3]);
	* Set params:       set_entvar(index, EntVars:var, Float:dest[3]);
	*/
	var_oldorigin,

	/*
	* Description:      -
	* Member type:      class Vector
	* Get params:       get_entvar(index, EntVars:var, Float:output[3]);
	* Set params:       set_entvar(index, EntVars:var, Float:dest[3]);
	*/
	var_velocity,

	/*
	* Description:      -
	* Member type:      class Vector
	* Get params:       get_entvar(index, EntVars:var, Float:output[3]);
	* Set params:       set_entvar(index, EntVars:var, Float:dest[3]);
	*/
	var_basevelocity,

	/*
	* Description:      Base velocity that was passed in to server physics so client can predict conveyors correctly. Server zeroes it, so we need to it store here, too.
	* Member type:      class Vector
	* Get params:       get_entvar(index, EntVars:var, Float:output[3]);
	* Set params:       set_entvar(index, EntVars:var, Float:dest[3]);
	*/
	var_clbasevelocity,

	/*
	* Description:      -
	* Member type:      class Vector
	* Get params:       get_entvar(index, EntVars:var, Float:output[3]);
	* Set params:       set_entvar(index, EntVars:var, Float:dest[3]);
	*/
	var_movedir,

	/*
	* Description:      Angles model.
	* Member type:      class Vector
	* Get params:       get_entvar(index, EntVars:var, Float:output[3]);
	* Set params:       set_entvar(index, EntVars:var, Float:dest[3]);
	*/
	var_angles,

	/*
	* Description:      Angle velocity (degrees per second).
	* Member type:      class Vector
	* Get params:       get_entvar(index, EntVars:var, Float:output[3]);
	* Set params:       set_entvar(index, EntVars:var, Float:dest[3]);
	*/
	var_avelocity,

	/*
	* Description:      Auto-decaying view angle adjustment.
	* Member type:      class Vector
	* Get params:       get_entvar(index, EntVars:var, Float:output[3]);
	* Set params:       set_entvar(index, EntVars:var, Float:dest[3]);
	*/
	var_punchangle,

	/*
	* Description:      View angle (player only).
	* Member type:      class Vector
	* Get params:       get_entvar(index, EntVars:var, Float:output[3]);
	* Set params:       set_entvar(index, EntVars:var, Float:dest[3]);
	*/
	var_v_angle,

	/*
	* Description:      -
	* Member type:      class Vector
	* Get params:       get_entvar(index, EntVars:var, Float:output[3]);
	* Set params:       set_entvar(index, EntVars:var, Float:dest[3]);
	*/
	var_endpos,

	/*
	* Description:      -
	* Member type:      class Vector
	* Get params:       get_entvar(index, EntVars:var, Float:output[3]);
	* Set params:       set_entvar(index, EntVars:var, Float:dest[3]);
	*/
	var_startpos,

	/*
	* Description:      -
	* Member type:      float
	* Get params:       Float:get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, Float:value);
	*/
	var_impacttime,

	/*
	* Description:      -
	* Member type:      float
	* Get params:       Float:get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, Float:value);
	*/
	var_starttime,

	/*
	* Description:      0:nothing, 1:force view angles, 2:add avelocity
	* Member type:      int
	* Get params:       get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, value);
	*/
	var_fixangle,

	/*
	* Description:      -
	* Member type:      float
	* Get params:       Float:get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, Float:value);
	*/
	var_idealpitch,

	/*
	* Description:      -
	* Member type:      float
	* Get params:       Float:get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, Float:value);
	*/
	var_pitch_speed,

	/*
	* Description:      -
	* Member type:      float
	* Get params:       Float:get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, Float:value);
	*/
	var_ideal_yaw,

	/*
	* Description:      -
	* Member type:      float
	* Get params:       Float:get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, Float:value);
	*/
	var_yaw_speed,

	/*
	* Description:      -
	* Member type:      int
	* Get params:       get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, value);
	*/
	var_modelindex,

	/*
	* Description:      -
	* Member type:      string_t
	* Get params:       get_entvar(index, EntVars:var, dest[], const lenght);
	* Set params:       set_entvar(index, EntVars:var, const source[]);
	*/
	var_model,

	/*
	* Description:      Player's viewmodel.
	* Member type:      string_t
	* Get params:       get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, const source[]);
	*/
	var_viewmodel,

	/*
	* Description:      The model that other players see.
	* Member type:      string_t
	* Get params:       get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, const source[]);
	*/
	var_weaponmodel,

	/*
	* Description:      BB min translated to world coord.
	* Member type:      class Vector
	* Get params:       get_entvar(index, EntVars:var, Float:output[3]);
	* Set params:       set_entvar(index, EntVars:var, Float:dest[3]);
	*/
	var_absmin,

	/*
	* Description:      BB max translated to world coord.
	* Member type:      class Vector
	* Get params:       get_entvar(index, EntVars:var, Float:output[3]);
	* Set params:       set_entvar(index, EntVars:var, Float:dest[3]);
	*/
	var_absmax,

	/*
	* Description:      Local BB min.
	* Member type:      class Vector
	* Get params:       get_entvar(index, EntVars:var, Float:output[3]);
	* Set params:       set_entvar(index, EntVars:var, Float:dest[3]);
	*/
	var_mins,

	/*
	* Description:      Local BB max.
	* Member type:      class Vector
	* Get params:       get_entvar(index, EntVars:var, Float:output[3]);
	* Set params:       set_entvar(index, EntVars:var, Float:dest[3]);
	*/
	var_maxs,

	/*
	* Description:      maxs - mins
	* Member type:      class Vector
	* Get params:       get_entvar(index, EntVars:var, Float:output[3]);
	* Set params:       set_entvar(index, EntVars:var, Float:dest[3]);
	*/
	var_size,

	/*
	* Description:      -
	* Member type:      float
	* Get params:       Float:get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, Float:value);
	*/
	var_ltime,

	/*
	* Description:      -
	* Member type:      float
	* Get params:       Float:get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, Float:value);
	*/
	var_nextthink,

	/*
	* Description:      -
	* Member type:      int
	* Get params:       get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, value);
	*/
	var_movetype,

	/*
	* Description:      -
	* Member type:      int
	* Get params:       get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, value);
	*/
	var_solid,

	/*
	* Description:      Skin selection for studio models.
	* Member type:      int
	* Get params:       get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, value);
	*/
	var_skin,

	/*
	* Description:      Sub-model selection for studio models.
	* Member type:      int
	* Get params:       get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, value);
	*/
	var_body,

	/*
	* Description:      -
	* Member type:      int
	* Get params:       get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, value);
	*/
	var_effects,

	/*
	* Description:      % of "normal" gravity.
	* Member type:      float
	* Get params:       Float:get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, Float:value);
	*/
	var_gravity,

	/*
	* Description:      Inverse elasticity of MOVETYPE_BOUNCE
	* Member type:      float
	* Get params:       Float:get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, Float:value);
	*/
	var_friction,

	/*
	* Description:      -
	* Member type:      int
	* Get params:       get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, value);
	*/
	var_light_level,

	/*
	* Description:      Animation sequence.
	* Member type:      int
	* Get params:       get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, value);
	*/
	var_sequence,

	/*
	* Description:      Movement animation sequence for player (0 for none)
	* Member type:      int
	* Get params:       get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, value);
	*/
	var_gaitsequence,

	/*
	* Description:      % playback position in animation sequences (0..255)
	* Member type:      float
	* Get params:       Float:get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, Float:value);
	*/
	var_frame,

	/*
	* Description:      World time when frame was set.
	* Member type:      float
	* Get params:       Float:get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, Float:value);
	*/
	var_animtime,

	/*
	* Description:      Animation playback rate (-8x to 8x)
	* Member type:      float
	* Get params:       Float:get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, Float:value);
	*/
	var_framerate,

	/*
	* Description:      Bone controller setting (0..255)
	* Member type:      byte [4]
	* Get params:       get_entvar(index, EntVars:var, element);
	* Set params:       set_entvar(index, EntVars:var, value, element);
	*/
	var_controller,

	/*
	* Description:      Blending amount between sub-sequences (0..255)
	* Member type:      byte [2]
	* Get params:       get_entvar(index, EntVars:var, element);
	* Set params:       set_entvar(index, EntVars:var, value, element);
	*/
	var_blending,

	/*
	* Description:      Sprite rendering scale (0..255)
	* Member type:      float
	* Get params:       Float:get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, Float:value);
	*/
	var_scale,

	/*
	* Description:      -
	* Member type:      int
	* Get params:       get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, value);
	*/
	var_rendermode,

	/*
	* Description:      -
	* Member type:      float
	* Get params:       Float:get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, Float:value);
	*/
	var_renderamt,

	/*
	* Description:      -
	* Member type:      class Vector
	* Get params:       get_entvar(index, EntVars:var, Float:output[3]);
	* Set params:       set_entvar(index, EntVars:var, Float:dest[3]);
	*/
	var_rendercolor,

	/*
	* Description:      -
	* Member type:      int
	* Get params:       get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, value);
	*/
	var_renderfx,

	/*
	* Description:      -
	* Member type:      float
	* Get params:       Float:get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, Float:value);
	*/
	var_health,

	/*
	* Description:      -
	* Member type:      float
	* Get params:       Float:get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, Float:value);
	*/
	var_frags,

	/*
	* Description:      bit mask for available weapons
	* Member type:      int
	* Get params:       get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, value);
	*/
	var_weapons,

	/*
	* Description:      -
	* Member type:      float
	* Get params:       Float:get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, Float:value);
	*/
	var_takedamage,

	/*
	* Description:      -
	* Member type:      int
	* Get params:       get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, value);
	*/
	var_deadflag,

	/*
	* Description:      eye position
	* Member type:      class Vector
	* Get params:       get_entvar(index, EntVars:var, Float:output[3]);
	* Set params:       set_entvar(index, EntVars:var, Float:dest[3]);
	*/
	var_view_ofs,

	/*
	* Description:      -
	* Member type:      int
	* Get params:       get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, value);
	*/
	var_button,

	/*
	* Description:      -
	* Member type:      int
	* Get params:       get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, value);
	*/
	var_impulse,

	/*
	* Description:      Entity pointer when linked into a linked list.
	* Member type:      struct edict_s *
	* Get params:       get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, value);
	*/
	var_chain,

	/*
	* Description:      -
	* Member type:      struct edict_s *
	* Get params:       get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, value);
	*/
	var_dmg_inflictor,

	/*
	* Description:      -
	* Member type:      struct edict_s *
	* Get params:       get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, value);
	*/
	var_enemy,

	/*
	* Description:      Entity pointer when MOVETYPE_FOLLOW
	* Member type:      struct edict_s *
	* Get params:       get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, value);
	*/
	var_aiment,

	/*
	* Description:      -
	* Member type:      struct edict_s *
	* Get params:       get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, value);
	*/
	var_owner,

	/*
	* Description:      -
	* Member type:      struct edict_s *
	* Get params:       get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, value);
	*/
	var_groundentity,

	/*
	* Description:      -
	* Member type:      int
	* Get params:       get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, value);
	*/
	var_spawnflags,

	/*
	* Description:      -
	* Member type:      int
	* Get params:       get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, value);
	*/
	var_flags,

	/*
	* Description:      lowbyte topcolor, highbyte bottomcolor
	* Member type:      int
	* Get params:       get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, value);
	*/
	var_colormap,

	/*
	* Description:      -
	* Member type:      int
	* Get params:       get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, value);
	*/
	var_team,

	/*
	* Description:      -
	* Member type:      float
	* Get params:       Float:get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, Float:value);
	*/
	var_max_health,

	/*
	* Description:      -
	* Member type:      float
	* Get params:       Float:get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, Float:value);
	*/
	var_teleport_time,

	/*
	* Description:      -
	* Member type:      float
	* Get params:       Float:get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, Float:value);
	*/
	var_armortype,

	/*
	* Description:      -
	* Member type:      float
	* Get params:       Float:get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, Float:value);
	*/
	var_armorvalue,

	/*
	* Description:      -
	* Member type:      int
	* Get params:       get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, value);
	*/
	var_waterlevel,

	/*
	* Description:      -
	* Member type:      int
	* Get params:       get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, value);
	*/
	var_watertype,

	/*
	* Description:      -
	* Member type:      string_t
	* Get params:       get_entvar(index, EntVars:var, dest[], const lenght);
	* Set params:       set_entvar(index, EntVars:var, const source[]);
	*/
	var_target,

	/*
	* Description:      -
	* Member type:      string_t
	* Get params:       get_entvar(index, EntVars:var, dest[], const lenght);
	* Set params:       set_entvar(index, EntVars:var, const source[]);
	*/
	var_targetname,

	/*
	* Description:      -
	* Member type:      string_t
	* Get params:       get_entvar(index, EntVars:var, dest[], const lenght);
	* Set params:       set_entvar(index, EntVars:var, const source[]);
	*/
	var_netname,

	/*
	* Description:      -
	* Member type:      string_t
	* Get params:       get_entvar(index, EntVars:var, dest[], const lenght);
	* Set params:       set_entvar(index, EntVars:var, const source[]);
	*/
	var_message,

	/*
	* Description:      -
	* Member type:      float
	* Get params:       Float:get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, Float:value);
	*/
	var_dmg_take,

	/*
	* Description:      -
	* Member type:      float
	* Get params:       Float:get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, Float:value);
	*/
	var_dmg_save,

	/*
	* Description:      -
	* Member type:      float
	* Get params:       Float:get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, Float:value);
	*/
	var_dmg,

	/*
	* Description:      -
	* Member type:      float
	* Get params:       Float:get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, Float:value);
	*/
	var_dmgtime,

	/*
	* Description:      -
	* Member type:      string_t
	* Get params:       get_entvar(index, EntVars:var, dest[], const lenght);
	* Set params:       set_entvar(index, EntVars:var, const source[]);
	*/
	var_noise,

	/*
	* Description:      -
	* Member type:      string_t
	* Get params:       get_entvar(index, EntVars:var, dest[], const lenght);
	* Set params:       set_entvar(index, EntVars:var, const source[]);
	*/
	var_noise1,

	/*
	* Description:      -
	* Member type:      string_t
	* Get params:       get_entvar(index, EntVars:var, dest[], const lenght);
	* Set params:       set_entvar(index, EntVars:var, const source[]);
	*/
	var_noise2,

	/*
	* Description:      -
	* Member type:      string_t
	* Get params:       get_entvar(index, EntVars:var, dest[], const lenght);
	* Set params:       set_entvar(index, EntVars:var, const source[]);
	*/
	var_noise3,

	/*
	* Description:      -
	* Member type:      float
	* Get params:       Float:get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, Float:value);
	*/
	var_speed,

	/*
	* Description:      -
	* Member type:      float
	* Get params:       Float:get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, Float:value);
	*/
	var_air_finished,

	/*
	* Description:      -
	* Member type:      float
	* Get params:       Float:get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, Float:value);
	*/
	var_pain_finished,

	/*
	* Description:      -
	* Member type:      float
	* Get params:       Float:get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, Float:value);
	*/
	var_radsuit_finished,

	/*
	* Description:      -
	* Member type:      struct edict_s *
	* Get params:       get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, value);
	*/
	var_pContainingEntity,

	/*
	* Description:      -
	* Member type:      int
	* Get params:       get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, value);
	*/
	var_playerclass,

	/*
	* Description:      -
	* Member type:      float
	* Get params:       Float:get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, Float:value);
	*/
	var_maxspeed,

	/*
	* Description:      -
	* Member type:      float
	* Get params:       Float:get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, Float:value);
	*/
	var_fov,

	/*
	* Description:      -
	* Member type:      int
	* Get params:       get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, value);
	*/
	var_weaponanim,

	/*
	* Description:      -
	* Member type:      int
	* Get params:       get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, value);
	*/
	var_pushmsec,

	/*
	* Description:      -
	* Member type:      int
	* Get params:       get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, value);
	*/
	var_bInDuck,

	/*
	* Description:      -
	* Member type:      int
	* Get params:       get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, value);
	*/
	var_flTimeStepSound,

	/*
	* Description:      -
	* Member type:      int
	* Get params:       get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, value);
	*/
	var_flSwimTime,

	/*
	* Description:      -
	* Member type:      int
	* Get params:       get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, value);
	*/
	var_flDuckTime,

	/*
	* Description:      -
	* Member type:      int
	* Get params:       get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, value);
	*/
	var_iStepLeft,

	/*
	* Description:      -
	* Member type:      float
	* Get params:       Float:get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, Float:value);
	*/
	var_flFallVelocity,

	/*
	* Description:      -
	* Member type:      int
	* Get params:       get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, value);
	*/
	var_gamestate,

	/*
	* Description:      -
	* Member type:      int
	* Get params:       get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, value);
	*/
	var_oldbuttons,

	/*
	* Description:      -
	* Member type:      int
	* Get params:       get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, value);
	*/
	var_groupinfo,

	/*
	* Description:      -
	* Member type:      int
	* Get params:       get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, value);
	*/
	var_iuser1,

	/*
	* Description:      -
	* Member type:      int
	* Get params:       get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, value);
	*/
	var_iuser2,

	/*
	* Description:      -
	* Member type:      int
	* Get params:       get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, value);
	*/
	var_iuser3,

	/*
	* Description:      -
	* Member type:      int
	* Get params:       get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, value);
	*/
	var_iuser4,

	/*
	* Description:      -
	* Member type:      float
	* Get params:       Float:get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, Float:value);
	*/
	var_fuser1,

	/*
	* Description:      -
	* Member type:      float
	* Get params:       Float:get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, Float:value);
	*/
	var_fuser2,

	/*
	* Description:      -
	* Member type:      float
	* Get params:       Float:get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, Float:value);
	*/
	var_fuser3,

	/*
	* Description:      -
	* Member type:      float
	* Get params:       Float:get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, Float:value);
	*/
	var_fuser4,

	/*
	* Description:      -
	* Member type:      class Vector
	* Get params:       get_entvar(index, EntVars:var, Float:output[3]);
	* Set params:       set_entvar(index, EntVars:var, Float:dest[3]);
	*/
	var_vuser1,

	/*
	* Description:      -
	* Member type:      class Vector
	* Get params:       get_entvar(index, EntVars:var, Float:output[3]);
	* Set params:       set_entvar(index, EntVars:var, Float:dest[3]);
	*/
	var_vuser2,

	/*
	* Description:      -
	* Member type:      class Vector
	* Get params:       get_entvar(index, EntVars:var, Float:output[3]);
	* Set params:       set_entvar(index, EntVars:var, Float:dest[3]);
	*/
	var_vuser3,

	/*
	* Description:      -
	* Member type:      class Vector
	* Get params:       get_entvar(index, EntVars:var, Float:output[3]);
	* Set params:       set_entvar(index, EntVars:var, Float:dest[3]);
	*/
	var_vuser4,

	/*
	* Description:      -
	* Member type:      struct edict_s *
	* Get params:       get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, value);
	*/
	var_euser1,

	/*
	* Description:      -
	* Member type:      struct edict_s *
	* Get params:       get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, value);
	*/
	var_euser2,

	/*
	* Description:      -
	* Member type:      struct edict_s *
	* Get params:       get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, value);
	*/
	var_euser3,

	/*
	* Description:      -
	* Member type:      struct edict_s *
	* Get params:       get_entvar(index, EntVars:var);
	* Set params:       set_entvar(index, EntVars:var, value);
	*/
	var_euser4
};

/**
* enum UCmd
*/
enum UCmd
{
	/*
	* Description:      -
	* Member type:      short
	* Get params:       get_ucmd(const ucmd, UCmd:var);
	* Set params:       set_ucmd(const ucmd, UCmd:var, value);
	*/
	ucmd_lerp_msec = BEGIN_MEMBER_REGION(usercmd),

	/*
	* Description:      -
	* Member type:      byte
	* Get params:       get_ucmd(const ucmd, UCmd:var);
	* Set params:       set_ucmd(const ucmd, UCmd:var, value);
	*/
	ucmd_msec,

	/*
	* Description:      -
	* Member type:      vec3_t
	* Get params:       get_ucmd(const ucmd, UCmd:var, Float:output[3]);
	* Set params:       set_ucmd(const ucmd, UCmd:var, Float:dest[3]);
	*/
	ucmd_viewangles,

	/*
	* Description:      -
	* Member type:      float
	* Get params:       Float:get_ucmd(const ucmd, UCmd:var);
	* Set params:       set_ucmd(const ucmd, UCmd:var, Float:value);
	*/
	ucmd_forwardmove,

	/*
	* Description:      -
	* Member type:      float
	* Get params:       Float:get_ucmd(const ucmd, UCmd:var);
	* Set params:       set_ucmd(const ucmd, UCmd:var, Float:value);
	*/
	ucmd_sidemove,

	/*
	* Description:      -
	* Member type:      float
	* Get params:       Float:get_ucmd(const ucmd, UCmd:var);
	* Set params:       set_ucmd(const ucmd, UCmd:var, Float:value);
	*/
	ucmd_upmove,

	/*
	* Description:      -
	* Member type:      byte
	* Get params:       get_ucmd(const ucmd, UCmd:var);
	* Set params:       set_ucmd(const ucmd, UCmd:var, value);
	*/
	ucmd_lightlevel,

	/*
	* Description:      -
	* Member type:      unsigned short
	* Get params:       get_ucmd(const ucmd, UCmd:var);
	* Set params:       set_ucmd(const ucmd, UCmd:var, value);
	*/
	ucmd_buttons,

	/*
	* Description:      -
	* Member type:      byte
	* Get params:       get_ucmd(const ucmd, UCmd:var);
	* Set params:       set_ucmd(const ucmd, UCmd:var, value);
	*/
	ucmd_impulse,

	/*
	* Description:      -
	* Member type:      byte
	* Get params:       get_ucmd(const ucmd, UCmd:var);
	* Set params:       set_ucmd(const ucmd, UCmd:var, value);
	*/
	ucmd_weaponselect,

	/*
	* Description:      -
	* Member type:      int
	* Get params:       get_ucmd(const ucmd, UCmd:var);
	* Set params:       set_ucmd(const ucmd, UCmd:var, value);
	*/
	ucmd_impact_index,

	/*
	* Description:      -
	* Member type:      vec3_t
	* Get params:       get_ucmd(const ucmd, UCmd:var, Float:output[3]);
	* Set params:       set_ucmd(const ucmd, UCmd:var, Float:dest[3]);
	*/
	ucmd_impact_position
};

enum NetAdrType
{
	NA_NULL = 0,
	NA_LOOPBACK,
	NA_BROADCAST,
	NA_IP,
	NA_IPX,				// Deprecated: GoldSrc
	NA_BROADCAST_IPX,	// Deprecated: GoldSrc
};

enum NetAdrVars
{
	/*
	* Description:      -
	* Member type:      NetAdrType
	* Get params:       get_netadr(const NetAdr:adr, const NetAdrVars:var);
	* Set params:       set_netadr(const NetAdr:adr, const NetAdrVars:var, const value);
	*/
	netadr_type = BEGIN_MEMBER_REGION(netadr),

	/*
	* Description:      -
	* Member type:      unsigned char [4]
	* Get params:       get_netadr(const NetAdr:adr, const NetAdrVars:var, dest[], const lenght); // Also returns a iplong value
	* Set params:       set_netadr(const NetAdr:adr, const NetAdrVars:var, const iplong_value); // Only iplong can to set
	*/
	netadr_ip,

	/*
	* Description:      -
	* Member type:      unsigned short
	* Get params:       get_netadr(const NetAdr:adr, const NetAdrVars:var);
	* Set params:       set_netadr(const NetAdr:adr, const NetAdrVars:var, const value);
	*/
	netadr_port
};

/**
* enum NetSrc
*/
enum NetSrc
{
	NS_CLIENT,
	NS_SERVER,
	NS_MULTICAST	// xxxMO
};

/**
* enum NetChan
*/
enum NetChan
{
	/*
	* Description:      NS_SERVER or NS_CLIENT, depending on channel
	* Member type:      int
	* Get params:       NetSrc:get_netchan(const index, NetChan:var);
	* Set params:       set_netchan(const index, NetChan:var, NetSrc:value);
	*/
	net_sock = BEGIN_MEMBER_REGION(netchan),

	/*
	* Description:      Address this channel is talking to
	* Member type:      NetAdr
	* Get params:       NetAdr:get_netchan(const index, NetChan:var);
	* Set params:       set_netchan(const index, NetChan:var, NetAdr:value);
	*/
	net_remote_address,

	/*
	* Description:      -
	* Member type:      int
	* Get params:       get_netchan(const index, NetChan:var);
	* Set params:       set_netchan(const index, NetChan:var, value);
	*/
	net_player_slot,

	/*
	* Description:      For timeouts. Time last message was received
	* Member type:      float
	* Get params:       Float:get_netchan(const index, NetChan:var);
	* Set params:       set_netchan(const index, NetChan:var, Float:value);
	*/
	net_last_received,

	/*
	* Description:      Time when channel was connected
	* Member type:      float
	* Get params:       Float:get_netchan(const index, NetChan:var);
	* Set params:       set_netchan(const index, NetChan:var, Float:value);
	*/
	net_connect_time,

	/*
	* Description:      Bandwidth choke. (Bytes per second)
	* Member type:      float
	* Get params:       Float:get_netchan(const index, NetChan:var);
	* Set params:       set_netchan(const index, NetChan:var, Float:value);
	*/
	net_rate,

	/*
	* Description:      If rh_get_realtime() > cleartime, free to send next packet.
	* Member type:      float
	* Get params:       Float:get_netchan(const index, NetChan:var);
	* Set params:       set_netchan(const index, NetChan:var, Float:value);
	*/
	net_cleartime,

	/*
	* Description:      A sequence number that increases with each incoming bunch of packets.
	* Member type:      int
	* Get params:       get_netchan(const index, NetChan:var);
	* Set params:       set_netchan(const index, NetChan:var, value);
	*/
	net_incoming_sequence,

	/*
	* Description:      The number of last outgoing message that has been ack'd.
	* Member type:      int
	* Get params:       get_netchan(const index, NetChan:var);
	* Set params:       set_netchan(const index, NetChan:var, value);
	*/
	net_incoming_acknowledged,

	/*
	* Description:      Single bit indicating the state of acknowledgment for the last reliable message.
	* Member type:      int
	* Get params:       get_netchan(const index, NetChan:var);
	* Set params:       set_netchan(const index, NetChan:var, value);
	*/
	net_incoming_reliable_acknowledged,

	/*
	* Description:      Single bit, maintained local that toggles between 0 and 1 to track the sequence of reliable messages received
	* Member type:      int
	* Get params:       get_netchan(const index, NetChan:var);
	* Set params:       set_netchan(const index, NetChan:var, value);
	*/
	net_incoming_reliable_sequence,

	/*
	* Description:      Message we are sending to remote
	* Member type:      int
	* Get params:       get_netchan(const index, NetChan:var);
	* Set params:       set_netchan(const index, NetChan:var, value);
	*/
	net_outgoing_sequence,

	/*
	* Description:      Whether the message contains reliable payload, single bit
	* Member type:      int
	* Get params:       get_netchan(const index, NetChan:var);
	* Set params:       set_netchan(const index, NetChan:var, value);
	*/
	net_reliable_sequence,

	/*
	* Description:      Outgoing sequence number of last send that had reliable data
	* Member type:      int
	* Get params:       get_netchan(const index, NetChan:var);
	* Set params:       set_netchan(const index, NetChan:var, value);
	*/
	net_last_reliable_sequence
};

/**
* Message argument types used with GetMessageArgType()
*/
enum MsgArgType
{
	ArgByte,
	ArgChar,
	ArgShort,
	ArgLong,
	ArgAngle,
	ArgCoord,
	ArgString,
	ArgEntity,
};

/**
* Message data types used with SetMessageData()/GetMessageData()
*                              HasModifiedMessageData()/ResetModifiedMessageData()
*/
enum MsgDataType
{
	/*
	* Description:      Any part of the message
	*/
	MsgAny,

	/*
	* Description:      The destination of the message
	* Arg type:         integer
	* Get params:       new dest = GetMessageData(MsgDest);
	* Set params:       SetMessageData(MsgDest, MSG_ALL);
	*/
	MsgDest,

	/*
	* Description:      The index of the message
	* Arg type:         integer
	* Get params:       new msg_id = GetMessageData(MsgMsgId);
	* Set params:       SetMessageData(MsgMsgId, const msg_id);
	*/
	MsgMsgId,

	/*
	* Description:      The origin of the message
	* Arg type:         float [3]
	* Get params:       GetMessageData(MsgOrigin, Float:dstVector[3]);
	* Set params:       SetMessageData(MsgOrigin, Float:srcVector[3]);
	*/
	MsgOrigin,

	/*
	* Description:      The index of the recipient client
	* Arg type:         integer
	* Get params:       new targetId = GetMessageData(MsgTargetId);
	* Set params:       SetMessageData(MsgTargetId, const targetId); (acceptable indexes 0-32, 0 index also as -1 means NULLENT)
	*/
	MsgTargetId,

	/*
	* Description:      The arguments of the message
	*
	* Arg type:         string (ArgString)
	* Get params:       bool:GetMessageData(MsgArg, const argnumber, value[], const maxlen);
	* Set params:       bool:SetMessageData(MsgArg, const argnumber, const value[]);
	*
	* Arg type:         float (ArgAngle, ArgCoord)
	* Get params:       Float:GetMessageData(MsgArg, const argnumber, &Float:value = 0.0);
	* Set params:       bool:SetMessageData(MsgArg, const argnumber, const Float:value);
	*
	* Arg type:         integer (ArgByte, ArgChar, ArgShort, ArgLong, ArgEntity)
	* Get params:       GetMessageData(MsgArg, const argnumber);
	* Set params:       bool:SetMessageData(MsgArg, const argnumber, const value);
	*/
	MsgArg
};

/**
* Blocking behavior types for messages
* Flags for natives SetMessageBlock()/GetMessageBlock()
*/
enum MsgBlockType
{
	MSG_BLOCK_NOT,  // Not a block
	MSG_BLOCK_ONCE, // Block once
	MSG_BLOCK_SET   // Set block
};
