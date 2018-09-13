#pragma once

#include <vector>
#include <cstdint>

using std::vector;
using std::uint8_t;

class HuffmanCompression {
public:
	static vector<bool> Compress(const vector<uint8_t> &input);
	static vector<uint8_t> Decompress(const vector<bool>& input);
};

