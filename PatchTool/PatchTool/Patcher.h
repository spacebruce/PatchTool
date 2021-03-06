#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <direct.h>  
#include <array>
#include "FileHelpers.h"

#include "PatchBase.h"
#include "IPSPatcher.h"
#include "UPSPatcher.h"
#include "BPSPatcher.h"

enum class PatchFormat
{
	Invalid,
	IPS,
	UPS,
	BPS,
};

class Patcher
{
private:
	std::array< std::pair< std::string, PatchFormat>, 3> FormatIdent =
	{
		std::make_pair("PATCH",PatchFormat::IPS),
		std::make_pair("UPS1", PatchFormat::UPS),
		std::make_pair("BPS1", PatchFormat::BPS),
	};

	std::string RomPath, PatchPath, OutPath;

	void SetPaths(const std::string &RomPath, const std::string &PatchPath, const std::string &OutPath);
	std::string FindPath(const std::string & Path);
public:
	Patcher(char * RomPath, char * PatchPath, char * OutPath);
	Patcher(const std::string &RomPath, const std::string &PatchPath, const std::string &OutPath);

	void Run();
};