#ifndef TINYSOUNDSTREAM_HPP
#define TINYSOUNDSTREAM_HPP

#include <dsound.h>

class SoundStream
{
public:
	enum
	{
		SOUND_BUFFER_SIZE = 735 * 4,
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
	void Callback(void);
private:
	IDirectSound* direct_sound;
	IDirectSoundBuffer* primary_buffer;
	IDirectSoundBuffer* secondry_buffer;
	int frequency;
	int volume;
	bool request;
	size_t stream_buffer_size;
	bool preload;
	int stream_buffer_toggle;
	bool directsound_playing;
	unsigned int position_add;
	unsigned int before_position;
	unsigned int position;
	void Release(void);
	void WriteData(int stream_buffer_index, const void* buffer);
	SoundStream(SoundStream&);
	SoundStream& operator = (SoundStream&);
};

#endif
