#if defined _reapi_vtc_included
	#endinput
#endif
#define _reapi_vtc_included

/*
* Checks whether the player is talking at the moment.
*
* @param index      Client index
*
* @return           true if client is speaking, false otherwise
*/
native bool:VTC_IsClientSpeaking(const index);

/*
* Mutes the player.
*
* @param index      Client index
*
* @noreturn
*/
native VTC_MuteClient(const index);

/*
* Unmutes the player.
*
* @param index      Client index
*
* @noreturn
*/
native VTC_UnmuteClient(const index);

/*
* Checks whether the player is muted at the moment.
*
* @param index      Client index
*
* @return           true if client is muted, false otherwise
*/
native bool:VTC_IsClientMuted(const index);

/*
* Play the audio file via the voice stream.
*
* @param receiver       Receiver index (0 = play to all clients)
* @param soundFilePath  The path to the sound file, relative to the game directory; must be a valid PCM WAV file (8 or 16 bit)
*
* @note Usage example:
*       VTC_PlaySound(id, "sound/ambience/Opera.wav");
*
* @noreturn
*/
native VTC_PlaySound(const receiver, const soundFilePath[]);

/*
* Blocks the player's voice packets entirely.
* Unlike VTC_MuteClient, a blocked player is not treated as speaking:
* VTC_OnClientStartSpeak/VTC_OnClientStopSpeak are not called for this player.
*
* @param index      Client index
*
* @noreturn
*
* @note            Requires VoiceTranscoder API 3.1+, otherwise the native fails
*/
native VTC_BlockClient(const index);

/*
* Unblocks the player's voice packets.
*
* @param index      Client index
*
* @noreturn
*
* @note            Requires VoiceTranscoder API 3.1+, otherwise the native fails
*/
native VTC_UnblockClient(const index);

/*
* Checks whether the player's voice is blocked at the moment.
*
* @param index      Client index
*
* @return           true if client's voice is blocked, false otherwise
*
* @note             Requires VoiceTranscoder API 3.1+, otherwise the native fails and returns false
*/
native bool:VTC_IsClientBlocked(const index);

/*
* Called when the player started talking.
*
* @param index      Client index
*
* @noreturn
*/
forward VTC_OnClientStartSpeak(const index);

/*
* Called when the player stopped talking.
*
* @param index      Client index
*
* @noreturn
*/
forward VTC_OnClientStopSpeak(const index);
