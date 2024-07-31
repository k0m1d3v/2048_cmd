#include <iostream>
#include <iomanip>
#include <conio.h>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <iostream>

using namespace std;

const int GRID_SIZE = 4;

string IntToString(int num)
{
    if (num == 0) return " ";
    stringstream ss;
    ss << num;
    return ss.str();
}

void ClearConsole()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hConsole == INVALID_HANDLE_VALUE)
    {
        return;
    }

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
    {
        return;
    }

    COORD topLeft = { 0, 0 };
    DWORD consoleSize = csbi.dwSize.X * csbi.dwSize.Y;
    DWORD charsWritten;

    FillConsoleOutputCharacter(hConsole, ' ', consoleSize, topLeft, &charsWritten);
    FillConsoleOutputAttribute(hConsole, csbi.wAttributes, consoleSize, topLeft, &charsWritten);
    SetConsoleCursorPosition(hConsole, topLeft);
}

void PrintGrid(int grid[GRID_SIZE][GRID_SIZE])
{
    ClearConsole();
    cout << "+------+------+------+------+" << endl;
    for (int i = 0; i < GRID_SIZE; ++i)
    {
        for (int j = 0; j < GRID_SIZE; ++j)
        {
            cout << "| " << setw(4) << IntToString(grid[i][j]) << " ";
        }
        cout << "|" << endl;
        cout << "+------+------+------+------+" << endl;
    }
}

enum Direction { UP, DOWN, LEFT, RIGHT, NONE };

Direction GetArrowKeyInput()
{
    int ch = _getch();
    if (ch == 224)
    {
        switch (_getch())
        {
        case 72:
            return UP;
        case 80:
            return DOWN;
        case 75:
            return LEFT;
        case 77:
            return RIGHT;
        default:
            return NONE;
        }
    }
    return NONE;
}

void AddRandomTile(int grid[GRID_SIZE][GRID_SIZE])
{
    int emptyCells[GRID_SIZE * GRID_SIZE][2];
    int emptyCount = 0;

    for (int i = 0; i < GRID_SIZE; ++i)
    {
        for (int j = 0; j < GRID_SIZE; ++j)
        {
            if (grid[i][j] == 0)
            {
                emptyCells[emptyCount][0] = i;
                emptyCells[emptyCount][1] = j;
                emptyCount++;
            }
        }
    }

    if (emptyCount > 0)
    {
        int randomIndex = rand() % emptyCount;
        int row = emptyCells[randomIndex][0];
        int col = emptyCells[randomIndex][1];
        grid[row][col] = (rand() % 2 + 1) * 2; // Add 2 or 4
    }
}

void InitializeGrid(int grid[GRID_SIZE][GRID_SIZE])
{
    for (int i = 0; i < GRID_SIZE; ++i)
    {
        for (int j = 0; j < GRID_SIZE; ++j)
        {
            grid[i][j] = 0;
        }
    }
    AddRandomTile(grid);
    AddRandomTile(grid);
}

bool MoveTiles(int grid[GRID_SIZE][GRID_SIZE], Direction dir)
{
    bool moved = false;
    switch (dir)
    {
    case UP:
        for (int col = 0; col < GRID_SIZE; ++col)
        {
            for (int row = 1; row < GRID_SIZE; ++row)
            {
                if (grid[row][col] != 0)
                {
                    int targetRow = row;
                    while (targetRow > 0 && grid[targetRow - 1][col] == 0)
                    {
                        grid[targetRow - 1][col] = grid[targetRow][col];
                        grid[targetRow][col] = 0;
                        targetRow--;
                        moved = true;
                    }
                }
            }
        }
        break;
    case DOWN:
        for (int col = 0; col < GRID_SIZE; ++col)
        {
            for (int row = GRID_SIZE - 2; row >= 0; --row)
            {
                if (grid[row][col] != 0)
                {
                    int targetRow = row;
                    while (targetRow < GRID_SIZE - 1 && grid[targetRow + 1][col] == 0)
                    {
                        grid[targetRow + 1][col] = grid[targetRow][col];
                        grid[targetRow][col] = 0;
                        targetRow++;
                        moved = true;
                    }
                }
            }
        }
        break;
    case LEFT:
        for (int row = 0; row < GRID_SIZE; ++row)
        {
            for (int col = 1; col < GRID_SIZE; ++col)
            {
                if (grid[row][col] != 0)
                {
                    int targetCol = col;
                    while (targetCol > 0 && grid[row][targetCol - 1] == 0)
                    {
                        grid[row][targetCol - 1] = grid[row][targetCol];
                        grid[row][targetCol] = 0;
                        targetCol--;
                        moved = true;
                    }
                }
            }
        }
        break;
    case RIGHT:
        for (int row = 0; row < GRID_SIZE; ++row)
        {
            for (int col = GRID_SIZE - 2; col >= 0; --col)
            {
                if (grid[row][col] != 0)
                {
                    int targetCol = col;
                    while (targetCol < GRID_SIZE - 1 && grid[row][targetCol + 1] == 0)
                    {
                        grid[row][targetCol + 1] = grid[row][targetCol];
                        grid[row][targetCol] = 0;
                        targetCol++;
                        moved = true;
                    }
                }
            }
        }
        break;
    default:
        break;
    }
    return moved;
}

bool MergeTiles(int grid[GRID_SIZE][GRID_SIZE], Direction dir)
{
    bool merged = false;
    switch (dir)
    {
    case UP:
        for (int col = 0; col < GRID_SIZE; ++col)
        {
            for (int row = 1; row < GRID_SIZE; ++row)
            {
                if (grid[row][col] != 0 && grid[row][col] == grid[row - 1][col])
                {
                    grid[row - 1][col] *= 2;
                    grid[row][col] = 0;
                    merged = true;
                }
            }
        }
        break;
    case DOWN:
        for (int col = 0; col < GRID_SIZE; ++col)
        {
            for (int row = GRID_SIZE - 2; row >= 0; --row)
            {
                if (grid[row][col] != 0 && grid[row][col] == grid[row + 1][col])
                {
                    grid[row + 1][col] *= 2;
                    grid[row][col] = 0;
                    merged = true;
                }
            }
        }
        break;
    case LEFT:
        for (int row = 0; row < GRID_SIZE; ++row)
        {
            for (int col = 1; col < GRID_SIZE; ++col)
            {
                if (grid[row][col] != 0 && grid[row][col] == grid[row][col - 1])
                {
                    grid[row][col - 1] *= 2;
                    grid[row][col] = 0;
                    merged = true;
                }
            }
        }
        break;
    case RIGHT:
        for (int row = 0; row < GRID_SIZE; ++row)
        {
            for (int col = GRID_SIZE - 2; col >= 0; --col)
            {
                if (grid[row][col] != 0 && grid[row][col] == grid[row][col + 1])
                {
                    grid[row][col + 1] *= 2;
                    grid[row][col] = 0;
                    merged = true;
                }
            }
        }
        break;
    default:
        break;
    }
    return merged;
}

bool CanMove(int grid[GRID_SIZE][GRID_SIZE])
{
    for (int i = 0; i < GRID_SIZE; ++i)
    {
        for (int j = 0; j < GRID_SIZE; ++j)
        {
            if (grid[i][j] == 0)
                return true;
            if (i < GRID_SIZE - 1 && grid[i][j] == grid[i + 1][j])
                return true;
            if (j < GRID_SIZE - 1 && grid[i][j] == grid[i][j + 1])
                return true;
        }
    }
    return false;
}

int main()
{
    srand(static_cast<unsigned int>(time(0)));

    int grid[GRID_SIZE][GRID_SIZE];
    InitializeGrid(grid);

    while (true)
    {
        PrintGrid(grid);

        if (!CanMove(grid))
        {
            cout << "Game Over!" << endl;
            break;
        }

        Direction dir = GetArrowKeyInput();
        bool moved = MoveTiles(grid, dir);
        bool merged = MergeTiles(grid, dir);
        if (moved || merged)
        {
            MoveTiles(grid, dir); // Move again to fill gaps after merging
            AddRandomTile(grid);
        }
    }

    return 0;
}
