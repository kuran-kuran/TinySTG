#ifndef GAME_HPP
#define GAME_HPP

#include "TinyLibrary.hpp"
#include "Player.hpp"
#include "Resource.hpp"

class Shot;
class Bullet;
class Explode;
class Enemy;

class Game
{
public:
	enum
	{
		SHOT_MAX = 32,
		BULLET_MAX = 64,
		ENEMY_MAX = 32,
		EXPLODE_MAX = 24,
		STAGE_MAX = 6,
		BOSS_APPEAR_COUNT = 2 * Screen::FPS * 30,
		ENEMY1 = 1,
		ENEMY2,
		ENEMY3,
		ENEMY4,
		ENEMY5,
		BOSS1,
		BOSS2,
		BOSS3,
		BOSS4,
		BOSS5,
		BOSS6,
		POWER_UP,
		BULLET_RATE_MAX = 1000,
		BULLET_TYPE_NORMAL = 0,
		BULLET_TYPE_DIFFUSE,
		SPAWN_MAX = 3,
		SPAWN_INTERVAL = 40,
		ITEM_INTERVAL = 28 * Screen::FPS,
		DRAW_STAGE_TIME = Screen::FPS * 3,
		DRAW_WARNING_TIME = Screen::FPS * 2,
		LEFT = 3,
		EASY = 0,
		NORMAL,
		HARD,
		VERY_HARD,
	};
	Game(void);
	~Game(void);
	void Initialize(int stage, int difficulty);
	void Finalize(void);
	bool Update(void);
	void Draw(void);
	unsigned char GetBackColor(void);
	void SetShot(void);
	void GetVector(int& vx, int& vy, int target_x, int target_y, int x, int y, int speed);
	void SetBulletToFighter(int x, int y, int speed, int type, int count);
	void SetBullet(int x, int y, int vx, int vy);
	void SetExplode(int x, int y, int vx, int vy);
	void SetEnemy(int x, int y, int type, int item);
	bool FireBullet(void);
	int GetBulletSpeed(void);
	void GetFighterPosition(int& x, int& y);
	void Spawn(void);
	void SpawnBoss(void);
	int SpawnNumber(int stage);
	void DeleteEnemy(void);
	void Bomber(void);
	int GetDifficulty(void);
private:
	void Spawn(int index);
	void ClearCharacter(void);
	Game(Game&);
	Game& operator = (Game&);
	struct ShotInfo
	{
		int x;
		int y;
		int vx;
		int vy;
		int chr;
	};
	struct SpawnInfo
	{
		int interval;
		int interval2;
		int interval_count;
		int interval2_count;
		int repeat_count;
		int rapid;
		int spawn_number;
		int enemy_number;
		int spawn_x;
		int spawn_y;
	};
	int stage;
	Player player;
	Shot* shot[SHOT_MAX];
	int shot_cool_time_count;
	Bullet* bullet[BULLET_MAX];
	Explode* explode[EXPLODE_MAX];
	Enemy* enemy[ENEMY_MAX];
	int bullet_rate;
	int bullet_rate_min;
	int bullet_rate_interval;
	int bullet_rate_interval_count;
	int bullet_speed;
	int bullet_speed_add;
	SpawnInfo spawn_info[3];
	int spawn;
	bool boss;
	int boss_appear_count;
	int item_interval;
	int bg_y;
	int draw_stage;
	int draw_warning;
	int difficulty;
	int left;
	int continue_count;
	int continue_interval;
	bool gameover;
	int gameover_count;
	unsigned int before_button;
	int score;
	int highscore;
	int enemy_move;
};

#endif
