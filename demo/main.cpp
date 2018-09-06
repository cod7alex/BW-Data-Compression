#include <iostream>
#include <string>
#include "../MoveToFrontEncoding.h"
#include "../BWTransform.h"

using namespace std;

int main()
{
	string inputStr = "ABRACADABRA!";
	// string inputStr = "yabbadabbado";
	vector<uint8_t> input (inputStr.begin(), inputStr.end());

	auto encoded = MoveToFrontEncoding::Encode(input);

	cout << "Encoding string 'ABRACADABRA!' using Move-to-Front encoding:" << endl;
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

	cout << "Creating suffix array for string 'ABRACADABRA!'" << endl;
	auto suffix_array = SuffixArray(input);
	for (int i = 0; i < suffix_array.Length(); i++)
	{
		cout << suffix_array.Index(i) << " ";
	}
	cout << endl;

	cout << "Applying Burrows-Wheeler transform to string 'ABRACADABRA!'" << endl;
	auto transformed = BWTransform::Transform(input);
	cout << std::hex;
	for (auto i : transformed)
	{
		cout << (int)i << " ";
	}
	cout << endl << std::dec;

	cout << "Applying inverse Burrows-Wheeler transform" << endl;
	auto transformedBack = BWTransform::InverseTransform(transformed);
	for (auto i : transformedBack)
	{
		cout << (char)i;
	}
	cout << endl;

	getchar();

	return 0;
}