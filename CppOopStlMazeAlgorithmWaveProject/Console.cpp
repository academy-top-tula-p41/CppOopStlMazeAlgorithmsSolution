#include "Console.h"

Console::Console() 
	: consoleOutput{ GetStdHandle(STD_OUTPUT_HANDLE) }{}

void Console::Clear()
{
	system("cls");
}

void Console::WritePosition(int row, int column, std::string source)
{
	COORD position;
	position.X = column;
	position.Y = row;
	SetConsoleCursorPosition(consoleOutput, position);

	unsigned long count;
	WriteConsoleA(consoleOutput,
					source.c_str(),
					source.size(),
					&count,
					nullptr);
}
