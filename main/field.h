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

bool operator==(const Cell &cell1, const Cell &cell2);

void drawFields(sf::RenderWindow &window, sf::Sprite &sprite);
void drawAppearListField(sf::RenderWindow &window, sf::Sprite &sprite);
void drawScore(sf::RenderWindow &window, int score, sf::Text &gameScoreText);
void initGameGrid(std::vector<std::vector<Cell>> &gameGrid);
void initEmptyCell(Cell &cell);
bool isGameGridFull(const std::vector<std::vector<Cell>> &gameGrid);
bool checkOutOfBorder(int x, int y);
