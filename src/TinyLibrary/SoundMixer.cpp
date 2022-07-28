#include <stdio.h>
#include <string.h>
#include "Dependent.hpp"
#include "SoundMixer.hpp"
#include "AdpcmDecorder.hpp"

SoundMixer::SoundMixer(void)
{
	for(int i = 0; i < CHANNEL_COUNT; ++ i)
	{
		this->file[i] = NULL;
		this->buffer[i] = NULL;
		this->size[i] = 0;
		this->sample[i] = 0;
		this->loop[i] = false;
		this->type[i] = 0;
		this->adpcm_decorder[i] = NULL;
	}
}

SoundMixer::~SoundMixer(void)
{
	Clear();
}

void SoundMixer::Append(int channel, const unsigned char* buffer, int size, bool loop, int type)
{
	this->file[channel] = NULL;
	this->buffer[channel] = buffer;
	this->size[channel] = size;
	this->sample[channel] = 0;
	this->loop[channel] = loop;
	this->type[channel] = type;
	if(type == 1)
	{
		this->adpcm_decorder[channel] = new AdpcmDecorder();
	}
	else
	{
		this->adpcm_decorder[channel] = NULL;
	}
}

void SoundMixer::Append(int channel, const char* path, bool loop, int type)
{
	TinyFile* file = new TinyFile;
	if(file->Open(path, TinyFile::READ))
	{
		this->file[channel] = file;
	}
	else
	{
		this->file[channel] = NULL;
	}
	this->buffer[channel] = NULL;
	this->size[channel] = 0;
	this->sample[channel] = 0;
	this->loop[channel] = loop;
	this->type[channel] = type;
	if (type == 1)
	{
		this->adpcm_decorder[channel] = new AdpcmDecorder();
	}
	else
	{
		this->adpcm_decorder[channel] = NULL;
	}
}

void SoundMixer::Delete(int channel)
{
	if(this->file[channel] != NULL)
	{
		this->file[channel]->Close();
		delete this->file[channel];
		this->file[channel] = NULL;
	}
	this->buffer[channel] = NULL;
	this->size[channel] = 0;
	this->sample[channel] = 0;
	this->loop[channel] = false;
}

void SoundMixer::Read(unsigned char* buffer, int size)
{
	memset(buffer, 0x80, size);
	for(int channel = 0; channel < CHANNEL_COUNT; ++ channel)
	{
		if(this->buffer[channel] == NULL)
		{
			if(this->file[channel] == NULL)
			{
				// this channel is free
				continue;
			}
			int file_size = static_cast<int>(this->file[channel]->GetSize());
			if(file_size == 0)
			{
				continue;
			}
			if(this->type[channel] == 1)
			{
				file_size *= 2;
			}
			int mix_size = size;
			int left_size = file_size - this->sample[channel];
			if(left_size < size)
			{
				mix_size = left_size;
			}
			if(mix_size > 0)
			{
				unsigned char* mix_buffer = new unsigned char[mix_size];
				if(this->type[channel] == 1)
				{
					size_t read_size = mix_size / 2;
					unsigned char* read_buffer = new unsigned char[read_size];
					this->file[channel]->Read(read_buffer, read_size);
					this->adpcm_decorder[channel]->Decord(mix_buffer, read_buffer, static_cast<int>(read_size));
					delete [] read_buffer;
				}
				else
				{
					this->file[channel]->Read(mix_buffer, mix_size);
				}
				MixBuffer(buffer, mix_buffer, mix_size);
				delete[] mix_buffer;
				this->sample[channel] += mix_size;
				if(this->loop[channel] == true)
				{
					if(size > left_size)
					{
						int left_mix_size = size - left_size;
						if(left_mix_size > 0)
						{
							unsigned char* mix_buffer = new unsigned char[left_mix_size];
							this->file[channel]->Seek(0, TinyFile::TOP);
							this->sample[channel] = 0;
							if(this->type[channel] == 1)
							{
								size_t read_size = left_mix_size / 2;
								unsigned char* read_buffer = new unsigned char[read_size];
								this->file[channel]->Read(read_buffer, read_size);
								this->adpcm_decorder[channel]->Clear();
								this->adpcm_decorder[channel]->Decord(mix_buffer, read_buffer, static_cast<int>(read_size));
								delete[] read_buffer;
							}
							else
							{
								this->file[channel]->Read(mix_buffer, left_mix_size);
							}
							MixBuffer(&buffer[mix_size], mix_buffer, left_mix_size);
							delete[] mix_buffer;
							this->sample[channel] += left_mix_size;
						}
					}
				}
			}
			if(this->sample[channel] >= file_size)
			{
				this->file[channel]->Seek(0, TinyFile::TOP);
				this->sample[channel] = 0;
				if(this->loop[channel] == false)
				{
					Delete(channel);
				}
			}
		}
		else
		{
			// Memory data
			int mix_size = size;
			int left_size = this->size[channel] - this->sample[channel];
			if(left_size < size)
			{
				mix_size = left_size;
			}
			if(mix_size > 0)
			{
				MixBuffer(buffer, &this->buffer[channel][this->sample[channel]], mix_size);
				this->sample[channel] += mix_size;
				if(this->loop[channel] == true)
				{
					if(size > left_size)
					{
						int left_mix_size = size - left_size;
						if(left_mix_size > 0)
						{
							this->sample[channel] = 0;
							if(this->type[channel] == 1)
							{
								size_t read_size = left_mix_size / 2;
								unsigned char* mix_buffer = new unsigned char[left_mix_size];
								this->adpcm_decorder[channel]->Decord(mix_buffer, &this->buffer[channel][this->sample[channel] / 2], static_cast<int>(read_size));
								MixBuffer(&buffer[mix_size], mix_buffer, left_mix_size);
								delete [] mix_buffer;
							}
							else
							{
								MixBuffer(&buffer[mix_size], &this->buffer[channel][this->sample[channel]], left_mix_size);
							}
							this->sample[channel] += left_mix_size;
						}
					}
				}
			}
			if(this->sample[channel] >= this->size[channel])
			{
				this->sample[channel] = 0;
				if(this->loop[channel] == false)
				{
					Delete(channel);
				}
			}
		}
	}
}

void SoundMixer::Clear(void)
{
	for(int channel = 0; channel < CHANNEL_COUNT; ++ channel)
	{
		Delete(channel);
	}
}

void SoundMixer::MixBuffer(unsigned char* destination_buffer, const unsigned char* source_buffer, int size)
{
	for(int sample = 0; sample < size; ++ sample)
	{
		short data = static_cast<short>(source_buffer[sample]) + static_cast<short>(destination_buffer[sample]) - 128;
		data = ((data > 0) ? ((data < 255) ? data : 255) : 0);
		destination_buffer[sample] = static_cast<unsigned char>(data);
	}
}
