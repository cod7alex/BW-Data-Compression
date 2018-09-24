#include <memory>
#include <queue>

#include "HuffmanCompression.h"

struct Node
{
	uint8_t value;
	int frequency;
	std::unique_ptr<Node> left;
	std::unique_ptr<Node> right;

	Node(uint8_t value, int frequency)
		: value(value), frequency(frequency), left(nullptr), right(nullptr)
	{  }

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

void BuildLookupTable(const Node &node, const vector<bool>& prefix, vector<vector<bool>> &table);

std::unique_ptr<Node> CreateTrie(const vector<uint8_t>& input);

void WriteTree(const Node &node, vector<bool>& compressed);

std::unique_ptr<Node> ReadTrie(vector<bool>::const_iterator& inputIterator);

vector<bool> HuffmanCompression::Compress(const vector<uint8_t>& input)
{
	const auto head = CreateTrie(input);

	vector<vector<bool>> lookup_table(256);
	BuildLookupTable(*head, vector<bool>(), lookup_table);

	vector<bool> compressed;

	WriteTree(*head, compressed);

	for (auto input_value : input)
	{
		auto encoded = lookup_table[input_value];
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
	auto trie = ReadTrie(position);

	vector<uint8_t> output;

	while (position != input.end())
	{
		auto node = trie.get();
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

void BuildLookupTable(const Node &node, const vector<bool>& prefix, vector<vector<bool>> &table)
{
	if (!node.IsLeaf())
	{
		auto left_prefix = prefix;
		left_prefix.push_back(false);
		BuildLookupTable(*node.left, left_prefix, table);

		auto right_prefix = prefix;
		right_prefix.push_back(true);
		BuildLookupTable(*node.right, right_prefix, table);
	}
	else
	{
		table[node.value] = prefix;
	}
}

std::unique_ptr<Node> CreateTrie(const vector<uint8_t>& input)
{
	vector<int> frequency(256);
	for (auto value : input)
	{
		frequency[value]++;
	}

	// storing pointers but going to convert these guys to unique_ptr later
	std::priority_queue<Node*, vector<Node*>, NodeCmp> trie;
	for (auto i = 0; i < 256; i++)
	{
		if (frequency[i] > 0)
		{
			trie.push(new Node(static_cast<uint8_t>(i), frequency[i]));
		}
	}

	while (trie.size() > 1)
	{
		const auto left = trie.top(); trie.pop();
		const auto right = trie.top(); trie.pop();
		auto parent = new Node(0, left->frequency + right->frequency, std::unique_ptr<Node>(left), std::unique_ptr<Node>(right));
		trie.push(parent);
	}

	auto head = std::unique_ptr<Node>(trie.top());
	trie.pop();

	return head;
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

std::unique_ptr<Node> ReadTrie(vector<bool>::const_iterator& inputIterator)
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

		return std::make_unique<Node>(Node(value, -1));
	}
	auto left = ReadTrie(inputIterator);
	auto right = ReadTrie(inputIterator);

	return std::make_unique<Node>(Node(0, -1, std::move(left), std::move(right)));
}
