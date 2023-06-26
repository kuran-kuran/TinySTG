#if defined(SDL)

#include <stdio.h>
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
#if 0
	// コントローラー
	if(this->gameController != NULL)
	{
		if(SDL_GameControllerGetButton(this->gameController, SDL_CONTROLLER_BUTTON_A) == 1)
		{
			this->button |= BUTTON_1;
		}
		if(SDL_GameControllerGetButton(this->gameController, SDL_CONTROLLER_BUTTON_B) == 1)
		{
			this->button |= BUTTON_2;
		}
		if(SDL_GameControllerGetButton(this->gameController, SDL_CONTROLLER_BUTTON_START) == 1)
		{
			this->button |= BUTTON_START;
		}
		if(SDL_GameControllerGetButton(this->gameController, SDL_CONTROLLER_BUTTON_BACK) == 1)
		{
			this->button |= BUTTON_SELECT;
		}
		if(SDL_GameControllerGetButton(this->gameController, SDL_CONTROLLER_BUTTON_LEFTSHOULDER) == 1)
		{
			this->button |= BUTTON_START;
		}
		if(SDL_GameControllerGetButton(this->gameController, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER) == 1)
		{
			this->button |= BUTTON_SELECT;
		}
		if(SDL_GameControllerGetButton(this->gameController, SDL_CONTROLLER_BUTTON_DPAD_UP) == 1)
		{
			this->button |= BUTTON_UP;
		}
		if(SDL_GameControllerGetButton(this->gameController, SDL_CONTROLLER_BUTTON_DPAD_DOWN) == 1)
		{
			this->button |= BUTTON_DOWN;
		}
		if(SDL_GameControllerGetButton(this->gameController, SDL_CONTROLLER_BUTTON_DPAD_LEFT) == 1)
		{
			this->button |= BUTTON_LEFT;
		}
		if(SDL_GameControllerGetButton(this->gameController, SDL_CONTROLLER_BUTTON_DPAD_RIGHT) == 1)
		{
			this->button |= BUTTON_RIGHT;
		}
	}
#endif
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
}

unsigned int Controller::GetButton(void)
{
	return this->button;
}

#endif
