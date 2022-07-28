#include "AdpcmDecorder.hpp"

AdpcmDecorder::AdpcmDecorder(void)
{
	Clear();
}

AdpcmDecorder::~AdpcmDecorder(void)
{
}

void AdpcmDecorder::Clear(void)
{
	beforePcm = 0;
	expectancy = 16;
}

void AdpcmDecorder::Decord(unsigned char* outputBuffer, const unsigned char* inputBuffer, int inputBufferSize)
{
	for(int i = 0; i < inputBufferSize; ++ i)
	{
		unsigned char highData = inputBuffer[i] >> 4;
		int rate = static_cast<int>(highData) - 8;
		int variation = expectancy * rate;
		beforePcm = beforePcm + variation;
		unsigned int pcm8bit = beforePcm / 258 + 128;
		outputBuffer[i * 2] = static_cast<unsigned char>(pcm8bit);
		expectancy = GetNextExpectancy(rate, expectancy);

		unsigned char lowData = inputBuffer[i] & 0xF;
		rate = static_cast<int>(lowData) - 8;
		variation = expectancy * rate;
        beforePcm = beforePcm + variation;
		pcm8bit = beforePcm / 258 + 128;
		outputBuffer[i * 2 + 1] = static_cast<unsigned char>(pcm8bit);
		expectancy = GetNextExpectancy(rate, expectancy);
	}
}

int AdpcmDecorder::GetNextExpectancy(int rate, int expectancy)
{
    int result = 0;
    if(rate == 0)
    {
        result = expectancy * 233 / 256;
    }
    else if((rate == 1) || (rate == -1))
    {
        result = expectancy * 233 / 256;
    }
    else if((rate == 2) || (rate == -2))
    {
        result = expectancy * 233 / 256;
    }
    else if((rate == 3) || (rate == -3))
    {
        result = expectancy * 233 / 256;
    }
    else if((rate == 4) || (rate == -4))
    {
        result = expectancy * 310 / 256;
    }
    else if((rate == 5) || (rate == -5))
    {
        result = expectancy * 375 / 256;
    }
    else if((rate == 6) || (rate == -6))
    {
        result = expectancy * 453 / 256;
    }
    else if((rate == 7) || (rate == -7))
    {
        result = expectancy * 549 / 256;
    }
    return result;
}
