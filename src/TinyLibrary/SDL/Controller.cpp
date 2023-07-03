#if defined(SDL)

#include <stdio.h>
#include <string>
#include "Controller.hpp"

Controller* Controller::controller = NULL;

Controller::Controller(void)
:button(0)
,touch(false)
,joystick(NULL)
{
	if(SDL_InitSubSystem(SDL_INIT_JOYSTICK) != 0)
	{
		throw "Error: SDL_Init(SDL_INIT_JOYSTICK)";
	}
	int numJoysticks = SDL_NumJoysticks();
	if(numJoysticks > 0)
	{
		this->joystick = SDL_JoystickOpen(0);
	}
}

Controller::~Controller(void)
{
	if(this->joystick != NULL)
	{
		SDL_JoystickClose(this->joystick);
		this->joystick = NULL;
	}
	SDL_QuitSubSystem(SDL_INIT_JOYSTICK);
}

void Controller::Initialize(void)
{
	Controller::controller = new Controller;
}

void Controller::Finalize(void)
{
	if(Controller::controller != NULL)
	{
		delete Controller::controller;
		Controller::controller = NULL;
	}
}

Controller& Controller::GetInstance()
{
	return *Controller::controller;
}

void Controller::Reset(void)
{
	this->button = 0;
}

void Controller::Update(void)
{
	this->button = 0;
#ifdef _WIN32
	if(SDL_JoystickGetButton(this->joystick, 0) != 0)
	{
		this->button |= BUTTON_1;
	}
	if(SDL_JoystickGetButton(this->joystick, 1) != 0)
	{
		this->button |= BUTTON_2;
	}
	Uint8 hat = SDL_JoystickGetHat(this->joystick, 0);
	if(hat & SDL_HAT_UP)
	{
		this->button |= BUTTON_UP;
	}
	if(hat & SDL_HAT_RIGHT)
	{
		this->button |= BUTTON_RIGHT;
	}
	if(hat & SDL_HAT_DOWN)
	{
		this->button |= BUTTON_DOWN;
	}
	if(hat & SDL_HAT_LEFT)
	{
		this->button |= BUTTON_LEFT;
	}
	int xMove = SDL_JoystickGetAxis(this->joystick, 0);
	if(xMove < -16384)
	{
		this->button |= BUTTON_LEFT;
	}
	else if(xMove > 16384)
	{
		this->button |= BUTTON_RIGHT;
	}
	int yMove = SDL_JoystickGetAxis(this->joystick, 1);
	if(yMove < -16384)
	{
		this->button |= BUTTON_UP;
	}
	else if(yMove > 16384)
	{
		this->button |= BUTTON_DOWN;
	}
	// キーボード
	const Uint8* state = SDL_GetKeyState(NULL);
	if(state != NULL)
	{
		if(state[SDLK_z] == 1)
		{
			this->button |= BUTTON_1;
		}
		if(state[SDLK_x] == 1)
		{
			this->button |= BUTTON_2;
		}
		if(state[SDLK_q] == 1)
		{
			this->button |= BUTTON_QUIT;
		}
		if(state[SDLK_s] == 1)
		{
			this->button |= BUTTON_START;
		}
		if(state[SDLK_SPACE] == 1)
		{
			this->button |= BUTTON_START;
			this->button |= BUTTON_SELECT;
		}
		if(state[SDLK_UP] == 1)
		{
			this->button |= BUTTON_UP;
		}
		if(state[SDLK_DOWN] == 1)
		{
			this->button |= BUTTON_DOWN;
		}
		if(state[SDLK_LEFT] == 1)
		{
			this->button |= BUTTON_LEFT;
		}
		if(state[SDLK_RIGHT] == 1)
		{
			this->button |= BUTTON_RIGHT;
		}
	}
#else
	const Uint8* state = SDL_GetKeyState(NULL);
	if(state != NULL)
	{
		if(state[SDLK_b] == 1)
		{
			this->button |= BUTTON_1;
		}
		if(state[SDLK_a] == 1)
		{
			this->button |= BUTTON_2;
		}
		if(state[SDLK_y] == 1)
		{
			this->button |= BUTTON_3;
		}
		if(state[SDLK_x] == 1)
		{
			this->button |= BUTTON_4;
		}
		if(state[SDLK_m] == 1)
		{
			this->button |= BUTTON_L;
		}
		if(state[SDLK_n] == 1)
		{
			this->button |= BUTTON_R;
		}
		if(state[SDLK_q] == 1)
		{
			this->button |= BUTTON_QUIT;
		}
		if(state[SDLK_s] == 1)
		{
			this->button |= BUTTON_START;
		}
		if(state[SDLK_k] == 1)
		{
			this->button |= BUTTON_SELECT;
		}
		if(state[SDLK_u] == 1)
		{
			this->button |= BUTTON_UP;
		}
		if(state[SDLK_d] == 1)
		{
			this->button |= BUTTON_DOWN;
		}
		if(state[SDLK_l] == 1)
		{
			this->button |= BUTTON_LEFT;
		}
		if(state[SDLK_r] == 1)
		{
			this->button |= BUTTON_RIGHT;
		}
	}
#endif
}

unsigned int Controller::GetButton(void)
{
	return this->button;
}

#endif
