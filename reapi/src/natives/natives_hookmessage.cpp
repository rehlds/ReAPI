#include "precompiled.h"

#define CHECK_PARAMBOUNDS(x, y) if (unlikely(x > (size_t)y)) { AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: invalid message argument %d/max:%d", __FUNCTION__, x, y); }

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
*
* native MessageHook:RegisterMessage(const msg_id, const callback[], post = 0);
*/
cell AMX_NATIVE_CALL RegisterMessage(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_msgid, arg_handler, arg_post };

	CHECK_REQUIREMENTS(ReHLDS);

	int msg_id = params[arg_msgid];

	// svc_bad (0) is not allowed for hook
	if (msg_id <= 0 || msg_id >= MAX_USERMESSAGES)
		return INVALID_MESSAGEHOOK;

	char namebuf[256];
	const char *funcname = getAmxString(amx, params[arg_handler], namebuf);

	int funcid;
	if (unlikely(g_amxxapi.amx_FindPublic(amx, funcname, &funcid) != AMX_ERR_NONE))
	{
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: public function \"%s\" not found.", __FUNCTION__, funcname);
		return INVALID_MESSAGEHOOK;
	}

	int post = params[arg_post];
	return g_messageHookManager.addHook(amx, msg_id, funcname, post != 0);
}

/**
* Unregisters a game message hook identified by the specified handle.
*
* @param handle     The handle of the message hook to unregister.
*
* @return           Returns true if the message hook is successfully unregistered, otherwise false.
*
* native bool:UnregisterMessage(const MessageHook:handle);
*/
cell AMX_NATIVE_CALL UnregisterMessage(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_handle };

	CHECK_REQUIREMENTS(ReHLDS);

	if (!g_messageHookManager.removeHook(amx, params[arg_handle]))
	{
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: Failed to remove message hook with handle %d.", __FUNCTION__, params[arg_handle]);
		return FALSE;
	}

	return TRUE;
}

/**
* Enables a game message hook identified by the specified handle.
*
* @param handle     The handle of the message hook to enable.
*
* @return           Returns true if the message hook is successfully enabled, otherwise false.
*
* native bool:EnableHookMessage(const MessageHook:handle);
*/
cell AMX_NATIVE_CALL EnableHookMessage(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_handle };

	CHECK_REQUIREMENTS(ReHLDS);

	cell handle = params[arg_handle];
	auto hook = g_messageHookManager.getAmxxHook(handle);
	if (!hook)
	{
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: Message hook with handle %d not found.", __FUNCTION__, handle);
		return FALSE;
	}

	hook->SetState(FSTATE_ENABLED);
	return TRUE;
}

/**
* Disables a game message hook identified by the specified handle.
*
* @param handle     The handle of the message hook to disable.
*
* @return           Returns true if the message hook is successfully disabled, otherwise false.
*
* native bool:DisableHookMessage(const MessageHook:handle);
*/
cell AMX_NATIVE_CALL DisableHookMessage(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_handle };

	CHECK_REQUIREMENTS(ReHLDS);

	cell handle = params[arg_handle];
	auto hook = g_messageHookManager.getAmxxHook(handle);
	if (!hook)
	{
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: Message hook with handle %d not found.", __FUNCTION__, handle);
		return FALSE;
	}

	hook->SetState(FSTATE_STOPPED);
	return TRUE;
}

/**
* Sets the message data in the current game message.
*
* @param type       The type of the message data that can be changed
* @param ...        Additional args depending on the type of the message argument being retrieved (For more details, look at the enum MsgArgType)
*
* @return           Returns true if the message data is successfully set, otherwise false.
*
* native bool:SetMessageData(const MsgDataType:type, any:...);
*/
cell AMX_NATIVE_CALL SetMessageData(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_type, arg_value, arg_param_value };

	CHECK_REQUIREMENTS(ReHLDS);

	if (!g_activeMessageContext)
	{
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: trying to set argument without active hook.", __FUNCTION__);
		return FALSE;
	}

	IMessage::DataType type = static_cast<IMessage::DataType>(params[arg_type]);
	cell *value = getAmxAddr(amx, params[arg_value]);

	switch (type)
	{
	case IMessage::DataType::Dest:
		g_activeMessageContext->setDest(static_cast<IMessage::Dest>(*value));
		break;
	case IMessage::DataType::Index:
		g_activeMessageContext->setId(*value);
		break;
	case IMessage::DataType::Origin:
		g_activeMessageContext->setOrigin(*(Vector *)value);
		break;
	case IMessage::DataType::Edict:
		g_activeMessageContext->setEdict(edictByIndexAmx(*value, 0 /* most friendly to use 0 as invalid index for message */));
		break;
	case IMessage::DataType::Param:
	{
		// bool:SetMessageData(MsgDataType:type, const argnumber, const value[]);
		cell *value = getAmxAddr(amx, params[arg_param_value]);
		size_t number = *getAmxAddr(amx, params[arg_value]) - 1;

		CHECK_PARAMBOUNDS(number, g_activeMessageContext->getParamCount());

		switch (g_activeMessageContext->getParamType(number))
		{
		case IMessage::ParamType::String:
			char stringbuf[256];
			g_activeMessageContext->setParamString(number, getAmxString(amx, params[arg_param_value], stringbuf));
			break;
		case IMessage::ParamType::Angle:
		case IMessage::ParamType::Coord:
			g_activeMessageContext->setParamFloat(number, amx_CellToFloat(*value));
			break;
		case IMessage::ParamType::Byte:
		case IMessage::ParamType::Char:
		case IMessage::ParamType::Short:
		case IMessage::ParamType::Long:
		case IMessage::ParamType::Entity:
			g_activeMessageContext->setParamInt(number, *value);
			break;
		default:
			return FALSE;
		}

		break;
	}
	default:
		return FALSE;
	}

	return TRUE;
}

/**
* Gets the message data value in the current game message
*
* @param type       The type of message data that can be get
* @param ...        Additional args depending on the type of the message argument being retrieved (For more details, look at the enum MsgArgType)
*
* @return           Returns value of argument in the current message
*
* native any:GetMessageData(MsgDataType:type, any:...);
*/
cell AMX_NATIVE_CALL GetMessageData(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_type, arg_value, arg_param_value, arg_maxlen };

	CHECK_REQUIREMENTS(ReHLDS);

	if (!g_activeMessageContext)
	{
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: trying to get argument without active hook.", __FUNCTION__);
		return FALSE;
	}

	IMessage::DataType type = static_cast<IMessage::DataType>(params[arg_type]);
	switch (type)
	{
	case IMessage::DataType::Dest:
		return static_cast<cell>(g_activeMessageContext->getDest());
	case IMessage::DataType::Index:
		return g_activeMessageContext->getId();
	case IMessage::DataType::Origin:
		*(Vector *)getAmxAddr(amx, params[arg_value]) = g_activeMessageContext->getOrigin();
		return TRUE;
	case IMessage::DataType::Edict:
		return indexOfEdictAmx(g_activeMessageContext->getEdict(), 0 /* most friendly to use 0 as invalid index for message */);
	case IMessage::DataType::Param:
	{
		size_t number = *getAmxAddr(amx, params[arg_value]) - 1; // arg_value as number

		CHECK_PARAMBOUNDS(number, g_activeMessageContext->getParamCount());

		cell *dstAddr = getAmxAddr(amx, params[arg_param_value]);

		switch (g_activeMessageContext->getParamType(number))
		{
		case IMessage::ParamType::String:
		{
			if (PARAMS_COUNT != 4)
				return FALSE;

			const char *argString = g_activeMessageContext->getParamString(number);
			setAmxString(dstAddr, argString ? argString : "", params[arg_maxlen]);
			return TRUE;
		}
		case IMessage::ParamType::Angle:
		case IMessage::ParamType::Coord:
		{
			cell value = amx_FloatToCell(g_activeMessageContext->getParamFloat(number));
			if (PARAMS_COUNT > 2)
				*dstAddr = value;

			return value;
		}
		case IMessage::ParamType::Entity:
		case IMessage::ParamType::Byte:
		case IMessage::ParamType::Char:
		case IMessage::ParamType::Short:
		case IMessage::ParamType::Long:
			return g_activeMessageContext->getParamInt(number);
		default:
			break;
		}

		break;
	}
	default:
		break;
	}

	return FALSE;
}

/**
* Gets the message data original value in the current game message.
*
* @param type       The type of message data that can be get
* @param ...        Additional args depending on the type of the message argument being retrieved (For more details, look at the enum MsgArgType)
*
* @return           Returns original value of argument in the current message
*
* native any:GetMessageOrigData(MsgDataType:type, any:...);
*/
cell AMX_NATIVE_CALL GetMessageOrigData(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_type, arg_value, arg_param_value, arg_maxlen };

	CHECK_REQUIREMENTS(ReHLDS);

	if (!g_activeMessageContext)
	{
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: trying to get argument without active hook.", __FUNCTION__);
		return FALSE;
	}

	IMessage::DataType type = static_cast<IMessage::DataType>(params[arg_type]);
	switch (type)
	{
	case IMessage::DataType::Dest:
		return static_cast<cell>(g_activeMessageContext->getOriginalDest());
	case IMessage::DataType::Index:
		return g_activeMessageContext->getId();
	case IMessage::DataType::Origin:
		*(Vector *)getAmxAddr(amx, params[arg_value]) = g_activeMessageContext->getOriginalOrigin();
		return TRUE;
	case IMessage::DataType::Edict:
		return indexOfEdictAmx(g_activeMessageContext->getOriginalEdict());
	case IMessage::DataType::Param:
	{
		size_t number = *getAmxAddr(amx, params[arg_value]) - 1; // arg_value as number

		CHECK_PARAMBOUNDS(number, g_activeMessageContext->getParamCount());

		cell *dstAddr = getAmxAddr(amx, params[arg_param_value]);

		switch (g_activeMessageContext->getParamType(number))
		{
		case IMessage::ParamType::String:
		{
			if (PARAMS_COUNT != 4)
				return FALSE;

			const char *argString = g_activeMessageContext->getOriginalParamString(number);
			setAmxString(dstAddr, argString ? argString : "", params[arg_maxlen]);
			return TRUE;
		}
		case IMessage::ParamType::Angle:
		case IMessage::ParamType::Coord:
		{
			cell value = amx_FloatToCell(g_activeMessageContext->getOriginalParamFloat(number));
			if (PARAMS_COUNT > 2)
				*dstAddr = value;
			return value;
		}
		case IMessage::ParamType::Entity:
		case IMessage::ParamType::Byte:
		case IMessage::ParamType::Char:
		case IMessage::ParamType::Short:
		case IMessage::ParamType::Long:
			return g_activeMessageContext->getOriginalParamInt(number);
		default:
			break;
		}

		break;
	}
	default:
		break;
	}

	return FALSE;
}

/**
* Retrieves the type of the argument at the specified number in the current game message.
*
* @param number     The number of the argument to retrieve the type for.
*
* @return           Returns the type of the argument, look at the enum MsgArgType
*
* native MsgArgType:GetMessageArgType(const number);
*/
cell AMX_NATIVE_CALL GetMessageArgType(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_number };

	CHECK_REQUIREMENTS(ReHLDS);

	if (!g_activeMessageContext)
	{
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: trying to get argument without active hook.", __FUNCTION__);
		return FALSE;
	}

	size_t number = params[arg_number] - 1;

	CHECK_PARAMBOUNDS(number, g_activeMessageContext->getParamCount());

	return static_cast<cell>(g_activeMessageContext->getParamType(number));
}

/**
* Retrieves the number of argument in the current game message.
*
* @return           Returns the number of argument in the current game message.
*
* native GetMessageArgsNum();
*/
cell AMX_NATIVE_CALL GetMessageArgsNum(AMX *amx, cell *params)
{
	enum args_e { arg_count };

	CHECK_REQUIREMENTS(ReHLDS);

	if (!g_activeMessageContext)
	{
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: trying to get argument without active hook.", __FUNCTION__);
		return FALSE;
	}

	return g_activeMessageContext->getParamCount();
}

/**
* Sets the block type for the specified message ID.
*
* @param msgid      The ID of the message to set the block type for.
* @param type       The type of block to set for the message, look at the enum MsgBlockType
*
* @return           Returns true if the block type is successfully set, otherwise false.
*
* native bool:SetMessageBlock(const msgid, MsgBlockType:type);
*/
cell AMX_NATIVE_CALL SetMessageBlock(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_id, arg_type };

	CHECK_REQUIREMENTS(ReHLDS);

	int msg_id = params[arg_id];

	// svc_bad (0) is not allowed for hook
	if (msg_id <= 0 || msg_id >= MAX_USERMESSAGES)
		return FALSE;

	g_RehldsMessageManager->setMessageBlock(msg_id, static_cast<IMessage::BlockType>(params[arg_type]));
	return TRUE;
}

/**
* Retrieves the block type for the specified message ID.
*
* @param msgid      The ID of the message to retrieve the block type for.
*
* @return           Returns the block type of the specified message, look at the enum MsgBlockType
*
* native MsgBlockType:GetMessageBlock(const msgid);
*/
cell AMX_NATIVE_CALL GetMessageBlock(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_id };

	CHECK_REQUIREMENTS(ReHLDS);

	int msg_id = params[arg_id];

	// svc_bad (0) is not allowed for hook
	if (msg_id <= 0 || msg_id >= MAX_USERMESSAGES)
		return FALSE;

	return static_cast<cell>(g_RehldsMessageManager->getMessageBlock(params[arg_id]));
}

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
*
* native bool:IsMessageDataModified(MsgDataType:type = MsgAny, const number = -1);
*/
cell AMX_NATIVE_CALL IsMessageDataModified(AMX *amx, cell *params)
{
	CHECK_REQUIREMENTS(ReHLDS);

	enum args_e { arg_count, arg_type, arg_number };

	CHECK_REQUIREMENTS(ReHLDS);

	if (!g_activeMessageContext)
	{
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: trying to get argument without active hook.", __FUNCTION__);
		return FALSE;
	}

	size_t number = -1;

	IMessage::DataType type = static_cast<IMessage::DataType>(params[arg_type]);
	if (type == IMessage::DataType::Param)
	{
		number = params[arg_number] - 1;

		CHECK_PARAMBOUNDS(number, g_activeMessageContext->getParamCount());
	}

	return g_activeMessageContext->isDataModified(type, number) ? TRUE : FALSE;
}

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
*
* native bool:ResetModifiedMessageData(MsgDataType:type = MsgAny, const number = -1);
*/
cell AMX_NATIVE_CALL ResetModifiedMessageData(AMX *amx, cell *params)
{
	enum args_e { arg_count, arg_type, arg_number };

	CHECK_REQUIREMENTS(ReHLDS);

	if (!g_activeMessageContext)
	{
		AMXX_LogError(amx, AMX_ERR_NATIVE, "%s: trying to set argument without active hook.", __FUNCTION__);
		return FALSE;
	}

	size_t number = -1;

	IMessage::DataType type = static_cast<IMessage::DataType>(params[arg_type]);
	if (type == IMessage::DataType::Param && params[arg_number] != -1)
	{
		number = params[arg_number] - 1;

		CHECK_PARAMBOUNDS(number, g_activeMessageContext->getParamCount());
	}

	return g_activeMessageContext->resetModifiedData(type, number) ? TRUE : FALSE;
}

AMX_NATIVE_INFO HookMessage_Natives[] =
{
	{ "RegisterMessage",          RegisterMessage          },
	{ "UnregisterMessage",        UnregisterMessage        },

	{ "EnableHookMessage",        EnableHookMessage        },
	{ "DisableHookMessage",       DisableHookMessage       },

	{ "SetMessageData",           SetMessageData           },
	{ "GetMessageData",           GetMessageData           },

	{ "GetMessageOrigData",       GetMessageOrigData       },

	{ "GetMessageArgType",        GetMessageArgType        },
	{ "GetMessageArgsNum",        GetMessageArgsNum        },

	{ "SetMessageBlock",          SetMessageBlock          },
	{ "GetMessageBlock",          GetMessageBlock          },

	{ "IsMessageDataModified",    IsMessageDataModified    },
	{ "ResetModifiedMessageData", ResetModifiedMessageData },

	{ nullptr, nullptr }
};

void RegisterNatives_HookMessage()
{
	g_amxxapi.AddNatives(HookMessage_Natives);
}
