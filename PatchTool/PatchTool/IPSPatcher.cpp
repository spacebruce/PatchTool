#include "pch.h"
#include "IPSPatcher.h"

IPSPatcher::IPSPatcher(std::vector<char> &RomFile, const std::vector<char>& PatchFile, std::size_t Position)
{
	this->RomFile = RomFile;
	this->PatchFile = PatchFile;
	this->Position = Position;
}

IPSPatcher::~IPSPatcher()
{
}

std::vector<char> IPSPatcher::Run()
{
	std::cout << "Patching in IPS mode\n";

	const std::string EOFString = std::string("EOF");

	//Start reading...
	while (File::Chunk(PatchFile, Position, EOFString.length()) != EOFString)
	{
		// Chunk header
		uint32_t Offset = 0;
		uint32_t Size = 0;
		{
			uint8_t Byte1, Byte2, Byte3;
			Byte1 = PatchFile[Position + 0];
			Byte2 = PatchFile[Position + 1];
			Byte3 = PatchFile[Position + 2];
			Offset = (Byte1 << 16) + (Byte2 << 8) + (Byte3);

			Byte1 = PatchFile[Position + 3];
			Byte2 = PatchFile[Position + 4];
			Size = (Byte1 << 8) + (Byte2);
			//std::cout << "Chunk o:" << Offset << " s:" << Size << "\n";
		}
		Position += 5;

		//If RLE'd chunk
		if (Size == 0)
		{
			uint32_t RLELength = 0;
			{
				uint8_t Byte1 = PatchFile[Position + 0];
				uint8_t Byte2 = PatchFile[Position + 1];
				RLELength = (Byte1 << 8) + (Byte2);
			}
			uint8_t Byte = PatchFile[Position + 2];
			//std::cout << "RLE " << Byte << " x" << RLESize << "\n";

			if (Offset + RLELength > RomFile.size())
			{
				RomFile.resize(Offset + RLELength, 0);
			}

			for (uint32_t i = 0; i < RLELength; ++i)
			{
				RomFile[Offset + i] = Byte;
			}
			Position += 3;
		}
		else
		{
			if (Offset + Size > RomFile.size())
			{
				RomFile.resize(Offset + Size, 0);
			}

			for (std::size_t i = 0; i < Size; ++i)
			{
				RomFile[Offset + i] = PatchFile[Position + i];
			}
			Position += Size;
		}
	}

	return RomFile;
}