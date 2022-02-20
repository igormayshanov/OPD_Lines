#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

struct Cell
{
    size_t x;
    size_t y;
    size_t color;
    bool empty;
    bool selected;
};

void drawFields(sf::RenderWindow &window, sf::Sprite &sprite);

void initGameGrid(std::vector<std::vector<Cell>> &gameGrid);
bool isGameGridFull(const std::vector<std::vector<Cell>> &gameGrid);
