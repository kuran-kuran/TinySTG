#ifndef RESOURCE_DATA_HPP
#define RESOURCE_DATA_HPP

#include <stdlib.h>

enum
{
	DUMMY_DATA = 0,
	DATA_MAX,
	IMAGE_START = 0,
	BOSS1,
	BOSS2,
	BOSS3,
	BOSS4,
	BOSS5,
	BOSS6,
	BULLET1,
	BULLET2,
	ENEMY1,
	ENEMY2,
	ENEMY3,
	ENEMY4,
	ENEMY5,
	EXPLODE1,
	EXPLODE2,
	EXPLODE3,
	EXPLODE4,
	EXPLODE5,
	EXPLODE6,
	EXPLODE7,
	EXPLODE8,
	FIGHTER,
	POWER,
	SHOT_CENTER,
	SHOT_LEFT,
	SHOT_RIGHT,
	FIGHTER_SMALL,
	BOMBER_SMALL,
	BG,
	TUCHIKURE_LOGO,
	OTOYOI_LOGO,
	DAIMON_LOGO,
	TITLE_LOGO,
	IMAGE_MAX,
	DUMMY_BG_MAP = 0,
	BG_MAP_MAX,
	SOUND_BGM = 0,
	SOUND_CRUSH,
	SOUND_CRUSH_BOSS,
	SOUND_GUN,
	SOUND_POWER_UP,
	SOUND_BOMB,
	SOUND_GET,
	SOUND_BGM_SHORT,
	SOUND_MAX
};

struct Data
{
	const void* buffer;
	int size;
};

struct ImageData
{
	const void* buffer;
	int width;
	int height;
};

struct BgMapData
{
	const unsigned char* buffer;
	int width;
	int height;
	int back_red;
	int back_green;
	int back_blue;
};

struct SoundData
{
	const unsigned char* buffer;
	int size;
};

const Data* GetData(int number);
const ImageData* GetImageData(int number);
const BgMapData* GetBgMapData(int number);
const SoundData* GetSoundData(int number);

#endif
