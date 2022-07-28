#ifndef POWERUP_HPP
#define POWERUP_HPP

#include "Enemy.hpp"

class PowerUp : public Enemy
{
public:
	enum
	{
		WIDTH = 10 << 10,
		HEIGHT = 10 << 10,
		WIDTH_HALF = WIDTH / 2,
		HEIGHT_HALF = HEIGHT / 2,
		LEFT_LIMIT = WIDTH_HALF,
		RIGHT_LIMIT = (Screen::WIDTH << 10) - WIDTH_HALF,
		TOP_LIMIT = HEIGHT_HALF,
		BOTTOM_LIMIT = (Screen::HEIGHT << 10) - HEIGHT_HALF,
		// Collision
		PLAYER_COLLISION_WIDTH = 7 << 10,
		PLAYER_COLLISION_HEIGHT = 7 << 10,
	};
	PowerUp(void);
	~PowerUp(void);
	void Initialize(int type, int x, int y);
	void Update(void);
	void Draw(void);
	bool Collision(Shot& shot);
	bool Collision(int x, int y);
private:
	PowerUp(PowerUp&);
	PowerUp& operator = (PowerUp&);
};

#endif
