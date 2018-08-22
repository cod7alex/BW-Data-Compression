#pragma once
#include <vector>

using std::vector;

class MoveToFrontEncoding
{
public:
	static vector<uint8_t> Encode(vector<uint8_t> input);
	static vector<uint8_t> Decode(vector<uint8_t> input);
private:
	static vector<uint8_t> InitAlphabet();
	static int FindAlphabetIndex(const vector<uint8_t>& alphabet, uint8_t value);
	static void MoveToFront(vector< uint8_t>& alphabet, int index);
};

