#pragma once
#include "PatchBase.h"

class UPSPatcher : public PatchBase
{
private:
	// ?????
	template <typename CharT>
	static uintmax_t ReadVariableWidthInteger(const std::vector<CharT> &Data, std::size_t &Position);
public:
	UPSPatcher(std::vector<char>& RomFile, const std::vector<char>& PatchFile, std::size_t Position);
	std::vector<char> Run();
};

