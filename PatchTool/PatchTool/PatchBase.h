#pragma once
#include <vector>
#include <string>
#include <iostream>
#include "FileHelpers.h"

class PatchBase
{
protected:	//oh so THAT's what protected does. Today I learned.
	std::string RomPath, PatchPath, OutPath;
	std::vector<char> RomFile, PatchFile;
	std::size_t Position = 0;
public:
	virtual ~PatchBase(void) = default;
	virtual std::vector<char> Run() = 0;
};