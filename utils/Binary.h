#pragma once

#include <vector>
#include <iostream>

namespace binary
{
	inline std::vector<bool> ReadFromStream(std::istream &stream)
	{
		unsigned int data_size;
		stream >> data_size;

		std::vector<bool> binary;
		binary.reserve(data_size);

		std::vector<unsigned char> buffer((std::istreambuf_iterator<char>(stream)),
			(std::istreambuf_iterator<char>()));

		unsigned char byte = buffer.at(0);
		size_t bit_count = 0;
		size_t byte_count = 0;
		for (unsigned int i = 0; i < data_size; i++)
		{
			const auto bit = byte >> (8 - bit_count - 1);
			if ((bit & 1) == 1)
				binary.push_back(true);
			else
				binary.push_back(false);

			++bit_count;
			if (bit_count == 8)
			{
				byte = buffer[++byte_count];
				bit_count = 0;
			}
		}

		return binary;
	}

	inline void WriteToStream(const std::vector<bool> &binary_data, std::ostream &output)
	{
		const unsigned int data_size = binary_data.size();
		output << data_size;

		unsigned char buffer = 0;
		size_t count = 0;

		for (auto bit : binary_data)
		{
			if (bit)
				buffer |= 1UL << (8 - count - 1);
			++count;
			if(count == 8)
			{
				output << buffer;
				buffer = 0;
				count = 0;
			}
		}

		// write last byte
		output << buffer;
	}
};
