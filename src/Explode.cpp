#include <stdlib.h>
#include "Resource.hpp"
#include "Explode.hpp"

Explode::Explode(void)
:x(0)
,y(0)
,vx(0)
,vy(0)
,status(STATUS_DEAD)
,animation_count(0)
{
}

Explode::~Explode(void)
{
}

void Explode::Initialize(int x, int y, int vx, int vy)
{
	this->x = x;
	this->y = y;
	this->vx = vx >> 1;
	this->vy = vy >> 1;
	this->status = STATUS_ALIVE;
	this->animation_count = 0;
}

void Explode::Update(void)
{
	if(this->status == STATUS_DEAD)
	{
		return;
	}
	this->x += this->vx;
	this->y += this->vy;
	++ this->animation_count;
	if(this->animation_count > 31)
	{
		this->status = STATUS_DEAD;
	}
}

void Explode::Draw(void)
{
	if(this->status == STATUS_DEAD)
	{
		return;
	}
	Screen& screen = Screen::GetInstance();
	const ImageData* image_data;
	int chr = EXPLODE1 + (this->animation_count >> 2);
	image_data = GetImageData(chr);
	int x = (this->x - WIDTH_HALF) >> 10;
	int y = (this->y - HEIGHT_HALF) >> 10;
	screen.DrawSprite(image_data->buffer, x, y, image_data->width, image_data->height, 0);
}

unsigned int Explode::GetStatus(void)
{
	return this->status;
}
