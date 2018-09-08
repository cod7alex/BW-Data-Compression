#include <iostream>
#include <string>
#include "../MoveToFrontEncoding.h"
#include "../BWTransform.h"

using namespace std;

int main()
{
	// string inputStr = "abbbaabbbbaccabbaaabc";
	string inputStr = "ABRACADABRA!";
	// string inputStr = "yabbadabbado";
	vector<uint8_t> input (inputStr.begin(), inputStr.end());

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

	getchar();

	return 0;
}