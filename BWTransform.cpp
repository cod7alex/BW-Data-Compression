#include "BWTransform.h"

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
			// write byte representation of i (which is index of first input suffix in suffix array) to front of output
			// writing in inverse order to get correct representation in result
			for (size_t j = 0; j < 4; j++)
			{
				transform_result.push_front(i >> 8 * j);
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

vector<uint8_t> BWTransform::InverseTransform(vector<uint8_t> input)
{
	auto first = 0;
	for (size_t i = 0; i < 4; i++)
	{
		first |= input[i] >> (3 - i) * 8;
	}

	// 'first' value takes 4 bytes
	const size_t output_size = input.size() - 4;

	vector<std::pair<int, size_t>> first_column(output_size);

	size_t i = 0;
	for (auto it = input.begin() + 4; it != input.end(); ++it)
	{
		first_column[i].first = *it;
		first_column[i].second = i;
		++i;
	}

	// TODO Radix Sort me
	std::sort(first_column.begin(), first_column.end(),
	          [](const std::pair<int, size_t>& left, const std::pair<int, size_t>& right) -> bool
	          {
		          return left.first < right.first;
	          });

	vector<int> last_forward_mapping(output_size);
	for (size_t j = 0; j < output_size; j++)
	{
		const auto mapping_index = first_column[j].second;
		last_forward_mapping[mapping_index] = j;
	}

	vector<uint8_t> inverse_transformed(output_size);

	size_t move_backward_position = first;
	for (int j = output_size - 1; j >= 0; j--)
	{
		inverse_transformed[j] = input[4 + move_backward_position];
		move_backward_position = last_forward_mapping[move_backward_position];
	}

	return inverse_transformed;
}
