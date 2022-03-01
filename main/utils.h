#pragma once

#include <SFML/Graphics.hpp>
#include <random>
#include <list>
#include <string>
#include <vector>
#include "ball.h"

struct PRNG
{
    std::mt19937 engine;
};

void initGenerator(PRNG &generator);
size_t getRandomIndex(PRNG &generator, size_t size);
std::string GameStateToString(GameState gameState);
void print2Vector(const std::vector<std::vector<Cell>> &gameGrid);
void printVector(const std::vector<Cell> &inVector);
void printList(const std::list<Cell> &inList);
sf::Vector2i getCellPositionWhenMousePressed(sf::Vector2i mousePosition);