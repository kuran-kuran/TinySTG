#ifndef BOSS3_HPP
#define BOSS3_HPP

#include "Enemy.hpp"

class Boss3 : public Enemy
{
public:
	enum
	{
		WIDTH = 62 << 10,
		HEIGHT = 54 << 10,
		WIDTH_HALF = WIDTH / 2,
		HEIGHT_HALF = HEIGHT / 2,
		BULLET_COUNT_START = 20,
		BULLET_COUNT = 25,
		BULLET_COUNT2 = 250,
		HIT_INTERVAL = 10,
		// Phase
		PHASE_APPEAR = 0,
		PHASE_MOVE_UP,
		PHASE_MOVE,
		// Collision
		PLAYER_COLLISION_WIDTH = 28 << 10,
		PLAYER_COLLISION_HEIGHT = 20 << 10,
		SHOT_COLLISION_WIDTH = 28 << 10,
		SHOT_COLLISION_HEIGHT = 20 << 10,
	};
	Boss3(void);
	~Boss3(void);
	void Initialize(int type, int x, int y);
	void Update(void);
	void Draw(void);
	bool Collision(Shot& shot);
	bool Collision(int x, int y);
private:
	Boss3(Boss3&);
	Boss3& operator = (Boss3&);
	int bullet_count;
	int bullet_count2;
	int phase;
	int next_phase;
	int hit_interval;
};

#endif
