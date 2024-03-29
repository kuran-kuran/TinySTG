#ifndef DEPENDENT_HPP
#define DEPENDENT_HPP

#if defined(SDL2)
	#include <string>
	#include "SDL2/Screen.hpp"
	#include "SDL2/Controller.hpp"
	#include "SDL2/SoundStream.hpp"
	#include "SDL2/TinyFile.hpp"
	#define String std::string
#elif defined(SDL)
	#include <string>
	#include "SDL/Screen.hpp"
	#include "SDL/Controller.hpp"
	#include "SDL/SoundStream.hpp"
	#include "SDL/TinyFile.hpp"
	#define String std::string
#elif defined(_WIN32)
	#include <string>
	#include "Windows/Screen.hpp"
	#include "Windows/Controller.hpp"
	#include "Windows/SoundStream.hpp"
	#include "Windows/TinyFile.hpp"
	#define String std::string
#elif defined(__ANDROID__)
	#error unsupported Android
#elif defined(__APPLE__)
	#error unsupported iOS
#elif defined(ARDUINO_SAMD_ZERO) && defined(CRYSTALLESS) && defined(USBCON)
	#include "TinyArcade/Screen.hpp"
	#include "TinyArcade/Controller.hpp"
	#include "TinyArcade/SoundStream.hpp"
	#include "TinyArcade/TinyFile.hpp"
#elif defined(ARDUINO_SAMD_ZERO) && defined(USBCON)
	#error unsupported GamebuinoMETA
//	#include "GamebuinoMETA/Screen.hpp"
//	#include "GamebuinoMETA/Controller.hpp"
//	#include "GamebuinoMETA/SoundStream.hpp"
//	#include "GamebuinoMETA/TinyFile.hpp"
#elif defined(ADAFRUIT_PYBADGE_M4_EXPRESS) || defined(ADAFRUIT_PYGAMER_M4_EXPRESS)
	#include "PyBadge/Screen.hpp"
	#include "PyBadge/Controller.hpp"
	#include "PyBadge/SoundStream.hpp"
	#include "PyBadge/TinyFile.hpp"
#else
	#include <string>
	#include "Pico/Pico.h"
	#include "Pico/Screen.hpp"
	#include "Pico/Controller.hpp"
	#include "Pico/SoundStream.hpp"
	#include "Pico/TinyFile.hpp"
	#define String std::string
#endif

#endif
