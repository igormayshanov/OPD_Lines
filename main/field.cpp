#include <iostream>
#include <vector>
#include "field.h"
#include "gameconst.h"

using namespace sf;
using namespace std;
using namespace game;

void drawFields(sf::RenderWindow &window, sf::Sprite &sprite)
{
    // Отрисовка спрайта ячейки
    for (int i = MIN_FIELD_SIZE; i < MAX_FIELD_SIZE; i++)
        for (int j = MIN_FIELD_SIZE; j < MAX_FIELD_SIZE; j++)
        {
            sprite.setTextureRect(IntRect(CELL_SPRITE_OFFSET * CELL_WIDTH, 0, CELL_WIDTH, CELL_WIDTH));
            sprite.setPosition(OFFSET_FIELD.x + i * CELL_WIDTH, OFFSET_FIELD.y + j * CELL_WIDTH);
            window.draw(sprite);
        }
}

void initEmptyCell(Cell &cell)
{
    cell.empty = true;
    cell.selected = false;
    cell.color = CELL_SPRITE_OFFSET;
}

void initGameGrid(std::vector<std::vector<Cell>> &gameGrid)
{
    for (int i = 0; i < gameGrid.size(); i++)
    {
        for (int j = 0; j < gameGrid.size(); j++)
        {
            gameGrid[i][j].x = j;
            gameGrid[i][j].y = i;
            initEmptyCell(gameGrid[i][j]);
        }
    }
}

bool isGameGridFull(const std::vector<std::vector<Cell>> &gameGrid)
{
    for (int i = 0; i < MAX_FIELD_SIZE; i++)
    {
        for (int j = 0; j < MAX_FIELD_SIZE; j++)
        {
            if (gameGrid[i][j].empty == true)
                return false;
        }
    }
    return true;
}

bool checkOutOfBorder(int x, int y, int fieldSize)
{
    if (x >= 0 && x < fieldSize && y >= 0 && y < fieldSize)
        return true;
    else
        return false;
}
