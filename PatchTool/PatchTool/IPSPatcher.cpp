#include "pch.h"
#include "IPSPatcher.h"

IPSPatcher::IPSPatcher(std::vector<char> *RomFile, std::vector<char> *PatchFile)
{
	this->RomFile = RomFile;
	this->PatchFile = PatchFile;
}

IPSPatcher::~IPSPatcher()
{
}

void IPSPatcher::Run()
{
	std::size_t Position = 5;

	std::cout << "Patching in IPS mode\n";

	const std::string EOFString = std::string("EOF");

	//Start reading...
	while (File::Chunk(*PatchFile, Position, EOFString.length()) != EOFString)
	{
		// Chunk header
		uint32_t Offset = 0;
		uint32_t Size = 0;
		{
			uint8_t Byte1, Byte2, Byte3;
			Byte1 = *(&PatchFile->at(Position + 0));
			Byte2 = *(&PatchFile->at(Position + 1));
			Byte3 = *(&PatchFile->at(Position + 2));
			Offset = (Byte1 << 16) + (Byte2 << 8) + (Byte3);

			Byte2 = *(&PatchFile->at(Position + 3));
			Byte3 = *(&PatchFile->at(Position + 4));
			Size = (Byte1 << 8) + (Byte2);
			//std::cout << "Chunk o:" << Offset << " s:" << Size << "\n";
		}
		Position += 5;

		//If RLE'd chunk
		if (Size == 0)
		{
			uint32_t RLELength = 0;
			{
				uint8_t Byte1 = *(&PatchFile->at(Position + 0));
				uint8_t Byte2 = *(&PatchFile->at(Position + 1));
				RLELength = (Byte1 << 8) + (Byte2);
			}
			uint8_t Byte = *(&PatchFile->at(Position + 2));
			//std::cout << "RLE " << Byte << " x" << RLESize << "\n";

			if (Offset + RLELength > RomFile->size())
			{
				RomFile->resize(Offset + RLELength, 0);
			}

			for (uint32_t i = 0; i < RLELength; ++i)
			{
				RomFile->data[Offset + i] = (static_cast<char>(Byte));
			}
			Position += 3;
		}
		else
		{
			if (Offset + Size > RomFile->size())
			{
				RomFile->resize(Offset + Size, 0);
			}

			for (std::size_t i = 0; i < Size; ++i)
			{
				RomFile->data[Offset + i] = PatchFile->at(Position + i);
			}
			Position += Size;
		}

	}
}