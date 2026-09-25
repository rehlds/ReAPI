#pragma once

#include <cstddef>

const char VOICETRANSCODER_VERSION[] = "2017RC5";

const size_t VOICETRANSCODER_API_VERSION_MAJOR = 3;
const size_t VOICETRANSCODER_API_VERSION_MINOR = 1;

template <typename ...T_ARGS>
class IEvent {
public:
	virtual ~IEvent() {}

	typedef void (* handler_t)(T_ARGS...);

	virtual void operator+=(handler_t callback) = 0;
	virtual void operator-=(handler_t callback) = 0;
};

class IVoiceTranscoderAPI {
public:
	virtual ~IVoiceTranscoderAPI() {}

	virtual size_t MajorVersion() = 0;
	virtual size_t MinorVersion() = 0;

	virtual bool IsClientSpeaking(size_t clientIndex) = 0;

	virtual IEvent<size_t>& OnClientStartSpeak() = 0;
	virtual IEvent<size_t>& OnClientStopSpeak() = 0;

	virtual void MuteClient(size_t clientIndex) = 0;
	virtual void UnmuteClient(size_t clientIndex) = 0;
	virtual bool IsClientMuted(size_t clientIndex) = 0;

	virtual void PlaySound(size_t receiverClientIndex, const char *soundFilePath) = 0;

	virtual void BlockClient(size_t clientIndex) = 0;
	virtual void UnblockClient(size_t clientIndex) = 0;
	virtual bool IsClientBlocked(size_t clientIndex) = 0;
};
