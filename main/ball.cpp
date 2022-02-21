#include <iostream>
#include <list>
#include <vector>
#include <cstdlib> // для функции rand()
#include <set>
#include <random>
#include <ctime>
#include "ball.h"
#include "field.h"
#include "gameconst.h"
#include "utils.h"
using namespace sf;
using namespace std;
using namespace game;

void generateAppearList(std::list<Cell> &appearList)
{
    Cell appBall;
    appearList.clear();
    std::set<size_t> usedColors;
    std::set<size_t> usedX;
    std::set<size_t> usedY;
    PRNG generator;
    initGenerator(generator);
    for (int i = 0; i < game::APPEAR_BALLS; i++)
    {
        size_t colorIndex = 0;
        size_t x = 0;
        size_t y = 0;
        while (true)
        {
            colorIndex = getRandomIndex(generator, game::END_BALL_SPRITE_OFFSET);
            x = getRandomIndex(generator, game::MAX_FIELD_SIZE - 1);
            y = getRandomIndex(generator, game::MAX_FIELD_SIZE - 1);

            // Проверяем, что индекс ранее не встречался
            if (usedColors.find(colorIndex) == usedColors.end() && usedX.find(x) == usedX.end() && usedY.find(y) == usedY.end())
            {
                // Если не встречался, добавляем в множество и выходим из цикла: уникальный индекс найден
                usedColors.insert(colorIndex);
                usedX.insert(x);
                usedY.insert(y);
                break;
            }
        }
        std::cout << x << " , " << y << " color= " << colorIndex << std::endl;
        appBall.color = colorIndex;
        appBall.x = x;
        appBall.y = y;
        appBall.empty = false;
        appBall.selected = false;
        appearList.push_back(appBall);
    }
}

void addBallInGrid(const Cell &ball, std::vector<std::vector<Cell>> &gameGrid)
{
    Cell insertableBall = ball;
    for (int i = 0; i < gameGrid.size(); i++)
    {
        for (int j = 0; j < gameGrid.size(); j++)
        {
            if (i == insertableBall.y && j == insertableBall.x)
            {
                if (gameGrid[i][j].empty)
                {
                    gameGrid[i][j] = insertableBall;
                    gameGrid[i][j].empty = false;
                }
            }
        }
    }
}

void arrangeBallsRandomly(std::list<Cell> &appearList, std::vector<std::vector<Cell>> &gameGrid)
{
    for (Cell next : appearList)
    {
        // std::cout << next.x << " , " << next.y << " color= " << next.color << std::endl;
        addBallInGrid(next, gameGrid);
    }
}

void print2Vector(const std::vector<std::vector<Cell>> &gameGrid)
{
    for (int i = 0; i < gameGrid.size(); i++)
    {
        for (int j = 0; j < gameGrid.size(); j++)
        {
            //cout << gameGrid[i][j].x << "," << gameGrid[i][j].y << " ";
            std::cout << gameGrid[i][j].color << "  ";
        }
        std::cout << std::endl;
    }
}

void drawBalls(sf::RenderWindow &window, Sprite &sprite, std::vector<std::vector<Cell>> &gameGrid)
{
    for (int i = game::MIN_FIELD_SIZE; i < game::MAX_FIELD_SIZE; i++)
        for (int j = game::MIN_FIELD_SIZE; j < game::MAX_FIELD_SIZE; j++)
        {
            if (!gameGrid[i][j].empty)
            {
                sprite.setTextureRect(IntRect(gameGrid[i][j].color * game::CELL_WIDTH, 0, game::CELL_WIDTH, game::CELL_WIDTH));
                sprite.setPosition(game::OFFSET_FIELD.x + j * game::CELL_WIDTH, game::OFFSET_FIELD.y + i * game::CELL_WIDTH);
                window.draw(sprite);
            }
        }
}

void setSelectedBall(Cell &ball)
{
    ball.selected = true;
}