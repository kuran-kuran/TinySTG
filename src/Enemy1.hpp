#ifndef ENEMY1_HPP
#define ENEMY1_HPP

#include "Enemy.hpp"

class Enemy1 : public Enemy
{
public:
	enum
	{
		WIDTH = 10 << 10,
		HEIGHT = 10 << 10,
		WIDTH_HALF = WIDTH / 2,
		HEIGHT_HALF = HEIGHT / 2,
		BULLET_COUNT = 20,
	};
	Enemy1(void);
	~Enemy1(void);
	void Initialize(int type, int x, int y);
	void Update(void);
	void Draw(void);
private:
	Enemy1(Enemy1&);
	Enemy1& operator = (Enemy1&);
	int bullet_count;
};

#endif
