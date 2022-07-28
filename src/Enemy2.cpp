#include <stdlib.h>
#include "TinyLibrary.hpp"
#include "Resource.hpp"
#include "Enemy2.hpp"
#include "Global.hpp"

Enemy2::Enemy2(void)
{
	this->bullet_count = Mathmatics::Rand() % BULLET_COUNT;
}

Enemy2::~Enemy2(void)
{
}

void Enemy2::Initialize(int type, int x, int y)
{
	this->type = type;
	this->x = x;
	this->y = y;
	this->vx = 0;
	this->vy = 1 << 10;
	this->hp = 1;
	this->status = STATUS_ALIVE;
}

void Enemy2::Update(void)
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
	if(this->y > ((Screen::HEIGHT / 3) << 10))
	{
		this->vy += 40;
	}
	-- this->bullet_count;
	if((this->bullet_count < 0) && (global.game->FireBullet() == true))
	{
		global.game->SetBulletToFighter(this->x, this->y, global.game->GetBulletSpeed(), Game::BULLET_TYPE_NORMAL, 1);
		this->bullet_count = BULLET_COUNT;
	}
}

void Enemy2::Draw(void)
{
	if(this->status == STATUS_DEAD)
	{
		return;
	}
	Screen& screen = Screen::GetInstance();
	const ImageData* image_data;
	image_data = GetImageData(ENEMY2);
	int x = (this->x - WIDTH_HALF) >> 10;
	int y = (this->y - HEIGHT_HALF) >> 10;
	screen.DrawSprite(image_data->buffer, x, y, image_data->width, image_data->height, 0);
}
