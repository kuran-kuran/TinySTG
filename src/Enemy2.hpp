#ifndef ENEMY2_HPP
#define ENEMY2_HPP

#include "Enemy.hpp"

class Enemy2 : public Enemy
{
public:
	enum
	{
		WIDTH = 10 << 10,
		HEIGHT = 10 << 10,
		WIDTH_HALF = WIDTH / 2,
		HEIGHT_HALF = HEIGHT / 2,
		BULLET_COUNT = 23,
	};
	Enemy2(void);
	~Enemy2(void);
	void Initialize(int type, int x, int y);
	void Update(void);
	void Draw(void);
private:
	Enemy2(Enemy2&);
	Enemy2& operator = (Enemy2&);
	int bullet_count;
};

#endif
