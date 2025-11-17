#include "Maze.h"
#include <iomanip>

void Maze::FindStart()
{
    // top wall
    int column{};
    for (auto cell : maze[0])
    {
        if (cell == (int)CellType::Space)
        {
            this->start.row = 0;
            this->start.column = column;
            this->maze[0][column] = (int)CellType::Start;
            return;
        }
        column++;
    }

    // left wall
    for (int row{ 1 }; row < maze.size(); row++)
    {
        if (maze[row][0] == (int)CellType::Space)
        {
            this->start.row = row;
            this->start.column = 0;
            this->maze[row][0] = (int)CellType::Start;
            return;
        }
    }
}

void Maze::FindFinish()
{
    // bottom wall
    int bottom{ (int)maze.size() - 1 };

    int column{};
    for (auto cell : maze[bottom])
    {
        if (cell == (int)CellType::Space)
        {
            this->finish.row = bottom;
            this->finish.column = column;
            this->maze[bottom][column] = (int)CellType::Finish;
            return;
        }
        column++;
    }

    // right wall
    int right{ (int)maze[0].size() - 1 };

    for (int row{}; row < bottom; row++)
    {
        if (maze[row][right] == (int)CellType::Space)
        {
            this->finish.row = row;
            this->finish.column = right;
            this->maze[row][right] = (int)CellType::Finish;
            return;
        }
    }

}

bool Maze::IsValidWay(Cell cell)
{
    size_t height{ maze.size() };
    size_t width{ maze[0].size() };

    if (cell.row < 0 || cell.row >= height
            || cell.column < 0 || cell.column >= width)
        return false;

    if (maze[cell.row][cell.column] == (int)CellType::Wall)
        return false;

    if (maze[cell.row][cell.column] == (int)CellType::Way)
        return false;

    return true;
}

bool Maze::NextWay(Cell cell)
{
    if (cell == finish)
    {
        maze[cell.row][cell.column] = (int)CellType::Way;
        return true;
    }

    for (auto offset : offsets)
    {
        int orow = cell.row + offset.row;
        int ocolumn = cell.column + offset.column;

        if (IsValidWay(Cell{ orow, ocolumn }))
        {
            maze[cell.row][cell.column] = (int)CellType::Way;
            Show();

            if (NextWay(Cell{ orow, ocolumn }))
                return true;

            maze[cell.row][cell.column] = (int)CellType::Space;
            Show();
        }
    }

    return false;
}

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

void Maze::GetMaze()
{
    std::ifstream fileMaze(this->fileName, std::ios::in);
    std::string lineMaze;

    while (std::getline(fileMaze, lineMaze))
    {
        // std::cout << lineMaze << "\n";
        std::vector<int> lineMatrixMaze;
        for (char symbol : lineMaze)
        {
            SymbolType st = (SymbolType)symbol;
            switch (st)
            {
            case SymbolType::Space:
                lineMatrixMaze.push_back((int)CellType::Space);
                break;
            case SymbolType::Wall:
                lineMatrixMaze.push_back((int)CellType::Wall);
                break;
            default:
                break;
            }
        }
        this->maze.push_back(lineMatrixMaze);
    }
        
    fileMaze.close();

    FindStart();
    FindFinish();
}

void Maze::Show()
{
    int row{};
    for (auto line : maze)
    {
        int column{};
        for (auto cell : line)
        {
            CellType cellType{ (CellType)cell };
            switch (cellType)
            {
            case CellType::Space:
                std::cout << (char)ConsoleType::Space;
                std::cout << (char)ConsoleType::Space;
                break;
            case CellType::Wall:
                std::cout << (char)ConsoleType::Wall;
                std::cout << (char)ConsoleType::Wall;
                break;
            case CellType::Start:
                std::cout << " S";
                break;
            case CellType::Finish:
                std::cout << " F";
                break;
            case CellType::Way:
                std::cout << std::setw(2) << (char)ConsoleType::Way;
                break;
            default:
                if (std::ranges::find(this->way, Cell{row, column}) != way.end())
                    std::cout << std::setw(2) << (char)ConsoleType::Way;
                else
                    std::cout << "  ";
                break;
            }
            column++;
        }
        row++;
        std::cout << "\n";
    }
    std::cout << "\n";

    if (this->way.size() > 0)
    {
        std::cout << "Way: ";
        for (auto wayCell : this->way)
            std::cout << wayCell << " ";
        std::cout << "\n";
    }
    
}


void Maze::WaveAlgorithm()
{
    //const std::vector<Cell> offsets{ { -1, 0 }, { 0, 1 }, { 1, 0 }, { 0, -1 } };
    Fronts fronts;

    bool isFinish{ true };
    bool isBreak{ false };
    bool frontCurrent{ false };
    int frontNumber{ 1 };

    fronts[frontCurrent].push_back(this->start);

    while (true)
    {
        fronts[!frontCurrent].clear();

        for (auto frontCell : fronts[frontCurrent])
        {
            int crow{ frontCell.row };
            int ccolumn{ frontCell.column };

            for (auto offset : offsets)
            {
                int orow{ crow + offset.row };
                int ocolumn{ ccolumn + offset.column };
                if (orow < 0 || orow >= maze.size() ||
                        ocolumn < 0 || ocolumn >= maze[0].size())
                    continue;
                if (maze[orow][ocolumn] == (int)CellType::Space ||
                    maze[orow][ocolumn] == (int)CellType::Finish)
                {
                    fronts[!frontCurrent].push_back(Cell{ orow, ocolumn });
                    if (maze[orow][ocolumn] == (int)CellType::Finish)
                    {
                        maze[orow][ocolumn] = frontNumber;
                        isBreak = true;
                        break;
                    }
                    maze[orow][ocolumn] = frontNumber;
                }
            }
            if (isBreak) break;
        }
        if (isBreak) break;
        if (fronts[!frontCurrent].size() == 0)
        {
            isFinish = false;
            break;
        }
        frontCurrent = !frontCurrent;
        frontNumber++;
    }
    

    if (!isFinish) return;

    this->way.push_back(finish);

    while (true)
    {
        int wrow = this->way.back().row;
        int wcolumn = this->way.back().column;

        for (auto offset : offsets)
        {
            int orow = wrow + offset.row;
            int ocolumn = wcolumn + offset.column;
            if (orow < 0 || orow >= maze.size() || ocolumn < 0 || ocolumn >= maze[0].size())
                continue;
            if (maze[orow][ocolumn] == frontNumber - 1)
            {
                this->way.push_back(Cell{ orow, ocolumn });
                break;
            }
        }

        if (frontNumber - 1 == 0)
            break;
        frontNumber--;
    }
    this->way.push_back(this->start);

    /*std::reverse(way.begin(), way.end());
    std::reverse(std::begin(way), std::end(way));

    std::ranges::reverse(way.begin(), way.end());
    std::ranges::reverse(std::begin(way), std::end(way));
    std::ranges::reverse(std::ranges::begin(way),
                         std::ranges::end(way));*/
    
    std::ranges::reverse(way);
}

void Maze::GreedyAlgorithm()
{
    NextWay(start);
}
