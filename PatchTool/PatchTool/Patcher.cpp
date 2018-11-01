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

void Patcher::Run()
{
	std::vector<char> RomFile, PatchFile;

	//Load stuff
	try { RomFile = File::Load<char>(RomPath); }
	catch (const std::exception & exception) { std::cout << exception.what(); return; }
	try { PatchFile = File::Load<char>(PatchPath); }
	catch (const std::exception & exception) { std::cout << exception.what(); return; }

	const std::size_t RomSize = RomFile.size();
	const std::size_t PatchSize = PatchFile.size();

	if (RomSize == 0)
	{
		std::cout << "Empty target file!\n";
		return;
	}

	std::size_t Position = 0;

	//Identify patch format
	PatchFormat Format = PatchFormat::Invalid;
	PatchBase * Patcher;

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

	switch (Format)
	{
		case PatchFormat::IPS:	Patcher = new IPSPatcher(RomFile, PatchFile, Position);	break;
		case PatchFormat::UPS:	Patcher = new UPSPatcher(RomFile, PatchFile, Position);	break;
		case PatchFormat::BPS:	Patcher = new BPSPatcher(RomFile, PatchFile, Position);	break;
		case PatchFormat::Invalid: default:
			std::cout << "Unrecognised patch format!\n";
			return;
		break;
	}

	//Try to patch file
	RomFile = Patcher->Run();

	//Report stuff
	if (RomFile.size() != RomSize)
	{
		std::cout << "Rom resized from " << RomSize / 1024 << "KB to " << RomFile.size() / 1024 << "KB\n";
	}

	//Write rom file out
	try {	File::Write<char>(OutPath, RomFile);	}
	catch (const std::exception & exception) { std::cout << exception.what(); return; }

	delete Patcher;
	
	std::cout << "Done!\n";
}