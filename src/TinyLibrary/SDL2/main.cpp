#if defined(SDL2)

#include <SDL.h>
#include <SDL_image.h>
#include "../../TinyLibrary.hpp"
#include "../../MainLoop.hpp"
#include "../../Global.hpp"

int main(int argc, char* argv[])
{
	MainLoop_Setup();
	bool quit = false;
	while(quit == false)
	{
        MainLoop_Loop();
		MainLoop_NoWaitLoop();
//		SDL_Delay(1000 / CONFIG_FPS);
	}
	return 0;
}

#endif
