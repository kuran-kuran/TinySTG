#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "Shot.hpp"

class Enemy
{
public:
	enum
	{
		// Collision
		PLAYER_COLLISION_WIDTH = 4 << 10,
		PLAYER_COLLISION_HEIGHT = 4 << 10,
		SHOT_COLLISION_WIDTH = 7 << 10,
		SHOT_COLLISION_HEIGHT = 7 << 10,
		// Status
		STATUS_DEAD = 0,
		STATUS_ALIVE,
	};
	Enemy(void);
	~Enemy(void);
	virtual void Initialize(int type, int x, int y);
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;
	void Move(void);
	unsigned int GetStatus(void);
	void SetStatus(unsigned int status);
	void GetPosition(int& x, int& y);
	void GetVector(int& vx, int& vy);
	virtual bool Collision(Shot& shot);
	virtual bool Collision(int x, int y);
	int GetType(void);
	void SetItem(int item);
	int GetItem(void);
	bool Damage(void);
protected:
	int type;
	int x;
	int y;
	int vx;
	int vy;
	int hp;
	int status;
	int item;
private:
	Enemy(Enemy&);
	Enemy& operator = (Enemy&);
};

#endif
