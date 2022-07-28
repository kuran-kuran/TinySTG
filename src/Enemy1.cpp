#include <stdlib.h>
#include "TinyLibrary.hpp"
#include "Resource.hpp"
#include "Enemy1.hpp"
#include "Global.hpp"

Enemy1::Enemy1(void)
{
	this->bullet_count = Mathmatics::Rand() % BULLET_COUNT;
}

Enemy1::~Enemy1(void)
{
}

void Enemy1::Initialize(int type, int x, int y)
{
	this->type = type;
	this->x = x;
	this->y = y;
	if(x < (Screen::WIDTH << 9))
	{
		this->vx = 1 << 10;
	}
	else
	{
		this->vx = -1 << 10;
	}
	this->vy = 0;
	this->hp = 2;
	this->status = STATUS_ALIVE;
}

void Enemy1::Update(void)
{
	if(this->status == STATUS_DEAD)
	{
		return;
	}
	Global& global = Global::GetInstance();
	Move();
	if((this->vx < 0) && (this->x < (-10 << 10)))
	{
		this->status = STATUS_DEAD;
	}
	else if((this->vx > 0) && (this->x > (Screen::WIDTH << 10) + WIDTH_HALF))
	{
		this->status = STATUS_DEAD;
	}
	this->vy -= 7;
	-- this->bullet_count;
	if((this->bullet_count < 0) && (global.game->FireBullet() == true))
	{
		global.game->SetBulletToFighter(this->x, this->y + (2 << 10), global.game->GetBulletSpeed(), Game::BULLET_TYPE_NORMAL, 1);
		this->bullet_count = BULLET_COUNT;
	}
}

void Enemy1::Draw(void)
{
	if(this->status == STATUS_DEAD)
	{
		return;
	}
	Screen& screen = Screen::GetInstance();
	const ImageData* image_data;
	image_data = GetImageData(ENEMY1);
	int x = (this->x - WIDTH_HALF) >> 10;
	int y = (this->y - HEIGHT_HALF) >> 10;
	screen.DrawSprite(image_data->buffer, x, y, image_data->width, image_data->height, 0);
}
