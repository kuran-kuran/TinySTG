#if defined(SDL)

#include <stdio.h>
#include <string.h>
#include "SoundStream.hpp"

SoundStream* SoundStream::sound_stream;

SoundStream::SoundStream(int frequency)
:
	//@@audioDeviceID()
ringBuffer()
,writableSize(0)
,volume(0)
{
	this->ringBuffer.Create(SOUND_BUFFER_SIZE);
	// SDL‰Šú‰»
	if(SDL_InitSubSystem(SDL_INIT_AUDIO) != 0)
	{
		throw "Error: SDL_Init(SDL_INIT_AUDIO)";
	}
	SDL_AudioSpec specs;
	SDL_AudioSpec obtained;
	SDL_memset(&specs, 0, sizeof(specs));
	specs.freq = frequency;
	specs.format = AUDIO_U8;
	specs.channels = 1;
	specs.samples = SOUND_BUFFER_SIZE / 2;
	specs.callback = SoundStream::Callback;
	if (SDL_OpenAudio(&specs, &obtained) != 0) {
		throw "Error: SDL_OpenAudio";
	}
	SDL_PauseAudio(0);
#if 0
	this->audioDeviceID = SDL_OpenAudioDevice(nullptr, PLAYBACK_DEV, &specs, nullptr, 0);
	if(this->audioDeviceID == 0)
	{
		throw "Error: SDL_OpenAudioDevice";
	}
	SDL_PauseAudioDevice(this->audioDeviceID, 0);
#endif
}

SoundStream::~SoundStream(void)
{
	SDL_CloseAudio();
	this->ringBuffer.Release();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

void SoundStream::Initialize(int frequency)
{
	SoundStream::sound_stream = new SoundStream(frequency);
}

void SoundStream::Finalize(void)
{
	if(SoundStream::sound_stream != NULL)
	{
		delete SoundStream::sound_stream;
		SoundStream::sound_stream = NULL;
	}
}

SoundStream& SoundStream::GetInstance()
{
	return *SoundStream::sound_stream;
}

void SoundStream::Update(void)
{
	this->writableSize = static_cast<int>(this->ringBuffer.GetWritableSize());
}

int SoundStream::GetWritableSize(void) const
{
	return this->writableSize;
}

bool SoundStream::Write(const unsigned char* buffer)
{
	this->ringBuffer.Write(buffer, this->writableSize);
	this->writableSize = 0;
	return true;
}

// volume = 0-10
void SoundStream::SetVolume(int volume)
{
	this->volume = volume;
}

int SoundStream::GetVolume(void)
{
	return this->volume;
}

// 10: << 2
// 9: << 1
// 8: << 0
// 7: >> 1
// 6: >> 2
// 5: >> 3
// 4: >> 4
// 3: >> 5
// 2: >> 6
// 1: >> 7
// 0: >> 8
void SoundStream::Callback(void*, Uint8* stream, int len)
{
	SoundStream& sound_stream = SoundStream::GetInstance();
	int readableSize = static_cast<int>(sound_stream.ringBuffer.GetReadableSize());
	unsigned char* buffer = new unsigned char[len];
	memset(buffer, 0, len);
	int index = 0;
	if(len > readableSize)
	{
		index = len - readableSize;
	}
	if(len < readableSize)
	{
		readableSize = len;
	}
	if(readableSize > 0)
	{
		sound_stream.ringBuffer.Read(&buffer[index], readableSize);
	}
	memcpy(stream, buffer, len);
}

#endif
