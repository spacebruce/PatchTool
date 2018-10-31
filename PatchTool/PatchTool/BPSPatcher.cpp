#include "pch.h"
#include "BPSPatcher.h"


// ?????
template <typename CharT>
static uintmax_t BPSPatcher::ReadVariableWidthInteger(const std::vector<CharT> &Data, std::size_t &Position)
{
	uintmax_t result = 0, shift = 0;
	for (;;) {
		uint8_t octet = Data[Position];
		++Position;
		if (octet & 0x80) {
			result += (octet & 0x7f) << shift;
			break;
		}
		result += (octet | 0x80) << shift;
		shift += 7;
	}

	return result;
}

BPSPatcher::BPSPatcher(std::vector<char>& RomFile, const std::vector<char>& PatchFile, std::size_t Position)
{
	this->RomFile = RomFile;
	this->PatchFile = PatchFile;
	this->Position = Position;
}

std::vector<char> BPSPatcher::Run()
{
	std::size_t SourceSize, TargetSize, MetadataSize;
	SourceSize = ReadVariableWidthInteger<char>(PatchFile, Position);
	TargetSize = ReadVariableWidthInteger<char>(PatchFile, Position);
	MetadataSize = ReadVariableWidthInteger<char>(PatchFile, Position);

	std::string MetaData;
	MetaData.reserve(MetadataSize);
	for (size_t i = 0; i < MetadataSize; ++i)
	{
		MetaData[i] = PatchFile[Position];
		++Position;
	}
	std::cout << MetaData << std::endl;

	//Allocate new rom file
	std::vector<char> OutFile = std::vector<char>(0, TargetSize);	



	return OutFile;
}