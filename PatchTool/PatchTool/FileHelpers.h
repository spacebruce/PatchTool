#pragma once
#include <vector>
#include <string>
#include <fstream>

template< typename CharT >
std::vector<CharT> FileLoad(const std::string & path)
{
	using ifstream = std::basic_fstream<CharT>;
	using vector = std::vector<CharT>;

	ifstream file = ifstream(path, std::ios::binary | std::ios::ate);

	if (file.fail())
		throw std::invalid_argument("Error loading file " + path);

	// Maybe not necessary? Maybe just return an empty vector?
	if (file.eof())
		throw std::length_error("Empty File " + path);

	auto length = file.tellg();
	using pos_type = typename std::ifstream::pos_type;
	file.seekg(static_cast<pos_type>(0));

	using size_type = typename vector::size_type;
	vector data = vector(static_cast<size_type>(length));
	file.read(data.data(), length);

	return data;
}

template< typename CharT >
std::string FileChunk(const std::vector<CharT> &Data, const std::size_t &Start, const std::size_t &Length)
{
	if ((Start + Length) > Data.size())
		throw std::length_error("Out of range");

	std::vector<char>::const_iterator first = Data.begin() + Start;
	std::vector<char>::const_iterator last = Data.begin() + Start + Length;
	std::vector<char> SubVector(first, last);

	std::string Chunk = std::string(SubVector.begin(), SubVector.end());

	return Chunk;
}