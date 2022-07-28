#include <stdlib.h>
#include "TinyLibrary.hpp"
#include "Resource.hpp"
#include "Boss1.hpp"
#include "Global.hpp"

Boss1::Boss1(void)
:bullet_count(BULLET_COUNT_START)
,phase(PHASE_APPEAR)
,hit_interval(0)
{
}

Boss1::~Boss1(void)
{
}

void Boss1::Initialize(int type, int x, int y)
{
	this->type = type;
	this->x = x;
	this->y = y;
	this->vx = 0;
	this->vy = 1024 / 2;
	this->hp = 60;
	this->status = STATUS_ALIVE;
	this->phase = PHASE_APPEAR;
}

void Boss1::Update(void)
{
	if(this->status == STATUS_DEAD)
	{
		return;
	}
	Global& global = Global::GetInstance();
	Move();
	switch(this->phase)
	{
	case PHASE_APPEAR:
		if(this->y > (20 << 10))
		{
			this->phase = PHASE_MOVE;
			this->vx = (Mathmatics::Rand() & 1) ? -1 << 8 : 1 << 8;
			this->vy = (Mathmatics::Rand() & 1) ? -1 << 8 : 1 << 8;
		}
		break;
	case PHASE_MOVE:
		if(this->vx < 0)
		{
			if(this->x < (25 << 10))
			{
				this->vx = -this->vx;
			}
		}
		else
		{
			if(this->x > (71 << 10))
			{
				this->vx = -this->vx;
			}
		}
		if(this->vy < 0)
		{
			if(this->y < (5 << 10))
			{
				this->vy = -this->vy;
			}
		}
		else
		{
			if(this->y > (25 << 10))
			{
				this->vy = -this->vy;
			}
		}
		break;
	}
	-- this->bullet_count;
	if((this->bullet_count < 0) && (global.game->FireBullet() == true))
	{
		global.game->SetBulletToFighter(this->x, this->y, global.game->GetBulletSpeed(), Game::BULLET_TYPE_DIFFUSE, 2 + Mathmatics::Rand() % 3);
		this->bullet_count = BULLET_COUNT;
	}
	if(this->hit_interval > 0)
	{
		-- this->hit_interval;
	}
}

void Boss1::Draw(void)
{
	if(this->status == STATUS_DEAD)
	{
		return;
	}
	Screen& screen = Screen::GetInstance();
	const ImageData* image_data;
	image_data = GetImageData(BOSS1);
	int x = (this->x - WIDTH_HALF) >> 10;
	int y = (this->y - HEIGHT_HALF) >> 10;
	screen.DrawSprite(image_data->buffer, x, y, image_data->width, image_data->height, 0);
}

bool Boss1::Collision(Shot& shot)
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

bool Boss1::Collision(int x, int y)
{
	if((abs(this->x - x) < PLAYER_COLLISION_WIDTH) && (abs(this->y - y) < PLAYER_COLLISION_HEIGHT))
	{
		return true;
	}
	return false;
}
