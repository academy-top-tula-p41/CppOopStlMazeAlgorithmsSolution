#include <iostream>

#include "Maze.h"


int main()
{
    Maze maze;
    maze.GetMazeFile();
    maze.GetMaze();
    maze.Show();
    //maze.WaveAlgorithm();
    maze.GreedyAlgorithm();
    maze.Show();
}
