#if defined(ADAFRUIT_PYBADGE_M4_EXPRESS) || defined(ADAFRUIT_PYGAMER_M4_EXPRESS)

#include "Arcada.hpp"
#include "Controller.hpp"

Controller* Controller::controller = NULL;

Controller::Controller(void)
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
	Adafruit_Arcada* arcada = GetArcada();
	uint8_t pressed_buttons = arcada->readButtons();
	this->button = 0;
	if(pressed_buttons & ARCADA_BUTTONMASK_A)
	{
		this->button |= BUTTON_1;
	}
	if(pressed_buttons & ARCADA_BUTTONMASK_B)
	{
		this->button |= BUTTON_2;
	}
	if(pressed_buttons & ARCADA_BUTTONMASK_SELECT)
	{
		this->button |= BUTTON_SELECT;
	}
	if(pressed_buttons & ARCADA_BUTTONMASK_START)
	{
		this->button |= BUTTON_START;
	}
	if(pressed_buttons & ARCADA_BUTTONMASK_UP)
	{
		this->button |= BUTTON_UP;
	}
	if(pressed_buttons & ARCADA_BUTTONMASK_DOWN)
	{
		this->button |= BUTTON_DOWN;
	}
	if(pressed_buttons & ARCADA_BUTTONMASK_LEFT)
	{
		this->button |= BUTTON_LEFT;
	}
	if(pressed_buttons & ARCADA_BUTTONMASK_RIGHT)
	{
		this->button |= BUTTON_RIGHT;
	}
}

unsigned int Controller::GetButton(void)
{
	return this->button;
}

#endif
