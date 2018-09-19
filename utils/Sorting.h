#pragma once

#include <vector>
#include <functional>

namespace sorting
{
	template<typename T, typename A>
	void CountingSort(
		const std::vector<T, A>& unsorted,
		const int max_value, 
		const std::function<int(const T& item)>& get_value,
		std::vector<T, A>& result)
	{
		const size_t size = max_value + 1;
		std::vector<int> occurence_count(size, 0);

		for (const auto suffix : unsorted)
		{
			const auto value = get_value(suffix);
			++occurence_count[value];
		}

		// switch from counts to positions in output
		for (size_t i = 1; i < size; i++)
		{
			occurence_count[i] += occurence_count[i - 1];
		}

		for (int i = unsorted.size() - 1; i >= 0; i--)
		{
			const auto value = get_value(unsorted[i]);
			const auto new_index = occurence_count[value];
			--occurence_count[value];
			result[new_index - 1] = unsorted[i];
		}
	}

	template<typename T, typename A>
	std::vector<T, A> CountingSort(
		const std::vector<T, A>& unsorted,
		const int max_value,
		const std::function<int(const T& item)>& get_value)
	{
		std::vector<T> result(unsorted.size());

		CountingSort<T>(unsorted, max_value, get_value, result);

		return result;
	}
}
