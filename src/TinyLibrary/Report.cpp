#include <stdarg.h>
#include <stdio.h>
#include <string>
#include <vector>
#include "Dependent.hpp"

static const bool ENABLE_LOG = false;
static const char* LOGFILE = "log.txt";
static const int REPORT_BUFFER_SPLIT_SIZE = 500;

void Report(const char* text, ...)
{
	if(ENABLE_LOG == false)
	{
		return;
	}
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
#else
	char* output_text;
	va_list arglist;
	va_start(arglist, text);
	vasprintf(&output_text, text, arglist);
	va_end(arglist);
	std::string output_text_string = output_text;
	free(output_text);
	std::string::size_type length = output_text_string.length();
	if(length < REPORT_BUFFER_SPLIT_SIZE)
	{
		file.Write(output_text_string.c_str(), output_text_string.size());
	}
	else
	{
		std::string::size_type start = 0;
		while(length > REPORT_BUFFER_SPLIT_SIZE)
		{
			std::string output(output_text_string, start, REPORT_BUFFER_SPLIT_SIZE);
			file.Write(output.c_str(), output.size());
			start += REPORT_BUFFER_SPLIT_SIZE;
			length -= REPORT_BUFFER_SPLIT_SIZE;
		}
		if(length > 0)
		{
			std::string output(output_text_string, start, length);
			file.Write(output.c_str(), output.size());
		}
	}
#endif
	file.Close();
}
