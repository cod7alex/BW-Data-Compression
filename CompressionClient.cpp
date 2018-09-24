#include "CompressionClient.h"
#include "BurrowsWheelerCompression.h"
#include "utils/Binary.h"

#include <fstream>
#include <cstdint>
#include <string>
#include <stdexcept>

using std::uint8_t;

void BWCompressionClient::CompressFile(const std::string& filename, const std::string& destination_filename)
{
	std::ifstream input_file(filename);

	std::vector<uint8_t> data;

	if (!input_file.is_open())
	{
		throw std::runtime_error("Invalid input filename");
	}

	std::copy(std::istreambuf_iterator<char>(input_file), std::istreambuf_iterator<char>(), std::back_inserter(data));
	input_file.close();

	const auto compressed = BurrowsWheelerCompression::Compress(data);

	std::ofstream output_file(destination_filename, std::ios_base::binary);

	binary::WriteToStream(compressed, output_file);

	output_file.close();
}

void BWCompressionClient::DecompressFile(const std::string& filename, const std::string& destination_filename)
{
	std::ifstream input_file(filename, std::ios_base::binary);

	std::vector<bool> data;

	if (!input_file.is_open())
	{
		throw std::runtime_error("Invalid input filename");
	}

	const auto binary_data = binary::ReadFromStream(input_file);

	const auto decompressed = BurrowsWheelerCompression::Decompress(binary_data);

	std::ofstream output_file(destination_filename);

	std::copy(decompressed.begin(), decompressed.end(), std::ostreambuf_iterator<char>(output_file));

	output_file.close();
}
