#pragma once
#include "pch.h"
#include "FileHelpers.h"

bool File::Exists(const std::string &Path)
{
	std::ifstream file = std::ifstream(Path);
	return !(file.fail());
}