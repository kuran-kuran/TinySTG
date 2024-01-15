#ifndef TINYSOUNDMIXER_HPP
#define TINYSOUNDMIXER_HPP

#include "Dependent.hpp"

class AdpcmDecorder;

class SoundMixer
{
public:
	enum
	{
		CHANNEL_COUNT = 5,
	};
	SoundMixer(void);
	~SoundMixer(void);
	void Append(int channel, const unsigned char* buffer, int size, bool loop, int type);
	void Append(int channel, const char* path, bool loop, int type);
	void Delete(int channel);
	void Read(unsigned char* buffer, int size);
	void Clear(void);
private:
	void MixBuffer(unsigned char* destination_buffer, const unsigned char* source_buffer, int size);
	TinyFile* file[CHANNEL_COUNT];
	const unsigned char* buffer[CHANNEL_COUNT];
	int size[CHANNEL_COUNT];
	int sample[CHANNEL_COUNT];
	bool loop[CHANNEL_COUNT];
	int type[CHANNEL_COUNT];
	AdpcmDecorder* adpcm_decorder[CHANNEL_COUNT];
	SoundMixer(SoundMixer&);
	SoundMixer& operator = (SoundMixer&);
};

#endif
