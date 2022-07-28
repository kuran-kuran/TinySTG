#if defined(ADAFRUIT_PYBADGE_M4_EXPRESS) || defined(ADAFRUIT_PYGAMER_M4_EXPRESS)

#include <Adafruit_Arcada.h>

Adafruit_Arcada arcada;
bool initializdArcada = false;

Adafruit_Arcada* GetArcada(void)
{
	if(initializdArcada == false)
	{
		Serial.begin(9600);
		arcada.arcadaBegin();
		arcada.displayBegin();
		arcada.display->fillScreen(ARCADA_BLACK);
		arcada.setBacklight(255);
		arcada.filesysBeginMSD();
		initializdArcada = true;
	}
	return &arcada;
}

#endif
