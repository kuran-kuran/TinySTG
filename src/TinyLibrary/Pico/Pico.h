#if defined(SDL2)
#elif defined(_WIN32)
#elif defined(__ANDROID__)
#elif defined(__APPLE__)
#elif defined(ARDUINO_SAMD_ZERO) && defined(CRYSTALLESS) && defined(USBCON)
#elif defined(ARDUINO_SAMD_ZERO) && defined(USBCON)
#elif defined(ADAFRUIT_PYBADGE_M4_EXPRESS) || defined(ADAFRUIT_PYGAMER_M4_EXPRESS)
#else
	#ifdef __cplusplus
	extern "C" {
	#endif
		#include "DEV_Config.h"
	#ifdef __cplusplus
	}
	#endif
	#if defined(PICO_SDK_VERSION_STRING)
		#define _PICO_
	#endif
#endif
