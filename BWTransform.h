#pragma once

#include "SuffixArray.h"
#include <cstdint>

using std::uint8_t;

class BWTransform {
public:
	static vector<uint8_t> Transform(const vector<uint8_t> &input);
	static vector<uint8_t> InverseTransform(const vector<uint8_t> &input);
};

