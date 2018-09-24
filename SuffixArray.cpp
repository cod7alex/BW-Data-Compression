#include "SuffixArray.h"
#include "utils/Sorting.h"

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

void RadixSort(vector<Suffix>& suffixes, int max_rank);

vector<int> SuffixArray::BuildSuffixArray(const vector<uint8_t>& input)
{
	const auto input_size = input.size();

	vector<Suffix> suffixes(input.size());

	for (size_t i = 0; i < input_size; i++)
	{
		suffixes[i].index = i;
		suffixes[i].rank = input[i];
		suffixes[i].next_rank = input[(i + 1) % input_size];
	}

	// sort by first two characters
	RadixSort(suffixes, 255);

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

void RadixSort(vector<Suffix>& suffixes, const int max_rank)
{
	vector<Suffix> temp(suffixes.size());

	// radix sort in two passes - first by next_rank and then by rank
	const auto get_next_rank = [](const Suffix& suffix) -> int
	{
		return suffix.next_rank;
	};

	const auto get_rank = [](const Suffix& suffix) -> int
	{
		return suffix.rank;
	};

	sorting::CountingSort<Suffix>(suffixes, max_rank, get_next_rank, temp);
	sorting::CountingSort<Suffix>(temp, max_rank, get_rank, suffixes);
}
