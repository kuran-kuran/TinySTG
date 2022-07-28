#include <stdlib.h>
#include "TinyLibrary.hpp"
#include "Resource.hpp"
#include "PowerUp.hpp"
#include "Global.hpp"

PowerUp::PowerUp(void)
{
}

PowerUp::~PowerUp(void)
{
}

void PowerUp::Initialize(int type, int x, int y)
{
	this->type = type;
	this->x = x;
	this->y = y;
	this->vx = (Mathmatics::Rand() % 500) - 250;
	if((Mathmatics::Rand() % 1000) < 500)
	{
		this->vy = 150 + Mathmatics::Rand() % 100;
	}
	else
	{
		this->vy = -(150 + Mathmatics::Rand() % 100);
	}
	this->hp = 0;
	this->status = STATUS_ALIVE;
}

void PowerUp::Update(void)
{
	if(this->status == STATUS_DEAD)
	{
		return;
	}
	Global& global = Global::GetInstance();
	this->x += this->vx;
	this->y += this->vy;
	if(this->x < LEFT_LIMIT)
	{
		this->x = LEFT_LIMIT;
		this->vx = -this->vx;
	}
	else if(this->x >= RIGHT_LIMIT)
	{
		this->x = RIGHT_LIMIT;
		this->vx = -this->vx;
	}
	if(this->y < TOP_LIMIT)
	{
		this->y = TOP_LIMIT;
		this->vy = -this->vy;
	}
	else if(this->y >= BOTTOM_LIMIT)
	{
		this->y = BOTTOM_LIMIT;
		this->vy = -this->vy;
	}
}

void PowerUp::Draw(void)
{
	if(this->status == STATUS_DEAD)
	{
		return;
	}
	Screen& screen = Screen::GetInstance();
	const ImageData* image_data;
	image_data = GetImageData(POWER);
	int x = (this->x - WIDTH_HALF) >> 10;
	int y = (this->y - HEIGHT_HALF) >> 10;
	screen.DrawSprite(image_data->buffer, x, y, image_data->width, image_data->height, 0);
}

bool PowerUp::Collision(Shot& shot)
{
	return false;
}

bool PowerUp::Collision(int x, int y)
{
	if((abs(this->x - x) < PLAYER_COLLISION_WIDTH) && (abs(this->y - y) < PLAYER_COLLISION_HEIGHT))
	{
		return true;
	}
	return false;
}
