#include "TinyLibrary.hpp"
#include "Global.hpp"

Global* Global::global = NULL;

Global::Global(void)
:phase(PHASE_TUCHIKURE_LOGO)
,back_color(0)
,before_button(0)
,game(NULL)
,count(0)
,draw_volume_count(0)
,score(0)
,micros_time(0)
,interval_time(0)
,screen_flip_time(0)
{
	this->gamedata.highscore = 10000;
	this->gamedata.difficulty = 1;
	this->gamedata.sound = 1;
#if defined(ADAFRUIT_PYBADGE_M4_EXPRESS) || defined(ADAFRUIT_PYGAMER_M4_EXPRESS)
	this->gamedata.volume = 7;
#else
	this->gamedata.volume = 10;
#endif
}

Global::~Global(void)
{
}

void Global::Initialize(void)
{
	if(Global::global == NULL)
	{
		Global::global = new Global;
	}
}

void Global::Finalize(void)
{
	if(Global::global != NULL)
	{
		delete Global::global;
		Global::global = NULL;
	}
}

Global& Global::GetInstance()
{
	return *Global::global;
}
