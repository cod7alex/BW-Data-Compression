#include "BurrowsWheelerCompression.h"
#include "BWTransform.h"
#include "MoveToFrontEncoding.h"
#include "HuffmanCompression.h"

std::vector<bool> BurrowsWheelerCompression::Compress(const std::vector<uint8_t> &data)
{
	const auto transformed = BWTransform::Transform(data);
	const auto encoded = MoveToFrontEncoding::Encode(transformed);

	return HuffmanCompression::Compress(encoded);
}
std::vector<uint8_t> BurrowsWheelerCompression::Decompress(const std::vector<bool> &compressed)
{
	const auto decompressed = HuffmanCompression::Decompress(compressed);
	const auto decoded = MoveToFrontEncoding::Decode(decompressed);

	return BWTransform::InverseTransform(decoded);
}