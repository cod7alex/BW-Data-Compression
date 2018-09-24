#include <deque>

#include "BWTransform.h"
#include "utils/Sorting.h"

vector<uint8_t> BWTransform::Transform(const vector<uint8_t>& input)
{
	auto suffix_array = SuffixArray(input);
	const auto length = suffix_array.Length();

	std::deque<uint8_t> transform_result;

	for (size_t i = 0; i < length; i++)
	{
		const auto current_suffix_index = suffix_array.Index(i);
		if (current_suffix_index == 0)
		{
			// write byte representation of i (which is index of original string in suffix array) to front of output
			// writing in inverse order to get correct representation in result
			for (size_t j = 0; j < 4; j++)
			{
				const uint8_t byte = (i >> 8 * j) & 0xFF;
				transform_result.push_front(byte);
			}

			transform_result.push_back(input[input.size() - 1]);
		}
		else
		{
			transform_result.push_back(input[current_suffix_index - 1]);
		}
	}

	return vector<uint8_t>(transform_result.begin(), transform_result.end());
}

vector<int> GetFirstColumnMapping(const vector<uint8_t> &input, const size_t skip);

vector<uint8_t> BWTransform::InverseTransform(const vector<uint8_t> &input)
{
	auto first = 0;
	for (size_t i = 0; i < 4; i++)
	{
		first |= input[i] << (3 - i) * 8;
	}

	// 'first' value takes 4 bytes
	const size_t output_size = input.size() - 4;

	const auto first_column_mapping = GetFirstColumnMapping(input, 4);

	vector<uint8_t> inverse_transformed(output_size);

	size_t move_backward_position = first;
	for (int j = output_size - 1; j >= 0; j--)
	{
		inverse_transformed[j] = input[4 + move_backward_position];
		move_backward_position = first_column_mapping[move_backward_position];
	}

	return inverse_transformed;
}

vector<int> GetFirstColumnMapping(const vector<uint8_t> &input, const size_t skip)
{
	const auto size = input.size() - skip;
	vector<std::pair<uint8_t, size_t>> first_column(size);

	size_t initial_index = 0;
	for (auto it = input.begin() + 4; it != input.end(); ++it)
	{
		first_column[initial_index].first = *it;
		first_column[initial_index].second = initial_index;
		++initial_index;
	}

	const auto get_value = [](const std::pair<uint8_t, size_t> &value_index_pair) -> uint8_t
	{
		return value_index_pair.first;
	};

	const auto sorted_first_column = sorting::CountingSort<std::pair<uint8_t, size_t>>(first_column, 255, get_value);

	vector<int> mapping(size);
	for (size_t i = 0; i < size; i++)
	{
		const auto mapping_index = sorted_first_column[i].second;
		mapping[mapping_index] = i;
	}

	return mapping;
}
