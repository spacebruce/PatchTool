// PatchTool.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "pch.h"
#include <iostream>

#include "Patcher.h"

int main(int argCount, char * args[])
{
    std::cout << "Patch tool!\n"; 
	if (argCount != 4)
	{
		std::cout << "Patcher requires a Base file, a Patch file and an output!";
		return 0;
	}
	else
	{
		Patcher patch = Patcher(args[1], args[2], args[3]);
	}
	return 0;
}