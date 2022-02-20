#ifndef GAMECONST_H
#define GAMECONST_H

// Определите собственное пространство имен для хранения констант
namespace game
{
const int WINDOW_WIDTH = 700;
const int WINDOW_HEIGTH = 700;
const int MAX_FPS = 60;
const int ANTIALIASING_LEVEL = 4;

const int CELL_WIDTH = 40;
const sf::Vector2i OFFSET_FIELD(100, 100);
const size_t MAX_FIELD_SIZE = 9;
const size_t MIN_FIELD_SIZE = 0;

const int directionsX[] = {1, 0, -1, 0};
const int directionsY[] = {0, 1, 0, -1};
const size_t NUMBER_OF_COLOR = 7;
const size_t BALL_SPRITE_OFFSET = 0;
const size_t END_BALL_SPRITE_OFFSET = 6;
const size_t CELL_SPRITE_OFFSET = 7;

// static const sf::Color RED = sf::Color(255, 0, 0);
// static const sf::Color PINK = sf::Color(255, 20, 147);
// static const sf::Color ORANGE = sf::Color(255, 165, 0);
// static const sf::Color YELLOW = sf::Color(255, 255, 0);
// static const sf::Color PURPLE = sf::Color(128, 0, 128);
// static const sf::Color LIME = sf::Color(0, 255, 0);
// static const sf::Color DEEP_SKY_BLUE = sf::Color(0, 191, 255);
}
#endif