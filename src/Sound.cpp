#include "TinyLibrary.hpp"
#include "Global.hpp"
#include "Sound.hpp"

Sound* Sound::sound;

Sound::Sound(void)
:sound_handle()
{
	for(int i = 0; i < SOUND_MAX; ++ i)
	{
		sound_handle[i] = 0;
	}
}

Sound::~Sound(void)
{
	for(int i = 0; i < SOUND_MAX; ++ i)
	{
		this->sound_handle[i] = 0;
	}
}

void Sound::Initialize(void)
{
	Sound::sound = new Sound();
	SoundPlayer::Initialize(11025);
}

void Sound::Finalize(void)
{
	SoundPlayer::Finalize();
	if(Sound::sound != NULL)
	{
		delete Sound::sound;
		Sound::sound = NULL;
	}
}

Sound& Sound::GetInstance()
{
	return *Sound::sound;
}

void Sound::Load(void)
{
	SoundPlayer& sound_player = SoundPlayer::GetInstance();
	if (TinyFile::Exist("TinySTG/bgm.adp") == true)
	{
		this->sound_handle[SOUND_BGM] = sound_player.Append(0, "TinySTG/bgm.adp", true, SoundPlayer::ADP);
	}
	else if(TinyFile::Exist("TinySTG/bgm.raw") == true)
	{
		this->sound_handle[SOUND_BGM] = sound_player.Append(0, "TinySTG/bgm.raw", true, SoundPlayer::RAW);
	}
	else
	{
#if defined(ADAFRUIT_PYBADGE_M4_EXPRESS) || defined(ADAFRUIT_PYGAMER_M4_EXPRESS) || defined(_WIN32)
		const SoundData* sound_data = GetSoundData(SOUND_BGM_SHORT);
		this->sound_handle[SOUND_BGM] = sound_player.Append(0, sound_data->buffer, sound_data->size, true, SoundPlayer::RAW);
#endif
	}
	const SoundData* sound_data = GetSoundData(SOUND_CRUSH);
	this->sound_handle[SOUND_CRUSH] = sound_player.Append(1, sound_data->buffer, sound_data->size, false, SoundPlayer::RAW);
	sound_data = GetSoundData(SOUND_CRUSH_BOSS);
	this->sound_handle[SOUND_CRUSH_BOSS] = sound_player.Append(2, sound_data->buffer, sound_data->size, false, SoundPlayer::RAW);
	sound_data = GetSoundData(SOUND_GUN);
	this->sound_handle[SOUND_GUN] = sound_player.Append(3, sound_data->buffer, sound_data->size, false, SoundPlayer::RAW);
	sound_data = GetSoundData(SOUND_POWER_UP);
	this->sound_handle[SOUND_POWER_UP] = sound_player.Append(4, sound_data->buffer, sound_data->size, false, SoundPlayer::RAW);
	sound_data = GetSoundData(SOUND_BOMB);
	this->sound_handle[SOUND_BOMB] = sound_player.Append(4, sound_data->buffer, sound_data->size, false, SoundPlayer::RAW);
	sound_data = GetSoundData(SOUND_GET);
	this->sound_handle[SOUND_GET] = sound_player.Append(4, sound_data->buffer, sound_data->size, false, SoundPlayer::RAW);
}

void Sound::Release(void)
{
	SoundPlayer& sound_player = SoundPlayer::GetInstance();
	sound_player.StopAll();
	sound_player.Delete(this->sound_handle[SOUND_BGM]);
	sound_player.Delete(this->sound_handle[SOUND_CRUSH]);
	sound_player.Delete(this->sound_handle[SOUND_CRUSH_BOSS]);
	sound_player.Delete(this->sound_handle[SOUND_GUN]);
	sound_player.Delete(this->sound_handle[SOUND_POWER_UP]);
	sound_player.Delete(this->sound_handle[SOUND_BOMB]);
	sound_player.Delete(this->sound_handle[SOUND_GET]);
}

void Sound::Play(int sound_number)
{
	Global& global = Global::GetInstance();
	if(global.gamedata.sound == false)
	{
		return;
	}
	SoundPlayer& sound_player = SoundPlayer::GetInstance();
	sound_player.Play(this->sound_handle[sound_number]);
}

void Sound::StopAll(void)
{
	SoundPlayer& sound_player = SoundPlayer::GetInstance();
	sound_player.StopAll();
}

void Sound::Update(void)
{
	SoundPlayer& sound_player = SoundPlayer::GetInstance();
	sound_player.Update();
}
