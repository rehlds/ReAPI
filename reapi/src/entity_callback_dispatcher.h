#pragma once

#include "amx_hook.h"

// Manages entity member functions such as (m_pfnThink, m_pfnTouch, m_pfnUse) and dispatches callbacks to AMXX plugins
class CEntityCallbackDispatcher
{
public:
	bool SetThink     (AMX *amx, CBaseEntity *pEntity, const char *pszCallback, const cell *pParams, size_t iParamsLen);
	bool SetTouch     (AMX *amx, CBaseEntity *pEntity, const char *pszCallback, const cell *pParams, size_t iParamsLen);
	bool SetUse       (AMX *amx, CBaseEntity *pEntity, const char *pszCallback, const cell *pParams, size_t iParamsLen);
	bool SetBlocked   (AMX *amx, CBaseEntity *pEntity, const char *pszCallback, const cell *pParams, size_t iParamsLen);
	bool SetMoveDone  (AMX *amx, CBaseEntity *pEntity, const char *pszCallback, const cell *pParams, size_t iParamsLen);

	void UnsetThink   (AMX *amx, CBaseEntity *pEntity);
	void UnsetTouch   (AMX *amx, CBaseEntity *pEntity);
	void UnsetUse     (AMX *amx, CBaseEntity *pEntity);
	void UnsetBlocked (AMX *amx, CBaseEntity *pEntity);
	void UnsetMoveDone(AMX *amx, CBaseEntity *pEntity);

	// Enumeration of possible callback types for entity events
	enum CallbackType
	{
		None,		// No specific callback type, means unset
		Think,
		Touch,
		Use,
		Blocked,
		MoveDone,
	};

	//
	// @brief Deletes all registered callbacks
	void DeleteAllCallbacks();

	//
	// @brief Deletes existing callbacks of the specified type associated with the given entity
	//
	// @param pEntity Pointer to the entity for which callbacks should be deleted.
	// @param type    Type of callback to delete (default:None to delete all types)
	//
	void DeleteExistingCallbacks(CBaseEntity *pEntity, CallbackType type = None);

	// Are we in the middle of processing callbacks?
	bool IsProcessingCallbacks() const { return m_bIsProcessingCallbacks; }

	//
	// @brief Dispatches callbacks associated with the specified entity and callback type
	//
	// This function iterates through registered callbacks and executes them for the
	// given entity and callback type
	//
	//
	// @param pEntity Pointer to the entity for which callbacks should be dispatched
	// @param type    Type of callback to dispatch
	// @param args    Arguments to pass to the callbacks
	//
	template <typename ...f_args>
	void DispatchCallbacks(CBaseEntity *pEntity, CallbackType type, volatile f_args... args)
	{
		// Flag to indicate that callback processing is currently active.
		// This flag is set to true while callback processing to prevent immediate deletion
		// of registered callbacks that may be involved in caused AMXX plugin callbacks
		// Callbacks marked for deletion are prune after callback processing is complete
		m_bIsProcessingCallbacks = true;

		// Iterate through the list of registered callbacks
		for (std::list<EntityCallback *>::const_iterator it = m_callbacks.begin();
				it != m_callbacks.end(); it++)
		{
			const EntityCallback *callback = (*it);

			// Check if the callback is associated with the specified entity and callback type
			if (callback->m_pEntity == pEntity && callback->m_callbackType == type)
			{
				// Check if user parameters provided for this callback
				if (callback->m_nUserParamBlockSize > 0)
				{
					// Execute the callback with the provided arguments and user parameters
					g_amxxapi.ExecuteForward(callback->GetFwdIndex(), args..., g_amxxapi.PrepareCellArrayA(callback->m_pUserParams, callback->m_nUserParamBlockSize, true));
				}
				else
				{
					// Execute the callback with the provided arguments
					g_amxxapi.ExecuteForward(callback->GetFwdIndex(), args...);
				}

				break;
			}
		}

		// Reset the flag to indicate that callback processing has concluded
		// From this point onward, entity callbacks will be immediately deleted on the spot as it is,
		// without any deferred removals or processing
		m_bIsProcessingCallbacks = false;

		// Is there at least one candidate marked for deletion?
		if (!m_callbacksMarkForDeletion.empty())
		{
			for (std::list<EntityCallback *>::const_iterator it = m_callbacksMarkForDeletion.begin();
				it != m_callbacksMarkForDeletion.end(); it++)
			{
				const EntityCallback *toDelete = (*it);

				const std::list<EntityCallback *>::const_iterator callbackIt =
					std::find(m_callbacks.begin(), m_callbacks.end(), toDelete);

				// Check if the marked for deletion callback
				// was found in the list of registered callbacks
				if (callbackIt != m_callbacks.end())
				{
					m_callbacks.erase(callbackIt);
					delete toDelete;
				}
			}

			m_callbacksMarkForDeletion.clear();
		}
	}

private:
	// EntityCallback - a class representing a registered callback for an entity
	class EntityCallback: public CAmxxHookBase
	{
	public:
		EntityCallback(AMX *amx, const char *funcname, int index,
			CBaseEntity *pEntity, const cell *pParams, size_t iParamsLen, CallbackType type
			) :
				CAmxxHookBase(amx, funcname, index, -1),
				m_pEntity(pEntity), m_callbackType(type)
		{
			if (iParamsLen > 0) {
				m_nUserParamBlockSize = iParamsLen + 1;
				m_pUserParams = new cell[m_nUserParamBlockSize];
				Q_memcpy(m_pUserParams, pParams, sizeof(cell) * iParamsLen);
				m_pUserParams[iParamsLen] = 0;
			} else {
				m_nUserParamBlockSize = 0;
				m_pUserParams = nullptr;
			}
		}

		~EntityCallback()
		{
			if (m_pUserParams)
				delete[] m_pUserParams;
			m_pUserParams = nullptr;
			m_nUserParamBlockSize = 0;
		}

		// Pointer to the entity for which the callback is registered
		CBaseEntity *m_pEntity;

		// Type of the callback (e.g., Think, Touch, Use, and so on)
		CallbackType m_callbackType;

		// User-provided data to be passed to their callback function
		cell *m_pUserParams;

		// The length of user-provided parameters to be passed to their callback function
		size_t m_nUserParamBlockSize;
	};

	// Flag indicating that callback processing is currently in progress
	bool m_bIsProcessingCallbacks;

	// List of registered callbacks
	std::list<EntityCallback *> m_callbacks;

	// List of callbacks marked for deletion after dispatching callbacks is complete
	std::list<EntityCallback *> m_callbacksMarkForDeletion;
};

CEntityCallbackDispatcher &EntityCallbackDispatcher();
