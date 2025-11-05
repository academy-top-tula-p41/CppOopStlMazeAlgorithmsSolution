#pragma once
#include <iostream>

#include <vector>
#include <string>
#include <filesystem>
#include <algorithm>
#include <fstream>

using Matrix = std::vector<std::vector<int>>;
namespace fs = std::filesystem;

enum class CellType
{
	Space  = 0,
	Wall   = -1,
	Start  = -2,
	Finish = -3
};

enum class SymbolType
{
	Space = ' ',
	Wall = '#',
};


class Maze
{
	Matrix maze;
	std::string fileName;

public:
	void GetMazeFile();
	Matrix GetMaze();
};

