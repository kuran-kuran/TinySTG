#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <SDL.h>

class Controller
{
public:
	enum
	{
		// Button
		BUTTON_UP = 0x01,
		BUTTON_DOWN = 0x02,
		BUTTON_LEFT = 0x04,
		BUTTON_RIGHT = 0x08,
		BUTTON_1 = 0x10,
		BUTTON_2 = 0x20,
		BUTTON_SELECT = 0x40,
		BUTTON_START = 0x80,
		BUTTON_3 = 0x100,
		BUTTON_4 = 0x200,
		BUTTON_L = 0x400,
		BUTTON_R = 0x800,
		BUTTON_QUIT = 0x1000,
	};
	Controller(void);
	~Controller(void);
	static Controller* controller;
	static Controller& GetInstance();
	static void Initialize(void);
	static void Finalize(void);
	void Reset(void);
	void Update(void);
	unsigned int GetButton(void);
private:
	unsigned int button;
	bool touch;
	SDL_GameController* gameController;
};

#endif
