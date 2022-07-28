#include <stdlib.h>
#include "TinyLibrary.hpp"
#include "Resource.hpp"
#include "Boss3.hpp"
#include "Global.hpp"

Boss3::Boss3(void)
:bullet_count(BULLET_COUNT_START)
,bullet_count2(BULLET_COUNT2)
,phase(PHASE_APPEAR)
,hit_interval(0)
{
}

Boss3::~Boss3(void)
{
}

void Boss3::Initialize(int type, int x, int y)
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

void Boss3::Update(void)
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
		if(this->y > (32 << 10))
		{
			this->vy = -2 << 10;
			this->phase = PHASE_MOVE_UP;
		}
		break;
	case PHASE_MOVE_UP:
		this->vy += 50;
		if(this->y < (0 << 10))
		{
			this->vx = (Mathmatics::Rand() & 1) ? -1 << 7 : 1 << 7;
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
			if(this->y < (0 << 10))
			{
				this->vy = -this->vy;
			}
		}
		else
		{
			if(this->y > (10 << 10))
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
		if(this->bullet_count2 == 50)
		{
			int speed = 800;
			global.game->SetBullet(this->x, this->y + (20 << 10), (724 * speed) >> 10, (724 * speed) >> 10);
			global.game->SetBullet(this->x, this->y + (20 << 10), (265 * speed) >> 10, (989 * speed) >> 10);
			global.game->SetBullet(this->x, this->y + (20 << 10), (-263 * speed) >> 10, (990 * speed) >> 10);
			global.game->SetBullet(this->x, this->y + (20 << 10), (-723 * speed) >> 10, (725 * speed) >> 10);
		}
		if(this->bullet_count2 == 40)
		{
			int speed = 800;
			global.game->SetBullet(this->x, this->y + (20 << 10), (887 * speed) >> 10, (512 * speed) >> 10);
			global.game->SetBullet(this->x, this->y + (20 << 10), (512 * speed) >> 10, (887 * speed) >> 10);
			global.game->SetBullet(this->x, this->y + (20 << 10), 0, (1024 * speed) >> 10);
			global.game->SetBullet(this->x, this->y + (20 << 10), (-510 * speed) >> 10, (888 * speed) >> 10);
			global.game->SetBullet(this->x, this->y + (20 << 10), (-886 * speed) >> 10, (512 * speed) >> 10);
		}
		if(this->bullet_count2 == 30)
		{
			int speed = 800;
			global.game->SetBullet(this->x, this->y + (20 << 10), (724 * speed) >> 10, (724 * speed) >> 10);
			global.game->SetBullet(this->x, this->y + (20 << 10), (265 * speed) >> 10, (989 * speed) >> 10);
			global.game->SetBullet(this->x, this->y + (20 << 10), (-263 * speed) >> 10, (990 * speed) >> 10);
			global.game->SetBullet(this->x, this->y + (20 << 10), (-723 * speed) >> 10, (725 * speed) >> 10);
		}
		if(this->bullet_count2 == 20)
		{
			if((global.game->FireBullet() == true) || (global.game->FireBullet() == true) || (global.game->FireBullet() == true))
			{
				int speed = 800;
				global.game->SetBullet(this->x, this->y + (20 << 10), (887 * speed) >> 10, (512 * speed) >> 10);
				global.game->SetBullet(this->x, this->y + (20 << 10), (512 * speed) >> 10, (887 * speed) >> 10);
				global.game->SetBullet(this->x, this->y + (20 << 10), 0, (1024 * speed) >> 10);
				global.game->SetBullet(this->x, this->y + (20 << 10), (-510 * speed) >> 10, (888 * speed) >> 10);
				global.game->SetBullet(this->x, this->y + (20 << 10), (-886 * speed) >> 10, (512 * speed) >> 10);
			}
			else
			{
				this->bullet_count2 = BULLET_COUNT2;
			}
		}
		if(this->bullet_count2 == 10)
		{
			if((global.game->FireBullet() == true) || (global.game->FireBullet() == true))
			{
				int speed = 800;
				global.game->SetBullet(this->x, this->y + (20 << 10), (724 * speed) >> 10, (724 * speed) >> 10);
				global.game->SetBullet(this->x, this->y + (20 << 10), (265 * speed) >> 10, (989 * speed) >> 10);
				global.game->SetBullet(this->x, this->y + (20 << 10), (-263 * speed) >> 10, (990 * speed) >> 10);
				global.game->SetBullet(this->x, this->y + (20 << 10), (-723 * speed) >> 10, (725 * speed) >> 10);
			}
			else
			{
				this->bullet_count2 = BULLET_COUNT2;
			}
		}
	}
	-- this->bullet_count;
	if((this->bullet_count < 0) && (global.game->FireBullet() == true))
	{
		global.game->SetBulletToFighter(this->x, this->y + (6 << 10), global.game->GetBulletSpeed(), Game::BULLET_TYPE_DIFFUSE, 1 + (Mathmatics::Rand() & 1));
		this->bullet_count = BULLET_COUNT;
	}
	if(this->hit_interval > 0)
	{
		-- this->hit_interval;
	}
}

void Boss3::Draw(void)
{
	if(this->status == STATUS_DEAD)
	{
		return;
	}
	Screen& screen = Screen::GetInstance();
	const ImageData* image_data;
	image_data = GetImageData(BOSS3);
	int x = (this->x - WIDTH_HALF) >> 10;
	int y = (this->y - HEIGHT_HALF) >> 10;
	screen.DrawSprite(image_data->buffer, x, y, image_data->width, image_data->height, 0);
}

bool Boss3::Collision(Shot& shot)
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

bool Boss3::Collision(int x, int y)
{
	if((abs(this->x - x) < PLAYER_COLLISION_WIDTH) && (abs(this->y - y) < PLAYER_COLLISION_HEIGHT))
	{
		return true;
	}
	return false;
}
