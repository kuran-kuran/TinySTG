#ifndef SHOT_HPP
#define SHOT_HPP

class Enemy;

class Shot
{
public:
	enum
	{
		TOP_LIMIT = 5 << 10,
		STATUS_DEAD = 0,
		STATUS_ALIVE,
	};
	Shot(void);
	~Shot(void);
	void Initialize(int x, int y, int vx, int vy, int chr);
	void Update(void);
	void Draw(void);
	unsigned int GetStatus(void);
	void SetStatus(unsigned int status);
	void GetPosition(int& x, int& y);
	bool Collision(Enemy* enemy);
private:
	Shot(Shot&);
	Shot& operator = (Shot&);
	int x;
	int y;
	int vx;
	int vy;
	int chr;
	int status;
};

#endif
