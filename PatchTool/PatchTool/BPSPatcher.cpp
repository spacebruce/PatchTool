#include "pch.h"
#include "BPSPatcher.h"

BPSPatcher::BPSPatcher(std::vector<char> *RomFile, std::vector<char> *PatchFile)
{
	this->RomFile = RomFile;
	this->PatchFile = PatchFile;
}

void BPSPatcher::Run()
{
	std::size_t Position = 4;
	std::cout << "BPS is currently unimplemented!\n";
}