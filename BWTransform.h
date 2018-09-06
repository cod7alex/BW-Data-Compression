#pragma once
#include "SuffixArray.h"
#include <algorithm> // TODO remove me
#include <deque>

class BWTransform {
public:
	static vector<uint8_t> Transform(const vector<uint8_t> &input);
	static vector<uint8_t> InverseTransform(vector<uint8_t> input);
};

