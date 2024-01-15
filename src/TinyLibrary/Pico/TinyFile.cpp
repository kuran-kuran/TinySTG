#include "Pico.h"
#if defined(_PICO_)

#include "TinyFile.hpp"

TinyFile::TinyFile(void)
//:file()
{
}

TinyFile::~TinyFile(void)
{
}

bool TinyFile::Open(const char* filepath, unsigned int mode)
{
#if false
	Adafruit_Arcada* arcada = GetArcada();
	if(arcada->filesysBegin() == false)
	{
		return false;
	}
	unsigned int open_mode = 0;
	if(mode & READ)
	{
		open_mode |= O_READ;
	}
	else if(mode & WRITE)
	{
		open_mode |= (O_WRITE | O_CREAT | O_TRUNC);
	}
	if(mode & ADD)
	{
		open_mode |= O_APPEND;
	}
	this->file = arcada->open(filepath, open_mode);
	return this->file.isOpen();
#endif
	return false;
}

void TinyFile::Close(void)
{
#if false
	this->file.close();
#endif
}

void TinyFile::Sync(void)
{
#if false
	this->file.sync();
#endif
}

size_t TinyFile::Read(void* buffer, size_t size)
{
#if false
	return this->file.read(buffer, size);
#endif
	return 0;
}

void TinyFile::Write(const void* buffer, size_t size)
{
#if false
	this->file.write(buffer, size);
#endif
}

void TinyFile::Seek(long offset, unsigned int position)
{
#if false
	switch(position)
	{
	case TOP:
		this->file.seekSet(offset);
		break;
	case CURRENT:
		this->file.seekCur(offset);
		break;
	default:
		this->file.seekEnd(offset);
		break;
	}
#endif
}

size_t TinyFile::GetSize(void) const
{
#if false
	return this->file.fileSize();
#endif
	return 0;
}

long TinyFile::Tell(void)
{
#if false
	return this->file.curPosition();
#endif
	return 0;
}

// カレントディレクトリを設定する
void TinyFile::SetCurrentPath(const char* assetsPath)
{
}

size_t TinyFile::GetLoadSize(const char* filepath)
{
#if false
	TinyFile file;
	size_t size = 0;
	if(file.Open(filepath, READ) == true)
	{
		size = file.GetSize();
		file.Close();
	}
	return size;
#endif
	return 0;
}

bool TinyFile::Load(void* buffer, const char* filepath)
{
#if false
	TinyFile file;
	if(file.Open(filepath, READ) == false)
	{
		return false;
	}
	size_t size = file.GetSize();
	if(size > 0)
	{
		file.Read(buffer, size);
	}
	file.Close();
	return true;
#endif
	return false;
}

bool TinyFile::Exist(const char* filepath)
{
#if false
	TinyFile file;
	if(file.Open(filepath, READ) == false)
	{
		return false;
	}
	file.Close();
#endif
	return false;
}

#endif
