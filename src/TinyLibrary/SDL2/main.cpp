#if defined(SDL2)

#include <SDL.h>
#include <SDL_image.h>
#include "../../TinyLibrary.hpp"
#include "../../MainLoop.hpp"
#include "../../Global.hpp"

int main(int argc, char* argv[])
{
	MainLoop_Setup();
	Uint64 beforeTime= SDL_GetPerformanceCounter();
	Uint64 intervalTime = SDL_GetPerformanceFrequency() / Screen::FPS;
	bool quit = false;
	while(quit == false)
	{
		Uint64 nowTime = SDL_GetPerformanceCounter();
		Uint64 elapsed = nowTime - beforeTime;
		if(elapsed > intervalTime)
		{
			SDL_Event event;
			while(SDL_PollEvent(&event))
			{
				switch(event.type)
				{
				case SDL_QUIT:
					quit = true;
					break;
				}
			}
			MainLoop_Loop();
			MainLoop_NoWaitLoop();
			beforeTime = nowTime;
		}
	}
	return 0;
}

#endif
