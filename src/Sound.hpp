#ifndef SOUND_HPP
#define SOUND_HPP

#include "Resource.hpp"

class Sound
{
public:
	Sound(void);
	~Sound(void);
	static Sound* sound;
	static void Initialize(void);
	static void Finalize(void);
	static Sound& GetInstance();
	void Load(void);
	void Release(void);
	void Play(int sound_number);
	void StopAll(void);
	void Update(void);
private:
	Sound(Sound&);
	Sound& operator = (Sound&);
	int sound_handle[SOUND_MAX];
};

#endif
