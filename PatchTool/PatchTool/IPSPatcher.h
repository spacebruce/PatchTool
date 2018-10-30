#pragma once
#include "PatchBase.h"

class IPSPatcher : public PatchBase
{
public:
	IPSPatcher(std::vector<char> *RomFile, std::vector<char> *PatchFile);
	~IPSPatcher();

	void Run();
};

