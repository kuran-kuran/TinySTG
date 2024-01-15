#if defined(_WIN32) && !defined(SDL2)

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <Windows.h>
#include <shlwapi.h>
#include "TinyFile.hpp"

#pragma comment(lib, "shlwapi.lib")

TinyFile::TinyFile(void)
:file(NULL)
,filesize(0)
,mode(0)
{
}

TinyFile::~TinyFile(void)
{
}

bool TinyFile::Open(const char* filepath, unsigned int mode)
{
	if(this->file != NULL)
	{
		return false;
	}
	if(mode & READ)
	{
		fopen_s(&this->file, filepath, "rb");
	}
	else if(mode & WRITE)
	{
		fopen_s(&this->file, filepath, "wb");
		this->filesize = 0;
	}
	else
	{
		fopen_s(&this->file, filepath, "rb+");
		if(this->file == NULL)
		{
			fopen_s(&this->file, filepath, "wb+");
			this->filesize = 0;
		}
	}
	if(this->file == NULL)
	{
		return false;
	}
	if(!(mode & WRITE))
	{
		long position;
		fseek(this->file, 0, SEEK_END);
		position = ftell(this->file);
		if(position == -1)
		{
			return false;
		}
		this->filesize = static_cast<size_t>(position);
		fseek(this->file, 0, SEEK_SET);
	}
	this->mode = mode;
	return true;
}

void TinyFile::Close(void)
{
	fclose(this->file);
	this->file = NULL;
	this->filesize = 0;
}

void TinyFile::Sync(void)
{
}

size_t TinyFile::Read(void* buffer, size_t size)
{
	return fread(buffer, 1, size, this->file);
}

void TinyFile::Write(const void* buffer, size_t size)
{
	fwrite(buffer, 1, size, this->file);
}

void TinyFile::Seek(long offset, unsigned int position)
{
	switch(position)
	{
	case TOP:
		fseek(this->file, offset, SEEK_SET);
		break;
	case CURRENT:
		fseek(this->file, offset, SEEK_CUR);
		break;
	default:
		fseek(this->file, offset, SEEK_END);
		break;
	}
}

size_t TinyFile::GetSize(void) const
{
	if(this->mode & READ)
	{
		return this->filesize;
	}
	long position = ftell(this->file);
	if(position == -1)
	{
		return 0;
	}
	return static_cast<size_t>(position);
}

long TinyFile::Tell(void)
{
	return ftell(this->file);
}

// カレントディレクトリを設定する
void TinyFile::SetCurrentPath(const char* assetsPath)
{
	struct stat statBuffer;
	if((stat(assetsPath, &statBuffer) == 0) && (statBuffer.st_mode & _S_IFDIR))
	{
		// 指定のアセットがあるのでカレントディレクトリはそのまま
		return;
	}
	char path[MAX_PATH];
	if(GetModuleFileNameA(NULL, path, MAX_PATH) == 0)
	{
		return;
	}
	char drive[MAX_PATH];
	char dir[MAX_PATH];
	char filename[MAX_PATH];
	char ext[MAX_PATH];
	_splitpath_s(path, drive, MAX_PATH, dir, MAX_PATH, filename, MAX_PATH, ext, MAX_PATH);
	char* exepath = PathCombineA(path, drive, dir);
	SetCurrentDirectoryA(exepath);
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
