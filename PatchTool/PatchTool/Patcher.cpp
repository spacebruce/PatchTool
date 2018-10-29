#include "pch.h"
#include "Patcher.h"

Patcher::Patcher(char * RomPath, char * PatchPath, char * OutPath)
{
	SetPaths(std::string(RomPath), std::string(PatchPath), std::string(OutPath));
}

Patcher::Patcher(const std::string &RomPath, const std::string &PatchPath, const std::string &OutPath)
{
	SetPaths(RomPath, PatchPath, OutPath);
}

void Patcher::SetPaths(const std::string &RomPath, const std::string &PatchPath, const std::string &OutPath)
{	
	this->RomPath = FindPath(RomPath);
	this->PatchPath = FindPath(PatchPath);
	this->OutPath = OutPath;
	//std::cout << RomPath << "\n" << PatchPath << "\n" << OutPath << "\n";
}

std::string Patcher::FindPath(const std::string &Path)
{	
	if (File::Exists(Path))
	{
		std::cout << Path << "\n";
		return Path;
	}

	const std::string BasePath = std::string(_getcwd(NULL, _MAX_PATH)) + "\\";
	if (File::Exists(BasePath + Path))
	{
		std::cout << BasePath + Path << "\n";
		return (BasePath + Path);
	}
}

// ?????
template <typename CharT>
static uintmax_t Patcher::ReadVariableWidthInteger(const std::vector<CharT> &Data, std::size_t &Position)
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

void Patcher::Run()
{
	std::vector<char> RomFile, PatchFile;

	//Load stuff
	try { RomFile = File::Load<char>(RomPath); }
	catch (const std::exception & exception) { std::cout << exception.what(); return; }
	try { PatchFile = File::Load<char>(PatchPath); }
	catch (const std::exception & exception) { std::cout << exception.what(); return; }

	std::size_t RomSize = RomFile.size();
	std::size_t PatchSize = PatchFile.size();

	if (RomSize == 0)
	{
		std::cout << "Empty target file!\n";
		return;
	}

	PatchFormat Format = PatchFormat::Invalid;
	std::size_t Position = 0;

	//Identify patch format
	for (std::size_t i = 0; i < FormatIdent.size(); ++i)
	{
		if (File::Chunk(PatchFile, 0, FormatIdent[i].first.length()) == FormatIdent[i].first)
		{
			Format = FormatIdent[i].second;
			Position += FormatIdent[i].first.length();
			std::cout << FormatIdent[i].first << "\n";
			break;
		}
	}

	if (Format == PatchFormat::Invalid)
	{
		std::cout << "Unrecognised patch format!\n";
		return;
	}

	//Attempt patch
	switch (Format)
	{
		case PatchFormat::IPS:	PatchIPS(RomFile, PatchFile, Position);	break;
		case PatchFormat::UPS:	PatchUPS(RomFile, PatchFile, Position);	break;
	}

	//Report stuff
	if (RomFile.size() != RomSize)
	{
		std::cout << "Rom resized from " << RomSize / 1024 << "KB to " << RomFile.size() / 1024 << "KB\n";
	}

	//Write rom file out
	try {	File::Write<char>(OutPath, RomFile);	}
	catch (const std::exception & exception) { std::cout << exception.what(); return; }
	
	std::cout << "Done!\n";
}

void Patcher::PatchIPS(std::vector<char> &RomFile, const std::vector<char> &PatchFile, size_t Position)
{
	std::cout << "Patching in IPS mode\n";

	const std::string EOFString = std::string("EOF");

	//Start reading...
	while (File::Chunk(PatchFile, Position, EOFString.length()) != EOFString)
	{
		uint32_t Offset = 0;
		uint32_t Size = 0;

		// Chunk header
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
			uint32_t RLESize = 0;
			{
				uint8_t Byte1 = PatchFile[Position + 0];
				uint8_t Byte2 = PatchFile[Position + 1];
				RLESize = (Byte1 << 8) + (Byte2);
			}
			uint8_t Byte = PatchFile[Position + 2];
			//std::cout << "RLE " << Byte << " x" << RLESize << "\n";

			if (Offset + RLESize > RomFile.size())
			{
				RomFile.resize(Offset + RLESize, 0);
			}

			for (uint32_t i = 0; i < RLESize; ++i)
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
}

void Patcher::PatchUPS(std::vector<char> &RomFile, const std::vector<char>& PatchFile, size_t Position)
{
	std::cout << "UPS format isn't fully tested and may produce mangled files!\n";

	std::size_t SourceSize = static_cast<std::size_t>(ReadVariableWidthInteger(PatchFile, Position));
	std::size_t DestinationSize = static_cast<std::size_t>(ReadVariableWidthInteger(PatchFile, Position));

	if (SourceSize != DestinationSize)
	{
		RomFile.resize(DestinationSize);
	}

	size_t RomPointer = 0;

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

void Patcher::PatchBPS(std::vector<char> &RomFile, const std::vector<char>& PatchFile, size_t Position)
{
	std::cout << "Unimplemented!\n";
}