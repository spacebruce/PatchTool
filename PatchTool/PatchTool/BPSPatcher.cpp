#include "pch.h"
#include "BPSPatcher.h"

BPSPatcher::BPSPatcher(std::vector<char>& RomFile, const std::vector<char>& PatchFile, std::size_t Position)
{
	this->RomFile = RomFile;
	this->PatchFile = PatchFile;
	this->Position = Position;
}

void BPSPatcher::Run()
{
	std::size_t Position = 4;
	std::cout << "BPS is currently unimplemented!\n";
}