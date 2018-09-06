#pragma once

#include <vector>
#include <functional>

using std::vector;

class SuffixArray {
public:
	SuffixArray(const vector<uint8_t> &input);
	int Index(int i);
	size_t Length() const;
private:
	int length_;
	vector<int> suffix_array_;
	static vector<int> BuildSuffixArray(const vector<uint8_t> &input);
};

