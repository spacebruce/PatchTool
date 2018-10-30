#pragma once
#include "PatchBase.h"

class IPSPatcher : public PatchBase
{
public:
	IPSPatcher(std::vector<char> &RomFile, const std::vector<char>& PatchFile, std::size_t Position);
	~IPSPatcher();

	void Run();
};

