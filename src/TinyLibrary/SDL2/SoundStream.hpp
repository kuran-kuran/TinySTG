#ifndef TINYSOUNDSTREAM_HPP
#define TINYSOUNDSTREAM_HPP

#include <SDL.h>
#include "RingBuffer.hpp"

class SoundStream
{
public:
	enum
	{
		SOUND_BUFFER_SIZE = 800 * 2,
		MAX_VOLUME = 8
	};
	SoundStream(int frequency);
	~SoundStream(void);
	static SoundStream* sound_stream;
	static SoundStream& GetInstance();
	static void Initialize(int frequency);
	static void Finalize(void);
	void Update(void);
	int GetWritableSize(void) const;
	bool Write(const unsigned char* buffer);
	void SetVolume(int volume);
	int GetVolume(void);
	static void Callback(void*, Uint8* stream, int len);
private:
	SDL_AudioDeviceID audioDeviceID;
	RingBuffer ringBuffer;
	int writableSize;
	int volume;
	SoundStream(SoundStream&);
	SoundStream& operator = (SoundStream&);
};

#endif
