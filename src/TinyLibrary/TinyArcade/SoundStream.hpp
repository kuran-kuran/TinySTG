#ifndef TINYSOUNDSTREAM_HPP
#define TINYSOUNDSTREAM_HPP

class SoundStream
{
public:
	enum
	{
		SOUND_BUFFER_SIZE = 800,
		MAX_VOLUME = 10
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
	unsigned char* buffer;
	int buffer_size;
	int buffer_size_half;
	int sample_index;
	int write_buffer;
	bool request;
	int volume;
	void tcReset(void);
	bool tcIsSyncing(void);
	SoundStream(SoundStream&);
	SoundStream& operator = (SoundStream&);
};

#endif
