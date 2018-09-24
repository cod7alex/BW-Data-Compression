#include <iostream>
#include <string>
#include <limits>

#include "../MoveToFrontEncoding.h"
#include "../BWTransform.h"
#include "../HuffmanCompression.h"
#include "../CompressionClient.h"

using namespace std;

void test();
void compress(const std::string&, const std::string&);
void decompress(const std::string& input_path, const std::string& output_path);

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		cout << "You must specify mode parameter: '-t', '-c', '-d' and path to file for last two modes" << endl;
		return 0;
	}
	const auto mode = std::string(argv[1]);
	if (mode == "-t")
		test();
	else if (mode == "-c")
		compress(argv[2], argv[3]);
	else if (mode == "-d")
		decompress(argv[2], argv[3]);

	getchar();

	return 0;
}

void compress(const std::string& input_path, const std::string& output_path)
{
	BWCompressionClient::CompressFile(input_path, output_path);

	cout << "Compression done, check " << output_path << endl;
}

void decompress(const std::string& input_path, const std::string& output_path)
{
	BWCompressionClient::DecompressFile(input_path, output_path);

	cout << "Decompression done, check " << output_path << endl;
}

void test()
{
	// string inputStr = "banananb";
	// string inputStr = "ABRACADABRA!!";
	// string inputStr = "Implement a redundancy detector a";
	string inputStr = "Everyone wants to live in (a) Princeton"
		"Urban planner uncovers yearning for sense of place - and"
		"birds"
		""
		"By Kathleen McGinn Spring"
		"Princeton Packet Business Editor"
		"Monday, March 22, 1999"
		""
		"Anton Nelessen knows exactly where citizens of the 21st"
		"century want to live, because tens of thousands of them in"
		"Durban, South Africa, Orlando, Milwaukee, Atlanta, Metuchen"
		"and hundreds of other spots around the globe have told him."
		"And where they want to live is .....downtown Princeton."
		""
		"Well, perhaps not in Princeton per se, but in a place that"
		"By Kathleen McGinn Spring"
		"Princeton Packet Business Editor"
		"Monday, March 22, 1999"
		""
		"Anton Nelessen knows exactly where citizens of the 21st"
		"century want to live, because tens of thousands of them in"
		"Durban, South Africa, Orlando, Milwaukee, Atlanta, Metuchen"
		"and hundreds of other spots around the globe have told him."
		"And where they want to live is .....downtown Princeton."
		""
		"Well, perhaps not in Princeton per se, but in a place that"
		"By Kathleen McGinn Spring"
		"Princeton Packet Business Editor"
		"Monday, March 22, 1999"
		""
		"Anton Nelessen knows exactly where citizens of the 21st"
		"century want to live, because tens of thousands of them in"
		"Durban, South Africa, Orlando, Milwaukee, Atlanta, Metuchen"
		"and hundreds of other spots around the globe have told him."
		"And where they want to live is .....downtown Princeton."
		""
		"Well, perhaps not in Princeton per se, but in a place that"
		"By Kathleen McGinn Spring"
		"Princeton Packet Business Editor"
		"Monday, March 22, 1999"
		""
		"Anton Nelessen knows exactly where citizens of the 21st"
		"century want to live, because tens of thousands of them in"
		"Durban, South Africa, Orlando, Milwaukee, Atlanta, Metuchen"
		"and hundreds of other spots around the globe have told him."
		"And where they want to live is .....downtown Princeton."
		""
		"Well, perhaps not in Princeton per se, but in a place that"
		"By Kathleen McGinn Spring"
		"Princeton Packet Business Editor"
		"Monday, March 22, 1999"
		""
		"Anton Nelessen knows exactly where citizens of the 21st"
		"century want to live, because tens of thousands of them in"
		"Durban, South Africa, Orlando, Milwaukee, Atlanta, Metuchen"
		"and hundreds of other spots around the globe have told him."
		"And where they want to live is .....downtown Princeton."
		""
		"Well, perhaps not in Princeton per se, but in a place that";

	const vector<uint8_t> input(inputStr.begin(), inputStr.end());

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

	const auto transformed_back_string = string(transformed_back.begin(), transformed_back.end());
	const auto are_equal = inputStr == transformed_back_string;
	cout << std::boolalpha << "Comparing to initial string: " << are_equal << endl << std::dec;

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
	const auto bw_transformed = BWTransform::Transform(input);
	const auto move_to_front_encoded = MoveToFrontEncoding::Encode(bw_transformed);
	auto huffman_compressed = HuffmanCompression::Compress(move_to_front_encoded);

	cout << "Output with size of " << huffman_compressed.size() << " bits" << endl;
	for (auto i : huffman_compressed)
	{
		cout << (int)i;
	}
	cout << endl;

	cout << "Inverting Burrows-Wheeler compression" << endl;
	const auto huffman_decompressed = HuffmanCompression::Decompress(huffman_compressed);
	const auto move_to_front_decoded = MoveToFrontEncoding::Decode(huffman_decompressed);
	auto bw_inverse_transformed = BWTransform::InverseTransform(move_to_front_decoded);

	for (auto i : bw_inverse_transformed)
	{
		cout << (char)i;
	}
	cout << endl;
}
