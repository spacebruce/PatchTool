#pragma once
#include <vector>
#include <string>
#include <iostream>
#include "FileHelpers.h"

class PatchBase
{
protected:	//oh so THAT's what protected does. Today I learned.
	std::string* RomPath, PatchPath, OutPath;
	std::vector<char> *RomFile;
	std::vector<char> *PatchFile;
public:
	virtual ~PatchBase() = 0;
	virtual void Run() = 0;
};