#include "pch.h"
#include "BPSPatcher.h"

BPSPatcher::BPSPatcher(std::vector<char>& RomFile, const std::vector<char>& PatchFile, std::size_t Position)
{
	this->RomFile = RomFile;
	this->PatchFile = PatchFile;
	this->Position = Position;
}

std::vector<char> BPSPatcher::Run()
{
	std::cout << "BPS is currently unimplemented!\n";
	return RomFile;
}