#pragma once

#include <string>
#include <vector>

namespace netutils
{
	class Buffer
	{
	public:
		Buffer(unsigned int size);

		void WriteInt(int value);
		void WriteInt(std::size_t index, int value);

		void WriteShort(short value);
		void WriteShort(std::size_t index, short value);

		void WriteString(std::string value);
		void WriteString(std::size_t index, std::string value);

		int ReadInt();
		int ReadInt(std::size_t index);

		short ReadShort();
		short ReadShort(std::size_t index);

		std::string ReadString(std::size_t length);
		std::string ReadString(std::size_t index, std::size_t length);

		int Length();
		void Resize(std::size_t  length);
		void Clear();

		char* data;

	private:
		int readIndex = 0;
		int writeIndex = 0;
		int length;

		void InsertByte(int index, char byte);
	};
};