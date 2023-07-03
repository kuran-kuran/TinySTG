#ifndef RINGBUFFER_HPP
#define RINGBUFFER_HPP

class RingBuffer
{
public:
	RingBuffer(void);
	~RingBuffer(void);
	bool Create(size_t size);
	void Release(void);
	void Reset(void);
	size_t GetReadableSize(void) const;
	bool Read(void* buffer, size_t size);
	bool Skip(size_t size);
	size_t GetWritableSize(void) const;
	bool Write(const void* buffer, size_t size);
	size_t GetBufferSize(void) const;
private:
	char* buffer;
	size_t buffersize;
	size_t readpointer;
	size_t writepointer;
	bool empty;
private:
	RingBuffer(RingBuffer&);
	RingBuffer& operator = (RingBuffer&);
};

#endif
