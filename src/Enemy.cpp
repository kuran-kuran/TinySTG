#include <stdlib.h>
#include "Enemy.hpp"

Enemy::Enemy(void)
:type(0)
,x(0)
,y(0)
,vx(0)
,vy(0)
,hp(1)
,status(STATUS_DEAD)
,item(0)
{
}

Enemy::~Enemy(void)
{
}

void Enemy::Initialize(int type, int x, int y)
{
	this->type = type;
	this->x = x;
	this->y = y;
	this->status = STATUS_ALIVE;
}

void Enemy::Move(void)
{
	this->x += (this->vx >> 1);
	this->y += (this->vy >> 1);
}

unsigned int Enemy::GetStatus(void)
{
	return this->status;
}

void Enemy::SetStatus(unsigned int status)
{
	this->status = status;
}

void Enemy::GetPosition(int& x, int& y)
{
	x = this->x;
	y = this->y;
}

void Enemy::GetVector(int& vx, int& vy)
{
	vx = this->vx;
	vy = this->vy;
}

bool Enemy::Collision(Shot& shot)
{
	int shot_x = 0;
	int shot_y = 0;
	shot.GetPosition(shot_x, shot_y);
	if((abs(this->x - shot_x) < SHOT_COLLISION_WIDTH) && (abs(this->y - shot_y) < SHOT_COLLISION_HEIGHT))
	{
		-- this->hp;
		shot.SetStatus(Shot::STATUS_DEAD);
		if(this->hp <= 0)
		{
			return true;
		}
	}
	return false;
}

bool Enemy::Collision(int x, int y)
{
	if((abs(this->x - x) < PLAYER_COLLISION_WIDTH) && (abs(this->y - y) < PLAYER_COLLISION_HEIGHT))
	{
		return true;
	}
	return false;
}

int Enemy::GetType(void)
{
	return this->type;
}

void Enemy::SetItem(int item)
{
	this->item = item;
}

int Enemy::GetItem(void)
{
	return this->item;
}

bool Enemy::Damage(void)
{
	this->hp -= 2;
	if(this->hp <= 0)
	{
		return true;
	}
	return false;
}
