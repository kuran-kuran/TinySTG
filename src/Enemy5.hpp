#ifndef ENEMY5_HPP
#define ENEMY5_HPP

#include "TinyLibrary.hpp"
#include "Enemy.hpp"

class Enemy5 : public Enemy
{
public:
	enum
	{
		WIDTH = 16 << 10,
		HEIGHT = 16 << 10,
		WIDTH_HALF = WIDTH / 2,
		HEIGHT_HALF = HEIGHT / 2,
		BULLET_COUNT_START = 20,
		BULLET_COUNT = Screen::FPS,
		HIT_INTERVAL = 4,
		// Phase
		PHASE_ALIVE = 0,
		PHASE_DYING,
		// Collision
		PLAYER_COLLISION_WIDTH = 7 << 10,
		PLAYER_COLLISION_HEIGHT = 7 << 10,
		SHOT_COLLISION_WIDTH = 8 << 10,
		SHOT_COLLISION_HEIGHT = 8 << 10,
	};
	Enemy5(void);
	~Enemy5(void);
	void Initialize(int type, int x, int y);
	void Update(void);
	void Draw(void);
	bool Collision(Shot& shot);
	bool Collision(int x, int y);
private:
	Enemy5(Enemy5&);
	Enemy5& operator = (Enemy5&);
	int move_count;
	int bullet_count;
	int phase;
	int hit_interval;
};

#endif
