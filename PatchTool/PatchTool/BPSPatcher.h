#pragma once
#include "PatchBase.h"

class BPSPatcher : public PatchBase
{
public:
	BPSPatcher(std::vector<char> *RomFile, std::vector<char> *PatchFile);
	void Run();
};