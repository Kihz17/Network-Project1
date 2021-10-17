#include "Buffer.h"

#include <iostream>
#include <algorithm>

namespace netutils
{
	const unsigned int maxStringSize = 16; // The maximum amount of bytes we allow in a string (since they are not fixed sizes like primitives)

	Buffer::Buffer(unsigned int size)
		: writeIndex(0), readIndex(0)
	{
		this->data = new char[size];
		this->length = size;
	}

	void Buffer::WriteInt(int value)
	{
		this->InsertByte(writeIndex, (char) value);
		this->InsertByte(writeIndex + 1, (char) (value >> 8));
		this->InsertByte(writeIndex + 2, (char) (value >> 16));
		this->InsertByte(writeIndex + 3, (char) (value >> 24));
		writeIndex += 4;
	}

	void Buffer::WriteInt(std::size_t index, int value)
	{
		this->InsertByte(index, (char) value);
		this->InsertByte(index + 1, (char) (value >> 8));
		this->InsertByte(index + 2, (char) (value >> 16));
		this->InsertByte(index + 3, (char) (value >> 24));
	}

	void Buffer::WriteShort(short value)
	{
		this->InsertByte(writeIndex, (char)value);
		this->InsertByte(writeIndex + 1, (char)(value >> 8));
		writeIndex += 2;
	}

	void Buffer::WriteShort(std::size_t index, short value)
	{
		this->InsertByte(index, (char)value);
		this->InsertByte(index + 1, (char)(value >> 8));
	}

	void Buffer::WriteString(std::string value)
	{
		if (value.size() > maxStringSize)
		{
			std::cout << "String was too large for our buffer!" << std::endl;
			return;
		}

		const char* cStr = value.c_str();
		std::size_t shift = 0;
		for (int i = 0; i < value.size(); i++)
		{
			this->InsertByte(writeIndex + i, (char) (cStr[i] >> shift));
			shift += 8;
		}

		writeIndex += maxStringSize;
	}

	void Buffer::WriteString(std::size_t index, std::string value)
	{
		if (value.size() > maxStringSize)
		{
			std::cout << "String was too large for our buffer!" << std::endl;
			return;
		}

		const char* cStr = value.c_str();
		std::size_t shift = 0;
		for (int i = 0; i < value.size(); i++)
		{
			this->InsertByte(index + i, (char)(cStr[i] >> shift));
			shift += 8;
		}
	}

	int Buffer::ReadInt()
	{
		int value = this->data[readIndex];
		value |= this->data[readIndex + 1] << 8;
		value |= this->data[readIndex + 2] << 16;
		value |= this->data[readIndex + 3] << 24;
		readIndex += 4;
		return value;
	}

	int Buffer::ReadInt(std::size_t index)
	{
		int value = this->data[index];
		value |= this->data[index + 1] << 8;
		value |= this->data[index + 2] << 16;
		value |= this->data[index + 3] << 24;
		return value;
	}

	short Buffer::ReadShort()
	{
		short value = this->data[readIndex];
		value |= this->data[readIndex + 1] << 8;
		readIndex += 2;
		return value;
	}

	short Buffer::ReadShort(std::size_t index)
	{
		short value = this->data[index];
		value |= this->data[index + 1] << 8;
		return value;
	}

	std::string Buffer::ReadString()
	{
		std::string value(&this->data[readIndex], this->data[readIndex + maxStringSize]);
		readIndex += maxStringSize;
		return value;
	}

	std::string Buffer::ReadString(std::size_t index)
	{
		std::string value(&this->data[index], this->data[index + maxStringSize]);
		return value;
	}

	int Buffer::Length()
	{
		return this->length;
	}

	void Buffer::Resize(std::size_t length)
	{
		char* newBuffer = new char[length];
		memcpy(newBuffer, this->data, this->length * sizeof(char)); // Copy old contents into new
		this->length = length;
		delete[] this->data;
		this->data = newBuffer;
	}

	void Buffer::Clear()
	{
		for (int i = 0; i < this->length; i++)
		{
			this->data[i] = 0; // Zero out entire buffer
		}

		this->readIndex = 0;
		this->writeIndex = 0;
	}

	void Buffer::InsertByte(int index, char byte)
	{
		if (index >= this->length) // We have run out of space, we must resize
		{
			this->Resize(this->length * 2);
		}

		this->data[index] = byte;
	}
};