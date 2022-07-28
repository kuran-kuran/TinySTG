#if defined(ARDUINO_SAMD_ZERO) && defined(CRYSTALLESS) && defined(USBCON)

#include <stdarg.h>
#include <string.h>
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

Screen::Screen(int color_mode)
:tiny_screen(TinyScreen(TinyScreenPlus))
,color_mode(color_mode)
,current_screen_buffer(0)
,buffer_size(WIDTH * HEIGHT)
{
	this->tiny_screen.begin();
	if(color_mode == 8)
	{
		this->tiny_screen.setBitDepth(TSBitDepth8);
		this->tiny_screen.fontColor(TS_8b_White, TS_8b_Black);
	}
	else
	{
		this->buffer_size *= 2;
		this->tiny_screen.setBitDepth(TSBitDepth16);
		this->tiny_screen.fontColor(TS_16b_White, TS_16b_Black);
	}
	this->tiny_screen.setBrightness(7);
	this->tiny_screen.setFont(liberationSansNarrow_12ptFontInfo);
	this->screen_buffer[0] = new unsigned char[this->buffer_size];
	memset(this->screen_buffer[0], 0, this->buffer_size);
#if defined(USE_DMA)
	this->screen_buffer[1] = new unsigned char[this->buffer_size];
	memset(this->screen_buffer[1], 0, this->buffer_size);
	this->tiny_screen.initDMA();
#else
	this->screen_buffer[1] = NULL;
#endif
}

Screen::~Screen(void)
{
	for(int i = 0; i < 2; ++ i)
	{
		if(this->screen_buffer[i] != NULL)
		{
			delete [] this->screen_buffer[i];
			this->screen_buffer[i] = NULL;
		}
	}
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
		memset(this->screen_buffer[this->current_screen_buffer], color, this->buffer_size);
	}
	else
	{
		unsigned short* screen_buffer16 = reinterpret_cast<unsigned short*>(this->screen_buffer[this->current_screen_buffer]);
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
#if defined(USE_DMA)
	while(!this->tiny_screen.getReadyStatusDMA());
	this->tiny_screen.endTransfer();
#endif
	this->tiny_screen.goTo(0, 0);
	this->tiny_screen.setX(0, WIDTH - 1);
	this->tiny_screen.setY(0, HEIGHT - 1);
	this->tiny_screen.startData();
#if defined(USE_DMA)
	this->current_screen_buffer = 1 - this->current_screen_buffer;
	this->tiny_screen.writeBufferDMA(this->screen_buffer[this->current_screen_buffer], this->buffer_size);
#else
	this->tiny_screen.writeBuffer(this->screen_buffer[0], this->buffer_size);
	this->tiny_screen.endTransfer();
#endif
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
		clip_buffer -= (y * width);
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
	const char* clip_buffer = reinterpret_cast<const char*>(buffer) + source_y * source_width + source_x;
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
		clip_buffer -= (y * source_width);
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
					this->screen_buffer[this->current_screen_buffer][WIDTH * y + x] = pixel;
				}
			}
		}
	}
	else
	{
		const unsigned short* image = reinterpret_cast<const unsigned short*>(source_buffer);
		unsigned short* screen_buffer16 = reinterpret_cast<unsigned short*>(this->screen_buffer[this->current_screen_buffer]);
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
	vsprintf(output_text, text, arglist);
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
	vsprintf(output_text, text, arglist);
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
				this->screen_buffer[this->current_screen_buffer][destination_address + scan_x] = static_cast<unsigned char>(color);
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
	int scan_x;
	int scan_y;
	int source_address = 0;
	int destination_address = WIDTH * y + x;
	if(this->color_mode == 8)
	{
		const unsigned char* buffer8 = reinterpret_cast<const unsigned char*>(buffer);
		for(scan_y = 0; scan_y < height; ++ scan_y)
		{
			for(scan_x = 0; scan_x < width; ++ scan_x)
			{
				unsigned char color = buffer8[source_address + scan_x];
				if(static_cast<int>(color) != color_key)
				{
					this->screen_buffer[this->current_screen_buffer][destination_address + scan_x] = color;
				}
			}
			source_address += source_width;
			destination_address += WIDTH;
		}
	}
	else
	{
		const unsigned short* buffer16 = reinterpret_cast<const unsigned short*>(buffer);
		unsigned short* screen_buffer16 = reinterpret_cast<unsigned short*>(this->screen_buffer[this->current_screen_buffer]);
		for(scan_y = 0; scan_y < height; ++ scan_y)
		{
			for(scan_x = 0; scan_x < width; ++ scan_x)
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

// for Debug
TinyScreen* Screen::GetTinyScreen(void)
{
	return &this->tiny_screen;
}

#endif
