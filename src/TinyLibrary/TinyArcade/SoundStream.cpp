#if defined(ARDUINO_SAMD_ZERO) && defined(CRYSTALLESS) && defined(USBCON)

#include <TinyScreen.h>
#include "SoundStream.hpp"

SoundStream* SoundStream::sound_stream;

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
	analogWrite(A0, 0);
	// Enable GCLK for TCC2 and TC5 (timer counter input clock)
	GCLK->CLKCTRL.reg = static_cast<unsigned short>((GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_ID(GCM_TC4_TC5)));
	while(GCLK->STATUS.bit.SYNCBUSY);
	tcReset();
	// Set Timer counter Mode to 16 bits
	TC5->COUNT16.CTRLA.reg |= TC_CTRLA_MODE_COUNT16;
	// Set TC5 mode as match frequency
	TC5->COUNT16.CTRLA.reg |= TC_CTRLA_WAVEGEN_MFRQ;
	TC5->COUNT16.CTRLA.reg |= TC_CTRLA_PRESCALER_DIV1;
	TC5->COUNT16.CC[0].reg = static_cast<unsigned short>((SystemCoreClock / frequency - 1));
	while(tcIsSyncing());
	// Configure interrupt request
	NVIC_DisableIRQ(TC5_IRQn);
	NVIC_ClearPendingIRQ(TC5_IRQn);
	NVIC_SetPriority(TC5_IRQn, 0);
	NVIC_EnableIRQ(TC5_IRQn);
	// Enable the TC5 interrupt request
	TC5->COUNT16.INTENSET.bit.MC0 = 1;
	while(tcIsSyncing());
	// Enable TC
	TC5->COUNT16.CTRLA.reg |= TC_CTRLA_ENABLE;
	while(tcIsSyncing());
}

SoundStream::~SoundStream(void)
{
	// Disable TC5
	TC5->COUNT16.CTRLA.reg &= ~TC_CTRLA_ENABLE;
	while(tcIsSyncing());
	// Reset TCx
	TC5->COUNT16.CTRLA.reg = TC_CTRLA_SWRST;
	while(tcIsSyncing());
	while(TC5->COUNT16.CTRLA.bit.SWRST);
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

void SoundStream::tcReset(void)
{
	// Reset TCx
	TC5->COUNT16.CTRLA.reg = TC_CTRLA_SWRST;
	while(tcIsSyncing());
	while(TC5->COUNT16.CTRLA.bit.SWRST);
}

bool SoundStream::tcIsSyncing(void)
{
	return TC5->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY;
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
	while(DAC->STATUS.bit.SYNCBUSY == 1);
//	DAC->DATA.reg = static_cast<unsigned short>(this->buffer[this->sample_index] << 2); // 8bit->10bit
	if(this->volume >= 8)
	{
		int volume_shift = (this->volume - 8);
		DAC->DATA.reg = static_cast<unsigned short>(this->buffer[this->sample_index]) << volume_shift;
	}
	else
	{
		int volume_shift = (8 - this->volume);
		DAC->DATA.reg = static_cast<unsigned short>(this->buffer[this->sample_index]) >> volume_shift;
	}
	while(DAC->STATUS.bit.SYNCBUSY == 1);
	++ this->sample_index;
	if(this->sample_index >= this->buffer_size)
	{
		this->sample_index = 0;
	}
	TC5->COUNT16.INTFLAG.bit.MC0 = 1;
}

#ifdef __cplusplus
extern "C"
{
#endif
	void SoundStreamCallback(void)
	{
		SoundStream& sound_stream = SoundStream::GetInstance();
		sound_stream.Callback();
	}
	void TC5_Handler(void) __attribute__ ((weak, alias("SoundStreamCallback")));
#ifdef __cplusplus
}
#endif
#endif
