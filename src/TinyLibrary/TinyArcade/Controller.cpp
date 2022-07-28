#if defined(ARDUINO_SAMD_ZERO) && defined(CRYSTALLESS) && defined(USBCON)

#include <TinyScreen.h>
#include "Controller.hpp"

Controller* Controller::controller = NULL;

Controller::Controller(void)
:version(1)
{
	pinMode(4, INPUT_PULLUP);
	if(!digitalRead(4))
	{
		this->version = 2;
	}
	pinMode(BUTTON_LEFT_PIN, INPUT_PULLUP);
	pinMode(BUTTON_RIGHT_PIN, INPUT_PULLUP);
	if(this->version == 2)
	{
	    pinMode(DIR_UP_PIN, INPUT_PULLUP);
	    pinMode(DIR_DOWN_PIN, INPUT_PULLUP);
	    pinMode(DIR_LEFT_PIN, INPUT_PULLUP);
	    pinMode(DIR_RIGHT_PIN, INPUT_PULLUP);
	}
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
	if(this->version == 1)
	{
		int dir_x = analogRead(DIR_X_PIN) - 512;
		if(dir_x < -DIR_THRESHOLD)
		{
			this->button |= BUTTON_RIGHT;
		}
		else if(dir_x > DIR_THRESHOLD)
		{
			this->button |= BUTTON_LEFT;
		}
		int dir_y = analogRead(DIR_Y_PIN) - 512;
		if(dir_y < -DIR_THRESHOLD)
		{
			this->button |= BUTTON_UP;
		}
		else if(dir_y > DIR_THRESHOLD)
		{
			this->button |= BUTTON_DOWN;
		}
	}
	else
	{
		if(!digitalRead(DIR_UP_PIN))
		{
			this->button |= BUTTON_UP;
		}
		if(!digitalRead(DIR_DOWN_PIN))
		{
			this->button |= BUTTON_DOWN;
		}
		if(!digitalRead(DIR_LEFT_PIN))
		{
			this->button |= BUTTON_LEFT;
		}
		if(!digitalRead(DIR_RIGHT_PIN))
		{
			this->button |= BUTTON_RIGHT;
		}
	}
	if(!digitalRead(BUTTON_LEFT_PIN))
	{
		this->button |= BUTTON_1;
	}
	if(!digitalRead(BUTTON_RIGHT_PIN))
	{
		this->button |= BUTTON_2;
	}
}

unsigned int Controller::GetButton(void)
{
	return this->button;
}

#endif
