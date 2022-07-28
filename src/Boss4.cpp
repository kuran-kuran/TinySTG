#include <stdlib.h>
#include "TinyLibrary.hpp"
#include "Resource.hpp"
#include "Boss4.hpp"
#include "Global.hpp"

Boss4::Boss4(void)
:bullet_count(BULLET_COUNT_START)
,phase(PHASE_APPEAR)
,hit_interval(0)
,avoid_count(5)
,avoid_move_count(0)
{
}

Boss4::~Boss4(void)
{
}

void Boss4::Initialize(int type, int x, int y)
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

void Boss4::Update(void)
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
	case PHASE_AVOID:
		if(this->x < (Screen::WIDTH << 9))
		{
			this->phase = PHASE_AVOID_RIGHT;
		}
		else
		{
			this->phase = PHASE_AVOID_LEFT;
		}
		this->avoid_move_count = 15 + (Mathmatics::Rand() % 5);
		break;
	case PHASE_AVOID_LEFT:
		this->vx = -2 << 10;
		-- this->avoid_move_count;
		if(this->avoid_move_count < 12)
		{
			global.game->SetBullet(this->x, this->y - (2 << 10), 0, 2 << 10);
		}
		if(this->avoid_move_count < 0)
		{
			this->avoid_count = 5 + (Mathmatics::Rand() % 5);
			this->vx = (Mathmatics::Rand() & 1) ? -1 << 8 : 1 << 8;
			this->vy = (Mathmatics::Rand() & 1) ? -1 << 8 : 1 << 8;
			this->phase = PHASE_MOVE;
		}
		break;
	case PHASE_AVOID_RIGHT:
		this->vx = 2 << 10;
		-- this->avoid_move_count;
		if(this->avoid_move_count < 12)
		{
			global.game->SetBullet(this->x, this->y - (2 << 10), 0, 2 << 10);
		}
		if(this->avoid_move_count < 0)
		{
			this->avoid_count = 5 + (Mathmatics::Rand() % 5);
			this->vx = (Mathmatics::Rand() & 1) ? -1 << 8 : 1 << 8;
			this->vy = (Mathmatics::Rand() & 1) ? -1 << 8 : 1 << 8;
			this->phase = PHASE_MOVE;
		}
		break;
	}
	-- this->bullet_count;
	if((this->bullet_count < 0) && (global.game->FireBullet() == true))
	{
		global.game->SetBulletToFighter(this->x, this->y - (2 << 10), global.game->GetBulletSpeed(), Game::BULLET_TYPE_DIFFUSE, 4 + Mathmatics::Rand() % 3);
		this->bullet_count = BULLET_COUNT;
	}
	if(this->hit_interval > 0)
	{
		-- this->hit_interval;
	}
}

void Boss4::Draw(void)
{
	if(this->status == STATUS_DEAD)
	{
		return;
	}
	Screen& screen = Screen::GetInstance();
	const ImageData* image_data;
	image_data = GetImageData(BOSS4);
	int x = (this->x - WIDTH_HALF) >> 10;
	int y = (this->y - HEIGHT_HALF) >> 10;
	screen.DrawSprite(image_data->buffer, x, y, image_data->width, image_data->height, 0);
}

bool Boss4::Collision(Shot& shot)
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
			if(this->phase == PHASE_MOVE)
			{
				-- this->avoid_count;
				if(this->avoid_count < 0)
				{
					this->phase = PHASE_AVOID;
					this->avoid_count = 0;
				}
			}
		}
	}
	return false;
}

bool Boss4::Collision(int x, int y)
{
	if((abs(this->x - x) < PLAYER_COLLISION_WIDTH) && (abs(this->y - y) < PLAYER_COLLISION_HEIGHT))
	{
		return true;
	}
	return false;
}
