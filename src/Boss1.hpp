#ifndef BOSS1_HPP
#define BOSS1_HPP

#include "Enemy.hpp"

class Boss1 : public Enemy
{
public:
	enum
	{
		WIDTH = 22 << 10,
		HEIGHT = 30 << 10,
		WIDTH_HALF = WIDTH / 2,
		HEIGHT_HALF = HEIGHT / 2,
		BULLET_COUNT_START = 20,
		BULLET_COUNT = 35,
		HIT_INTERVAL = 10,
		// Phase
		PHASE_APPEAR = 0,
		PHASE_MOVE,
		// Collision
		PLAYER_COLLISION_WIDTH = 10 << 10,
		PLAYER_COLLISION_HEIGHT = 16 << 10,
		SHOT_COLLISION_WIDTH = 10 << 10,
		SHOT_COLLISION_HEIGHT = 16 << 10,
	};
	Boss1(void);
	~Boss1(void);
	void Initialize(int type, int x, int y);
	void Update(void);
	void Draw(void);
	bool Collision(Shot& shot);
	bool Collision(int x, int y);
private:
	Boss1(Boss1&);
	Boss1& operator = (Boss1&);
	int bullet_count;
	int phase;
	int hit_interval;
};

#endif
