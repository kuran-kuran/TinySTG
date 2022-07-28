#ifndef EXPLODE_HPP
#define EXPLODE_HPP

#include "TinyLibrary.hpp"

class Explode
{
public:
	enum
	{
		WIDTH = 20 << 10,
		HEIGHT = 20 << 10,
		WIDTH_HALF = WIDTH / 2,
		HEIGHT_HALF = HEIGHT / 2,
		STATUS_DEAD = 0,
		STATUS_ALIVE,
	};
	Explode(void);
	~Explode(void);
	void Initialize(int x, int y, int vx, int vy);
	void Update(void);
	void Draw(void);
	unsigned int GetStatus(void);
private:
	Explode(Explode&);
	Explode& operator = (Explode&);
	int x;
	int y;
	int vx;
	int vy;
	int status;
	int animation_count;
};

#endif
