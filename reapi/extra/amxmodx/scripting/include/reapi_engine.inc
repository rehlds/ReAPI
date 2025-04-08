#if defined _reapi_engine_included
	#endinput
#endif

#define _reapi_engine_included

#include <reapi_engine_const>

/*
* Sets entvars data for an entity.
* Use the var_* EntVars enum
*/
native set_entvar(const index, const EntVars:var, any:...);

/*
* Returns entvar data from an entity.
* Use the var_* EntVars enum
*/
native any:get_entvar(const index, const EntVars:var, any:...);

/*
* Sets usercmd data.
* Use the ucmd_* UCmd enum
*/
native set_ucmd(const ucmd, const UCmd:var, any:...);

/*
* Returns usercmd data from an entity.
* Use the ucmd_* UCmd enum
*/
native any:get_ucmd(const ucmd, const UCmd:var, any:...);

/*
* Sets netchan data.
* Use the net_* NetChan enum
*/
native set_netchan(const index, const NetChan:var, any:...);

/*
* Returns metchan data from an client.
* Use the net_* NetChan enum
*/
native any:get_netchan(const index, const NetChan:var, any:...);

/*
* Sets a NetAdr var.
*
* @param var        The specified mvar, look at the enum NetAdrVars
*
* @return           1 on success.
*/
native set_netadr(const adr, const NetAdrVars:var, any:...);

/*
* Returns a NetAdr var
*
* @param var        The specified mvar, look at the enum NetAdrVars
*
* @return           If an integer or boolean or one byte, array or everything else is passed via the 3rd argument and more, look at the argument list for the specified mvar
*/
native any:get_netadr(const adr, const NetAdrVars:var, any:...);

/*
* Gets value for key in buffer
*
* @param pbuffer    Pointer to buffer
* @param key        Key string
* @param value      Buffer to copy value to
* @param maxlen     Maximum size of the buffer
*
* @return           Number of cells written to buffer
* @error            If invalid buffer handler provided, an error will be thrown.
*/
native get_key_value(const pbuffer, const key[], const value[], const maxlen);

/*
* Sets value for key in buffer
*
* @param pbuffer    Pointer to buffer
* @param key        Key string
* @param value      Value to set
*
* @noreturn
* @error            If invalid buffer handler provided, an error will be thrown.
*/
native set_key_value(const pbuffer, const key[], const value[]);

/*
* Gets an AMXX string buffer from a infobuffer pointer
*
* @param buffer     Info string pointer
* @param value      String to copy value to
* @param maxlen     Maximum size of the output buffer
*
* @return           Returns a string buffer on infobuffer pointer
*/
native get_key_value_buffer(const pbuffer, const output[], const maxlen);

/*
* Sets value string to entire buffer
*
* @param buffer     Pointer to buffer
* @param value      Value to set
* @param maxlen     Maximum size of the value buffer to set, -1 means copy all characters
*
* @return           1 on success, 0 otherwise
*/
native set_key_value_buffer(const pbuffer, const value[], const maxlen = -1);

/*
* Gets the position of the bone
*
* @param entity     Entity index
* @param bone       Number of the bone
* @param vecOrigin  Array to store origin in
* @param vecAngles  Array to store angles in
*
* @return           1 on success, 0 otherwise
* @error            If the index is not within the range of 1 to maxEntities or
*                   the entity is not valid, an error will be thrown.
*/
native GetBonePosition(const entity, const bone, Float:vecOrigin[3], Float:vecAngles[3] = {0.0, 0.0, 0.0});

/*
* Gets the position of the attachment
*
* @param entity     Entity index
* @param attachment Number of the attachment
* @param vecOrigin  Array to store origin in
* @param vecAngles  Array to store angles in
*
* @return           1 on success, 0 otherwise
* @error            If the index is not within the range of 1 to maxEntities or
*                   the entity is not valid, an error will be thrown.
*/
native GetAttachment(const entity, const attachment, Float:vecOrigin[3], Float:vecAngles[3] = {0.0, 0.0, 0.0});

/*
* Sets body group value based on entity's model group
*
* @param entity     Entity index
* @param group      Number of entity's model group index
* @param value      Value to assign
*
* @return           1 on success, 0 otherwise
* @error            If the index is not within the range of 1 to maxEntities or
*                   the entity is not valid, an error will be thrown.
*
*/
native SetBodygroup(const entity, const group, const value);

/*
* Gets body group value based on entity's model group
*
* @param entity     Entity index
* @param group      Number of entity's model group index
*
* @return           Body group value
* @error            If the index is not within the range of 1 to maxEntities or
*                   the entity is not valid, an error will be thrown.
*
*/
native GetBodygroup(const entity, const group);

/*
* Gets sequence information based on entity's model current sequence index
*
* @param entity             Entity index
* @param piFlags            Sequence flags (1 = sequence loops)
* @param pflFrameRate       Sequence framerate
* @param pflGroundSpeed     Sequence ground speed
*
* @return                   True on success, false otherwise
* @error                    If the index is not within the range of 1 to maxEntities or
*                           the entity is not valid, an error will be thrown.
*
*/
native bool:GetSequenceInfo(const entity, &piFlags, &Float:pflFrameRate, &Float:pflGroundSpeed);

/*
* Test visibility of an entity from a given origin using either PVS or PAS
*
* @param entity     Entity index
* @param origin     Vector representing the origin from which visibility is checked
* @param type       Type of visibility check: VisibilityInPVS (Potentially Visible Set) or VisibilityInPAS (Potentially Audible Set)
*
* @return           0 - Not visible
*                   1 - Visible, passed by a leafnum
*                   2 - Visible, passed by a headnode
*
* @remarks          This function checks the visibility of the specified entity from the given origin, using either
*                   the Potentially Visible Set (PVS) or the Potentially Audible Set (PAS) depending on the provided type
*/
native CheckVisibilityInOrigin(const ent, Float:origin[3], CheckVisibilityType:type = VisibilityInPVS);

/*
* Sets the name of the map.
*
* @param mapname    New map name.
*
* @noreturn
*/
native rh_set_mapname(const mapname[]);

/*
* Gets the name of the map.
*
* @param output     Buffer to copy map name to
* @param len        Maximum buffer size
* @param type       MNT_SET will return the name of the current map
*                   MNT_TRUE will return the original map name independant of the name set with via rh_set_mapname
*
* @noreturn
*/
native rh_get_mapname(output[], len, MapNameType:type = MNT_SET);

/*
* Reverts back the original map name.
*
* @noreturn
*/
native rh_reset_mapname();

/*
* Emits a sound from an entity from the engine.
*
* @param entity     Entity index or use 0 to emit from worldspawn at the specified position
* @param recipient  Recipient index or use 0 to make all clients hear it
* @param channel    Channel to emit from
* @param sample     Sound file to emit
* @param vol        Volume in percents
* @param attn       Sound attenuation
* @param flags      Emit flags
* @param pitch      Sound pitch
* @param emitFlags  Additional Emit2 flags, look at the defines like SND_EMIT2_*
* @param origin     Specify origin and only on "param" entity worldspawn that is 0
*
* @return           true if the emission was successfull, false otherwise
*/
native bool:rh_emit_sound2(const entity, const recipient, const channel, const sample[], Float:vol = VOL_NORM, Float:attn = ATTN_NORM, const flags = 0, const pitch = PITCH_NORM, emitFlags = 0, const Float:origin[3] = {0.0,0.0,0.0});

/*
* Forces an userinfo update
*
* @param index      Client index
*
* @noreturn
*/
native rh_update_user_info(const index);

/*
* Kicks a client from server with message
*
* @param index      Client index
* @param message    Message that will be sent to client when it is deleted from server
*
* @noreturn
*
*/
native rh_drop_client(const index, const message[] = "");

/*
* -
*
* @param output     Buffer to copy the ip address
* @param len        Maximum buffer size
*
* @noreturn
*
*/
native rh_get_net_from(output[], len);

/*
* Returns client's netchan playing time in seconds.
*
* @param index      Client index
*
* @return           Netchan connection time in seconds or 0 if client index is invalid or client is not connected
*/
native rh_get_client_connect_time(const index);

/*
* Checks if a specific entity is present in the host's outgoing entity table for a given frame,
* indicating it has passed the visibility check (AddToFullPack) and is ready for client transmission.
*
* @param host       Host index for whom we are checking the entity. (Host cannot be a fake client)
* @param entity     Entity index to find in the table of entities for the given frame.
* @param frame      Frame index where to look. Default is -1, which checks the previous frame.
* @note             To check in the current frame, this native should be called at the end of the server frame.
*
* @return           Returns true if the entity is present in the host's outgoing entity table and
*                   ready to be sent to all clients in the given frame, otherwise false.
*/
native bool:rh_is_entity_fullpacked(const host, const entity, const frame = -1);

/*
* Get real game time throughout the entire server lifecycle.
*
* @return           Real game time
*/
native Float:rh_get_realtime();

/*
* Checks if server paused
*
* @return           Returns true if paused, otherwise false.
*
*/
native bool:rh_is_server_paused();

/*
* Set server pause state
*
* @param status    pause state
*
* @noreturn
*
*/
native rh_set_server_pause(const bool:status);

enum MessageHook
{
	INVALID_MESSAGEHOOK = 0
};

/**
* Registers a callback function to be called when a game message with the specified ID is received.
*
* @param msg_id     The ID of the message to register the callback for.
* @param callback   The name of the callback function.
* @param post       Whether the callback should be invoked before or after processing the message. (optional)
*
* @note The callback arguments have:
*   msg_id          - Message id
*   msg_dest        - Destination type (see MSG_* constants in messages_const.inc)
*   msg_entity      - Entity receiving the message
*
* @note             You can modify the message content using SetMessageData native before the original function is invoked.
*                   Also can reading the message content using GetMessageData native.
*
*                   In the callback function, use the return values from Hookchain return types, such as HC_CONTINUE, HC_SUPERCEDE, etc.
*                   to control the flow of message processing.
*
* @return           Returns a handle to the registered message hook.
*/
native MessageHook:RegisterMessage(const msg_id, const callback[], post = 0);

/**
* Unregisters a game message hook identified by the specified handle.
*
* @param handle     The handle of the message hook to unregister.
*
* @return           Returns true if the message hook is successfully unregistered, otherwise false.
*/
native bool:UnregisterMessage(const MessageHook:handle);

/**
* Enables a game message hook identified by the specified handle.
*
* @param handle     The handle of the message hook to enable.
*
* @return           Returns true if the message hook is successfully enabled, otherwise false.
*/
native bool:EnableHookMessage(const MessageHook:handle);

/**
* Disables a game message hook identified by the specified handle.
*
* @param handle     The handle of the message hook to disable.
*
* @return           Returns true if the message hook is successfully disabled, otherwise false.
*/
native bool:DisableHookMessage(const MessageHook:handle);

/**
* Sets the message data in the current game message.
*
* @param type       The type of the message data that can be changed
* @param ...        Additional args depending on the type of the message argument being retrieved (For more details, look at the enum MsgArgType)
*
* @return           Returns true if the message data is successfully set, otherwise false.
*/
native bool:SetMessageData(const MsgDataType:type, any:...);

/**
* Gets the message data value in the current game message
*
* @param type       The type of message data that can be get
* @param ...        Additional args depending on the type of the message argument being retrieved (For more details, look at the enum MsgArgType)
*
* @return           Returns value of argument in the current message
*/
native any:GetMessageData(const MsgDataType:type, any:...);

/**
* Gets the message data original value in the current game message.
*
* @param type       The type of message data that can be get
* @param ...        Additional args depending on the type of the message argument being retrieved (For more details, look at the enum MsgArgType)
*
* @return           Returns original value of argument in the current message
*/
native any:GetMessageOrigData(const MsgDataType:type, any:...);

/**
* Retrieves the type of the argument at the specified number in the current game message.
*
* @param number     The number of the argument to retrieve the type for.
*
* @return           Returns the type of the argument, look at the enum MsgArgType
*/
native MsgArgType:GetMessageArgType(const number);

/**
* Retrieves the number of argument in the current game message.
*
* @return           Returns the number of argument in the current game message.
*/
native GetMessageArgsNum();

/**
* Sets the block type for the specified message ID.
*
* @param msgid      The ID of the message to set the block type for.
* @param type       The type of block to set for the message, look at the enum MsgBlockType
*
* @return           Returns true if the block type is successfully set, otherwise false.
*/
native bool:SetMessageBlock(const msgid, MsgBlockType:type);

/**
* Retrieves the block type for the specified message ID.
*
* @param msgid      The ID of the message to retrieve the block type for.
*
* @return           Returns the block type of the specified message, look at the enum MsgBlockType
*/
native MsgBlockType:GetMessageBlock(const msgid);

/**
* Checks if the specified type of message data has been modified
*
* This native allows you to check if any part of the message data, such as its
* destination, type, origin, receiver, or any the specific argument of the message, has been modified
*
* @param type       The type of the data to check for modification
* This can be one of the following:
*                   - MsgAny:      Check if any part of the message has been modified
*                   - MsgDest:     Check if the destination has been modified
*                   - MsgIndex:    Check if the message ID has been modified
*                   - MsgOrigin:   Check if the origin has been modified
*                   - MsgTargetId: Check if the index of the recipient client has been modified
*                   - MsgArg:      Check if a specific argument of the message has been modified
*
* @param number     The number of the argument to check for modification (used only when type is MsgDataType:MsgArg)
*                   Default value is -1, which means the argument number is not applicable
*
* @return           Returns true if the specified data type has been modified, false otherwise
*/
native bool:IsMessageDataModified(MsgDataType:type = MsgAny, const number = -1);

/**
* Resets a specific type of message data to its original value
*
* @param type       The type of the data to check for modification
* This can be one of the following:
*                   - MsgAny:      Reset all modified message data to its original values
*                   - MsgDest:     Reset the destination to its original value
*                   - MsgIndex:    Reset the message ID to its original value
*                   - MsgOrigin:   Reset the origin to its original value
*                   - MsgTargetId: Reset the index of the recipient client to its original value
*                   - MsgArg:      Reset a specific argument of the message to its original value
*
* @param number     The number of the argument to reset (used only when type is MsgDataType:MsgArg)
*                   Default value is -1, which means all arguments will be reset.
*
* @return           Returns true if the modified data type was reset, otherwise false.
*/
native bool:ResetModifiedMessageData(MsgDataType:type = MsgAny, const number = -1);
