#if defined(ADAFRUIT_PYBADGE_M4_EXPRESS) || defined(ADAFRUIT_PYGAMER_M4_EXPRESS)

#include "Arcada.hpp"
#include "SoundStream.hpp"

SoundStream* SoundStream::sound_stream;

void SoundStreamCallback(void)
{
	SoundStream& sound_stream = SoundStream::GetInstance();
	sound_stream.Callback();
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
	this->buffer = new unsigned char[this->buffer_size];
	memset(this->buffer, 0x80, this->buffer_size);
	Adafruit_Arcada* arcada = GetArcada();
	analogWriteResolution(8);
	arcada->enableSpeaker(true);
	arcada->timerCallback(frequency, SoundStreamCallback);
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
	int play_buffer = 0;
	if(this->sample_index >= this->buffer_size_half)
	{
		play_buffer = 1;
	}
	if(this->write_buffer == play_buffer)
	{
		this->request = true;
	}
}

int SoundStream::GetWritableSize(void) const
{
	if(this->request == false)
	{
		return 0;
	}
	return this->buffer_size_half;
}

bool SoundStream::Write(const unsigned char* buffer)
{
	int play_buffer = 0;
	if(this->sample_index >= this->buffer_size_half)
	{
		play_buffer = 1;
	}
	this->write_buffer = 1 - play_buffer;
	memcpy(&this->buffer[this->buffer_size_half * write_buffer], buffer, this->buffer_size_half);
	this->request = false;
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
void SoundStream::Callback(void)
{
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
}

#endif
