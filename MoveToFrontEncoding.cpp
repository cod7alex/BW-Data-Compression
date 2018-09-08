#include "MoveToFrontEncoding.h"
#include <algorithm>
#include <stdexcept>

vector<uint8_t> MoveToFrontEncoding::Encode(const vector<uint8_t>& input)
{
	auto alphabet = InitAlphabet();

	vector<uint8_t> output(input.size());

	const auto size = input.size();
	for (size_t input_index = 0; input_index < size; ++input_index)
	{
		const auto current = input[input_index];

		const auto index = FindAlphabetIndex(alphabet, current);
		output[input_index] = index;

		MoveToFront(alphabet, index);
	}

	return output;
}

vector<uint8_t> MoveToFrontEncoding::Decode(const vector<uint8_t>& input)
{
	auto alphabet = InitAlphabet();

	vector<uint8_t> output(input.size());

	const auto size = input.size();
	for (size_t input_index = 0; input_index < size; ++input_index)
	{
		const auto current = input[input_index];

		output[input_index] = alphabet[current];

		MoveToFront(alphabet, current);
	}

	return output;
}

vector<uint8_t> MoveToFrontEncoding::InitAlphabet()
{
	const size_t kAlphabetSize = 256;
	vector<uint8_t> alphabet(kAlphabetSize);

	for (size_t i = 0; i < kAlphabetSize; ++i)
	{
		alphabet[i] = i;
	}

	return alphabet;
}

uint8_t MoveToFrontEncoding::FindAlphabetIndex(const vector<uint8_t>& alphabet, const uint8_t value)
{
	const auto size = alphabet.size();
	for (size_t alphabet_index = 0; alphabet_index < size; ++alphabet_index)
	{
		if (alphabet[alphabet_index] == value)
		{
			return alphabet_index;
		}
	}

	throw std::runtime_error("Invalid alphabet state during Move to Front encoding");
}

void MoveToFrontEncoding::MoveToFront(vector<uint8_t>& alphabet, const size_t index)
{
	const auto moved_item_iterator = alphabet.begin() + index;
	std::rotate(alphabet.begin(), moved_item_iterator, moved_item_iterator + 1);
}
