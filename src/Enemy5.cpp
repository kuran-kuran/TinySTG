#include <stdlib.h>
#include "TinyLibrary.hpp"
#include "Resource.hpp"
#include "Enemy5.hpp"
#include "Global.hpp"

Enemy5::Enemy5(void)
:move_count(0)
,bullet_count(BULLET_COUNT_START)
,phase(PHASE_ALIVE)
,hit_interval(0)
{
}

Enemy5::~Enemy5(void)
{
}

void Enemy5::Initialize(int type, int x, int y)
{
	this->type = type;
	this->x = x;
	this->y = y;
	if(this->x < (Screen::WIDTH << 9))
	{
		this->vx = 2 << 10;
	}
	else
	{
		this->vx = -2 << 10;
	}
	this->move_count = 10 + Mathmatics::Rand() % 30;
	this->vy = 1024 / 4;
	this->hp = 12;
	this->status = STATUS_ALIVE;
	this->phase = PHASE_ALIVE;
}

void Enemy5::Update(void)
{
	if(this->status == STATUS_DEAD)
	{
		return;
	}
	Global& global = Global::GetInstance();
	Move();
	if(this->y > (Screen::HEIGHT << 10) + HEIGHT_HALF)
	{
		this->status = STATUS_DEAD;
	}
	-- this->move_count;
	if(this->move_count < 0)
	{
		this->move_count = 0;
		this->vx = 0;
	}
	-- this->bullet_count;
	if((this->bullet_count < 0) && (global.game->FireBullet() == true))
	{
		int bullet_x = (-7 + 14 * (Mathmatics::Rand() & 1)) << 10;
		int bullet_y = (-7 + 14 * (Mathmatics::Rand() & 1)) << 10;
		global.game->SetBulletToFighter(this->x + bullet_x, this->y + bullet_y, global.game->GetBulletSpeed(), Game::BULLET_TYPE_NORMAL, 1);
		this->bullet_count = BULLET_COUNT;
	}
	if(this->hit_interval > 0)
	{
		-- this->hit_interval;
	}
}

void Enemy5::Draw(void)
{
	if(this->status == STATUS_DEAD)
	{
		return;
	}
	Screen& screen = Screen::GetInstance();
	const ImageData* image_data;
	image_data = GetImageData(ENEMY5);
	int x = (this->x - WIDTH_HALF) >> 10;
	int y = (this->y - HEIGHT_HALF) >> 10;
	screen.DrawSprite(image_data->buffer, x, y, image_data->width, image_data->height, 0);
}

bool Enemy5::Collision(Shot& shot)
{
	int shot_x = 0;
	int shot_y = 0;
	shot.GetPosition(shot_x, shot_y);
	if((abs(this->x - shot_x) < SHOT_COLLISION_WIDTH) && (abs(this->y - shot_y) < SHOT_COLLISION_HEIGHT))
	{
		shot.SetStatus(Shot::STATUS_DEAD);
		if(this->hit_interval == 0)
		{
			-- this->hp;
			if(this->hp <= 0)
			{
				return true;
			}
			this->hit_interval = HIT_INTERVAL;
		}
	}
	return false;
}

bool Enemy5::Collision(int x, int y)
{
	if((abs(this->x - x) < PLAYER_COLLISION_WIDTH) && (abs(this->y - y) < PLAYER_COLLISION_HEIGHT))
	{
		return true;
	}
	return false;
}
