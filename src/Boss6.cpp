#include <stdlib.h>
#include "TinyLibrary.hpp"
#include "Resource.hpp"
#include "Boss6.hpp"
#include "Global.hpp"

Boss6::Boss6(void)
:bullet_count(BULLET_COUNT)
,phase(PHASE_APPEAR)
,hit_interval(0)
{
}

Boss6::~Boss6(void)
{
}

void Boss6::Initialize(int type, int x, int y)
{
	this->type = type;
	this->x = x;
	this->y = y;
	this->vx = 0;
	this->vy = 1 << 7;
	this->hp = 110;
	this->status = STATUS_ALIVE;
	this->phase = PHASE_APPEAR;
}

void Boss6::Update(void)
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
		if(this->y > (-10 << 10))
		{
			this->vx = 0;
			this->vy = 0;
			this->phase = PHASE_WAIT;
		}
		break;
	case PHASE_WAIT:
		if(this->y > (20 << 10))
		{
			this->vy = 0;
		}
		else
		{
			this->vy = 10;
		}
		break;
	}
	if(this->hit_interval > 0)
	{
		-- this->hit_interval;
	}
	-- this->bullet_count;
	if((this->bullet_count < 0) && (global.game->FireBullet() == true))
	{
		int bullet_count = (this->y <= 0) ? 1 : (1 + (this->y >> 12));
		global.game->SetBulletToFighter(this->x - 20 + Mathmatics::Rand() % 40, this->y - 20 + Mathmatics::Rand() % 40, global.game->GetBulletSpeed(), Game::BULLET_TYPE_DIFFUSE, bullet_count);
		this->bullet_count = BULLET_COUNT;
	}
}

void Boss6::Draw(void)
{
	if(this->status == STATUS_DEAD)
	{
		return;
	}
	Screen& screen = Screen::GetInstance();
	const ImageData* image_data;
	image_data = GetImageData(BOSS6);
	int x = (this->x - WIDTH_HALF) >> 10;
	int y = (this->y - HEIGHT_HALF) >> 10;
	screen.DrawSprite(image_data->buffer, x, y, image_data->width, image_data->height, 0);
}

bool Boss6::Collision(Shot& shot)
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

bool Boss6::Collision(int x, int y)
{
	if((abs(this->x - x) < PLAYER_COLLISION_WIDTH) && (abs(this->y - y) < PLAYER_COLLISION_HEIGHT))
	{
		return true;
	}
	return false;
}
