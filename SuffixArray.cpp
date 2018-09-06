#include "SuffixArray.h"

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

void RadixPass(const vector<Suffix>& suffixes, vector<Suffix>& result,
               const std::function<int(const Suffix& suffix)>& get_value)
{
	vector<int> occurence_count(256 + 1, 0);

	for (auto suffix : suffixes)
	{
		const auto value = get_value(suffix) + 1;
		occurence_count[value]++;
	}

	// switch from counts to positions in output
	for (int i = 1; i < 257; i++)
	{
		occurence_count[i] += occurence_count[i - 1];
	}

	for (int i = suffixes.size() - 1; i >= 0; i--)
	{
		const auto value = get_value(suffixes[i]) + 1;
		const auto new_index = occurence_count[value];
		occurence_count[value]--;
		result[new_index - 1] = suffixes[i];
	}
}

void RadixSort(vector<Suffix>& suffixes)
{
	vector<Suffix> temp(suffixes.size());

	// radix sort in two passes - first by next_rank and then by rank
	RadixPass(suffixes, temp, [](const Suffix& suffix) -> int
	{
		return suffix.next_rank;
	});

	RadixPass(temp, suffixes, [](const Suffix& suffix) -> int
	{
		return suffix.rank;
	});
}

vector<int> SuffixArray::BuildSuffixArray(const vector<uint8_t>& input)
{
	const auto input_size = input.size();

	vector<Suffix> suffixes(input.size());
	for (size_t i = 0; i < input_size; i++)
	{
		suffixes[i].index = i;
		suffixes[i].rank = input[i];
		suffixes[i].next_rank = i + 1 < input_size ? input[i + 1] : -1;
	}

	// sort by first two characters
	RadixSort(suffixes);

	vector<int> start_index_to_suffix_mapping(input_size);

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
			const size_t next_index = suffixes[i].index + k / 2;
			suffixes[i].next_rank = (next_index < input_size)
				                        ? suffixes[start_index_to_suffix_mapping[next_index]].rank
				                        : -1;
		}

		// sort the suffixes according to first k characters
		RadixSort(suffixes);
	}

	vector<int> suffix_array(input_size);
	for (size_t i = 0; i < input_size; i++)
		suffix_array[i] = suffixes[i].index;

	return suffix_array;
}
