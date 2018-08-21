#include "MoveToFrontEncoding.h"

vector<uint8_t> MoveToFrontEncoding::Encode(vector<uint8_t> input)
{
	auto alphabet = InitAlphabet();

	vector<uint8_t> output(input.size());

	int size = input.size();
	for (int inputIndex = 0; inputIndex < size; ++inputIndex)
	{
		uint8_t current = input[inputIndex];

		auto index = FindAlphabetIndex(alphabet, current);
		output[inputIndex] = index;

		MoveToFront(alphabet, index, current);
	}

	return output;
}

vector<uint8_t> MoveToFrontEncoding::Decode(vector<uint8_t> input)
{
	auto alphabet = InitAlphabet();

	vector<uint8_t> output(input.size());

	int size = input.size();
	for (int inputIndex = 0; inputIndex < size; ++inputIndex)
	{
		uint8_t current = input[inputIndex];

		output[inputIndex] = alphabet[current];

		MoveToFront(alphabet, current, alphabet[current]);
	}

	return output;
}

vector<uint8_t> MoveToFrontEncoding::InitAlphabet()
{
	const int kAlphabetSize = 256;
	vector<uint8_t> alphabet(256);

	for (int i = 0; i < 256; ++i)
	{
		alphabet[i] = i;
	}

	return alphabet;
}

int MoveToFrontEncoding::FindAlphabetIndex(const vector<uint8_t>& alphabet, uint8_t value)
{
	int size = alphabet.size();
	for (int alphabetIndex = 0; alphabetIndex < size; ++alphabetIndex)
	{
		if (alphabet[alphabetIndex] == value)
		{
			return alphabetIndex;
		}
	}
}

void MoveToFrontEncoding::MoveToFront(vector<uint8_t>& alphabet, int index, uint8_t value)
{
	while (index > 0)
	{
		alphabet[index] = alphabet[index - 1];
		--index;
	}
	alphabet[0] = value;
}
