#include "SuffixArray.h"
#include <functional>
#include <algorithm>

SuffixArray::SuffixArray(const vector<uint8_t>& input)
	: length_(input.size())
{
	suffix_array_ = BuildSuffixArray(input);
}

int SuffixArray::Index(const int i)
{
	return suffix_array_.at(i);
}

size_t SuffixArray::Length() const
{
	return length_;
}

struct Suffix
{
	int index;
	int rank;
	int next_rank;
};

void RadixPass(const vector<Suffix>& suffixes, int max_rank, vector<Suffix>& result,
               const std::function<int(const Suffix& suffix)>& get_value)
{
	const size_t size = max_rank + 1;
	vector<int> occurence_count(size, 0);

	for (auto suffix : suffixes)
	{
		const auto value = get_value(suffix);
		occurence_count[value]++;
	}

	// switch from counts to positions in output
	for (size_t i = 1; i < size; i++)
	{
		occurence_count[i] += occurence_count[i - 1];
	}

	for (int i = suffixes.size() - 1; i >= 0; i--)
	{
		const auto value = get_value(suffixes[i]);
		const auto new_index = occurence_count[value];
		occurence_count[value]--;
		result[new_index - 1] = suffixes[i];
	}
}

void RadixSort(vector<Suffix>& suffixes, int max_rank)
{
	vector<Suffix> temp(suffixes.size());

	// radix sort in two passes - first by next_rank and then by rank
	RadixPass(suffixes, max_rank, temp, [](const Suffix& suffix) -> int
	{
		return suffix.next_rank;
	});

	RadixPass(temp, max_rank, suffixes, [](const Suffix& suffix) -> int
	{
		return suffix.rank;
	});
}

vector<int> SuffixArray::BuildSuffixArray(const vector<uint8_t>& input)
{
	const auto input_size = input.size();

	vector<Suffix> suffixes(input.size());
	int max_rank = 0;
	for (size_t i = 0; i < input_size; i++)
	{
		suffixes[i].index = i;
		suffixes[i].rank = input[i];
		suffixes[i].next_rank = input[(i + 1) % input_size];
		max_rank = std::max(max_rank, std::max(suffixes[i].rank, suffixes[i].next_rank));
	}

	// sort by first two characters
	RadixSort(suffixes, max_rank);

	vector<int> start_index_to_suffix_mapping(input_size);

	// now we can sort suffixes by first 4 characters
	// we assign relative ranks to sorted suffixes based on their position
	// first suffix has rank 0, suffixes with same symbols have same ranks
	// after that we can "construct" the 4-character suffix from two parts:
	// (rank of first 2-character sequence)|(rank of second 2-character sequence)
	// repeat the same for k
	for (size_t k = 4; k < 2 * input_size; k = k * 2)
	{
		auto rank = 0;
		auto previous_suffix_rank = suffixes[0].rank;
		suffixes[0].rank = rank;
		start_index_to_suffix_mapping[suffixes[0].index] = 0;

		// assigning rank to suffixes
		for (size_t i = 1; i < input_size; i++)
		{
			// if first rank and next ranks are same as that of previous suffix
			// assign the same new rank to this suffix
			if (suffixes[i].rank == previous_suffix_rank &&
				suffixes[i].next_rank == suffixes[i - 1].next_rank)
			{
				previous_suffix_rank = suffixes[i].rank;
				suffixes[i].rank = rank;
			}
			else
			{
				previous_suffix_rank = suffixes[i].rank;
				suffixes[i].rank = ++rank;
			}
			start_index_to_suffix_mapping[suffixes[i].index] = i;
		}

		// assign next rank to every suffix
		for (size_t i = 0; i < input_size; i++)
		{
			const size_t second_suffix_part_start_index = (suffixes[i].index + k / 2) % input_size;
			suffixes[i].next_rank = suffixes[start_index_to_suffix_mapping[second_suffix_part_start_index]].rank;
		}

		// sort the suffixes according to first k characters
		RadixSort(suffixes, rank);
	}

	vector<int> suffix_array(input_size);
	for (size_t i = 0; i < input_size; i++)
		suffix_array[i] = suffixes[i].index;

	return suffix_array;
}
