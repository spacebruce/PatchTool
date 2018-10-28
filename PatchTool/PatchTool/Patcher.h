#pragma once
#include <string>
#include <iostream>

class Patcher
{
private:
	const std::string PatchString = std::string("PATCH");
	const std::string EOFString = std::string("EOF");
	std::string RomPath, PatchPath, OutPath;
public:
	Patcher(char * RomPath, char * PatchPath, char * OutPath);
	Patcher(const std::string &RomPath, const std::string &PatchPath, const std::string &OutPath);
};