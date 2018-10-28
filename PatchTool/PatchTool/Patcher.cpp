#include "pch.h"
#include "Patcher.h"

Patcher::Patcher(char * RomPath, char * PatchPath, char * OutPath)
{
	this->RomPath = std::string(RomPath);
	this->PatchPath = std::string(PatchPath);
	this->OutPath = std::string(OutPath);
}

Patcher::Patcher(const std::string &RomPath, const std::string &PatchPath, const std::string &OutPath)
{
	this->RomPath = RomPath;
	this->PatchPath = PatchPath;
	this->OutPath = OutPath;
}

void Patcher::Run()
{
	std::vector<char> RomFile, PatchFile;
	try { RomFile = FileLoad<char>(RomPath); }
	catch (const std::exception & exception) { std::cout << exception.what(); return; }
	try { PatchFile = FileLoad<char>(PatchPath); }
	catch (const std::exception & exception) { std::cout << exception.what(); return; }

	//Check for "PATCH" header
	if (FileChunk(PatchFile, 0, PatchString.length()) != PatchString)
	{
		std::cout << "Bad patch file";
		return;
	}
	std::cout << "Good patch file";
}