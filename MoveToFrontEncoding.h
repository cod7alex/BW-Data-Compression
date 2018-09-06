#pragma once

#include <vector>

using std::vector;

class MoveToFrontEncoding {
public:
	static vector<uint8_t> Encode(const vector<uint8_t> &input);
	static vector<uint8_t> Decode(const vector<uint8_t> &input);
private:
	static vector<uint8_t> InitAlphabet();
	static uint8_t FindAlphabetIndex(const vector<uint8_t> &alphabet, uint8_t value);
	static void MoveToFront(vector<uint8_t> &alphabet, size_t index);
};

