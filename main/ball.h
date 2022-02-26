
#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <list>
#include <iostream>
#include "field.h"

enum class GameState
{
    init,
    wait,
    selectBall,
    selectCell,
    moveBall,
    appearNewBalls,
    deleteLines,
    stop,
};

void drawBalls(sf::RenderWindow &window, sf::Sprite &sprite, std::vector<std::vector<Cell>> &gameGrid);
void arrangeBallsRandomly(std::list<Cell> &appearList, std::vector<std::vector<Cell>> &gameGrid);
void generateAppearList(std::list<Cell> &appearList);
void showAppearList(sf::RenderWindow &window, sf::Sprite &sprite, const std::list<Cell> &appearList);
void addBallInGrid(const Cell &ball, std::vector<std::vector<Cell>> &gameGrid);
void setSelectedBall(Cell &ball);
void unsetSelectedBall(Cell &ball);
bool checkLines(Cell &inBall, std::vector<std::vector<Cell>> &gameGrid, std::list<Cell> &deletedLine);