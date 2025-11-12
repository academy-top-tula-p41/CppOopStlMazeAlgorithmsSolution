#pragma once
#include <iostream>

#include <array>
#include <vector>
#include <string>
#include <filesystem>
#include <algorithm>
#include <fstream>

#include "Console.h"

struct Cell
{
	int row{};
	int column{};

	friend bool operator==(const Cell& left, const Cell& right)
	{
		return (left.row == right.row && left.column == right.column);
	}

	friend std::ostream& operator<<(std::ostream& out, const Cell& cell)
	{
		out << "(" << cell.row << "," << cell.column << ")";
		return out;
	}
};

using Matrix = std::vector<std::vector<int>>;
using Front = std::vector<Cell>;
using Fronts = std::array<Front, 2>;


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

enum ConsoleType
{
	Space = 32,
	Wall = 178,
	Way = 249
};




class Maze
{
	Matrix maze;
	std::string fileName;
	Cell start;
	Cell finish;

	Front way;

	void FindStart();
	void FindFinish();
public:
	void GetMazeFile();
	void GetMaze();

	void WaveAlgorithm();
	void GreedyAlgorithm();

	void Show();
};

