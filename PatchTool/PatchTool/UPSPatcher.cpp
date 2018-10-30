#include "pch.h"
#include "UPSPatcher.h"

// ?????
template <typename CharT>
static uintmax_t UPSPatcher::ReadVariableWidthInteger(const std::vector<CharT> &Data, std::size_t &Position)
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

UPSPatcher::UPSPatcher(std::vector<char> &RomFile, const std::vector<char>& PatchFile, std::size_t Position)
{
	this->RomFile = RomFile;
	this->PatchFile = PatchFile;
	this->Position = Position;
}

void UPSPatcher::Run()
{
	std::cout << "UPS format isn't fully tested and may produce mangled files!\n";

	std::size_t SourceSize = static_cast<std::size_t>(ReadVariableWidthInteger(PatchFile, Position));
	std::size_t DestinationSize = static_cast<std::size_t>(ReadVariableWidthInteger(PatchFile, Position));

	if (SourceSize != DestinationSize)
	{
		RomFile.resize(DestinationSize);
	}

	std::size_t RomPointer = 0;

	while (RomPointer < DestinationSize)
	{
		std::size_t Skip = static_cast<std::size_t>(ReadVariableWidthInteger(PatchFile, Position));
		RomPointer += Skip;

		std::vector<char> Chunk;
		do
		{
			Chunk.emplace_back(PatchFile[Position]);
			++Position;
		} while (PatchFile[Position] != 0);

		for (std::size_t i = 0; i < Chunk.size(); ++i)
		{
			RomFile[RomPointer] = RomFile[RomPointer] ^ Chunk[i];
			++RomPointer;
		}
	}
	//skip checksums for now
}