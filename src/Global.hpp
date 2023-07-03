#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#include "Game.hpp"

class Global
{
public:
	enum
	{
		PHASE_TUCHIKURE_LOGO = 0,
		PHASE_DAIMON_LOGO,
		PHASE_OTOYOI_LOGO,
		PHASE_TITLE,
		PHASE_INITIALIZE_GAME,
		PHASE_GAME,
		PHASE_QUIT_MENU,
		PHASE_QUIT_MENU_WAIT
	};
	struct GameData
	{
		int highscore;
		int difficulty;
		int sound;
		int volume;
	};
	Global(void);
	~Global(void);
	static Global* global;
	static Global& GetInstance();
	static void Initialize(void);
	static void Finalize(void);
	int phase;
	int before_phase;
	unsigned char back_color;
	unsigned int before_button;
	Game* game;
	int count;
	int draw_volume_count;
	int score;
	GameData gamedata;
	unsigned long micros_time;
	unsigned long interval_time;
	unsigned long screen_flip_time;
	bool quit_menu;
	int quit_menu_wait_counter;
private:
	Global(Global&);
	Global& operator = (Global&);
};

#endif
