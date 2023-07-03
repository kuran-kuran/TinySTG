#ifndef CONFIG_H
#define CONFIG_H

#if defined(SDL2)
#define CONFIG_FPS 60
#define CONFIG_WIDTH 80
#define CONFIG_HEIGHT 64
#define CONFIG_TITLE "TinySTG"

#elif defined(SDL)
#define CONFIG_FPS 60
#define CONFIG_WIDTH 80
#define CONFIG_HEIGHT 64
#define CONFIG_FRAMEBUFFER_WIDTH 240
#define CONFIG_FRAMEBUFFER_HEIGHT 240
#define CONFIG_SCALE 3
#define CONFIG_Y_ADJUST ((CONFIG_FRAMEBUFFER_HEIGHT-(CONFIG_HEIGHT*CONFIG_SCALE))/2)
#define CONFIG_TITLE "TinySTG"

#elif defined(_WIN32)
// Windows setting
#define CONFIG_FPS 60
#define CONFIG_WIDTH 80
#define CONFIG_HEIGHT 64
#define CONFIG_TITLE L"TinySTG"

#elif defined(ARDUINO_SAMD_ZERO) && defined(CRYSTALLESS) && defined(USBCON)
// TinyArcade/PocketArcade setting
#define CONFIG_FPS 60

#elif defined(ADAFRUIT_PYBADGE_M4_EXPRESS) || defined(ADAFRUIT_PYGAMER_M4_EXPRESS)
// PyBadge/PyGamer setting
#define CONFIG_FPS 60

#else
// RaspberryPI Pico
#define CONFIG_FPS 60

#endif
#endif
