#include <stdlib.h>
#include "TinyLibrary.hpp"
#include "Player.hpp"
#include "Resource.hpp"
#include "Global.hpp"
#include "Bullet.hpp"

int Player::START_X;
int Player::START_Y;
int Player::START_TAKEOFF_Y;
int Player::LIMIT_X;
int Player::LIMIT_Y;

Player::Player(void)
:x(START_X)
,y(START_TAKEOFF_Y)
,vx(0)
,vy(0)
,status(STATUS_ALIVE)
,phase(PHASE_TAKEOFF)
,draw_counter(0)
,counter(0)
,shot_cool_time_count(0)
,shot_power(1)
,before_bomber(false)
,bomber(2)
{
	Player::START_X =(Screen::WIDTH / 2 - 5) << 10;
	Player::START_Y = (Screen::HEIGHT - 19) << 10;
	Player::START_TAKEOFF_Y = (Screen::HEIGHT + 6) << 10;
	Player::LIMIT_X = (Screen::WIDTH << 10) - WIDTH_HALF;
	Player::LIMIT_Y = (Screen::HEIGHT << 10) - HEIGHT_HALF;
}

Player::~Player(void)
{
}

void Player::Initialize(void)
{
	this->x = START_X;
	this->y = START_TAKEOFF_Y;
	this->vx = 0;
	this->vy = 0;
	this->status = STATUS_ALIVE;
	this->phase = PHASE_TAKEOFF;
	this->draw_counter = 0;
	this->counter = 0;
	this->shot_cool_time_count = 0;
	this->shot_power = 1;
	this->before_bomber = false;
	this->bomber = 2;
}

void Player::Update(void)
{
	if(this->status == STATUS_DEAD)
	{
		return;
	}
	Controller& controller = Controller::GetInstance();
	unsigned int button = controller.GetButton();
	this->vx = 0;
	this->vy = 0;
	if(this->phase != PHASE_TAKEOFF)
	{
		if(this->status == STATUS_ALIVE)
		{
			{
				if(button & Controller::BUTTON_LEFT)
				{
					this->vx = -1;
				}
				else if(button & Controller::BUTTON_RIGHT)
				{
					this->vx = 1;
				}
				if(button & Controller::BUTTON_UP)
				{
					this->vy = -1;
				}
				else if(button & Controller::BUTTON_DOWN)
				{
					this->vy = 1;
				}
				if(button & Controller::BUTTON_1)
				{
					if(this->before_bomber == false)
					{
						Bomber();
					}
					this->before_bomber = true;
				}
				else
				{
					this->before_bomber = false;
				}
				if(button & Controller::BUTTON_2)
				{
					-- this->shot_cool_time_count;
					if(this->shot_cool_time_count <= 0)
					{
						Global& global = Global::GetInstance();
						global.game->SetShot();
						this->shot_cool_time_count = SHOT_COOL_TIME;
					}
				}
				if((this->vx != 0) && (this->vy != 0))
				{
					this->vx *= SPEED_ASLANT;
					this->vy *= SPEED_ASLANT;
				}
				else
				{
					this->vx *= SPEED;
					this->vy *= SPEED;
				}
			}
		}
		this->x += this->vx;
		this->y += this->vy;
		if(this->x < WIDTH_HALF)
		{
			this->x = WIDTH_HALF;
		}
		else if(this->x > LIMIT_X)
		{
			this->x = LIMIT_X;
		}
		if(this->y < HEIGHT_HALF)
		{
			this->y = HEIGHT_HALF;
		}
		else if(this->y > LIMIT_Y)
		{
			this->y = LIMIT_Y;
		}
		if(this->counter > 0)
		{
			-- this->counter;
			if(this->counter == 0)
			{
				this->phase = PHASE_MOVE;
			}
		}
	}
	else
	{
		if(this->y > START_Y)
		{
			this->vy = -SPEED / 3;
			this->y += this->vy;
		}
		else
		{
			this->phase = PHASE_INVINCIBLE;
			this->counter = Screen::FPS * 2;
		}
	}
	++ this->draw_counter;
	if(this->draw_counter > 8)
	{
		this->draw_counter = 0;
	}
}

void Player::Draw(void)
{
	Screen& screen = Screen::GetInstance();
	if(this->status == STATUS_DEAD)
	{
		return;
	}
	if((this->phase < PHASE_MOVE) && (this->draw_counter < 4))
	{
		return;
	}
	int x = (this->x - WIDTH_HALF) >> 10;
	int y = (this->y - HEIGHT_HALF) >> 10;
	const ImageData* image_data = GetImageData(FIGHTER);
	screen.DrawSprite(image_data->buffer, x, y, image_data->width, image_data->height, 0);
}

unsigned int Player::GetStatus(void)
{
	return this->status;
}

void Player::SetStatus(unsigned int status)
{
	this->status = status;
}

void Player::GetPosition(int& x, int& y)
{
	x = this->x;
	y = this->y;
}

void Player::GetVector(int& vx, int& vy)
{
	vx = this->vx;
	vy = this->vy;
}

// result (true : hit)
bool Player::Collision(Bullet& bullet)
{
	if(this->phase != PHASE_MOVE)
	{
		return false;
	}
	if(this->status == STATUS_DEAD)
	{
		return false;
	}
	int x;
	int y;
	bullet.GetPosition(x, y);
	if((abs(this->x - x) <= Bullet::WIDTH_HALF) && (abs(this->y - y) <= Bullet::HEIGHT_HALF))
	{
		return true;
	}
	return false;
}

int Player::GetShotPower(void)
{
	return this->shot_power;
}

bool Player::ShotPowerUp(void)
{
	if(this->shot_power >= SHOT_UP_MAX)
	{
		return false;
	}
	++ this->shot_power;
	return true;
}

bool Player::IsInvincinle(void)
{
	if(this->phase < PHASE_MOVE)
	{
		return true;
	}
	return false;
}

int Player::GetBomber(void)
{
	return this->bomber;
}

bool Player::Bomber(void)
{
	if(this->bomber > 0)
	{
		Global& global = Global::GetInstance();
		global.game->Bomber();
		-- this->bomber;
		return true;
	}
	return false;
}
