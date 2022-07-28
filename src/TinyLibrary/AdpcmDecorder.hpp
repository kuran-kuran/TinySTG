#ifndef ADPCM_DECORDER
#define ADPCM_DECORDER

class AdpcmDecorder
{
public:
	AdpcmDecorder(void);
	~AdpcmDecorder(void);
	void Clear(void);
	void Decord(unsigned char* outputBuffer, const unsigned char* inputBuffer, int inputBufferSize);
private:
	int GetNextExpectancy(int rate, int expectancy);
	int beforePcm;
	int expectancy;
};

#endif
