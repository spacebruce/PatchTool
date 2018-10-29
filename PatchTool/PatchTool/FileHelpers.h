#pragma once
#include <vector>
#include <string>
#include <fstream>

namespace File
{
	/*
	bool Exists(const std::string & Path);

	template <typename CharT>
	std::vector<CharT> Load(const std::string & Path);

	template <typename CharT>
	void Write(const std::string & Path, const std::vector<CharT> Data);

	template< typename CharT >
	std::string Chunk(const std::vector<CharT> &Data, const std::size_t &Start, const std::size_t &Length);	
	*/
	bool Exists(const std::string & Path);

	template <typename CharT>
	std::vector<CharT> Load(const std::string & Path)
	{
		std::ifstream file(Path, std::ios::binary | std::ios::ate);

		if (file.fail())
			throw std::invalid_argument("Error loading file " + Path);

		auto length = file.tellg();
		using pos_type = typename std::ifstream::pos_type;
		file.seekg(static_cast<pos_type>(0));

		using size_type = typename std::vector<CharT>::size_type;
		std::vector<CharT> data = std::vector<CharT>(static_cast<size_type>(length));
		for (size_type i = 0; i < data.size() && !file.eof(); ++i)
			data[i] = static_cast<CharT>(file.get());

		file.close();
		return data;
	}

	template <typename CharT>
	void Write(const std::string & Path, const std::vector<CharT> Data)
	{
		std::ofstream file(Path, std::ios::out | std::ios::binary);
		if (file.fail())
			throw std::invalid_argument("Error opening file for writing " + Path);

		for (std::size_t i = 0; i < Data.size(); ++i)
		{
			file << Data[i];
		}
		file.close();
	}

	template< typename CharT >
	std::string Chunk(const std::vector<CharT> &Data, const std::size_t &Start, const std::size_t &Length)
	{
		if ((Start + Length) > Data.size())
			throw std::length_error("Out of range");

		std::vector<char>::const_iterator first = Data.begin() + Start;
		std::vector<char>::const_iterator last = Data.begin() + Start + Length;
		std::vector<char> SubVector(first, last);

		std::string Chunk = std::string(SubVector.begin(), SubVector.end());

		return Chunk;
	}
}