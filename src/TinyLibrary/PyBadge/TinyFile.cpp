#if defined(ADAFRUIT_PYBADGE_M4_EXPRESS) || defined(ADAFRUIT_PYGAMER_M4_EXPRESS)

#include "TinyFile.hpp"
#include "Arcada.hpp"

TinyFile::TinyFile(void)
:file()
{
}

TinyFile::~TinyFile(void)
{
}

bool TinyFile::Open(const char* filepath, unsigned int mode)
{
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
}

void TinyFile::Close(void)
{
	this->file.close();
}

void TinyFile::Sync(void)
{
	this->file.sync();
}

size_t TinyFile::Read(void* buffer, size_t size)
{
	return this->file.read(buffer, size);
}

void TinyFile::Write(const void* buffer, size_t size)
{
	this->file.write(buffer, size);
}

void TinyFile::Seek(long offset, unsigned int position)
{
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
}

size_t TinyFile::GetSize(void) const
{
	return this->file.fileSize();
}

long TinyFile::Tell(void)
{
	return this->file.curPosition();
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
