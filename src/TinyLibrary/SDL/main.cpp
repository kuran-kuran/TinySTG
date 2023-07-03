#if defined(SDL)

#ifdef _WIN32
#include <SDL.h>
#else
#include <SDL/SDL.h>
#endif
#include "../../TinyLibrary.hpp"
#include "../../MainLoop.hpp"
#include "../../Global.hpp"

int main(int argc, char* argv[])
{
	MainLoop_Setup();
	Uint32 beforeTime= SDL_GetTicks();
	Uint32 intervalTime = 1000 / Screen::FPS;
	bool exit = false;
	while(exit == false)
	{
		Uint32 nowTime = SDL_GetTicks();
		Uint32 elapsed = nowTime - beforeTime;
		if(elapsed > intervalTime)
		{
			exit = MainLoop_Loop();
			SDL_Event event;
			while(SDL_PollEvent(&event))
			{
				switch(event.type)
				{
				case SDL_QUIT:
					exit = true;
					break;
				}
			}
			beforeTime = nowTime;
		}
		MainLoop_NoWaitLoop();
	}
	return 0;
}

#endif
