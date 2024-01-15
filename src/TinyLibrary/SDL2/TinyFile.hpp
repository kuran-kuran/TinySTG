#ifndef TINYFILE_HPP
#define TINYFILE_HPP

#include <stdio.h>

class TinyFile
{
public:
	enum
	{
		READ = 0x00000001,
		WRITE = 0x00000002,
		ADD = 0x00000004,
		TOP = 0,
		CURRENT,
		BOTTOM
	};
	TinyFile(void);
	~TinyFile(void);
	bool Open(const char* filepath, unsigned int mode);
	void Close(void);
	void Sync(void);
	size_t GetSize(void) const;
	size_t Read(void* buffer, size_t size);
	void Write(const void* buffer, size_t size);
	void Seek(long offset, unsigned int position);
	long Tell(void);
	static void SetCurrentPath(const char* assetsPath);
	static size_t GetLoadSize(const char* filepath);
	static bool Load(void* buffer, const char* filepath);
	static bool Exist(const char* filepath);
private:
	TinyFile(TinyFile&);
	TinyFile& operator = (TinyFile&);
	FILE* file;
	size_t filesize;
	unsigned int mode;
};

#endif
