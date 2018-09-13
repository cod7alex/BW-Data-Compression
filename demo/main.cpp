#include <iostream>
#include <string>
#include "../MoveToFrontEncoding.h"
#include "../BWTransform.h"
#include "../HuffmanCompression.h"

using namespace std;

int main()
{
	// string inputStr = "abbbaabbbbaccabbaaabc";
	// string inputStr = "cabc";
	// string inputStr = "ABRACADABRA!";
	string inputStr = "yabbadabbado";
	vector<uint8_t> input(inputStr.begin(), inputStr.end());

	auto encoded = MoveToFrontEncoding::Encode(input);

	cout << "Encoding string '" << inputStr << "' using Move-to-Front encoding:" << endl;
	cout << std::hex;
	for (auto i : encoded)
	{
		cout << (int)i << " ";
	}
	cout << endl << std::dec;

	auto decoded = MoveToFrontEncoding::Decode(encoded);

	cout << "Decoding numbers back to string:" << endl;
	for (auto i : decoded)
	{
		cout << (char)i;
	}
	cout << endl;

	cout << "Creating suffix array for string '" << inputStr << "'" << endl;
	auto suffix_array = SuffixArray(input);
	for (int i = 0; i < suffix_array.Length(); i++)
	{
		cout << suffix_array.Index(i) << " ";
	}
	cout << endl;

	cout << "Applying Burrows-Wheeler transform to string '" << inputStr << "'" << endl;
	auto transformed = BWTransform::Transform(input);
	cout << std::hex;
	for (auto i : transformed)
	{
		cout << (int)i << " ";
	}
	cout << endl << std::dec;

	cout << "Applying inverse Burrows-Wheeler transform" << endl;
	auto transformed_back = BWTransform::InverseTransform(transformed);
	for (auto i : transformed_back)
	{
		cout << (char)i;
	}
	cout << endl;

	auto transformed_back_string = string(transformed_back.begin(), transformed_back.end());
	auto are_equal = inputStr == transformed_back_string;
	cout<< std::boolalpha << "Comparing to initial string: " << are_equal << endl << std::dec;

	cout << "Applying Huffman compression to string '" << inputStr << "'" << endl;
	auto compressed = HuffmanCompression::Compress(input);
	for (auto i : compressed)
	{
		cout << (int)i;
	}
	cout << endl;

	cout << "Decoding Huffman compressed bits" << endl;
	auto decompressed = HuffmanCompression::Decompress(compressed);
	for (auto i : decompressed)
	{
		cout << (char)i;
	}
	cout << endl;

	cout << "Applying full Burrows-Wheeler compression to string '" << inputStr << "'" << endl;
	cout << "'" << inputStr << "'" << " is " << inputStr.size() * std::numeric_limits<unsigned char>::digits << " bits" << endl;
	auto bw_transformed = BWTransform::Transform(input);
	auto move_to_front_encoded = MoveToFrontEncoding::Encode(bw_transformed);
	auto huffman_compressed = HuffmanCompression::Compress(move_to_front_encoded);

	cout << "Output with size of " << huffman_compressed.size() << " bits" << endl;
	for (auto i : huffman_compressed)
	{
		cout << (int)i;
	}
	cout << endl;

	cout << "Inverting Burrows-Wheeler compression" << endl;
	auto huffman_decompressed = HuffmanCompression::Decompress(huffman_compressed);
	auto move_to_front_decoded = MoveToFrontEncoding::Decode(huffman_decompressed);
	auto bw_inverse_transformed = BWTransform::InverseTransform(move_to_front_decoded);

	for (auto i : bw_inverse_transformed)
	{
		cout << (char)i;
	}
	cout << endl;

	getchar();

	return 0;
}