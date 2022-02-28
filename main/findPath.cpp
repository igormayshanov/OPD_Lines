#include <iostream>
#include <vector>
#include <queue>
#include "field.h"
#include "findPath.h"
#include "gameconst.h"

void printMatrix(int **matrix, int fieldSize)
{
    for (int i = 0; i < fieldSize; i++)
    {
        for (int j = 0; j < fieldSize; j++)
        {
            printf("%3d", matrix[i][j]);
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void findNextCellInPath(int x, int y, std::vector<std::vector<Cell>> &gameGrid, int **visited, int **path, std::queue<int> &plan, const Cell &endCell)
{
    int offset = 0;
    int ix, iy;
    if (!visited[y][x])
    {
        for (int i = 0; i < game::NUMBER_OF_DIRECTIONS; i++)
        {
            ix = x + game::DIRECTIONS_X[i];
            iy = y + game::DIRECTIONS_Y[i];
            if (checkOutOfBorder(ix, iy) && !visited[iy][ix] &&
                (gameGrid[iy][ix].empty || (gameGrid[iy][ix].x == endCell.x && gameGrid[iy][ix].y == endCell.y)))
            {
                path[iy][ix] = path[y][x] + 1;
                plan.push(ix);
                plan.push(iy);
            }
        }
        visited[y][x] = 1;
    }
}

void restorePath(Cell &startBall, Cell &endCell, std::vector<std::vector<Cell>> &gameGrid, int **path) //можно путь закинуть в стек
{
    int x = endCell.x;
    int y = endCell.y;
    int x_start = startBall.x;
    int y_start = startBall.y;
    int ix = 0;
    int iy = 0;
    while (path[y][x] != path[y_start][x_start] + 1)
    {
        for (int i = 0; i < game::NUMBER_OF_DIRECTIONS; i++)
        {
            gameGrid[y][x] = startBall;
            ix = x + game::DIRECTIONS_X[i];
            iy = y + game::DIRECTIONS_Y[i];
            if (checkOutOfBorder(ix, iy) && (path[y][x] != path[y_start][x_start] + 1) && (path[iy][ix] == path[y][x] - 1))
            {
                x = ix;
                y = iy;
                gameGrid[y][x] = startBall;
            }
        }
    }
}

bool findPathInGrid(std::vector<std::vector<Cell>> &gameGrid, Cell &startBall, Cell &endCell)
{
    int x_start, y_start, x_end, y_end, x, y;
    std::queue<int> cellVisitQueue;
    cellVisitQueue.push(startBall.x);
    cellVisitQueue.push(startBall.y);
    int **visited = new int *[game::MAX_FIELD_SIZE];
    int **path = new int *[game::MAX_FIELD_SIZE];
    for (int y = 0; y < game::MAX_FIELD_SIZE; y++)
    {
        visited[y] = new int[game::MAX_FIELD_SIZE]; /* массив для хранения информации о посещении клеток*/
        path[y] = new int[game::MAX_FIELD_SIZE];    /* массив для хранения найденных путей */
        for (int x = 0; x < game::MAX_FIELD_SIZE; x++)
        {
            visited[y][x] = 0;
            path[y][x] = -1;
        }
    }
    path[startBall.y][startBall.x] = 1;
    while (!cellVisitQueue.empty())
    {
        x = cellVisitQueue.front();
        cellVisitQueue.pop();
        y = cellVisitQueue.front();
        cellVisitQueue.pop();
        findNextCellInPath(x, y, gameGrid, visited, path, cellVisitQueue, endCell);
    }
    if (visited[endCell.y][endCell.x])
    {
        //std::cout << "There is a path" << std::endl;
        gameGrid[endCell.y][endCell.x].color = startBall.color;
        gameGrid[endCell.y][endCell.x].empty = false;
        gameGrid[endCell.y][endCell.x].selected = false;
        initEmptyCell(gameGrid[startBall.y][startBall.x]);
        return true;
    }
    else
    {
        return false;
    }
}
