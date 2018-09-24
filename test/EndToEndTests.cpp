#include <gtest/gtest.h>
#include <random>
#include <limits>

#include "../BWTransform.h"
#include "../MoveToFrontEncoding.h"
#include "../HuffmanCompression.h"
#include "../BurrowsWheelerCompression.h"

vector<uint8_t> GenerateRandomData(size_t size)
{
	std::mt19937 gen(time(0));
	std::uniform_int_distribution<int> dist(0, 255);

	vector<uint8_t> data(size);
	for (auto i = 0; i < size; i++)
	{
		data[i] = dist(gen);
	}

	return data;
}

TEST(BWTransform, EndToEnd)
{
	int size = 10;
	for (auto i = 1; i < 6; i++)
	{
		size *= 10;
		auto data = GenerateRandomData(size);
		auto transformed = BWTransform::Transform(data);
		auto transformed_back = BWTransform::InverseTransform(transformed);

		EXPECT_EQ(data, transformed_back);
	}
}

TEST(MoveToFront, EndToEnd)
{
	int size = 10;
	for (auto i = 1; i < 6; i++)
	{
		size *= 10;
		auto data = GenerateRandomData(size);
		auto transformed = MoveToFrontEncoding::Encode(data);
		auto transformed_back = MoveToFrontEncoding::Decode(transformed);

		EXPECT_EQ(data, transformed_back);
	}
}

TEST(Huffman, EndToEnd)
{
	int size = 10;
	for (auto i = 1; i < 6; i++)
	{
		size *= 10;
		auto data = GenerateRandomData(size);
		auto compressed = HuffmanCompression::Compress(data);
		auto decompressed = HuffmanCompression::Decompress(compressed);

		EXPECT_EQ(data, decompressed);
	}
}

TEST(FullCompression, EndToEnd)
{
	int size = 10;
	for (auto i = 1; i < 6; i++)
	{
		size *= 10;
		auto data = GenerateRandomData(size);
		auto compressed = BurrowsWheelerCompression::Compress(data);
		auto decompressed = BurrowsWheelerCompression::Decompress(compressed);

		EXPECT_EQ(data, decompressed);
	}
}

