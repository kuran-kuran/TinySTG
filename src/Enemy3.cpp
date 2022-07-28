#include <stdlib.h>
#include "TinyLibrary.hpp"
#include "Resource.hpp"
#include "Enemy3.hpp"
#include "Global.hpp"

Enemy3::Enemy3(void)
:turn_count(50)
,turn(false)
{
}

Enemy3::~Enemy3(void)
{
}

void Enemy3::Initialize(int type, int x, int y)
{
	Global& global = Global::GetInstance();
	this->type = type;
	this->x = x;
	this->y = y;
	int fighter_x;
	int fighter_y;
	global.game->GetFighterPosition(fighter_x, fighter_y);
	global.game->GetVector(this->vx, this->vy, fighter_x, fighter_y, this->x, this->y, 1 << 10);
	this->hp = 1;
	this->status = STATUS_ALIVE;
}

void Enemy3::Update(void)
{
	if(this->status == STATUS_DEAD)
	{
		return;
	}
	Global& global = Global::GetInstance();
	Move();
	if(this->turn == true)
	{
		if(this->x < LEFT_LIMIT)
		{
			this->status = STATUS_DEAD;
		}
		else if(this->x >= RIGHT_LIMIT)
		{
			this->status = STATUS_DEAD;
		}
		if(this->y < TOP_LIMIT)
		{
			this->status = STATUS_DEAD;
		}
		else if(this->y >= BOTTOM_LIMIT)
		{
			this->status = STATUS_DEAD;
		}
	}
	-- this->turn_count;
	if(this->turn_count < 0)
	{
		if(this->turn == false)
		{
			int fighter_x;
			int fighter_y;
			global.game->GetFighterPosition(fighter_x, fighter_y);
			global.game->GetVector(this->vx, this->vy, fighter_x, fighter_y, this->x, this->y, 1 << 10);
			this->vx = -this->vx;
			this->vy = -this->vy;
			this->turn = true;
		}
		if(((this->turn_count & 7) == 0) && (global.game->FireBullet() == true))
		{
			global.game->SetBulletToFighter(this->x, this->y, global.game->GetBulletSpeed(), Game::BULLET_TYPE_NORMAL, 1);
		}
	}
}

void Enemy3::Draw(void)
{
	if(this->status == STATUS_DEAD)
	{
		return;
	}
	Screen& screen = Screen::GetInstance();
	const ImageData* image_data;
	image_data = GetImageData(ENEMY3);
	int x = (this->x - WIDTH_HALF) >> 10;
	int y = (this->y - HEIGHT_HALF) >> 10;
	screen.DrawSprite(image_data->buffer, x, y, image_data->width, image_data->height, 0);
}
