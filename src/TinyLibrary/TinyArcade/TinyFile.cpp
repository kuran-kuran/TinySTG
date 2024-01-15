#if defined(ARDUINO_SAMD_ZERO) && defined(CRYSTALLESS) && defined(USBCON)

#include "TinyFile.hpp"

bool TinyFile::initialized = false;
#if defined(USE_SD_FAT)
SdFat TinyFile::sd_fat;
#endif

TinyFile::TinyFile(void)
#if defined(USE_SD_FAT)
:sd_file()
#endif
{
#if defined(USE_SD_FAT)
	if(TinyFile::initialized == false)
	{
		if(TinyFile::sd_fat.begin(10, SPI_FULL_SPEED))
		{
			TinyFile::initialized = true;
		}
	}
#endif
}

TinyFile::~TinyFile(void)
{
}

bool TinyFile::Open(const char* filepath, unsigned int mode)
{
#if defined(USE_SD_FAT)
	if(TinyFile::initialized == false)
	{
		return false;
	}
	unsigned char open_mode = 0;
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
	if(!this->sd_file.open(filepath, open_mode))
	{
		return false;
	}
	return true;
#else
	return false;
#endif
}

void TinyFile::Close(void)
{
#if defined(USE_SD_FAT)
	this->sd_file.close();
#endif
}

void TinyFile::Sync(void)
{
#if defined(USE_SD_FAT)
	this->sd_file.sync();
#endif
}

size_t TinyFile::Read(void* buffer, size_t size)
{
#if defined(USE_SD_FAT)
	return this->sd_file.read(buffer, size);
#else
	return 0;
#endif
}

void TinyFile::Write(const void* buffer, size_t size)
{
#if defined(USE_SD_FAT)
	this->sd_file.write(buffer, size);
#endif
}

void TinyFile::Seek(long offset, unsigned int position)
{
#if defined(USE_SD_FAT)
	switch(position)
	{
	case TOP:
		this->sd_file.seekSet(offset);
		break;
	case CURRENT:
		this->sd_file.seekCur(offset);
		break;
	default:
		this->sd_file.seekEnd(offset);
		break;
	}
#endif
}

size_t TinyFile::GetSize(void) const
{
#if defined(USE_SD_FAT)
	return this->sd_file.fileSize();
#else
	return 0;
#endif
}

long TinyFile::Tell(void)
{
#if defined(USE_SD_FAT)
	return this->sd_file.curPosition();
#else
	return 0;
#endif
}

// カレントディレクトリを設定する
void TinyFile::SetCurrentPath(const char* assetsPath)
{
}

size_t TinyFile::GetLoadSize(const char* filepath)
{
	TinyFile file;
	size_t size = 0;
	if(file.Open(filepath, READ) == true)
	{
		size = file.GetSize();
		file.Close();
	}
	return size;
}

bool TinyFile::Load(void* buffer, const char* filepath)
{
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
}

bool TinyFile::Exist(const char* filepath)
{
	TinyFile file;
	if(file.Open(filepath, READ) == false)
	{
		return false;
	}
	file.Close();
	return true;
}

#endif
