#if defined(_WIN32)

#include "Controller.hpp"
#include <windows.h>
#include <Xinput.h>

#pragma comment (lib, "xinput.lib")
#pragma comment (lib, "winmm.lib")

Controller* Controller::controller = NULL;

Controller::Controller(void)
:button(0)
,touch(false)
{
}

Controller::~Controller(void)
{
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
	// xinput
	XINPUT_STATE state;
	ZeroMemory(&state, sizeof(XINPUT_STATE));
	if(XInputGetState(0, &state) == ERROR_SUCCESS)
	{
		if(state.Gamepad.wButtons & XINPUT_GAMEPAD_A)
		{
			this->button |= BUTTON_1;
		}
		if(state.Gamepad.wButtons & XINPUT_GAMEPAD_B)
		{
			this->button |= BUTTON_2;
		}
		if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)
		{
			this->button |= BUTTON_UP;
		}
		if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
		{
			this->button |= BUTTON_DOWN;
		}
		if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)
		{
			this->button |= BUTTON_LEFT;
		}
		if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
		{
			this->button |= BUTTON_RIGHT;
		}
	}
	// DirectInput
	if(joyGetNumDevs() > 0)
	{
		static const DWORD center = 32767;
		static const DWORD threshold = 16384;
		JOYINFOEX joy_info_ex;
		joy_info_ex.dwSize = sizeof(JOYINFOEX);
		joy_info_ex.dwFlags = JOY_RETURNALL;
		if(joyGetPosEx(0, &joy_info_ex) == JOYERR_NOERROR)
		{
			if(joy_info_ex.dwButtons & 0x0001)
			{
				this->button |= BUTTON_1;
			}
			if(joy_info_ex.dwButtons & 0x0002)
			{
				this->button |= BUTTON_2;
			}
			if(joy_info_ex.dwYpos < center - threshold)
			{
				this->button |= BUTTON_UP;
			}
			if(joy_info_ex.dwYpos > center + threshold)
			{
				this->button |= BUTTON_DOWN;
			}
			if(joy_info_ex.dwXpos < center - threshold)
			{
				this->button |= BUTTON_LEFT;
			}
			if(joy_info_ex.dwXpos > center + threshold)
			{
				this->button |= BUTTON_RIGHT;
			}
			switch(joy_info_ex.dwPOV)
			{
			case 0:
				this->button |= BUTTON_UP;
				break;
			case 4500:
				this->button |= BUTTON_UP;
				this->button |= BUTTON_RIGHT;
				break;
			case 9000:
				this->button |= BUTTON_RIGHT;
				break;
			case 13500:
				this->button |= BUTTON_RIGHT;
				this->button |= BUTTON_DOWN;
				break;
			case 18000:
				this->button |= BUTTON_DOWN;
				break;
			case 22500:
				this->button |= BUTTON_LEFT;
				this->button |= BUTTON_DOWN;
				break;
			case 27000:
				this->button |= BUTTON_LEFT;
				break;
			case 31500:
				this->button |= BUTTON_LEFT;
				this->button |= BUTTON_UP;
				break;
			}
		}
	}
	// Keyboard
	if(GetAsyncKeyState('Z') & 0x8000)
	{
		this->button |= BUTTON_1;
	}
	if(GetAsyncKeyState('X') & 0x8000)
	{
		this->button |= BUTTON_2;
	}
	if(GetAsyncKeyState(VK_UP) & 0x8000)
	{
		this->button |= BUTTON_UP;
	}
	if(GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		this->button |= BUTTON_DOWN;
	}
	if(GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		this->button |= BUTTON_LEFT;
	}
	if(GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		this->button |= BUTTON_RIGHT;
	}
}

unsigned int Controller::GetButton(void)
{
	return this->button;
}

#endif
