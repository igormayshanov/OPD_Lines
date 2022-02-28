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
    bool operator==(const Cell &other) const
    {
        return std::tie(x, y, color, empty) == std::tie(other.x, other.y, other.color, other.empty);
    }
    bool operator!=(const Cell &other) const
    {
        return std::tie(x, y, color, empty) != std::tie(other.x, other.y, other.color, other.empty);
    }
};

void drawFields(sf::RenderWindow &window, sf::Sprite &sprite);
void drawAppearListField(sf::RenderWindow &window, sf::Sprite &sprite);
void drawScore(sf::RenderWindow &window, int score, sf::Text &gameScoreText);
void initGameGrid(std::vector<std::vector<Cell>> &gameGrid);
void initEmptyCell(Cell &cell);
bool isGameGridFull(const std::vector<std::vector<Cell>> &gameGrid);
bool checkOutOfBorder(int x, int y);
