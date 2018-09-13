#include "HuffmanCompression.h"
#include <memory>
#include <queue>

namespace
{
	struct Node
	{
		uint8_t value;
		int frequency;
		std::unique_ptr<Node> left;
		std::unique_ptr<Node> right;

		Node() {  }

		Node(uint8_t value, int frequency, std::unique_ptr<Node> left, std::unique_ptr<Node> right) 
			: value(value), frequency(frequency), left(std::move(left)), right(std::move(right))
		{  }

		bool IsLeaf() const
		{
			return  left == nullptr && right == nullptr;
		}
	};

	struct NodeCmp
	{
		bool operator()(const Node *lhs, const Node *rhs) const { return lhs->frequency > rhs->frequency; }
	};

	void BuildLookupTable(vector<vector<bool>> &table, const Node &node, const vector<bool>& prefix)
	{
		if (!node.IsLeaf())
		{
			auto leftPrefix = prefix;
			leftPrefix.push_back(false);
			BuildLookupTable(table, *node.left, leftPrefix);

			auto rightPrefix = prefix;
			rightPrefix.push_back(true);
			BuildLookupTable(table, *node.right, rightPrefix);
		}
		else
		{
			table[node.value] = prefix;
		}
	}

	void WriteTree(const Node &node, vector<bool>& compressed)
	{
		if (node.IsLeaf())
		{
			compressed.push_back(true);
			for (size_t i = 0; i < 8; i++)
			{
				compressed.push_back(((node.value >> (8 - i - 1)) & 1) == 1);
			}
			return;
		}
		compressed.push_back(false);
		WriteTree(*node.left, compressed);
		WriteTree(*node.right, compressed);
	}
}

std::unique_ptr<Node> ReadTree(vector<bool>::const_iterator& inputIterator)
{
	const bool is_leaf = *inputIterator;
	++inputIterator;

	if (is_leaf)
	{
		uint8_t value = 0;
		for (int i = 0; i < 8; i++)
		{
			if (*inputIterator)
			{
				value += 1 << (8 - i - 1);
			}
			++inputIterator;
		}

		return std::make_unique<Node>(Node{value, -1, nullptr, nullptr});
	}
	auto left = ReadTree(inputIterator);
	auto right = ReadTree(inputIterator);

	return std::make_unique<Node>(Node(0, -1, std::move(left), std::move(right)));
}

vector<bool> HuffmanCompression::Compress(const vector<uint8_t>& input)
{
	vector<int> frequency(256);
	for (auto value : input)
	{
		frequency[value]++;
	}

	// storing pointers but going to convert these guys to unique_ptr later
	std::priority_queue<Node*, vector<Node*>, NodeCmp> tree;
	for (int i = 0; i < 256; i++)
	{
		if (frequency[i] > 0)
		{
			tree.push(new Node{ (uint8_t) i, frequency[i], nullptr, nullptr });
		}
	}

	while (tree.size() > 1)
	{
		const auto left = tree.top(); tree.pop();
		const auto right = tree.top(); tree.pop();
		auto parent = new Node{ 0, left->frequency + right->frequency, std::unique_ptr<Node>(left), std::unique_ptr<Node>(right) };
		tree.push(parent);
	}

	auto head = std::unique_ptr<Node>(tree.top());
	tree.pop();

	vector<vector<bool>> lookup_table(256);
	BuildLookupTable(lookup_table, *head, vector<bool>());

	vector<bool> compressed;

	WriteTree(*head, compressed);

	for (auto inputValue : input)
	{
		auto encoded = lookup_table[inputValue];
		for (auto bit : encoded)
		{
			compressed.push_back(bit);
		}
	}

	return compressed;
}

vector<uint8_t> HuffmanCompression::Decompress(const vector<bool>& input)
{
	auto position = input.begin();
	auto tree = ReadTree(position);

	vector<uint8_t> output;

	while (position != input.end())
	{
		auto node = tree.get();
		while(!node->IsLeaf())
		{
			const auto bit = *position;
			++position;

			if (bit)
			{
				node = node->right.get();
			}
			else
			{
				node = node->left.get();
			}
		}

		output.push_back(node->value);
	}

	return output;
}
