#ifndef BULLET_HPP
#define BULLET_HPP

#include "TinyLibrary.hpp"

class Bullet
{
public:
	enum
	{
		WIDTH = 2 << 10,
		HEIGHT = 2 << 10,
		WIDTH_HALF = WIDTH / 2,
		HEIGHT_HALF = HEIGHT / 2,
		LEFT_LIMIT = -WIDTH,
		RIGHT_LIMIT = Screen::WIDTH << 10,
		TOP_LIMIT = -HEIGHT,
		BOTTOM_LIMIT = Screen::HEIGHT << 10,
		STATUS_DEAD = 0,
		STATUS_ALIVE,
	};
	Bullet(void);
	~Bullet(void);
	void Initialize(int x, int y, int vx, int vy);
	void Update(void);
	void Draw(void);
	unsigned int GetStatus(void);
	void GetPosition(int& x, int& y);
private:
	Bullet(Bullet&);
	Bullet& operator = (Bullet&);
	int x;
	int y;
	int vx;
	int vy;
	int status;
	int animation_count;
};

#endif
