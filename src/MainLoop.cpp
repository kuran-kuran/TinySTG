#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TinyLibrary.hpp"
#include "Global.hpp"
#include "Sound.hpp"

void MainLoop_Setup(void)
{
	Global::Initialize();
	Screen::Initialize(8);
	Controller::Initialize();
	Sound::Initialize();
	Global& global = Global::GetInstance();
	global.phase = Global::PHASE_TUCHIKURE_LOGO;
	global.back_color = 0;
	global.before_button = 1;
	global.count = 0;
	// ゲームデータ読み込み
	TinyFile file;
	if(file.Open("TinySTG/TinySTG.sav", TinyFile::READ) == true)
	{
		memset(&global.gamedata, 0, sizeof(global.gamedata));
		size_t size = file.GetSize();
		file.Read(&global.gamedata, size);
		file.Close();
		if(size == (sizeof(int) * 3))
		{
			global.gamedata.volume = 10;
		}
	}
	else if(file.Open("gamedata.bin", TinyFile::READ) == true)
	{
		memset(&global.gamedata, 0, sizeof(global.gamedata));
		size_t size = file.GetSize();
		file.Read(&global.gamedata, size);
		file.Close();
		if(size == (sizeof(int) * 3))
		{
			global.gamedata.volume = 10;
		}
	}
	Sound& sound = Sound::GetInstance();
	sound.Load();
	SoundStream& sound_stream = SoundStream::GetInstance();
	sound_stream.SetVolume(global.gamedata.volume);
}

void MainLoop_Finalize(void)
{
	Global& global = Global::GetInstance();
	if(global.game != NULL)
	{
		global.game->Finalize();
		delete global.game;
		global.game = NULL;
	}
	Sound& sound = Sound::GetInstance();
	sound.Release();
	Sound::Finalize();
	Controller::Finalize();
	Screen::Finalize();
	Global::Finalize();
}

bool MainLoop_Loop(void)
{
	bool exit = false;
	bool status;
	Global& global = Global::GetInstance();
	Screen& screen = Screen::GetInstance();
	Controller& controller = Controller::GetInstance();
	screen.Clear(global.back_color);
	screen.DrawBegin();
	unsigned int button = controller.GetButton();
	const ImageData* image_data;
	const unsigned char* bg_buffer;
	int draw_x;
	int draw_y;
	switch(global.phase)
	{
	case Global::PHASE_TUCHIKURE_LOGO:
		image_data = GetImageData(TUCHIKURE_LOGO);
		draw_x = (Screen::WIDTH - image_data->width) / 2;
		draw_y = (Screen::HEIGHT - image_data->height) / 2;
		screen.DrawSprite(image_data->buffer, draw_x, draw_y, image_data->width, image_data->height, 0);
		screen.DrawNumberFont(Screen::CENTER, Screen::HEIGHT - 7, "Tuchikure");
		global.back_color = 0;
		if((global.before_button == 0) && (button & (Controller::BUTTON_1 | Controller::BUTTON_2)))
		{
			global.phase = Global::PHASE_OTOYOI_LOGO;
		}
		++ global.count;
		if(global.count > Screen::FPS * 5)
		{
			global.phase = Global::PHASE_OTOYOI_LOGO;
			global.count = 0;
		}
		Mathmatics::Rand();
		break;
	case Global::PHASE_OTOYOI_LOGO:
		image_data = GetImageData(OTOYOI_LOGO);
		draw_x = (Screen::WIDTH - image_data->width) / 2;
		draw_y = (Screen::HEIGHT - image_data->height) / 2;
		screen.DrawSprite(image_data->buffer, draw_x, draw_y, image_data->width, image_data->height, 0);
		screen.DrawNumberFont(Screen::CENTER, Screen::HEIGHT - 7, "Sound Studio");
		global.back_color = 0;
		if((global.before_button == 0) && (button & (Controller::BUTTON_1 | Controller::BUTTON_2)))
		{
			global.phase = Global::PHASE_DAIMON_LOGO;
			global.count = 0;
		}
		++ global.count;
		if(global.count > Screen::FPS * 5)
		{
			global.phase = Global::PHASE_DAIMON_LOGO;
			global.count = 0;
		}
		Mathmatics::Rand();
		break;
	case Global::PHASE_DAIMON_LOGO:
		image_data = GetImageData(DAIMON_LOGO);
		draw_x = (Screen::WIDTH - image_data->width) / 2;
		draw_y = (Screen::HEIGHT - image_data->height) / 2;
		screen.DrawSprite(image_data->buffer, draw_x, draw_y, image_data->width, image_data->height, 0);
		global.back_color = 0;
		if((global.before_button == 0) && (button & (Controller::BUTTON_1 | Controller::BUTTON_2)))
		{
			global.phase = Global::PHASE_TITLE;
		}
		++ global.count;
		if(global.count > Screen::FPS * 5)
		{
			global.phase = Global::PHASE_TITLE;
			global.count = 0;
		}
		Mathmatics::Rand();
		break;
	case Global::PHASE_TITLE:
		image_data = GetImageData(BG);
		bg_buffer = reinterpret_cast<const unsigned char*>(image_data->buffer) + (265 * 96);
		screen.DrawSprite(bg_buffer, 0, 0, image_data->width, 64, -1);
		image_data = GetImageData(TITLE_LOGO);
		screen.DrawSprite(image_data->buffer, (Screen::WIDTH - image_data->width) / 2, 5, image_data->width, image_data->height, 0);
		global.back_color = 0;
		if(button != 0)
		{
			global.count = 0;
		}
		if((global.before_button == 0) && (button & (Controller::BUTTON_1 | Controller::BUTTON_2)))
		{
			global.phase = Global::PHASE_INITIALIZE_GAME;
			// ゲームデータ書き込み
			TinyFile file;
			if(file.Open("TinySTG/TinySTG.sav", TinyFile::WRITE) == true)
			{
				file.Write(&global.gamedata, sizeof(global.gamedata));
				file.Close();
			}
		}
		// Menu
		if(!(global.before_button & Controller::BUTTON_RIGHT) && (button & Controller::BUTTON_RIGHT))
		{
			++ global.gamedata.difficulty;
			if(global.gamedata.difficulty > Game::VERY_HARD)
			{
				global.gamedata.difficulty = Game::VERY_HARD;
			}
		}
		else if(!(global.before_button & Controller::BUTTON_LEFT) && (button & Controller::BUTTON_LEFT))
		{
			-- global.gamedata.difficulty;
			if(global.gamedata.difficulty < Game::EASY)
			{
				global.gamedata.difficulty = Game::EASY;
			}
		}
		switch(global.gamedata.difficulty)
		{
		case Game::EASY:
			screen.DrawNumberFont(Screen::RIGHT, Screen::HEIGHT - 6, "Easy");
			break;
		case Game::NORMAL:
			screen.DrawNumberFont(Screen::RIGHT, Screen::HEIGHT - 6, "Normal");
			break;
		case Game::HARD:
			screen.DrawNumberFont(Screen::RIGHT, Screen::HEIGHT - 6, "Hard");
			break;
		case Game::VERY_HARD:
			screen.DrawNumberFont(Screen::RIGHT, Screen::HEIGHT - 6, "Very hard");
			break;
		}
		screen.DrawNumberFont(0, Screen::HEIGHT - 20, "Press button");
		screen.DrawNumberFont(Screen::RIGHT, Screen::HEIGHT - 14, "to game start!");
		++ global.count;
		if(global.count > Screen::FPS * 30)
		{
			global.phase = Global::PHASE_TUCHIKURE_LOGO;
			global.count = 0;
		}
		screen.DrawNumberFont(0, 0, "%8d", global.score);
		screen.DrawNumberFont(Screen::RIGHT, 0, "%8d", global.gamedata.highscore);
		{
			Sound& sound = Sound::GetInstance();
			SoundStream& sound_stream = SoundStream::GetInstance();
			int volume = global.gamedata.volume;
			if(!(global.before_button & Controller::BUTTON_UP) && (button & Controller::BUTTON_UP))
			{
				++ volume;
				if(volume > SoundStream::MAX_VOLUME)
				{
					volume = SoundStream::MAX_VOLUME;
				}
				sound.Play(SOUND_GET);
				global.count = 0;
				global.draw_volume_count = 30;
			}
			else if(!(global.before_button & Controller::BUTTON_DOWN) && (button & Controller::BUTTON_DOWN))
			{
				-- volume;
				if(volume < 0)
				{
					volume = 0;
				}
				sound.Play(SOUND_GET);
				global.count = 0;
				global.draw_volume_count = 30;
			}
			sound_stream.SetVolume(volume);
			global.gamedata.volume = volume;
		}
		if(global.draw_volume_count > 0)
		{
			SoundStream& sound_stream = SoundStream::GetInstance();
			screen.DrawRectangle(0, 0, 12 * 4 + 1, 7, 0);
			screen.DrawNumberFont(1, 1, "Volume %02d/%02d", sound_stream.GetVolume(), SoundStream::MAX_VOLUME);
			-- global.draw_volume_count;
		}
		Mathmatics::Rand();
		break;
	case Global::PHASE_INITIALIZE_GAME:
		global.game = new Game;
		global.game->Initialize(1, global.gamedata.difficulty);
		global.phase = Global::PHASE_GAME;
		break;
	case Global::PHASE_GAME:
		status = global.game->Update();
		global.game->Draw();
		global.back_color = global.game->GetBackColor();
		if(status == true)
		{
			global.game->Finalize();
			delete global.game;
			global.game = NULL;
			global.phase = Global::PHASE_TUCHIKURE_LOGO;
			// ゲームデータ書き込み
			TinyFile file;
			if(file.Open("TinySTG/TinySTG.sav", TinyFile::WRITE) == true)
			{
				file.Write(&global.gamedata, sizeof(global.gamedata));
				file.Close();
			}
		}
		break;
	}
	if((button & Controller::BUTTON_START) && (button & Controller::BUTTON_SELECT))
	{
		exit = true;
	}
	controller.Update();
	screen.DrawEnd();
	global.before_button = button;
	return exit;
}

void MainLoop_NoWaitLoop(void)
{
	Sound& sound = Sound::GetInstance();
	sound.Update();
}
