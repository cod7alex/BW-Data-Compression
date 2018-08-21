#include <iostream>
#include <string>
#include "../MoveToFrontEncoding.h"

using namespace std;

int main()
{
	string inputStr = "ABRACADABRA!";
	vector<uint8_t> input (inputStr.begin(), inputStr.end());

	auto encoded = MoveToFrontEncoding::Encode(input);

	cout << "Encoding string 'ABRACADABRA!' using Move-to-Front encoding:" << endl;
	for (auto i : encoded)
	{
		cout << std::hex << (int)i << " ";
	}
	cout << endl;

	auto decoded = MoveToFrontEncoding::Decode(encoded);

	cout << "Decoding numbers back to string:" << endl;
	for (auto i : decoded)
	{
		cout << (char)i;
	}
	cout << endl;

	getchar();

	return 0;
}