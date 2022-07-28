#include <stdlib.h>
#include "TinyLibrary.hpp"
#include "Resource.hpp"
#include "Shot.hpp"

Shot::Shot(void)
:x(0)
,y(0)
,vx(0)
,vy(0)
,chr(0)
,status(STATUS_DEAD)
{
}

Shot::~Shot(void)
{
}

void Shot::Initialize(int x, int y, int vx, int vy, int chr)
{
	this->x = x;
	this->y = y;
	this->vx = vx >> 1;
	this->vy = vy >> 1;
	this->chr = chr;
	this->status = STATUS_ALIVE;
}

void Shot::Update(void)
{
	if(this->status == STATUS_DEAD)
	{
		return;
	}
	this->x += this->vx;
	this->y += this->vy;
	if(this->y < -TOP_LIMIT)
	{
		this->status = STATUS_DEAD;
	}
}

void Shot::Draw(void)
{
	if(this->status == STATUS_DEAD)
	{
		return;
	}
	Screen& screen = Screen::GetInstance();
	const ImageData* image_data;
	image_data = GetImageData(this->chr);
	screen.DrawSprite(image_data->buffer, (this->x >> 10) - (image_data->width >> 1), (this->y >> 10) - (image_data->height > 1), image_data->width, image_data->height, 0);
}

unsigned int Shot::GetStatus(void)
{
	return this->status;
}

void Shot::SetStatus(unsigned int status)
{
	this->status = status;
}

void Shot::GetPosition(int& x, int& y)
{
	x = this->x;
	y = this->y;
}

// result (true : hit)
bool Shot::Collision(Enemy* enemy)
{
	if(this->status == STATUS_DEAD)
	{
		return false;
	}
	return false;
}
