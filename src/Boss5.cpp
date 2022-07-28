#include <stdlib.h>
#include "TinyLibrary.hpp"
#include "Resource.hpp"
#include "Boss5.hpp"
#include "Global.hpp"

Boss5::Boss5(void)
:bullet_count(BULLET_COUNT_START)
,bullet_count2(BULLET_COUNT2)
,bullet_count3(0)
,phase(PHASE_APPEAR)
,hit_interval(0)
{
}

Boss5::~Boss5(void)
{
}

void Boss5::Initialize(int type, int x, int y)
{
	this->type = type;
	this->x = x;
	this->y = y;
	this->vx = 0;
	this->vy = 1024 / 2;
	this->hp = 90;
	this->status = STATUS_ALIVE;
	this->phase = PHASE_APPEAR;
}

void Boss5::Update(void)
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
		if(this->y > (16 << 10))
		{
			this->vx = (Mathmatics::Rand() & 1) ? -2 << 7 : 2 << 7;
			this->vy = (Mathmatics::Rand() & 1) ? -1 << 6 : 1 << 6;
			this->phase = PHASE_MOVE;
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
			if(this->y < (10 << 10))
			{
				this->vy = -this->vy;
			}
		}
		else
		{
			if(this->y > (20 << 10))
			{
				this->vy = -this->vy;
			}
		}
		break;
	}
	-- this->bullet_count2;
	if(this->bullet_count2 < 0)
	{
		this->bullet_count2 = BULLET_COUNT2;
	}
	if(this->phase == PHASE_MOVE)
	{
		int skip = 20 - global.game->GetDifficulty() * 3;
		if((this->bullet_count2 < 256) && ((this->bullet_count2 % skip) == 0))
		{
			int speed = 6;
			int vx = Mathmatics::Sin8(this->bullet_count2) * speed;
			int vy = Mathmatics::Cos8(this->bullet_count2) * speed;
			global.game->SetBullet(this->x, this->y + (2 << 10), vx, vy);
			vx = Mathmatics::Sin8((this->bullet_count2 + 64) & 255) * speed;
			vy = Mathmatics::Cos8((this->bullet_count2 + 64) & 255) * speed;
			global.game->SetBullet(this->x, this->y + (2 << 10), vx, vy);
			vx = Mathmatics::Sin8((this->bullet_count2 + 128) & 255) * speed;
			vy = Mathmatics::Cos8((this->bullet_count2 + 128) & 255) * speed;
			global.game->SetBullet(this->x, this->y + (2 << 10), vx, vy);
			vx = Mathmatics::Sin8((this->bullet_count2 + 192) & 255) * speed;
			vy = Mathmatics::Cos8((this->bullet_count2 + 192) & 255) * speed;
			global.game->SetBullet(this->x, this->y + (2 << 10), vx, vy);
		}
	}
	-- this->bullet_count;
	if((this->bullet_count2 >= 256) && (this->bullet_count < 0) && (global.game->FireBullet() == true))
	{
		if(this->bullet_count3 == 0)
		{
			global.game->SetBulletToFighter(this->x - (5 << 10), this->y + (8 << 10), global.game->GetBulletSpeed(), Game::BULLET_TYPE_DIFFUSE, 1 + (Mathmatics::Rand() & 1));
			global.game->SetBulletToFighter(this->x + (5 << 10), this->y + (8 << 10), global.game->GetBulletSpeed(), Game::BULLET_TYPE_DIFFUSE, 1 + (Mathmatics::Rand() & 1));
			this->bullet_count3 = 1;
		}
		else
		{
			global.game->SetBulletToFighter(this->x - (5 << 10), this->y + (12 << 10), global.game->GetBulletSpeed(), Game::BULLET_TYPE_DIFFUSE, 1 + (Mathmatics::Rand() & 1));
			global.game->SetBulletToFighter(this->x + (5 << 10), this->y + (12 << 10), global.game->GetBulletSpeed(), Game::BULLET_TYPE_DIFFUSE, 1 + (Mathmatics::Rand() & 1));
			this->bullet_count3 = 0;
		}
		this->bullet_count = BULLET_COUNT;
	}
	if(this->hit_interval > 0)
	{
		-- this->hit_interval;
	}
}

void Boss5::Draw(void)
{
	if(this->status == STATUS_DEAD)
	{
		return;
	}
	Screen& screen = Screen::GetInstance();
	const ImageData* image_data;
	image_data = GetImageData(BOSS5);
	int x = (this->x - WIDTH_HALF) >> 10;
	int y = (this->y - HEIGHT_HALF) >> 10;
	screen.DrawSprite(image_data->buffer, x, y, image_data->width, image_data->height, 0);
}

bool Boss5::Collision(Shot& shot)
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

bool Boss5::Collision(int x, int y)
{
	if((abs(this->x - x) < PLAYER_COLLISION_WIDTH) && (abs(this->y - y) < PLAYER_COLLISION_HEIGHT))
	{
		return true;
	}
	return false;
}
