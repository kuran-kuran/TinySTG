#ifndef BOSS5_HPP
#define BOSS5_HPP

#include "Enemy.hpp"

class Boss5 : public Enemy
{
public:
	enum
	{
		WIDTH = 63 << 10,
		HEIGHT = 29 << 10,
		WIDTH_HALF = WIDTH / 2,
		HEIGHT_HALF = HEIGHT / 2,
		BULLET_COUNT_START = 20,
		BULLET_COUNT = 25,
		BULLET_COUNT2 = 512,
		HIT_INTERVAL = 10,
		// Phase
		PHASE_APPEAR = 0,
		PHASE_MOVE,
		// Collision
		PLAYER_COLLISION_WIDTH = 12 << 10,
		PLAYER_COLLISION_HEIGHT = 14 << 10,
		SHOT_COLLISION_WIDTH = 14 << 10,
		SHOT_COLLISION_HEIGHT = 14 << 10,
	};
	Boss5(void);
	~Boss5(void);
	void Initialize(int type, int x, int y);
	void Update(void);
	void Draw(void);
	bool Collision(Shot& shot);
	bool Collision(int x, int y);
private:
	Boss5(Boss5&);
	Boss5& operator = (Boss5&);
	int bullet_count;
	int bullet_count2;
	int bullet_count3;
	int phase;
	int next_phase;
	int hit_interval;
};

#endif
