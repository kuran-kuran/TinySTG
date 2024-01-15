#include <stdarg.h>
#include <stdio.h>
#include <string>
#include <vector>
#include "Dependent.hpp"

//#define USE_OUTPUT_LOG
//#define USE_SCREEN_REPORT
#ifdef RGNANO
static const char* LOGFILE = "/mnt/log.txt";
#else
static const char* LOGFILE = "log.txt";
#endif

static String reportText;

void Report(const char* text, ...)
{
#ifdef USE_OUTPUT_LOG
	TinyFile file;
	file.Open(LOGFILE, TinyFile::ADD);
	file.Seek(0, TinyFile::BOTTOM);
#ifdef _WIN32
	std::vector<char> output_text(128);
	va_list arglist;
	va_start(arglist, text);
	va_list source = arglist;
	for(;;)
	{
		arglist = source;
		if(_vsnprintf_s(&output_text[0], output_text.size() - 1, _TRUNCATE, text, arglist) != -1)
		{
			break;
		}
		output_text.resize(output_text.size() * 2);
	}
	output_text.push_back('\0');
	va_end(arglist);
	file.Write(&output_text[0], strlen(&output_text[0]));
	reportText = &output_text[0];
#elif defined(ARDUINO_SAMD_ZERO) && defined(CRYSTALLESS) && defined(USBCON)
	std::vector<char> output_text(128);
	va_list arglist;
	va_start(arglist, text);
	va_list source = arglist;
	for(;;)
	{
		arglist = source;
		if(snprintf(&output_text[0], output_text.size() - 1, text, arglist) != -1)
		{
			break;
		}
		output_text.resize(output_text.size() * 2);
	}
	output_text.push_back('\0');
	va_end(arglist);
	file.Write(&output_text[0], strlen(&output_text[0]));
	reportText = &output_text[0];
#elif defined(ADAFRUIT_PYBADGE_M4_EXPRESS) || defined(ADAFRUIT_PYGAMER_M4_EXPRESS)
	std::vector<char> output_text(128);
	va_list arglist;
	va_start(arglist, text);
	va_list source = arglist;
	for(;;)
	{
		arglist = source;
		if(snprintf(&output_text[0], output_text.size() - 1, text, arglist) != -1)
		{
			break;
		}
		output_text.resize(output_text.size() * 2);
	}
	output_text.push_back('\0');
	va_end(arglist);
	file.Write(&output_text[0], strlen(&output_text[0]));
	reportText = &output_text[0];
#else
	char* output_text;
	va_list arglist;
	va_start(arglist, text);
	vasprintf(&output_text, text, arglist);
	va_end(arglist);
	reportText = output_text;
	file.Write(&output_text[0], strlen(&output_text[0]));
	free(output_text);
#endif
	file.Close();
#endif
}

void ReportScreenUpdate(void)
{
#ifdef USE_OUTPUT_LOG
#ifdef USE_SCREEN_REPORT
	if(reportText.empty() == true)
	{
		return;
	}
	Screen& screen = Screen::GetInstance();
	screen.DrawFont(0, 0, reportText.c_str());
#endif
#endif
}
