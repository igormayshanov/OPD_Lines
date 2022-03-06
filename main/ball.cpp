#include <SFML/Graphics.hpp>
#include <algorithm>
#include <iostream>
#include <list>
#include <vector>
#include <cstdlib>
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
void showAppearList(sf::RenderWindow &window, sf::Sprite &sprite, const std::list<Cell> &appearList)
{
    int i = 0;
    for (Cell ball : appearList)
    {
        sprite.setTextureRect(IntRect(ball.color * game::CELL_WIDTH, 0, game::CELL_WIDTH, game::CELL_WIDTH));
        sprite.setPosition(game::OFFSET_APPEAR_LIST.x + i * game::CELL_WIDTH, game::OFFSET_APPEAR_LIST.y);
        window.draw(sprite);
        i++;
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
        addBallInGrid(next, gameGrid);
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

void unsetSelectedBall(Cell &ball)
{
    ball.selected = false;
}

bool checkLines(Cell &inBall, std::vector<std::vector<Cell>> &gameGrid, std::list<Cell> &lineToDelete)
{
    lineToDelete.clear();
    int x = inBall.x;
    int y = inBall.y;
    int color = inBall.color;
    std::cout << "x= " << x << " y= " << y << " check color= " << color << " empty= " << inBall.empty << " selected= " << inBall.selected << std::endl;
    Cell ball;
    int i, j;
    bool findLine = false;
    i = 1;
    while (x + i < game::MAX_FIELD_SIZE && !gameGrid[y][x + i].empty && gameGrid[y][x + i].color == color) //проверяем вправо от шара
        i++;
    j = 1;
    while (x - j >= 0 && !gameGrid[y][x - j].empty && gameGrid[y][x - j].color == color) //проверяем влево от шара
        j++;
    int ballCounter = i + j - 1;
    if (ballCounter >= game::MIN_BALL_TO_DELETE)
    {
        ball.x = x + i;
        ball.y = y;
        for (int c = 0; c < ballCounter; c++)
        {
            ball.x--;
            lineToDelete.push_back(ball);
        }
        findLine = true;
    }
    i = 1;
    while (y + i < game::MAX_FIELD_SIZE && !gameGrid[y + i][x].empty && gameGrid[y + i][x].color == color) //проверяем вниз от шара
        i++;
    j = 1;
    while (y - j >= 0 && !gameGrid[y - j][x].empty && gameGrid[y - j][x].color == color) //проверяем вверх от шара
        j++;
    ballCounter = i + j - 1;
    if (ballCounter >= game::MIN_BALL_TO_DELETE)
    {
        ball.x = x;
        ball.y = y + i;
        for (int c = 0; c < ballCounter; c++)
        {
            ball.y--;
            lineToDelete.push_back(ball);
        }
        findLine = true;
    }
    // if (findLine)
    // {
    //     lineToDelete.remove(inBall);
    //     printList(lineToDelete);
    //     lineToDelete.push_back(inBall);
    // }
    return findLine;
}

int getScore(const std::list<Cell> &lineToDelete)
{
    return (lineToDelete.size() - game::MIN_BALL_TO_DELETE + 1) * lineToDelete.size();
}