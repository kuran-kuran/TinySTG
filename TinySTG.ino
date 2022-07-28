#include "src/TinyLibrary.hpp"
#include "src/MainLoop.hpp"
#include "src/Global.hpp"

void setup()
{
	MainLoop_Setup();
	Global& global = Global::GetInstance();
	global.micros_time = micros();
	global.screen_flip_time = 1000000 / Screen::FPS;
}

void loop()
{
	Global& global = Global::GetInstance();
	global.interval_time = micros() - global.micros_time;
	if(global.interval_time > global.screen_flip_time)
	{
		MainLoop_Loop();
		global.micros_time = micros();
	}
	MainLoop_NoWaitLoop();
}
