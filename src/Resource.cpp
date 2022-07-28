#include "TinyLibrary.hpp"
#include "Character.h"
#include "Bg.h"
#include "Crush.h"
#include "CrushBoss.h"
#include "Gun.h"
#include "PowerUp.h"
#include "Bomb.h"
#include "Get.h"
#include "bgm_short.h"
#include "TuchikureLogo.h"
#include "DaimonLogo.h"
#include "Otoyoi.h"
#include "TitleLogo.h"
#include "Resource.hpp"

Data data[DATA_MAX] = 
{
	// buffer, size
	{NULL, 0}
};

const ImageData image_data[IMAGE_MAX] =
{
	// buffer, width, height
	{NULL, 10, 10},
	{Boss1, 22, 30},
	{Boss2, 62, 19},
	{Boss3, 62, 54},
	{Boss4, 28, 24},
	{Boss5, 64, 30},
	{Boss6, 62, 62},
	{Bullet1, 2, 2},
	{Bullet2, 2, 2},
	{Enemy1, 10, 10},
	{Enemy2, 10, 10},
	{Enemy3, 10, 10},
	{Enemy4, 16, 16},
	{Enemy5, 16, 16},
	{Explode1, 20, 20},
	{Explode2, 20, 20},
	{Explode3, 20, 20},
	{Explode4, 20, 20},
	{Explode5, 20, 20},
	{Explode6, 20, 20},
	{Explode7, 20, 20},
	{Explode8, 20, 20},
	{Fighter, 10, 10},
	{Power, 10, 10},
	{ShotCenter, 4, 10},
	{ShotLeft, 5, 10},
	{ShotRight, 5, 10},
	{FighterSmall, 6, 6},
	{BomberSmall, 6, 6},
	{Bg, 96, 384},
	{TuchikureLogo, 96, 64},
	{Otoyoi, 96, 64},
	{DaimonLogo, 96, 64},
	{TitleLogo, 70, 26},
};

const BgMapData bg_map_data[BG_MAP_MAX] = 
{
	// buffer, width, height, back_red, back_green, back_blue
	{NULL, 0, 0, 0, 0, 0},
};

const SoundData sound_data[SOUND_MAX] = 
{
	// buffer, size
	{NULL, 0},
	{Crush_wave, sizeof(Crush_wave)},
	{CrushBoss_wave, sizeof(CrushBoss_wave)},
	{Gun_wave, sizeof(Gun_wave)},
	{PowerUp_wave, sizeof(PowerUp_wave)},
	{Bomb_wave, sizeof(Bomb_wave)},
	{Get_wave, sizeof(Get_wave)},
	{bgm_short_wave, sizeof(bgm_short_wave)},
};

const Data* GetData(int number)
{
	return &data[number];
}

const ImageData* GetImageData(int number)
{
	return &image_data[number];
}

const BgMapData* GetBgMapData(int number)
{
	return &bg_map_data[number];
}

const SoundData* GetSoundData(int number)
{
	return &sound_data[number];
}
