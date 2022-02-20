
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
    ballSelected,
    ballMove,
    nextBalls,
    lineStrip,
    stop,
};

void drawBalls(sf::RenderWindow &window, sf::Sprite &sprite, std::vector<std::vector<Cell>> &gameGrid);
void arrangeBallsRandomly(std::list<Cell> &appearList, std::vector<std::vector<Cell>> &gameGrid);
void generateAppearList(std::list<Cell> &appearList, int appearBalls);
void addBallInGrid(const Cell &ball, std::vector<std::vector<Cell>> &gameGrid);
void print2Vector(const std::vector<std::vector<Cell>> &gameGrid);
void setSelectedBall(Cell &ball);
void pollEvents(sf::RenderWindow &window); //опрашиваем и обрабатываем доступные события в цикле

void update(const sf::Vector2f &mousePosition); //обновляем игровое поле
