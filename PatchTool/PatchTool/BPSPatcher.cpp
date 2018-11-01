#include "pch.h"
#include "BPSPatcher.h"


// ?????
template <typename CharT>
static std::uint64_t BPSPatcher::ReadVariableWidthInteger(const std::vector<CharT> &Data, std::size_t &Position)
{
	std::uint64_t result = 0, shift = 0;
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
	std::cout << "BPS mode is untested and will probably produce broken files!" << std::endl;
	std::size_t SourceSize = static_cast<std::size_t>(ReadVariableWidthInteger(PatchFile, Position));
	std::size_t TargetSize = static_cast<std::size_t>(ReadVariableWidthInteger(PatchFile, Position));
	std::size_t MetadataSize = static_cast<std::size_t>(ReadVariableWidthInteger(PatchFile, Position));

	if (MetadataSize > 0)
	{
		std::string MetaData;
		MetaData.reserve(MetadataSize);
		for (size_t i = 0; i < MetadataSize; ++i)
		{
			MetaData[i] = PatchFile[Position];
			++Position;
		}
		std::cout << MetaData << std::endl;
	}

	//Allocate new rom file
	std::vector<char> OutFile;
	OutFile.resize(TargetSize, 0);

	std::size_t OutputPointer = 0;
	std::size_t OutputRelative = 0;
	std::size_t InputRelative = 0;

	const std::size_t EndPosition = PatchFile.size() - 12;
	while (Position < EndPosition)
	{
		std::uint64_t Data = static_cast<std::uint64_t>(ReadVariableWidthInteger<char>(PatchFile, Position));
		std::uint64_t Command = Data & 3;
		std::uint64_t Length = (Data >> 2) + 1;
		//std::cout << "Patch : " << Position << "  Output : " << OutputPointer << "\n";
		//std::cout << Data << " " << " C : " << Command << " L : " << Length << "\n";
		switch (Command)
		{
			case 0:	//SourceRead (Same data in both in and out files)
			{
				while (Length > 0)
				{
					--Length;
					char Byte = RomFile[OutputPointer];
					OutFile[OutputPointer] = Byte;
					++OutputPointer;
				}
			} break;
			case 1:	//PatchRead (Data stored in patch)
			{
				while (Length > 0)
				{
					--Length;
					char Byte = PatchFile[Position];
					++Position;
					OutFile[OutputPointer] = Byte;
					++OutputPointer;
				}
			} break;
			case 2:	//SourceCopy (Data stored *somewhere* in source file)
			{
				std::uint64_t Data = ReadVariableWidthInteger(PatchFile, Position);
				InputRelative += (Data & 1 ? -1 : +1) * (Data >> 1);
				while (Length > 0)
				{
					--Length;
					char Byte = RomFile[InputRelative];
					OutFile[OutputPointer] = Byte;
					++OutputPointer;
					++InputRelative;
				}
			} break;
			case 3: //TargetCopy (Data stored *somewhere* in output file)
			{
				std::uint64_t Data = ReadVariableWidthInteger(PatchFile, Position);
				OutputRelative += (Data & 1 ? -1 : +1) * (Data >> 1);
				while (Length > 0)
				{
					--Length;
					char Byte = OutFile[OutputRelative];
					OutFile[OutputPointer] = Byte;
					++OutputPointer;
					++OutputRelative;
				}
			} break;
			default:
			{
				std::cout << "!PANIC!" << std::endl;
			}; break;
		}
	}

	//Skip checksums for now

	return OutFile;
}