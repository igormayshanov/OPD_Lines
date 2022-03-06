#include <iostream>
#include <vector>
#include <sstream>
#include "field.h"
#include "gameconst.h"

using namespace sf;
using namespace std;
using namespace game;

bool operator==(const Cell &cell1, const Cell &cell2)
{
    return cell1.x == cell2.x && cell1.y == cell2.y && cell1.color == cell2.color && cell1.empty == cell2.empty;
}


void drawFields(sf::RenderWindow &window, sf::Sprite &sprite)
{
    // Отрисовка спрайта ячейки
    for (int i = game::MIN_FIELD_SIZE; i < game::MAX_FIELD_SIZE; i++)
        for (int j = game::MIN_FIELD_SIZE; j < game::MAX_FIELD_SIZE; j++)
        {
            sprite.setTextureRect(IntRect(game::CELL_SPRITE_OFFSET * game::CELL_WIDTH, 0, game::CELL_WIDTH, game::CELL_WIDTH));
            sprite.setPosition(game::OFFSET_FIELD.x + i * game::CELL_WIDTH, game::OFFSET_FIELD.y + j * game::CELL_WIDTH);
            window.draw(sprite);
        }
}
void drawAppearListField(sf::RenderWindow &window, sf::Sprite &sprite)
{
    // Отрисовка спрайта ячейки для шаров, которые появятся на следующем ходе
    for (int i = game::MIN_FIELD_SIZE; i < game::APPEAR_BALLS; i++)
    {
        sprite.setTextureRect(IntRect(game::CELL_SPRITE_OFFSET * game::CELL_WIDTH, 0, game::CELL_WIDTH, game::CELL_WIDTH));
        sprite.setPosition(game::OFFSET_APPEAR_LIST.x + i * game::CELL_WIDTH, game::OFFSET_APPEAR_LIST.y);
        window.draw(sprite);
    }
}

void initEmptyCell(Cell &cell)
{
    cell.empty = true;
    cell.selected = false;
    cell.color = game::CELL_SPRITE_OFFSET;
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
    for (int i = 0; i < game::MAX_FIELD_SIZE; i++)
    {
        for (int j = 0; j < game::MAX_FIELD_SIZE; j++)
        {
            if (gameGrid[i][j].empty == true)
                return false;
        }
    }
    return true;
}

bool checkOutOfBorder(int x, int y)
{
    if (x >= 0 && x < game::MAX_FIELD_SIZE && y >= 0 && y < game::MAX_FIELD_SIZE)
        return true;
    else
        return false;
}
void drawScore(sf::RenderWindow &window, int score, sf::Text &gameScoreText)
{
    std::ostringstream gameScoreString;
    gameScoreString << score;
    gameScoreText.setString(gameScoreString.str());
    window.draw(gameScoreText);
}
