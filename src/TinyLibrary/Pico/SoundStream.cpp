#include "Pico.h"
#if defined(_PICO_)

#include "SoundStream.hpp"

SoundStream* SoundStream::sound_stream;

void SoundStreamCallback(void)
{
#if false
	SoundStream& sound_stream = SoundStream::GetInstance();
	sound_stream.Callback();
#endif
}

SoundStream::SoundStream(int frequency)
:buffer(NULL)
,buffer_size(SOUND_BUFFER_SIZE)
,buffer_size_half(buffer_size / 2)
,sample_index(0)
,write_buffer(0)
,request(false)
,volume(10)
{
#if false
	this->buffer = new unsigned char[this->buffer_size];
	memset(this->buffer, 0x80, this->buffer_size);
	Adafruit_Arcada* arcada = GetArcada();
	analogWriteResolution(8);
	arcada->enableSpeaker(true);
	arcada->timerCallback(frequency, SoundStreamCallback);
#endif
}

SoundStream::~SoundStream(void)
{
}

void SoundStream::Initialize(int frequency)
{
	SoundStream::sound_stream = new SoundStream(frequency);
}

void SoundStream::Finalize(void)
{
#if false
	if(SoundStream::sound_stream != NULL)
	{
		delete SoundStream::sound_stream;
		SoundStream::sound_stream = NULL;
	}
#endif
}

SoundStream& SoundStream::GetInstance()
{
	return *SoundStream::sound_stream;
}

void SoundStream::Update(void)
{
#if false
	int play_buffer = 0;
	if(this->sample_index >= this->buffer_size_half)
	{
		play_buffer = 1;
	}
	if(this->write_buffer == play_buffer)
	{
		this->request = true;
	}
#endif
}

int SoundStream::GetWritableSize(void) const
{
#if false
	if(this->request == false)
	{
		return 0;
	}
	return this->buffer_size_half;
#endif
	return 0;
}

bool SoundStream::Write(const unsigned char* buffer)
{
#if false
	int play_buffer = 0;
	if(this->sample_index >= this->buffer_size_half)
	{
		play_buffer = 1;
	}
	this->write_buffer = 1 - play_buffer;
	memcpy(&this->buffer[this->buffer_size_half * write_buffer], buffer, this->buffer_size_half);
	this->request = false;
	return true;
#endif
	return false;
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
void SoundStream::Callback(void)
{
#if false
	if(this->volume >= 8)
	{
		int volume_shift = (this->volume - 8);
		analogWrite(ARCADA_RIGHT_AUDIO_PIN, static_cast<unsigned short>(this->buffer[this->sample_index] << volume_shift));
	}
	else
	{
		int volume_shift = (8 - this->volume);
		analogWrite(ARCADA_RIGHT_AUDIO_PIN, static_cast<unsigned short>(this->buffer[this->sample_index] >> volume_shift));
	}
	++ this->sample_index;
	if(this->sample_index >= this->buffer_size)
	{
		this->sample_index = 0;
	}
#endif
}

#endif
