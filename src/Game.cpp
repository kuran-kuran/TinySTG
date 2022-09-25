#include <stdio.h>
#include <string.h>
#include <math.h>
#include "TinyLibrary.hpp"
#include "Global.hpp"
#include "Resource.hpp"
#include "Sound.hpp"
#include "Shot.hpp"
#include "Bullet.hpp"
#include "Explode.hpp"
#include "Enemy.hpp"
#include "Enemy1.hpp"
#include "Enemy2.hpp"
#include "Enemy3.hpp"
#include "Enemy4.hpp"
#include "Enemy5.hpp"
#include "Boss1.hpp"
#include "Boss2.hpp"
#include "Boss3.hpp"
#include "Boss4.hpp"
#include "Boss5.hpp"
#include "Boss6.hpp"
#include "PowerUp.hpp"
#include "Game.hpp"

Game::Game(void)
:stage(1)
,player()
,bullet_rate(0)
,bullet_rate_min(0)
,bullet_speed(300)
,bullet_speed_add(500)
,spawn(2)
,boss(false)
,boss_appear_count(BOSS_APPEAR_COUNT)
,bullet_rate_interval(100)
,bullet_rate_interval_count(100)
,item_interval(ITEM_INTERVAL)
,bg_y((384 - 64) << 10)
,draw_stage(0)
,draw_warning(0)
,difficulty(0)
,left(LEFT)
,continue_count(0)
,continue_interval(Screen::FPS)
,gameover(true)
,gameover_count(0)
,before_button(0)
,score(0)
,highscore(0)
,enemy_move(0)
{
	for(int i = 0; i < SHOT_MAX; ++ i)
	{
		this->shot[i] = NULL;
	}
	for(int i = 0; i < BULLET_MAX; ++ i)
	{
		this->bullet[i] = NULL;
	}
	for(int i = 0; i < EXPLODE_MAX; ++ i)
	{
		this->explode[i] = NULL;
	}
	for(int i = 0; i < ENEMY_MAX; ++ i)
	{
		this->enemy[i] = NULL;
	}
//	for(int i = 0; i < SOUND_MAX; ++ i)
//	{
//		this->sound_handle[i] = -1;
//	}
}

Game::~Game(void)
{
}

void Game::Initialize(int stage, int difficulty)
{
	Global& global = Global::GetInstance();
	ClearCharacter();
	this->stage = stage;
	this->player.Initialize();
	this->shot_cool_time_count = 0;
	this->difficulty = difficulty;
	for(int i = 0; i < SPAWN_MAX; ++ i)
	{
		this->spawn_info[i].interval = SPAWN_INTERVAL;
		this->spawn_info[i].interval2 = SPAWN_INTERVAL * 3;
		this->spawn_info[i].interval_count = Screen::FPS + i * 30;
		this->spawn_info[i].interval2_count = 0;
		this->spawn_info[i].repeat_count = 0;
		this->spawn_info[i].rapid = 0;
		this->spawn_info[i].spawn_number = 0;
		this->spawn_info[i].enemy_number = 0;
		this->spawn_info[i].spawn_x = 0;
		this->spawn_info[i].spawn_y = 0;
	}
	this->boss = false;
	this->boss_appear_count = BOSS_APPEAR_COUNT;
	this->item_interval = ITEM_INTERVAL;
	this->draw_stage = DRAW_STAGE_TIME;
	this->continue_count = 0;
	this->continue_interval = 30;
	this->gameover = false;
	this->gameover_count = 0;
	this->score = 0;
	this->highscore = global.gamedata.highscore;
	this->enemy_move = 0;
	switch(difficulty)
	{
	case 0:
		this->bullet_rate = 0;
		this->bullet_rate_min = this->bullet_rate;
		this->bullet_rate_interval_count = 300;
		break;
	case 1:
		this->bullet_rate = 20;
		this->bullet_rate_min = this->bullet_rate;
		this->bullet_rate_interval_count = 200;
		break;
	case 2:
		this->bullet_rate = 200;
		this->bullet_rate_min = this->bullet_rate;
		this->bullet_rate_interval_count = 150;
		break;
	case 3:
		this->bullet_rate = 1000;
		this->bullet_rate_min = this->bullet_rate;
		this->bullet_rate_interval_count = 100;
		break;
	}
	this->bullet_rate_interval = this->bullet_rate_interval_count;
	this->left = LEFT;
	Sound& sound = Sound::GetInstance();
	sound.Play(SOUND_BGM);
	// boss test
//	SetEnemy((Screen::WIDTH / 2) << 10, -32 << 10, BOSS5, 0);
}

void Game::Finalize(void)
{
	Global& global = Global::GetInstance();
	Sound& sound = Sound::GetInstance();
	sound.StopAll();
	ClearCharacter();
	global.gamedata.highscore = this->highscore;
	global.score = this->score;
}

bool Game::Update(void)
{
	Controller& controller = Controller::GetInstance();
	unsigned int button = controller.GetButton();
	this->player.Update();
	unsigned int status = this->player.GetStatus();
	int fighter_x = 0;
	int fighter_y = 0;
	this->player.GetPosition(fighter_x, fighter_y);
	if((this->continue_count == 0) && (this->gameover == false) && (status == Player::STATUS_DEAD))
	{
		Sound& sound = Sound::GetInstance();
		sound.Play(SOUND_CRUSH_BOSS);
		SetExplode(fighter_x, fighter_y, 0, 0);
		for(int j = 0; j < (this->player.GetShotPower() >> 1); ++ j)
		{
			SetEnemy(fighter_x, fighter_y, POWER_UP, 0);
		}
		-- this->left;
		if(this->left > 0)
		{
			this->player.Initialize();
		}
		else
		{
			// continue?
			this->continue_count = 9;
		}
	}
	if(this->continue_count > 0)
	{
		if((this->before_button == 0) && (button & (Controller::BUTTON_1 | Controller::BUTTON_2)))
		{
			this->gameover = false;
			this->continue_count = 0;
			this->left = LEFT;
			this->score = 0;
			this->player.Initialize();
		}
	}
	if((this->gameover == true) && (this->gameover_count > 0))
	{
		if((this->before_button == 0) && (button & (Controller::BUTTON_1 | Controller::BUTTON_2)))
		{
			this->gameover_count = 1;
		}
	}
	for(int i = 0; i < SHOT_MAX; ++ i)
	{
		if(this->shot[i] == NULL)
		{
			continue;
		}
		this->shot[i]->Update();
		if(this->shot[i]->GetStatus() == Shot::STATUS_DEAD)
		{
			delete this->shot[i];
			this->shot[i] = NULL;
		}
		else
		{
			for(int j = 0; j < ENEMY_MAX; ++ j)
			{
				if(this->enemy[j] == NULL)
				{
					continue;
				}
				if(this->enemy[j]->GetStatus() == Enemy::STATUS_DEAD)
				{
					continue;
				}
				bool collision = this->enemy[j]->Collision(*this->shot[i]);
				if(collision == true)
				{
					this->enemy[j]->SetStatus(Enemy::STATUS_DEAD);
					int enemy_x;
					int enemy_y;
					int enemy_vx;
					int enemy_vy;
					this->enemy[j]->GetPosition(enemy_x, enemy_y);
					this->enemy[j]->GetVector(enemy_vx, enemy_vy);
					SetExplode(enemy_x, enemy_y, enemy_vx, enemy_vy);
					int item = this->enemy[j]->GetItem();
					if(item > 0)
					{
						SetEnemy(enemy_x, enemy_y, item, 0);
					}
					if(this->enemy[j]->GetType() >= BOSS1)
					{
						for(int k = 0; k < 8; ++ k)
						{
							SetExplode(enemy_x, enemy_y, (Mathmatics::Rand() % 5 - 2) << 10, (Mathmatics::Rand() % 5 - 2) << 10);
						}
						DeleteEnemy();
						this->boss = false;
						this->boss_appear_count = BOSS_APPEAR_COUNT;
						++ this->stage;
						if(this->stage > STAGE_MAX)
						{
							++ this->difficulty;
							if(this->difficulty > 3)
							{
								this->difficulty = 3;
							}
							switch(this->difficulty)
							{
							case 0:
								this->bullet_rate = 0;
								this->bullet_rate_min = this->bullet_rate;
								this->bullet_rate_interval_count = 300;
								break;
							case 1:
								this->bullet_rate = 20;
								this->bullet_rate_min = this->bullet_rate;
								this->bullet_rate_interval_count = 200;
								break;
							case 2:
								this->bullet_rate = 200;
								this->bullet_rate_min = this->bullet_rate;
								this->bullet_rate_interval_count = 150;
								break;
							case 3:
								this->bullet_rate = 1000;
								this->bullet_rate_min = this->bullet_rate;
								this->bullet_rate_interval_count = 100;
								break;
							}
							this->stage = 1;
						}
						if(this->stage == STAGE_MAX)
						{
							this->spawn = 2;
						}
						else
						{
							this->spawn = 3;
						}
						this->item_interval = ITEM_INTERVAL;
						Sound& sound = Sound::GetInstance();
						sound.Play(SOUND_CRUSH_BOSS);
						this->draw_stage = DRAW_STAGE_TIME;
						this->score += 3000;
					}
					else
					{
						if(this->bullet_rate == BULLET_RATE_MAX)
						{
							// revenge bullet
							SetBulletToFighter(enemy_x, enemy_y, GetBulletSpeed(), Game::BULLET_TYPE_NORMAL, 1);
						}
						Sound& sound = Sound::GetInstance();
						sound.Play(SOUND_CRUSH);
						if(this->enemy[j]->GetType() >= ENEMY4)
						{
							this->score += 300;
						}
						else
						{
							this->score += 100;
						}
					}
				}
			}
		}
	}
	for(int i = 0; i < BULLET_MAX; ++ i)
	{
		if(this->bullet[i] == NULL)
		{
			continue;
		}
		this->bullet[i]->Update();
		if(this->bullet[i]->GetStatus() == Bullet::STATUS_DEAD)
		{
			delete this->bullet[i];
			this->bullet[i] = NULL;
		}
		else
		{
			bool collision = this->player.Collision(*this->bullet[i]);
			if(collision == true)
			{
				delete this->bullet[i];
				this->bullet[i] = NULL;
				if(this->player.IsInvincinle() == false)
				{
					this->player.SetStatus(Player::STATUS_DEAD);
					this->bullet_rate /= 2;
					if(this->bullet_rate < this->bullet_rate_min)
					{
						this->bullet_rate = this->bullet_rate_min;
					}
				}
			}
		}
	}
	for(int i = 0; i < EXPLODE_MAX; ++ i)
	{
		if(this->explode[i] == NULL)
		{
			continue;
		}
		this->explode[i]->Update();
		if(this->explode[i]->GetStatus() == Shot::STATUS_DEAD)
		{
			delete this->explode[i];
			this->explode[i] = NULL;
		}
	}
	for(int i = 0; i < ENEMY_MAX; ++ i)
	{
		if(this->enemy[i] == NULL)
		{
			continue;
		}
		if(this->enemy_move == 1)
		{
			this->enemy[i]->Update();
		}
		else
		{
			this->enemy[i]->Move();
		}
		if(this->enemy[i]->GetStatus() == Shot::STATUS_DEAD)
		{
			delete this->enemy[i];
			this->enemy[i] = NULL;
		}
		else
		{
			if(status == Player::STATUS_ALIVE)
			{
				if(this->enemy[i]->Collision(fighter_x, fighter_y) == true)
				{
					if(this->enemy[i]->GetStatus() == Enemy::STATUS_ALIVE)
					{
						if(this->enemy[i]->GetType() == POWER_UP)
						{
							this->enemy[i]->SetStatus(Enemy::STATUS_DEAD);
							bool result = this->player.ShotPowerUp();
							if(result == false)
							{
								// power max
								Sound& sound = Sound::GetInstance();
								sound.Play(SOUND_GET);
								this->score += 1000;
							}
							else
							{
								Sound& sound = Sound::GetInstance();
								sound.Play(SOUND_POWER_UP);
							}
						}
						else if(this->player.IsInvincinle() == false)
						{
							this->player.SetStatus(Player::STATUS_DEAD);
							this->bullet_rate /= 2;
							if(this->bullet_rate < this->bullet_rate_min)
							{
								this->bullet_rate = this->bullet_rate_min;
							}
						}
					}
				}
			}
		}
	}
	this->enemy_move = 1 - this->enemy_move;
	if((this->boss_appear_count > 0) || ((this->boss == true) && (this->stage == STAGE_MAX)))
	{
		Spawn();
		if((this->continue_count == 0) && (this->gameover == false))
		{
			-- this->boss_appear_count;
		}
	}
	else if(this->boss == false)
	{
		SpawnBoss();
	}
	if((this->continue_count == 0) && (this->gameover == false))
	{
		-- this->bullet_rate_interval;
		if(this->bullet_rate_interval <= 0)
		{
			++ this->bullet_rate;
			if(this->bullet_rate > BULLET_RATE_MAX)
			{
				this->bullet_rate = BULLET_RATE_MAX;
			}
//			dms::Report("bullet_rate = %d", this->bullet_rate);
			this->bullet_rate_interval = this->bullet_rate_interval_count;
		}
	}
	if(this->boss == false)
	{
		-- this->item_interval;
		if(this->item_interval <= 0)
		{
			SetEnemy((8 + Mathmatics::Rand() % (Screen::WIDTH - 16)) << 10, -10 << 10, ENEMY4, POWER_UP);
			this->item_interval = ITEM_INTERVAL;
		}
	}
	this->bg_y -= (1 << 5);
	if(this->bg_y < 0)
	{
		this->bg_y = (384 - Screen::HEIGHT) << 10;
	}
	if(this->draw_stage > 0)
	{
		-- this->draw_stage;
	}
	if(this->draw_warning > 0)
	{
		-- this->draw_warning;
	}
	if((this->gameover == false) && (this->continue_count > 0))
	{
		-- this->continue_interval;
		if(this->continue_interval <= 0)
		{
			-- this->continue_count;
			if(this->continue_count <= 0)
			{
				this->gameover = true;
				this->gameover_count = Screen::FPS * 5;
			}
			this->continue_interval = Screen::FPS;
		}
	}
	if(this->score > this->highscore)
	{
		this->highscore = this->score;
	}
	this->before_button = button;
	if(this->gameover_count > 0)
	{
		-- this->gameover_count;
		if(this->gameover_count == 0)
		{
			return true;
		}
	}
	return false;
}

void Game::Draw(void)
{
	Screen& screen = Screen::GetInstance();
	const ImageData* image_data = GetImageData(BG);
	const unsigned char* buffer = reinterpret_cast<const unsigned char*>(image_data->buffer) + ((static_cast<size_t>(this->bg_y >> 10)) * 96);
	int draw_x = (Screen::WIDTH - image_data->width) / 2;
	screen.DrawSprite(buffer, draw_x, 0, image_data->width, 64, -1);
	for(int i = 0; i < ENEMY_MAX; ++ i)
	{
		if(this->enemy[i] == NULL)
		{
			continue;
		}
		this->enemy[i]->Draw();
	}
	for(int i = 0; i < EXPLODE_MAX; ++ i)
	{
		if(this->explode[i] == NULL)
		{
			continue;
		}
		this->explode[i]->Draw();
	}
	for(int i = 0; i < SHOT_MAX; ++ i)
	{
		if(this->shot[i] == NULL)
		{
			continue;
		}
		this->shot[i]->Draw();
	}
	this->player.Draw();
	for(int i = 0; i < BULLET_MAX; ++ i)
	{
		if(this->bullet[i] == NULL)
		{
			continue;
		}
		this->bullet[i]->Draw();
	}
	image_data = GetImageData(FIGHTER_SMALL);
	for(int i = 0; i < this->left - 1; ++ i)
	{
		screen.DrawSprite(image_data->buffer, i * 7, Screen::HEIGHT - image_data->height, image_data->width, image_data->height, 0);
	}
	image_data = GetImageData(BOMBER_SMALL);
	for(int i = 0; i < this->player.GetBomber(); ++ i)
	{
		screen.DrawSprite(image_data->buffer, Screen::WIDTH - (i + 1) * 7, Screen::HEIGHT - image_data->height, image_data->width, image_data->height, 0);
	}
	if((this->continue_count == 0) && (this->gameover == false))
	{
		if(this->draw_stage > 0)
		{
			screen.DrawFont(Screen::CENTER, 18, "STAGE %d", this->stage);
		}
		if(this->draw_warning > 0)
		{
			screen.DrawFont(Screen::CENTER, 8, "WARNING!");
			screen.DrawNumberFont(Screen::CENTER, 16, "the enemy is");
			screen.DrawNumberFont(Screen::CENTER, 22, "approaching fast");
		}
	}
	if(this->continue_count > 0)
	{
		screen.DrawFont(Screen::CENTER, 16, "CONTINUE?");
		screen.DrawFont(Screen::CENTER, 26, "%d", this->continue_count);
	}
	if(this->gameover == true)
	{
		screen.DrawFont(Screen::CENTER, 16, "GAME OVER");
	}
	int score = this->score;
	if(score > 99999999)
	{
		score = 99999999;
	}
	screen.DrawNumberFont(0, 0, "%8d", score);
	int highscore = this->highscore;
	if(highscore > 99999999)
	{
		highscore = 99999999;
	}
	screen.DrawNumberFont(Screen::RIGHT, 0, "%8d", highscore);
}

unsigned char Game::GetBackColor(void)
{
	return 0;
}

void Game::SetShot(void)
{
	// äÔêî
	static const ShotInfo shot_table_odd[Player::SHOT_UP_MAX - 1] =
	{
		{3 << 10, 0 << 10, 0 << 8, -4 << 10, SHOT_CENTER},
		{-2 << 10, 2 << 10, -1 << 8, -4 << 10, SHOT_LEFT},
		{7 << 10, 2 << 10, 1 << 8, -4 << 10, SHOT_RIGHT},
		{-4 << 10, 4 << 10, -2 << 8, -4 << 10, SHOT_LEFT},
		{9 << 10, 4 << 10, 2 << 8, -4 << 10, SHOT_RIGHT},
		{-6 << 10, 6 << 10, -3 << 8, -4 << 10, SHOT_LEFT},
		{11 << 10, 6 << 10, 3 << 8, -4 << 10, SHOT_RIGHT},
	};
	// ãÙêî
	static const ShotInfo shot_table_even[Player::SHOT_UP_MAX] =
	{
		{0 << 10, 0 << 10, 0 << 8, -4 << 10, SHOT_LEFT},
		{5 << 10, 0 << 10, 0 << 8, -4 << 10, SHOT_RIGHT},
		{-2 << 10, 2 << 10, -1 << 8, -4 << 10, SHOT_LEFT},
		{7 << 10, 2 << 10, 1 << 8, -4 << 10, SHOT_RIGHT},
		{-4 << 10, 4 << 10, -2 << 8, -4 << 10, SHOT_LEFT},
		{9 << 10, 4 << 10, 2 << 8, -4 << 10, SHOT_RIGHT},
		{-6 << 10, 6 << 10, -3 << 8, -4 << 10, SHOT_LEFT},
		{11 << 10, 6 << 10, 3 << 8, -4 << 10, SHOT_RIGHT},
	};
	int shot_power = this->player.GetShotPower();
	int shot_free_count = 0;
	for(int i = 0; i < SHOT_MAX; ++ i)
	{
		if(this->shot[i] == NULL)
		{
			++ shot_free_count;
		}
	}
	if(shot_power > shot_free_count)
	{
		return;
	}
	int fighter_x;
	int fighter_y;
	this->player.GetPosition(fighter_x, fighter_y);
	for(int i = 0; i < shot_power; ++ i)
	{
		int x = fighter_x - (3 << 10);
		int y = fighter_y - (5 << 10);
		int vx;
		int vy;
		int chr;
		if((shot_power % 2) == 1)
		{
			x += shot_table_odd[i].x;
			y += shot_table_odd[i].y;
			vx = shot_table_odd[i].vx;
			vy = shot_table_odd[i].vy;
			chr = shot_table_odd[i].chr;
		}
		else
		{
			x += shot_table_even[i].x;
			y += shot_table_even[i].y;
			vx = shot_table_even[i].vx;
			vy = shot_table_even[i].vy;
			chr = shot_table_even[i].chr;
		}
		for(int j = 0; j < SHOT_MAX; ++ j)
		{
			if(this->shot[j] == NULL)
			{
				this->shot[j] = new Shot;
				this->shot[j]->Initialize(x, y, vx, vy, chr);
				break;
			}
		}
	}
	Sound& sound = Sound::GetInstance();
	sound.Play(SOUND_GUN);
}

void Game::GetVector(int& vx, int& vy, int target_x, int target_y, int x, int y, int speed)
{
	float angle = atan2f(static_cast<float>(target_y - y), static_cast<float>(target_x - x));
	vx = static_cast<int>(cosf(angle) * static_cast<float>(speed));
	vy = static_cast<int>(sinf(angle) * static_cast<float>(speed));
}

void Game::SetBulletToFighter(int x, int y, int speed, int type, int count)
{
	if(this->player.GetStatus() == Player::STATUS_DEAD)
	{
		return;
	}
	int fighter_x;
	int fighter_y;
	this->player.GetPosition(fighter_x, fighter_y);
	for(int i = 0; i < count; ++ i)
	{
		for(int j = 0; j < BULLET_MAX; ++ j)
		{
			if(this->bullet[j] == NULL)
			{
				int target_x = fighter_x;
				int target_y = fighter_y;
				if(type == BULLET_TYPE_DIFFUSE)
				{
					target_x += ((Mathmatics::Rand() % 20 - 10) << 10);
					target_y += ((Mathmatics::Rand() % 20 - 10) << 10);
					speed += (Mathmatics::Rand() % 200 - 100);
					if(speed < 0)
					{
						speed = 256;
					}
				}
				int vx;
				int vy;
				GetVector(vx, vy, target_x, target_y, x, y, speed);
				this->bullet[j] = new Bullet;
				this->bullet[j]->Initialize(x, y, vx, vy);
				break;
			}
		}
	}
}

void Game::SetBullet(int x, int y, int vx, int vy)
{
	for(int i = 0; i < BULLET_MAX; ++ i)
	{
		if(this->bullet[i] == NULL)
		{
			this->bullet[i] = new Bullet;
			this->bullet[i]->Initialize(x, y, vx, vy);
			break;
		}
	}
}

void Game::SetExplode(int x, int y, int vx, int vy)
{
	for(int i = 0; i < EXPLODE_MAX; ++ i)
	{
		if(this->explode[i] == NULL)
		{
			this->explode[i] = new Explode;
			this->explode[i]->Initialize(x, y, vx, vy);
			break;
		}
	}
}

void Game::SetEnemy(int x, int y, int type, int item)
{
	for(int i = 0; i < ENEMY_MAX; ++ i)
	{
		if(this->enemy[i] == NULL)
		{
			switch(type)
			{
			case ENEMY1:
				this->enemy[i] = new Enemy1;
				break;
			case ENEMY2:
				this->enemy[i] = new Enemy2;
				break;
			case ENEMY3:
				this->enemy[i] = new Enemy3;
				break;
			case ENEMY4:
				this->enemy[i] = new Enemy4;
				break;
			case ENEMY5:
				this->enemy[i] = new Enemy5;
				break;
			case BOSS1:
				this->enemy[i] = new Boss1;
				break;
			case BOSS2:
				this->enemy[i] = new Boss2;
				break;
			case BOSS3:
				this->enemy[i] = new Boss3;
				break;
			case BOSS4:
				this->enemy[i] = new Boss4;
				break;
			case BOSS5:
				this->enemy[i] = new Boss5;
				break;
			case BOSS6:
				this->enemy[i] = new Boss6;
				break;
			case POWER_UP:
				this->enemy[i] = new PowerUp;
				break;
			}
			this->enemy[i]->Initialize(type, x, y);
			this->enemy[i]->SetItem(item);
			break;
		}
	}
}

void Game::ClearCharacter(void)
{
	for(int i = 0; i < SHOT_MAX; ++ i)
	{
		if(this->shot[i] != NULL)
		{
			delete this->shot[i];
			this->shot[i] = NULL;
		}
	}
	for(int i = 0; i < BULLET_MAX; ++ i)
	{
		if(this->bullet[i] != NULL)
		{
			delete this->bullet[i];
			this->bullet[i] = NULL;
		}
	}
	for(int i = 0; i < EXPLODE_MAX; ++ i)
	{
		if(this->explode[i] != NULL)
		{
			delete this->explode[i];
			this->explode[i] = NULL;
		}
	}
	for(int i = 0; i < ENEMY_MAX; ++ i)
	{
		if(this->enemy[i] != NULL)
		{
			delete this->enemy[i];
			this->enemy[i] = NULL;
		}
	}
}

bool Game::FireBullet(void)
{
	if((this->stage == STAGE_MAX) && (this->boss == true))
	{
		if((Mathmatics::Rand() % 1000) < (this->bullet_rate + 50))
		{
			return true;
		}
	}
	if((Mathmatics::Rand() % 1000) < this->bullet_rate)
	{
		return true;
	}
	return false;
}

int Game::GetBulletSpeed(void)
{
	return this->bullet_speed + (Mathmatics::Rand() % this->bullet_speed_add);
}

void Game::GetFighterPosition(int& x, int& y)
{
	this->player.GetPosition(x, y);
}

void Game::Spawn(void)
{
	for(int i = 0; i < this->spawn; ++ i)
	{
		Spawn(i);
	}
}

void Game::Spawn(int index)
{
	this->spawn_info[index].interval2_count --;
	if(this->spawn_info[index].interval2_count > 0)
	{
		return;
	}
	this->spawn_info[index].interval_count --;
	if(this->spawn_info[index].interval_count > 0)
	{
		return;
	}
	-- this->spawn_info[index].repeat_count;
	if(this->spawn_info[index].repeat_count <= 0)
	{
		// éüâÒèoåªÇÃìGÇíäëIÇ∑ÇÈ
		this->spawn_info[index].spawn_number = SpawnNumber(this->stage);
		switch(this->spawn_info[index].spawn_number)
		{
		case 0:
			this->spawn_info[index].enemy_number = 0;
			break;
		case 1:
			this->spawn_info[index].enemy_number = ENEMY1;
			this->spawn_info[index].spawn_x = -5 << 10;
			this->spawn_info[index].spawn_y = (10 + Mathmatics::Rand() % 20) << 10;
			this->spawn_info[index].repeat_count = 3 + (Mathmatics::Rand() & 3);
			this->spawn_info[index].rapid = Mathmatics::Rand() & 1;
			break;
		case 2:
			this->spawn_info[index].enemy_number = ENEMY1;
			this->spawn_info[index].spawn_x = (Screen::WIDTH + 5) << 10;
			this->spawn_info[index].spawn_y = (10 + Mathmatics::Rand() % 20) << 10;
			this->spawn_info[index].repeat_count = 3 + (Mathmatics::Rand() & 3);
			this->spawn_info[index].rapid = Mathmatics::Rand() & 1;
			break;
		case 3:
			this->spawn_info[index].enemy_number = ENEMY2;
			this->spawn_info[index].spawn_x = (5 + Mathmatics::Rand() % (Screen::WIDTH - 10)) << 10;
			this->spawn_info[index].spawn_y = -10 << 10;
			this->spawn_info[index].repeat_count = 3 + (Mathmatics::Rand() & 3);
			this->spawn_info[index].rapid = Mathmatics::Rand() & 1;
			break;
		case 4:
			this->spawn_info[index].enemy_number = ENEMY3;
			this->spawn_info[index].spawn_x = (5 + Mathmatics::Rand() % (Screen::WIDTH - 10)) << 10;
			this->spawn_info[index].spawn_y = -10 << 10;
			this->spawn_info[index].repeat_count = 3 + (Mathmatics::Rand() & 3);
			this->spawn_info[index].rapid = Mathmatics::Rand() & 1;
			break;
		case 5:
			this->spawn_info[index].enemy_number = ENEMY4;
			this->spawn_info[index].spawn_x = (8 + Mathmatics::Rand() % (Screen::WIDTH - 16)) << 10;
			this->spawn_info[index].spawn_y = -10 << 10;
			this->spawn_info[index].repeat_count = 1 + (Mathmatics::Rand() % 3);
			this->spawn_info[index].rapid = 0;
			break;
		case 6:
			this->spawn_info[index].enemy_number = ENEMY5;
			this->spawn_info[index].spawn_x = -5 << 10;
			this->spawn_info[index].spawn_y = (5 + Mathmatics::Rand() % 15) << 10;
			this->spawn_info[index].repeat_count = 1 + (Mathmatics::Rand() % 3);
			this->spawn_info[index].rapid = Mathmatics::Rand() & 1;
			break;
		case 7:
			this->spawn_info[index].enemy_number = ENEMY5;
			this->spawn_info[index].spawn_x = (Screen::WIDTH + 5) << 10;
			this->spawn_info[index].spawn_y = (5 + Mathmatics::Rand() % 15) << 10;
			this->spawn_info[index].repeat_count = 1 + (Mathmatics::Rand() % 3);
			this->spawn_info[index].rapid = Mathmatics::Rand() & 1;
			break;
		}
		this->spawn_info[index].interval2_count = this->spawn_info[index].interval * 3;
		return;
	}
	else if(this->spawn_info[index].rapid == 0)
	{
		// ç¿ïWÇïœÇ¶ÇÈ
		switch(this->spawn_info[index].spawn_number)
		{
		case 1:
			this->spawn_info[index].spawn_y = (10 + Mathmatics::Rand() % 20) << 10;
			break;
		case 2:
			this->spawn_info[index].spawn_y = (10 + Mathmatics::Rand() % 20) << 10;
			break;
		case 3:
		case 4:
			this->spawn_info[index].spawn_x = (5 + Mathmatics::Rand() % (Screen::WIDTH - 10)) << 10;
			break;
		case 5:
			this->spawn_info[index].spawn_x = (8 + Mathmatics::Rand() % (Screen::WIDTH - 16)) << 10;
			break;
		case 6:
		case 7:
			this->spawn_info[index].spawn_y = (5 + Mathmatics::Rand() % 15) << 10;
			this->spawn_info[index].spawn_y = (5 + Mathmatics::Rand() % 15) << 10;
			break;
		}
	}
	if(this->spawn_info[index].enemy_number > 0)
	{
		SetEnemy(this->spawn_info[index].spawn_x, this->spawn_info[index].spawn_y, this->spawn_info[index].enemy_number, 0);
	}
	this->spawn_info[index].interval_count = this->spawn_info[index].interval;
}

void Game::SpawnBoss(void)
{
	switch(this->stage)
	{
	case 1:
		SetEnemy((Screen::WIDTH / 2) << 10, -15 << 10, BOSS1, 0);
		break;
	case 2:
		SetEnemy((Screen::WIDTH / 2) << 10, -15 << 10, BOSS2, 0);
		break;
	case 3:
		SetEnemy((Screen::WIDTH / 2) << 10, -32 << 10, BOSS3, 0);
		break;
	case 4:
		SetEnemy((Screen::WIDTH / 2) << 10, -32 << 10, BOSS4, 0);
		break;
	case 5:
		SetEnemy((Screen::WIDTH / 2) << 10, -32 << 10, BOSS5, 0);
		break;
	case 6:
		SetEnemy((Screen::WIDTH / 2) << 10, -32 << 10, BOSS6, 0);
		break;
	default:
		SetEnemy((Screen::WIDTH / 2) << 10, -15 << 10, BOSS1, 0);
		break;
	}
	this->boss = true;
	this->draw_warning = DRAW_WARNING_TIME;
}

int Game::SpawnNumber(int stage)
{
	int draw_table[STAGE_MAX][7] =
	{
		{0, 0, 21, 21, 4, 0, 0}, // stage1 spawn rate
		{4, 4, 16, 16, 6, 0, 0}, // stage2 spawn rate
		{5, 5, 10, 10, 8, 4, 4}, // stage3 spawn rate
		{7, 7, 8, 8, 8, 4, 4},   // stage4 spawn rate
		{10, 10, 2, 2, 0, 2, 2}, // stage5 spawn rate
		{5, 5, 10, 10, 8, 4, 4}, // stage6 spawn rate
	};
	int max_number = 0;
	for(int i = 0; i < static_cast<int>(sizeof(draw_table[stage - 1]) / sizeof(int)); ++ i)
	{
		max_number += draw_table[stage - 1][i];
	}
	int sum = 0;
	int draw_number = Mathmatics::Rand() % max_number;
	for(int i = 0; i < static_cast<int>(sizeof(draw_table[stage - 1]) / sizeof(int)); ++ i)
	{
		sum += draw_table[stage - 1][i];
		if(draw_number < sum)
		{
			return i + 1;
		}
	}
	return 0;
}

void Game::DeleteEnemy(void)
{
	for(int i = 0; i < BULLET_MAX; ++ i)
	{
		if(this->bullet[i] != NULL)
		{
			delete this->bullet[i];
			this->bullet[i] = NULL;
		}
	}
	for(int i = 0; i < ENEMY_MAX; ++ i)
	{
		if(this->enemy[i] != NULL)
		{
			int enemy_x;
			int enemy_y;
			int enemy_vx;
			int enemy_vy;
			this->enemy[i]->GetPosition(enemy_x, enemy_y);
			this->enemy[i]->GetVector(enemy_vx, enemy_vy);
			SetExplode(enemy_x, enemy_y, enemy_vx, enemy_vy);
			delete this->enemy[i];
			this->enemy[i] = NULL;
		}
	}
}

void Game::Bomber(void)
{
	Sound& sound = Sound::GetInstance();
	sound.Play(SOUND_CRUSH_BOSS);
	for(int i = 0; i < BULLET_MAX; ++ i)
	{
		if(this->bullet[i] != NULL)
		{
			delete this->bullet[i];
			this->bullet[i] = NULL;
		}
	}
	for(int i = 0; i < ENEMY_MAX; ++ i)
	{
		if(this->enemy[i] != NULL)
		{
			int enemy_x;
			int enemy_y;
			int enemy_vx;
			int enemy_vy;
			if(this->enemy[i]->GetType() >= BOSS1)
			{
				continue;
			}
			else if(this->enemy[i]->GetType() == POWER_UP)
			{
				continue;
			}
			else if(this->enemy[i]->GetType() >= ENEMY4)
			{
				if(this->enemy[i]->Damage() == false)
				{
					continue;
				}
				this->score += 300;
			}
			else
			{
				this->score += 100;
			}
			this->enemy[i]->GetPosition(enemy_x, enemy_y);
			this->enemy[i]->GetVector(enemy_vx, enemy_vy);
			SetExplode(enemy_x, enemy_y, enemy_vx, enemy_vy);
			delete this->enemy[i];
			this->enemy[i] = NULL;
		}
	}
	int fighter_x;
	int fighter_y;
	this->player.GetPosition(fighter_x, fighter_y);
	for(int i = 0; i < 8; ++ i)
	{
		SetExplode(fighter_x, fighter_y, (Mathmatics::Rand() % 5 - 2) << 10, (Mathmatics::Rand() % 5 - 2) << 10);
	}
}

int Game::GetDifficulty(void)
{
	return this->difficulty;
}
