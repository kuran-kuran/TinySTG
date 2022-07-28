#ifndef BOSS2_HPP
#define BOSS2_HPP

#include "Enemy.hpp"

class Boss2 : public Enemy
{
public:
	enum
	{
		WIDTH = 62 << 10,
		HEIGHT = 19 << 10,
		WIDTH_HALF = WIDTH / 2,
		HEIGHT_HALF = HEIGHT / 2,
		BULLET_COUNT = 30 * 6,
		HIT_INTERVAL = 14,
		// Phase
		PHASE_APPEAR = 0,
		PHASE_MOVE,
		BULLET_PHASE_WAIT1 = 0,
		BULLET_PHASE_ATTACK1,
		BULLET_PHASE_WAIT2,
		BULLET_PHASE_ATTACK2,
		// Collision
		PLAYER_COLLISION_WIDTH = 28 << 10,
		PLAYER_COLLISION_HEIGHT = 7 << 10,
		SHOT_COLLISION_WIDTH = 28 << 10,
		SHOT_COLLISION_HEIGHT = 7 << 10,
	};
	Boss2(void);
	~Boss2(void);
	void Initialize(int type, int x, int y);
	void Update(void);
	void Draw(void);
	bool Collision(Shot& shot);
	bool Collision(int x, int y);
private:
	Boss2(Boss2&);
	Boss2& operator = (Boss2&);
	int bullet_count;
	int phase;
	int hit_interval;
	int bullet_phase;
	int bullet_phase_count;
};

#endif
