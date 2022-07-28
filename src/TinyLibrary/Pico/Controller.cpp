#include "Pico.h"
#if defined(_PICO_)

#ifdef __cplusplus
extern "C" {
#endif
#include "DEV_Config.h"
#include "Infrared.h"
#ifdef __cplusplus
}
#endif

#include "Controller.hpp"

Controller* Controller::controller = NULL;

Controller::Controller(void)
{
    SET_Infrared_PIN(BUTTON_RIGHT_PIN);    
    SET_Infrared_PIN(BUTTON_LEFT_PIN);
	SET_Infrared_PIN(DIR_UP_PIN);
    SET_Infrared_PIN(DIR_DOWN_PIN);
    SET_Infrared_PIN(DIR_LEFT_PIN);
    SET_Infrared_PIN(DIR_RIGHT_PIN);
    SET_Infrared_PIN(DIR_CENTER_PIN);
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
	if(DEV_Digital_Read(BUTTON_RIGHT_PIN) == 0)
	{
		this->button |= BUTTON_1;
	}
	if(DEV_Digital_Read(BUTTON_LEFT_PIN) == 0)
	{
		this->button |= BUTTON_2;
	}
	if(DEV_Digital_Read(DIR_CENTER_PIN) == 0)
	{
		this->button |= BUTTON_START;
	}
	if(DEV_Digital_Read(DIR_UP_PIN) == 0)
	{
		this->button |= BUTTON_UP;
	}
	if(DEV_Digital_Read(DIR_DOWN_PIN) == 0)
	{
		this->button |= BUTTON_DOWN;
	}
	if(DEV_Digital_Read(DIR_LEFT_PIN) == 0)
	{
		this->button |= BUTTON_LEFT;
	}
	if(DEV_Digital_Read(DIR_RIGHT_PIN) == 0)
	{
		this->button |= BUTTON_RIGHT;
	}
}

unsigned int Controller::GetButton(void)
{
	return this->button;
}

#endif
