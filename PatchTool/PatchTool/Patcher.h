#pragma once

#include <string>
class Patcher
{
	std::string RomPath, PatchPath, OutPath;
public:
	Patcher(char * RomPath, char * PatchPath, char * OutPath);
	Patcher(const std::string &RomPath, const std::string &PatchPath, const std::string &OutPath);
