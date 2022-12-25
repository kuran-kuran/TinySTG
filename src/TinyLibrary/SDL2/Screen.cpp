#if defined(SDL2)

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#if defined(_WIN64)
#pragma comment(lib, "../../lib/sdl2/sdl2/lib/x64/SDL2.lib")
#pragma comment(lib, "../../lib/sdl2/sdl2/lib/x64/SDL2main.lib")
#pragma comment(lib, "../../lib/sdl2/sdl2_image/lib/x64/SDL2_image.lib")
#else
#pragma comment(lib, "../../lib/sdl2/sdl2/lib/x86/SDL2.lib")
#pragma comment(lib, "../../lib/sdl2/sdl2/lib/x86/SDL2main.lib")
#pragma comment(lib, "../../lib/sdl2/sdl2_image/lib/x86/SDL2_image.lib")
#endif
#include "../Mathmatics.hpp"
#include "Screen.hpp"

#if defined(USE_COLOR256)
#include "../Font6x8.h"
#include "../Font4x6.h"
#elif defined(USE_COLOR65536)
#include "../Font6x8_65536.h"
#include "../Font4x6_65536.h"
#endif

Screen* Screen::screen = NULL;

const unsigned int Screen::color_table[256] =
{
	0xFF000000, 0xFF550000, 0xFFAA0000, 0xFFFF0000, 0xFF002400, 0xFF552400, 0xFFAA2400, 0xFFFF2400, 0xFF004900, 0xFF554900, 0xFFAA4900, 0xFFFF4900, 0xFF006D00, 0xFF556D00, 0xFFAA6D00, 0xFFFF6D00,
	0xFF009200, 0xFF559200, 0xFFAA9200, 0xFFFF9200, 0xFF00B600, 0xFF55B600, 0xFFAAB600, 0xFFFFB600, 0xFF00DB00, 0xFF55DB00, 0xFFAADB00, 0xFFFFDB00, 0xFF00FF00, 0xFF55FF00, 0xFFAAFF00, 0xFFFFFF00,
	0xFF000024, 0xFF550024, 0xFFAA0024, 0xFFFF0024, 0xFF002424, 0xFF552424, 0xFFAA2424, 0xFFFF2424, 0xFF004924, 0xFF554924, 0xFFAA4924, 0xFFFF4924, 0xFF006D24, 0xFF556D24, 0xFFAA6D24, 0xFFFF6D24,
	0xFF009224, 0xFF559224, 0xFFAA9224, 0xFFFF9224, 0xFF00B624, 0xFF55B624, 0xFFAAB624, 0xFFFFB624, 0xFF00DB24, 0xFF55DB24, 0xFFAADB24, 0xFFFFDB24, 0xFF00FF24, 0xFF55FF24, 0xFFAAFF24, 0xFFFFFF24,
	0xFF000049, 0xFF550049, 0xFFAA0049, 0xFFFF0049, 0xFF002449, 0xFF552449, 0xFFAA2449, 0xFFFF2449, 0xFF004949, 0xFF554949, 0xFFAA4949, 0xFFFF4949, 0xFF006D49, 0xFF556D49, 0xFFAA6D49, 0xFFFF6D49,
	0xFF009249, 0xFF559249, 0xFFAA9249, 0xFFFF9249, 0xFF00B649, 0xFF55B649, 0xFFAAB649, 0xFFFFB649, 0xFF00DB49, 0xFF55DB49, 0xFFAADB49, 0xFFFFDB49, 0xFF00FF49, 0xFF55FF49, 0xFFAAFF49, 0xFFFFFF49,
	0xFF00006D, 0xFF55006D, 0xFFAA006D, 0xFFFF006D, 0xFF00246D, 0xFF55246D, 0xFFAA246D, 0xFFFF246D, 0xFF00496D, 0xFF55496D, 0xFFAA496D, 0xFFFF496D, 0xFF006D6D, 0xFF556D6D, 0xFFAA6D6D, 0xFFFF6D6D,
	0xFF00926D, 0xFF55926D, 0xFFAA926D, 0xFFFF926D, 0xFF00B66D, 0xFF55B66D, 0xFFAAB66D, 0xFFFFB66D, 0xFF00DB6D, 0xFF55DB6D, 0xFFAADB6D, 0xFFFFDB6D, 0xFF00FF6D, 0xFF55FF6D, 0xFFAAFF6D, 0xFFFFFF6D,
	0xFF000092, 0xFF550092, 0xFFAA0092, 0xFFFF0092, 0xFF002492, 0xFF552492, 0xFFAA2492, 0xFFFF2492, 0xFF004992, 0xFF554992, 0xFFAA4992, 0xFFFF4992, 0xFF006D92, 0xFF556D92, 0xFFAA6D92, 0xFFFF6D92,
	0xFF009292, 0xFF559292, 0xFFAA9292, 0xFFFF9292, 0xFF00B692, 0xFF55B692, 0xFFAAB692, 0xFFFFB692, 0xFF00DB92, 0xFF55DB92, 0xFFAADB92, 0xFFFFDB92, 0xFF00FF92, 0xFF55FF92, 0xFFAAFF92, 0xFFFFFF92,
	0xFF0000B6, 0xFF5500B6, 0xFFAA00B6, 0xFFFF00B6, 0xFF0024B6, 0xFF5524B6, 0xFFAA24B6, 0xFFFF24B6, 0xFF0049B6, 0xFF5549B6, 0xFFAA49B6, 0xFFFF49B6, 0xFF006DB6, 0xFF556DB6, 0xFFAA6DB6, 0xFFFF6DB6,
	0xFF0092B6, 0xFF5592B6, 0xFFAA92B6, 0xFFFF92B6, 0xFF00B6B6, 0xFF55B6B6, 0xFFAAB6B6, 0xFFFFB6B6, 0xFF00DBB6, 0xFF55DBB6, 0xFFAADBB6, 0xFFFFDBB6, 0xFF00FFB6, 0xFF55FFB6, 0xFFAAFFB6, 0xFFFFFFB6,
	0xFF0000DB, 0xFF5500DB, 0xFFAA00DB, 0xFFFF00DB, 0xFF0024DB, 0xFF5524DB, 0xFFAA24DB, 0xFFFF24DB, 0xFF0049DB, 0xFF5549DB, 0xFFAA49DB, 0xFFFF49DB, 0xFF006DDB, 0xFF556DDB, 0xFFAA6DDB, 0xFFFF6DDB,
	0xFF0092DB, 0xFF5592DB, 0xFFAA92DB, 0xFFFF92DB, 0xFF00B6DB, 0xFF55B6DB, 0xFFAAB6DB, 0xFFFFB6DB, 0xFF00DBDB, 0xFF55DBDB, 0xFFAADBDB, 0xFFFFDBDB, 0xFF00FFDB, 0xFF55FFDB, 0xFFAAFFDB, 0xFFFFFFDB,
	0xFF0000FF, 0xFF5500FF, 0xFFAA00FF, 0xFFFF00FF, 0xFF0024FF, 0xFF5524FF, 0xFFAA24FF, 0xFFFF24FF, 0xFF0049FF, 0xFF5549FF, 0xFFAA49FF, 0xFFFF49FF, 0xFF006DFF, 0xFF556DFF, 0xFFAA6DFF, 0xFFFF6DFF,
	0xFF0092FF, 0xFF5592FF, 0xFFAA92FF, 0xFFFF92FF, 0xFF00B6FF, 0xFF55B6FF, 0xFFAAB6FF, 0xFFFFB6FF, 0xFF00DBFF, 0xFF55DBFF, 0xFFAADBFF, 0xFFFFDBFF, 0xFF00FFFF, 0xFF55FFFF, 0xFFAAFFFF, 0xFFFFFFFF
};

Screen::Screen(int color_mode)
:color_mode(color_mode)
,buffer_size(WIDTH * HEIGHT)
{
	if(color_mode == 16)
	{
		this->buffer_size *= 2;
	}
	this->screen_buffer = new unsigned char[this->buffer_size];
	memset(this->screen_buffer, 0, this->buffer_size);
	// SDL‰Šú‰»
	if(SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		throw "Error: SDL_Init(SDL_INIT_VIDEO)";
	}
	atexit(SDL_Quit);
	this->window = SDL_CreateWindow("Hello World!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
	if(this->window == NULL)
	{
		throw "Error: SDL_CreateWindow";
	}
	this->renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if(this->renderer == NULL)
	{
		throw "Error: SDL_CreateRenderer";
	}
	SDL_RenderSetLogicalSize(renderer, WIDTH, HEIGHT);
	SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	this->screenSurface = SDL_CreateRGBSurface(0, WIDTH, HEIGHT, 32, 0, 0, 0, 0);
	if(this->screenSurface == NULL)
	{
		throw "Error: SDL_CreateRenderer";
	}
	memset(this->screenSurface->pixels, 0, WIDTH * HEIGHT * sizeof(unsigned int));
}

Screen::~Screen(void)
{
	if(this->screen_buffer != NULL)
	{
		delete [] this->screen_buffer;
		this->screen_buffer = NULL;
	}
	SDL_DestroyRenderer(this->renderer);
	SDL_DestroyWindow(this->window);
}

void Screen::Initialize(int color_mode)
{
	Screen::screen = new Screen(color_mode);
}

void Screen::Finalize(void)
{
	if(Screen::screen != NULL)
	{
		delete Screen::screen;
		Screen::screen = NULL;
	}
}

Screen& Screen::GetInstance()
{
	return *Screen::screen;
}

void Screen::Clear(unsigned int color)
{
	if((this->color_mode == 8) || (color == 0))
	{
		memset(this->screen_buffer, color, this->buffer_size);
	}
	else
	{
		unsigned short* screen_buffer16 = reinterpret_cast<unsigned short*>(this->screen_buffer);
		unsigned short color16 = static_cast<unsigned short>(color);
		for(int i = 0; i < WIDTH * HEIGHT; ++ i)
		{
			screen_buffer16[i] = color16;
		}
	}
}

void Screen::DrawBegin(void)
{
}

void Screen::DrawEnd(void)
{
	unsigned int* frameBuffer = reinterpret_cast<unsigned int*>(this->screenSurface->pixels);
	if(this->color_mode == 8)
	{
		for(int y = 0; y < HEIGHT; ++ y)
		{
			for(int x = 0; x < WIDTH; ++ x)
			{
				unsigned char color_index = screen_buffer[y * WIDTH + x];
				unsigned int color = Screen::color_table[color_index];
				frameBuffer[y * WIDTH + x] = color;
			}
		}
	}
	else
	{
		unsigned short* screen_buffer16 = reinterpret_cast<unsigned short*>(this->screen_buffer);
		for(int y = 0; y < HEIGHT; ++ y)
		{
			for(int x = 0; x < WIDTH; ++ x)
			{
				unsigned short color = screen_buffer16[y * WIDTH + x];
				frameBuffer[y * WIDTH + x] = Color16To32(color);
			}
		}
	}
	SDL_Texture* screenTexture = SDL_CreateTextureFromSurface(this->renderer, this->screenSurface);
	SDL_Rect sourceRect = {
		0, 0, WIDTH, HEIGHT
	};
	SDL_Rect destRect =
	{
		0, 0, WIDTH, HEIGHT
	};
	SDL_RenderCopy(this->renderer, screenTexture, &sourceRect, &destRect);
	SDL_DestroyTexture(screenTexture);
	SDL_RenderPresent(this->renderer);
}

void Screen::DrawSprite(const void* buffer, int x, int y, int width, int height, int color_key)
{
	if(x + width <= 0)
	{
		return;
	}
	if(y + height <= 0)
	{
		return;
	}
	if(x >= WIDTH)
	{
		return;
	}
	if(y >= HEIGHT)
	{
		return;
	}
	int clip_width = width;
	int clip_height = height;
	const char* clip_buffer = reinterpret_cast<const char*>(buffer);
	if(x > WIDTH - width)
	{
		clip_width = width - (x - (WIDTH - width));
	}
	if (x < 0)
	{
		clip_buffer -= x;
		clip_width += x;
		x = 0;
	}
	if(y > HEIGHT - height)
	{
		clip_height = height - (y - (HEIGHT - height));
	}
	if(y < 0)
	{
		clip_buffer -= (static_cast<size_t>(y) * static_cast<size_t>(width));
		clip_height += y;
		y = 0;
	}
	DrawSpriteNoClip(clip_buffer, x, y, clip_width, clip_height, color_key, width);
}

void Screen::DrawSprite(const void* buffer, int x, int y, int width, int height, int source_x, int source_y, int source_width, int color_key)
{
	if(x + width <= 0)
	{
		return;
	}
	if(y + height <= 0)
	{
		return;
	}
	if(x >= WIDTH)
	{
		return;
	}
	if(y >= HEIGHT)
	{
		return;
	}
	int clip_width = width;
	int clip_height = height;
	const char* clip_buffer = reinterpret_cast<const char*>(buffer) + static_cast<size_t>(source_y) * static_cast<size_t>(source_width) + static_cast<size_t>(source_x);
	if(x > WIDTH - width)
	{
		clip_width = width - (x - (WIDTH - width));
	}
	if (x < 0)
	{
		clip_buffer -= x;
		clip_width += x;
		x = 0;
	}
	if(y > HEIGHT - height)
	{
		clip_height = height - (y - (HEIGHT - height));
	}
	if (y < 0)
	{
		clip_buffer -= (static_cast<size_t>(y) * static_cast<size_t>(source_width));
		clip_height += y;
		y = 0;
	}
	DrawSpriteNoClip(clip_buffer, x, y, clip_width, clip_height, color_key, source_width);
}

void Screen::DrawSprite(int destination_x, int destination_y, int destination_left, int destination_top, int destination_right, int destination_bottom, const void* source_buffer, int source_x, int source_y, int source_width, int source_height, int color_key, int rotate, int scale)
{
	rotate = -rotate;
	scale >>= 1;
	if(scale <= 0)
	{
		return;
	}
	int sin = Mathmatics::Sin8(rotate);
	int cos = Mathmatics::Cos8(rotate);
	int x;
	int y;
	int xx;
	int yy;
	int pixel_x;
	int pixel_y;
	if(this->color_mode == 8)
	{
		const unsigned char* image = reinterpret_cast<const unsigned char*>(source_buffer);
		for(y = destination_top; y <= destination_bottom; ++ y)
		{
			yy = y - destination_y;
			for(x = destination_left; x <= destination_right; ++ x)
			{
				unsigned char pixel = 0;
				xx = x - destination_x;
				pixel_x = source_x + (xx * cos - yy * sin + (1 << 6)) / scale;
				pixel_y = source_y + (xx * sin + yy * cos + (1 << 6)) / scale;
				if((pixel_x >= 0) && (pixel_y >= 0) && (pixel_x < source_width) && (pixel_y < source_height))
				{
					pixel = image[pixel_y * source_width + pixel_x];
				}
				if(pixel != color_key)
				{
					this->screen_buffer[WIDTH * y + x] = pixel;
				}
			}
		}
	}
	else
	{
		const unsigned short* image = reinterpret_cast<const unsigned short*>(source_buffer);
		unsigned short* screen_buffer16 = reinterpret_cast<unsigned short*>(this->screen_buffer);
		for(y = destination_top; y <= destination_bottom; ++ y)
		{
			yy = y - destination_y;
			for(x = destination_left; x <= destination_right; ++ x)
			{
				unsigned short pixel = 0;
				xx = x - destination_x;
				pixel_x = source_x + (xx * cos - yy * sin + (1 << 6)) / scale;
				pixel_y = source_y + (xx * sin + yy * cos + (1 << 6)) / scale;
				if((pixel_x >= 0) && (pixel_y >= 0) && (pixel_x < source_width) && (pixel_y < source_height))
				{
					pixel = image[pixel_y * source_width + pixel_x];
				}
				if(pixel != color_key)
				{
					screen_buffer16[WIDTH * y + x] = pixel;
				}
			}
		}
	}
}

void Screen::DrawSpriteFrame(const void* buffer, int x, int y, int width, int height, int source_width, int source_height, int color_key)
{
	if((width == source_width) && (height == source_height))
	{
		DrawSprite(buffer, x, y, width, height, color_key);
		return;
	}
	int width_half;
	if(width > source_width)
	{
		width_half = source_width / 2;
	}
	else
	{
		width_half = width / 2;
	}
	int height_half;
	if(height > source_height)
	{
		height_half = source_height / 2;
	}
	else
	{
		height_half = height / 2;
	}
	// back color
	unsigned int color;
	if(this->color_mode == 8)
	{
		const unsigned char* buffer8 = reinterpret_cast<const unsigned char*>(buffer);
		color = static_cast<unsigned int>(buffer8[source_width * (y / 2) + (x / 2)]);
	}
	else
	{
		const unsigned short* buffer16 = reinterpret_cast<const unsigned short*>(buffer);
		color = static_cast<unsigned int>(buffer16[source_width * (y / 2) + (x / 2)]);
	}
	DrawRectangle(x, y, width, height, color);
	// left-top
	int draw_width = width_half;
	int draw_height = height_half;
	int draw_x = x;
	int draw_y = y;
	int source_x = 0;
	int source_y = 0;
	DrawSprite(buffer, draw_x, draw_y, draw_width, draw_height, source_x, source_y, source_width, color_key);
	// right-top
	draw_width = width - width_half;
	draw_height = height_half;
	draw_x = x + width - draw_width;
	draw_y = y;
	source_x = source_width - draw_width;
	source_y = 0;
	DrawSprite(buffer, draw_x, draw_y, draw_width, draw_height, source_x, source_y, source_width, color_key);
	// left-bottom
	draw_width = width_half;
	draw_height = height - height_half;
	draw_x = x;
	draw_y = y + height - draw_height;
	source_x = 0;
	source_y = source_height - draw_height;
	DrawSprite(buffer, draw_x, draw_y, draw_width, draw_height, source_x, source_y, source_width, color_key);
	// right-bottom
	draw_width = width - width_half;
	draw_height = height - height_half;
	draw_x = x + width - draw_width;
	draw_y = y + height - draw_height;
	source_x = source_width - draw_width;
	source_y = source_height - draw_height;
	DrawSprite(buffer, draw_x, draw_y, draw_width, draw_height, source_x, source_y, source_width, color_key);
}

void Screen::DrawFont(int x, int y, const char* text, ...)
{
	char* output_text = reinterpret_cast<char*>(malloc(128));
	va_list arglist;
	va_start(arglist, text);
#if defined(_WIN32)
	vsprintf_s(output_text, 128, text, arglist);
#else
	vsprintf(output_text, text, arglist);
#endif
	va_end(arglist);
	unsigned char* output_text_temp = reinterpret_cast<unsigned char*>(output_text);
	const void* font = NULL;
	if(this->color_mode == 8)
	{
#if defined(USE_COLOR256)
		font = Font6x8;
#endif
	}
	else
	{
#if defined(USE_COLOR65536)
		font = Font6x8_65536;
#endif
	}
	if(font == NULL)
	{
		free(output_text);
		return;
	}
	if(x == CENTER)
	{
		x = ((WIDTH - 6 * static_cast<int>(strlen(output_text))) >> 1);
	}
	else if(x == LEFT)
	{
		x = 0;
	}
	else if(x == RIGHT)
	{
		x = WIDTH - 6 * static_cast<int>(strlen(output_text));
	}
	if(y == CENTER)
	{
		y = ((HEIGHT - 8) >> 1);
	}
	else if(y == TOP)
	{
		y = 0;
	}
	else if(x == BOTTOM)
	{
		y = HEIGHT - 8;
	}
	while(*output_text_temp != 0)
	{
		int index = static_cast<int>(static_cast<unsigned char>(*output_text_temp)) - 0x20;
		if(index >= 0x80)
		{
			index -= 0x20;
		}
		int source_x = (index % 16) * 6;
		int source_y = (index / 16) * 8;
		DrawSprite(font, x, y, 6, 8, source_x, source_y, 96, 0);
		x += 6;
		++ output_text_temp;
	}
	free(output_text);
}

void Screen::DrawJapaneseFont(int x, int y, const char* text)
{
	char* text_temp = const_cast<char*>(text);
	const void* font = NULL;
	if(this->color_mode == 8)
	{
#if defined(USE_COLOR256)
		font = Font6x8;
#endif
	}
	else
	{
#if defined(USE_COLOR65536)
		font = Font6x8_65536;
#endif
	}
	if(font == NULL)
	{
		return;
	}
	if(x == CENTER)
	{
		x = ((WIDTH - 6 * static_cast<int>(strlen(text))) >> 1);
	}
	else if(x == LEFT)
	{
		x = 0;
	}
	else if(x == RIGHT)
	{
		x = WIDTH - 6 * static_cast<int>(strlen(text));
	}
	if(y == CENTER)
	{
		y = ((HEIGHT - 8) >> 1);
	}
	else if(y == TOP)
	{
		y = 0;
	}
	else if(x == BOTTOM)
	{
		y = HEIGHT - 8;
	}
	while(*text_temp != 0)
	{
		int index = static_cast<int>(static_cast<unsigned char>(*text_temp)) - 0x20;
		if(index >= 0x80)
		{
			index -= 0x20;
		}
		int source_x = (index % 16) * 6;
		int source_y = (index / 16) * 8;
		DrawSprite(font, x, y, 6, 8, source_x, source_y, 96, 0);
		x += 6;
		++ text_temp;
	}
}

void Screen::DrawNumberFont(int x, int y, const char* text, ...)
{
	char* output_text = reinterpret_cast<char*>(malloc(128));
	va_list arglist;
	va_start(arglist, text);
#if defined(_WIN32)
	vsprintf_s(output_text, 128, text, arglist);
#else
	vsprintf(output_text, text, arglist);
#endif
	va_end(arglist);
	char* output_text_temp = output_text;
	const void* font = NULL;
	if(this->color_mode == 8)
	{
#if defined(USE_COLOR256)
		font = Font4x6;
#endif
	}
	else
	{
#if defined(USE_COLOR65536)
		font = Font4x6_65536;
#endif
	}
	if(font == NULL)
	{
		free(output_text);
		return;
	}
	if(x == CENTER)
	{
		x = ((WIDTH - 4 * static_cast<int>(strlen(output_text))) >> 1);
	}
	else if(x == LEFT)
	{
		x = 0;
	}
	else if(x == RIGHT)
	{
		x = WIDTH - 4 * static_cast<int>(strlen(output_text));
	}
	if(y == CENTER)
	{
		y = ((HEIGHT - 6) >> 1);
	}
	else if(y == TOP)
	{
		y = 0;
	}
	else if(x == BOTTOM)
	{
		y = HEIGHT - 6;
	}
	while(*output_text_temp != 0)
	{
		if(*output_text_temp != ' ')
		{
			int source_x = (static_cast<int>(*output_text_temp - 0x20) % 8) * 4;
			int source_y = (static_cast<int>(*output_text_temp - 0x20) / 8) * 6;
			DrawSprite(font, x, y, 4, 5, source_x, source_y, 32, 0);
		}
		x += 4;
		++ output_text_temp;
	}
	free(output_text);
}

void Screen::DrawRectangle(int x, int y, int width, int height, unsigned int color)
{
	int destination_address = WIDTH * y + x;
	if(this->color_mode == 8)
	{
		for(int scan_y = 0; scan_y < height; ++ scan_y)
		{
			for(int scan_x = 0; scan_x < width; ++ scan_x)
			{
				this->screen_buffer[destination_address + scan_x] = static_cast<unsigned char>(color);
			}
			destination_address += WIDTH;
		}
	}
	else
	{
		unsigned short* screen_buffer16 = reinterpret_cast<unsigned short*>(this->screen_buffer);
		for(int scan_y = 0; scan_y < height; ++ scan_y)
		{
			for(int scan_x = 0; scan_x < width; ++ scan_x)
			{
				screen_buffer16[destination_address + scan_x] = static_cast<unsigned short>(color);
			}
			destination_address += WIDTH;
		}
	}
}

void Screen::DrawSpriteNoClip(const void* buffer, int x, int y, int width, int height, int color_key, int source_width)
{
	int source_address = 0;
	int destination_address = WIDTH * y + x;
	if(this->color_mode == 8)
	{
		const unsigned char* buffer8 = reinterpret_cast<const unsigned char*>(buffer);
		for(int scan_y = 0; scan_y < height; ++ scan_y)
		{
			for(int scan_x = 0; scan_x < width; ++ scan_x)
			{
				unsigned char color = buffer8[source_address + scan_x];
				if(static_cast<int>(color) != color_key)
				{
					this->screen_buffer[destination_address + scan_x] = color;
				}
			}
			source_address += source_width;
			destination_address += WIDTH;
		}
	}
	else
	{
		const unsigned short* buffer16 = reinterpret_cast<const unsigned short*>(buffer);
		unsigned short* screen_buffer16 = reinterpret_cast<unsigned short*>(this->screen_buffer);
		for(int scan_y = 0; scan_y < height; ++ scan_y)
		{
			for(int scan_x = 0; scan_x < width; ++ scan_x)
			{
				unsigned short color = buffer16[source_address + scan_x];
				if(static_cast<int>(color) != color_key)
				{
					screen_buffer16[destination_address + scan_x] = color;
				}
			}
			source_address += source_width;
			destination_address += WIDTH;
		}
	}
}

// color16 GGGR RRRR BBBB BGGG
// color32 AAAA AAAA BBBB BBBB GGGG GGGG RRRR RRRR
unsigned int Screen::Color16To32(unsigned short color16)
{
	unsigned int color32 = 0;
	unsigned short color = (color16 >> 8) | ((color16 & 0xFF00) << 8);
	unsigned int red = (color & 0x1F) * 8;
	unsigned int green = ((color >> 5) & 0x3F) * 4;
	unsigned int blue = ((color >> 11) & 0x1F) * 8;
	unsigned int alpha = 0xFF;
	color32 = (alpha << 24) + (blue << 16) + (green << 8) + red;
	return color32;
}

#endif
