#pragma once
#include <Windows.h>
#include <string>

class Console
{
	HANDLE consoleOutput;

public:
	Console();

	void Clear();
	void WritePosition(int row, int column, std::string source);
};

