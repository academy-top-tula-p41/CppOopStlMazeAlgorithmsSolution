#include "Maze.h"

void Maze::GetMazeFile()
{
    auto currentPath = fs::current_path();

    auto dirIterator = fs::directory_iterator(currentPath);

    /*for (auto entry : dirIterator)
        std::cout << "\t" << entry.path().filename().string() << "\n";*/

    /*std::for_each(
        fs::begin(dirIterator),
        fs::end(dirIterator),
        [](auto entry) 
        {
            std::cout << "\t" << entry.path().filename().string() << "\n";
        });*/

    int position{};

    std::ranges::for_each(
        dirIterator,
        [&position](auto entry)
        {
            if(!entry.is_directory() 
                && entry.path().extension() == ".maze")
                std::cout << ++position << ": " 
                          << entry.path().filename().string() 
                          << "\n";
        });

    std::cout << "Input number of file: ";
    std::cin >> position;

    std::string fileName;

    dirIterator = fs::directory_iterator(currentPath);

    std::ranges::for_each(
        dirIterator,
        [&fileName, &position](auto entry) 
        {
            if(entry.path().extension() == ".maze" && !(--position))
                fileName = entry.path().filename().string();
        });
    
    this->fileName = fileName;
}

Matrix Maze::GetMaze()
{
    std::ifstream fileMaze(this->fileName, std::ios::in);
    std::string lineMaze;

    while (std::getline(fileMaze, lineMaze))
        std::cout << lineMaze << "\n";

    return Matrix();
}

