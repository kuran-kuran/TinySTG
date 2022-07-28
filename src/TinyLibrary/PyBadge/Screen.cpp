#if defined(ADAFRUIT_PYBADGE_M4_EXPRESS) || defined(ADAFRUIT_PYGAMER_M4_EXPRESS)

#include <stdarg.h>
#include <string.h>
#include "../Mathmatics.hpp"
#include "Arcada.hpp"
#include "Screen.hpp"

#if defined(USE_COLOR256)
#include "../Font6x8.h"
#include "../Font4x6.h"
#elif defined(USE_COLOR65536)
#include "../Font6x8_65536.h"
#include "../Font4x6_65536.h"
#endif

Screen* Screen::screen = NULL;

const unsigned short Screen::color_table[256] =
{
	0x0000, 0x0050, 0x00A8, 0x00F8, 0x2001, 0x2051, 0x20A9, 0x20F9, 0x4002, 0x4052, 0x40AA, 0x40FA, 0x6003, 0x6053, 0x60AB, 0x60FB,
	0x8004, 0x8054, 0x80AC, 0x80FC, 0xA005, 0xA055, 0xA0AD, 0xA0FD, 0xC006, 0xC056, 0xC0AE, 0xC0FE, 0xE007, 0xE057, 0xE0AF, 0xE0FF,
	0x0400, 0x0450, 0x04A8, 0x04F8, 0x2401, 0x2451, 0x24A9, 0x24F9, 0x4402, 0x4452, 0x44AA, 0x44FA, 0x6403, 0x6453, 0x64AB, 0x64FB,
	0x8404, 0x8454, 0x84AC, 0x84FC, 0xA405, 0xA455, 0xA4AD, 0xA4FD, 0xC406, 0xC456, 0xC4AE, 0xC4FE, 0xE407, 0xE457, 0xE4AF, 0xE4FF,
	0x0900, 0x0950, 0x09A8, 0x09F8, 0x2901, 0x2951, 0x29A9, 0x29F9, 0x4902, 0x4952, 0x49AA, 0x49FA, 0x6903, 0x6953, 0x69AB, 0x69FB,
	0x8904, 0x8954, 0x89AC, 0x89FC, 0xA905, 0xA955, 0xA9AD, 0xA9FD, 0xC906, 0xC956, 0xC9AE, 0xC9FE, 0xE907, 0xE957, 0xE9AF, 0xE9FF,
	0x0D00, 0x0D50, 0x0DA8, 0x0DF8, 0x2D01, 0x2D51, 0x2DA9, 0x2DF9, 0x4D02, 0x4D52, 0x4DAA, 0x4DFA, 0x6D03, 0x6D53, 0x6DAB, 0x6DFB,
	0x8D04, 0x8D54, 0x8DAC, 0x8DFC, 0xAD05, 0xAD55, 0xADAD, 0xADFD, 0xCD06, 0xCD56, 0xCDAE, 0xCDFE, 0xED07, 0xED57, 0xEDAF, 0xEDFF,
	0x1200, 0x1250, 0x12A8, 0x12F8, 0x3201, 0x3251, 0x32A9, 0x32F9, 0x5202, 0x5252, 0x52AA, 0x52FA, 0x7203, 0x7253, 0x72AB, 0x72FB,
	0x9204, 0x9254, 0x92AC, 0x92FC, 0xB205, 0xB255, 0xB2AD, 0xB2FD, 0xD206, 0xD256, 0xD2AE, 0xD2FE, 0xF207, 0xF257, 0xF2AF, 0xF2FF,
	0x1600, 0x1650, 0x16A8, 0x16F8, 0x3601, 0x3651, 0x36A9, 0x36F9, 0x5602, 0x5652, 0x56AA, 0x56FA, 0x7603, 0x7653, 0x76AB, 0x76FB,
	0x9604, 0x9654, 0x96AC, 0x96FC, 0xB605, 0xB655, 0xB6AD, 0xB6FD, 0xD606, 0xD656, 0xD6AE, 0xD6FE, 0xF607, 0xF657, 0xF6AF, 0xF6FF,
	0x1B00, 0x1B50, 0x1BA8, 0x1BF8, 0x3B01, 0x3B51, 0x3BA9, 0x3BF9, 0x5B02, 0x5B52, 0x5BAA, 0x5BFA, 0x7B03, 0x7B53, 0x7BAB, 0x7BFB,
	0x9B04, 0x9B54, 0x9BAC, 0x9BFC, 0xBB05, 0xBB55, 0xBBAD, 0xBBFD, 0xDB06, 0xDB56, 0xDBAE, 0xDBFE, 0xFB07, 0xFB57, 0xFBAF, 0xFBFF,
	0x1F00, 0x1F50, 0x1FA8, 0x1FF8, 0x3F01, 0x3F51, 0x3FA9, 0x3FF9, 0x5F02, 0x5F52, 0x5FAA, 0x5FFA, 0x7F03, 0x7F53, 0x7FAB, 0x7FFB,
	0x9F04, 0x9F54, 0x9FAC, 0x9FFC, 0xBF05, 0xBF55, 0xBFAD, 0xBFFD, 0xDF06, 0xDF56, 0xDFAE, 0xDFFE, 0xFF07, 0xFF57, 0xFFAF, 0xFFFF
};

Screen::Screen(int color_mode)
:color_mode(color_mode)
,buffer_size(WIDTH * HEIGHT)
{
	if(color_mode == 16)
	{
		this->buffer_size *= 2;
	}
	Adafruit_Arcada* arcada = GetArcada();
	this->screen_buffer = new unsigned char[this->buffer_size];
	memset(this->screen_buffer, 0, this->buffer_size);
	if(arcada->createFrameBuffer(160, 128) == false)
	{
		arcada->haltBox("Could not allocate framebuffer");
	}
	this->frame_buffer = arcada->getFrameBuffer();
}

Screen::~Screen(void)
{
	if(this->screen_buffer != NULL)
	{
		delete [] this->screen_buffer;
		this->screen_buffer = NULL;
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
	Adafruit_Arcada* arcada = GetArcada();
	arcada->display->dmaWait();
}

void Screen::DrawEnd(void)
{
	Adafruit_Arcada* arcada = GetArcada();
	int width2 = WIDTH * 2;
	if(this->color_mode == 8)
	{
		for(int y = 0; y < HEIGHT; ++ y)
		{
			for(int x = 0; x < WIDTH; ++ x)
			{
				unsigned char color_index = screen_buffer[y * WIDTH + x];
				unsigned short color = Screen::color_table[color_index];
				int xx = x * 2;
				int yy = y * 2;
				frame_buffer[yy * width2 + xx] = color;
				frame_buffer[yy * width2 + xx + 1] = color;
				frame_buffer[(yy + 1) * width2 + xx] = color;
				frame_buffer[(yy + 1) * width2 + xx + 1] = color;
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
				int xx = x * 2;
				int yy = y * 2;
				frame_buffer[yy * width2 + xx] = color;
				frame_buffer[yy * width2 + xx + 1] = color;
				frame_buffer[(yy + 1) * width2 + xx] = color;
				frame_buffer[(yy + 1) * width2 + xx + 1] = color;
			}
		}
	}
	arcada->blitFrameBuffer(0, 0, false, true);
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

#endif
