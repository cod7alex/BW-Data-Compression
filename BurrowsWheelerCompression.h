#pragma once

#include <vector>
#include <cstdint>

using std::uint8_t;

class BurrowsWheelerCompression
{
public:
	static std::vector<bool> Compress(const std::vector<uint8_t> &data);
	static std::vector<uint8_t> Decompress(const std::vector<bool> &compressed);
};
