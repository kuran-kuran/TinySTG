#include <stdlib.h>
#include "Resource.hpp"
#include "Bullet.hpp"

Bullet::Bullet(void)
:x(0)
,y(0)
,vx(0)
,vy(0)
,status(STATUS_DEAD)
,animation_count(0)
{
}

Bullet::~Bullet(void)
{
}

void Bullet::Initialize(int x, int y, int vx, int vy)
{
	this->x = x;
	this->y = y;
	this->vx = vx >> 1;
	this->vy = vy >> 1;
	this->status = STATUS_ALIVE;
	this->animation_count = 0;
}

void Bullet::Update(void)
{
	if(this->status == STATUS_DEAD)
	{
		return;
	}
	this->x += this->vx;
	this->y += this->vy;
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
	++ this->animation_count;
	if(this->animation_count >= 8)
	{
		this->animation_count = 0;
	}
}

void Bullet::Draw(void)
{
	if(this->status == STATUS_DEAD)
	{
		return;
	}
	Screen& screen = Screen::GetInstance();
	const ImageData* image_data;
	int chr = BULLET1;
	if(this->animation_count < 4)
	{
		chr = BULLET2;
	}
	image_data = GetImageData(chr);
	int x = (this->x - WIDTH_HALF) >> 10;
	int y = (this->y - HEIGHT_HALF) >> 10;
	screen.DrawSprite(image_data->buffer, x, y, image_data->width, image_data->height, 0);
}

unsigned int Bullet::GetStatus(void)
{
	return this->status;
}

void Bullet::GetPosition(int& x, int& y)
{
	x = this->x;
	y = this->y;
}
