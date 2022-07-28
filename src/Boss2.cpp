#include <stdlib.h>
#include "TinyLibrary.hpp"
#include "Resource.hpp"
#include "Boss2.hpp"
#include "Global.hpp"

Boss2::Boss2(void)
:bullet_count(BULLET_COUNT)
,phase(PHASE_APPEAR)
,hit_interval(0)
,bullet_phase(BULLET_PHASE_WAIT1)
,bullet_phase_count(BULLET_COUNT)
{
}

Boss2::~Boss2(void)
{
}

void Boss2::Initialize(int type, int x, int y)
{
	this->type = type;
	this->x = x;
	this->y = y;
	this->vx = 0;
	this->vy = 1 << 7;
	this->hp = 80;
	this->status = STATUS_ALIVE;
	this->phase = PHASE_APPEAR;
	this->bullet_phase = BULLET_PHASE_WAIT2;
	this->bullet_phase_count = BULLET_COUNT;
}

void Boss2::Update(void)
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
		if(this->y > (15 << 10))
		{
			this->phase = PHASE_MOVE;
			this->vx = (Mathmatics::Rand() & 1) ? -1 << 7 : 1 << 7;
			this->vy = (Mathmatics::Rand() & 1) ? -1 << 7 : 1 << 7;
		}
		break;
	case PHASE_MOVE:
		if(this->vx < 0)
		{
			if(this->x < (35 << 10))
			{
				this->vx = -this->vx;
			}
		}
		else
		{
			if(this->x > (61 << 10))
			{
				this->vx = -this->vx;
			}
		}
		if(this->vy < 0)
		{
			if(this->y < (0 << 10))
			{
				this->vy = -this->vy;
			}
		}
		else
		{
			if(this->y > (15 << 10))
			{
				this->vy = -this->vy;
			}
		}
		break;
	}
	-- this->bullet_phase_count;
	-- this->bullet_count;
	switch(this->bullet_phase)
	{
	case BULLET_PHASE_WAIT1:
		if(this->bullet_phase_count < 0)
		{
			this->bullet_phase = BULLET_PHASE_ATTACK1;
			this->bullet_phase_count = BULLET_COUNT;
		}
		break;
	case BULLET_PHASE_ATTACK1:
		if((this->bullet_count < 0) && (global.game->FireBullet() == true))
		{
			global.game->SetBulletToFighter(this->x - (11 << 10), this->y + (6 << 10), global.game->GetBulletSpeed(), Game::BULLET_TYPE_NORMAL, 1);
			global.game->SetBulletToFighter(this->x + (11 << 10), this->y + (6 << 10), global.game->GetBulletSpeed(), Game::BULLET_TYPE_NORMAL, 1);
			this->bullet_count = 25;
		}
		if(((this->bullet_phase_count % 30) == 0) && (global.game->FireBullet() == true))
		{
			global.game->SetBulletToFighter(this->x - (15 << 10), this->y + (6 << 10), global.game->GetBulletSpeed(), Game::BULLET_TYPE_NORMAL, 1);
			global.game->SetBulletToFighter(this->x + (15 << 10), this->y + (6 << 10), global.game->GetBulletSpeed(), Game::BULLET_TYPE_NORMAL, 1);
		}
		if(this->bullet_phase_count < 0)
		{
			this->bullet_phase = BULLET_PHASE_WAIT2;
			this->bullet_phase_count = 40;
		}
		break;
	case BULLET_PHASE_WAIT2:
		if(this->bullet_phase_count < 0)
		{
			this->bullet_phase = BULLET_PHASE_ATTACK2;
			this->bullet_phase_count = BULLET_COUNT;
		}
		break;
	case BULLET_PHASE_ATTACK2:
		if((this->bullet_count < 0) && (global.game->FireBullet() == true))
		{
			global.game->SetBulletToFighter(this->x, this->y + (4 << 10), global.game->GetBulletSpeed(), Game::BULLET_TYPE_DIFFUSE, 4);
			this->bullet_count = 45;
		}
		if(((this->bullet_phase_count % 3) == 0))
		{
			global.game->SetBullet(this->x - (22 << 10), this->y + (5 << 10), 0, 2 << 10);
			global.game->SetBullet(this->x + (22 << 10), this->y + (5 << 10), 0, 2 << 10);
		}
		if(this->bullet_phase_count < 0)
		{
			this->bullet_phase = BULLET_PHASE_WAIT1;
			this->bullet_phase_count = 40;
		}
		break;
	}
	if(this->hit_interval > 0)
	{
		-- this->hit_interval;
	}
}

void Boss2::Draw(void)
{
	if(this->status == STATUS_DEAD)
	{
		return;
	}
	Screen& screen = Screen::GetInstance();
	const ImageData* image_data;
	image_data = GetImageData(BOSS2);
	int x = (this->x - WIDTH_HALF) >> 10;
	int y = (this->y - HEIGHT_HALF) >> 10;
	screen.DrawSprite(image_data->buffer, x, y, image_data->width, image_data->height, 0);
}

bool Boss2::Collision(Shot& shot)
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

bool Boss2::Collision(int x, int y)
{
	if((abs(this->x - x) < PLAYER_COLLISION_WIDTH) && (abs(this->y - y) < PLAYER_COLLISION_HEIGHT))
	{
		return true;
	}
	return false;
}
