// Tiny STG for RaspberryPI Pico + Waveshare 19340 v2

#include "src/TinyLibrary.hpp"
#include "src/MainLoop.hpp"
#include "src/Global.hpp"

// RaspberryPI Pico only
#ifdef _PICO_

#ifdef __cplusplus
extern "C" {
#endif
#include "DEV_Config.h"
#ifdef __cplusplus
}
#endif

int main(void)
{
	DEV_Delay_ms(100);
	DEV_Module_Init();
	MainLoop_Setup();
//	Global& global = Global::GetInstance();
//	global.micros_time = static_cast<unsigned long>(time_us_32());
//	global.screen_flip_time = 1000000 / Screen::FPS;
    for(;;)
	{
//		Global& global = Global::GetInstance();
//		global.interval_time = static_cast<unsigned long>(time_us_32()) - global.micros_time;
//		if(global.interval_time > global.screen_flip_time)
		{
			MainLoop_Loop();
//			global.micros_time = static_cast<unsigned long>(time_us_32());
		}
		MainLoop_NoWaitLoop();
	}
	DEV_Module_Exit();
    return 0;
}

#endif
