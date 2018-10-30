#pragma once
#include "PatchBase.h"

class BPSPatcher : public PatchBase
{
public:
	BPSPatcher(std::vector<char> &RomFile, const std::vector<char>& PatchFile, std::size_t Position);
	std::vector<char> Run();
};