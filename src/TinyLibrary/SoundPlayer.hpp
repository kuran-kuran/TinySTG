#ifndef TINYSOUNDPLAYER_HPP
#define TINYSOUNDPLAYER_HPP

#include "SoundMixer.hpp"

class SoundPlayer
{
public:
	enum
	{
		APPEND_MAX = 8,
		RAW = 0,
		ADP = 1
	};
	SoundPlayer(void);
	~SoundPlayer(void);
	static SoundPlayer* sound_player;
	static void Initialize(int frequency);
	static void Finalize(void);
	static SoundPlayer& GetInstance();
	int Append(int channel, const unsigned char* buffer, int size, bool loop, int type);
	int Append(int channel, const char* path, bool loop, int type);
	void Delete(int handle);
	void Play(int handle);
	void Stop(int handle);
	void StopAll(void);
	void Update(void);
private:
	int GetHandle(void);
	String* path[APPEND_MAX];
	int channel[APPEND_MAX]; // -1 = free, 0~ = in use
	const unsigned char* buffer[APPEND_MAX];
	int size[APPEND_MAX];
	bool loop[APPEND_MAX];
	int type[APPEND_MAX];
	SoundMixer sound_mixer;
	SoundPlayer(SoundPlayer&);
	SoundPlayer& operator = (SoundPlayer&);
};

#endif
