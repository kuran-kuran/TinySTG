#ifndef ENEMY3_HPP
#define ENEMY3_HPP

#include "Enemy.hpp"

class Enemy3 : public Enemy
{
public:
	enum
	{
		WIDTH = 10 << 10,
		HEIGHT = 10 << 10,
		WIDTH_HALF = WIDTH / 2,
		HEIGHT_HALF = HEIGHT / 2,
		LEFT_LIMIT = -WIDTH,
		TOP_LIMIT = -HEIGHT,
	};
	static int RIGHT_LIMIT;
	static int BOTTOM_LIMIT;
	Enemy3(void);
	~Enemy3(void);
	void Initialize(int type, int x, int y);
	void Update(void);
	void Draw(void);
private:
	Enemy3(Enemy3&);
	Enemy3& operator = (Enemy3&);
	int turn_count;
	bool turn;
};

#endif
