#pragma once

#include <string>

class BWCompressionClient
{
public:
	static void CompressFile(const std::string& filename, const std::string& destination_filename);
	static void DecompressFile(const std::string& filename, const std::string& destination_filename);
};
