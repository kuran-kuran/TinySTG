#include <stdlib.h>
#include "Dependent.hpp"
#include "SoundPlayer.hpp"

SoundPlayer* SoundPlayer::sound_player;

SoundPlayer::SoundPlayer(void)
:sound_mixer()
{
	for(int i = 0; i < APPEND_MAX; ++ i)
	{
		this->path[i] = NULL;
		this->channel[i] = 0;
		this->buffer[i] = NULL;
		this->size[i] = 0;
		this->loop[i] = false;
		this->type[i] = 0;
	}
}

SoundPlayer::~SoundPlayer(void)
{
}

void SoundPlayer::Initialize(int frequency)
{
	SoundStream::Initialize(frequency);
	SoundPlayer::sound_player = new SoundPlayer();
}

void SoundPlayer::Finalize(void)
{
	for(int i = 0; i < APPEND_MAX; ++ i)
	{
		if(SoundPlayer::sound_player->path[i] != NULL)
		{
			delete SoundPlayer::sound_player->path[i];
			SoundPlayer::sound_player->path[i] = NULL;
		}
	}
	if(SoundPlayer::sound_player != NULL)
	{
		delete SoundPlayer::sound_player;
		SoundPlayer::sound_player = NULL;
	}
	SoundStream::Finalize();
}

SoundPlayer& SoundPlayer::GetInstance()
{
	return *SoundPlayer::sound_player;
}

int SoundPlayer::Append(int channel, const unsigned char* buffer, int size, bool loop, int type)
{
	int handle = GetHandle();
	if(handle > -1)
	{
		if(this->path[handle] != NULL)
		{
			delete this->path[handle];
			this->path[handle] = NULL;
		}
		this->channel[handle] = channel;
		this->buffer[handle] = buffer;
		this->size[handle] = size;
		this->loop[handle] = loop;
		this->type[handle] = type;
	}
	return handle;
}

int SoundPlayer::Append(int channel, const char* path, bool loop, int type)
{
	if(TinyFile::Exist(path) == false)
	{
		return -1;
	}
	int handle = GetHandle();
	if(handle > -1)
	{
		this->path[handle] = new String(path);
		this->channel[handle] = channel;
		this->buffer[handle] = NULL;
		this->size[handle] = 0;
		this->loop[handle] = loop;
		this->type[handle] = type;
	}
	return handle;
}

void SoundPlayer::Delete(int handle)
{
	if(handle == -1)
	{
		return;
	}
	if(this->path[handle] != NULL)
	{
		delete this->path[handle];
		this->path[handle] = NULL;
	}
	this->channel[handle] = 0;
	this->buffer[handle] = NULL;
	this->size[handle] = 0;
	this->loop[handle] = false;
	this->type[handle] = 0;
}

void SoundPlayer::Play(int handle)
{
	if(handle == -1)
	{
		return;
	}
	if(this->path[handle] == NULL)
	{
		sound_mixer.Append(this->channel[handle], this->buffer[handle], this->size[handle], this->loop[handle], this->type[handle]);
	}
	else
	{
		sound_mixer.Append(this->channel[handle], this->path[handle]->c_str(), this->loop[handle], this->type[handle]);
	}
}

void SoundPlayer::Stop(int handle)
{
	if(handle == -1)
	{
		return;
	}
	sound_mixer.Delete(this->channel[handle]);
}

void SoundPlayer::StopAll(void)
{
	for(int i = 0; i < APPEND_MAX; ++ i)
	{
		if((this->path[i] != NULL) || (this->buffer[i] != NULL))
		{
			Stop(i);
			break;
		}
	}
}

void SoundPlayer::Update(void)
{
	SoundStream& sound_stream = SoundStream::GetInstance();
	sound_stream.Update();
	int writable_size = sound_stream.GetWritableSize();
	if(writable_size > 0)
	{
		unsigned char* temp = new unsigned char[writable_size];
		this->sound_mixer.Read(temp, writable_size);
		sound_stream.Write(temp);
		delete [] temp;
	}
}

int SoundPlayer::GetHandle(void)
{
	int handle = -1;
	for(int i = 0; i < APPEND_MAX; ++ i)
	{
		if((this->path[i] == NULL) && (this->buffer[i] == NULL))
		{
			handle = i;
			break;
		}
	}
	return handle;
}
