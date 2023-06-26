#include <string.h>
#include <stdlib.h>
#include "RingBuffer.hpp"

RingBuffer::RingBuffer(void)
:buffer(NULL)
,buffersize(0)
,readpointer(0)
,writepointer(0)
,empty(true)
{
}

RingBuffer::~RingBuffer(void)
{
	Release();
}

bool RingBuffer::Create(size_t size)
{
	Release();
	if(size == 0)
	{
		return false;
	}
	this->buffer = static_cast<char*>(malloc(size));
	this->buffersize = size;
	return true;
}

void RingBuffer::Release(void)
{
	if(this->buffer != NULL)
	{
		free(this->buffer);
		this->buffer = NULL;
	}
	this->buffersize = 0;
	this->readpointer = 0;
	this->writepointer = 0;
	this->empty = true;
}

void RingBuffer::Reset(void)
{
	this->readpointer = 0;
	this->writepointer = 0;
	this->empty = true;
}

size_t RingBuffer::GetReadableSize(void) const
{
	if(this->empty)
	{
		return 0;
	}
	size_t size;
	if(this->readpointer < this->writepointer)
	{
		size = this->writepointer - this->readpointer;
	}
	else if(this->readpointer > this->writepointer)
	{
		size = this->buffersize - this->readpointer + this->writepointer;
	}
	else
	{
		size = this->buffersize;
	}
	return size;
}

bool RingBuffer::Read(void* buffer, size_t size)
{
	if(buffer == NULL)
	{
		throw "Error: RingBuffer::Read";
	}
	size_t readablesize;
	readablesize = GetReadableSize();
	if(size > readablesize)
	{
		throw "Error: RingBuffer::Read";
	}
	if(size == readablesize)
	{
		this->empty = true;
	}
	if(this->readpointer < this->writepointer)
	{
		memcpy(buffer, &this->buffer[this->readpointer], size);
	}
	else
	{
		size_t copysize;
		char* copybuffer;
		copybuffer = static_cast<char*>(buffer);
		copysize = this->buffersize - this->readpointer;
		if(size < copysize)
		{
			copysize = size;
		}
		memcpy(copybuffer, &this->buffer[this->readpointer], copysize);
		copybuffer += copysize;
		copysize = size - copysize;
		if(copysize != 0)
		{
			memcpy(copybuffer, this->buffer, copysize);
		}
	}
	this->readpointer = (this->readpointer + size) % this->buffersize;
	return true;
}

bool RingBuffer::Skip(size_t size)
{
	size_t readablesize;
	readablesize = GetReadableSize();
	if(size > readablesize)
	{
		throw "Error: RingBuffer::Skip";
	}
	if(size == readablesize)
	{
		this->empty = true;
	}
	this->readpointer = (this->readpointer + size) % this->buffersize;
	return true;
}

size_t RingBuffer::GetWritableSize(void) const
{
	size_t size;
	if(this->writepointer < this->readpointer)
	{
		size = this->readpointer - this->writepointer;
	}
	else if(this->writepointer > this->readpointer)
	{
		size = this->buffersize - this->writepointer + this->readpointer;
	}
	else if(this->empty)
	{
		size = this->buffersize;
	}
	else
	{
		size = 0;
	}
	return size;
}

bool RingBuffer::Write(const void* buffer, size_t size)
{
	if(buffer == NULL)
	{
		throw "Error: RingBuffer::Write";
	}
	if(size == 0)
	{
		return true;
	}
	if(this->writepointer < this->readpointer)
	{
		memcpy(&this->buffer[this->writepointer], buffer, size);
	}
	else
	{
		size_t copysize;
		const char* copybuffer;
		copybuffer = reinterpret_cast<const char*>(buffer);
		copysize = this->buffersize - this->writepointer;
		if(size < copysize)
		{
			copysize = size;
		}
		memcpy(&this->buffer[this->writepointer], copybuffer, copysize);
		copybuffer += copysize;
		copysize = size - copysize;
		if(copysize != 0)
		{
			memcpy(this->buffer, copybuffer, copysize);
		}
	}
	this->writepointer = (this->writepointer + size) % this->buffersize;
	this->empty = false;
	return true;
}

size_t RingBuffer::GetBufferSize(void) const
{
	return this->buffersize;
}
