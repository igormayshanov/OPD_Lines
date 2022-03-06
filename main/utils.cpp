#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <ctime>
#include <cassert>
#include <list>
#include "utils.h"
#include "gameconst.h"

void initGenerator(PRNG &generator)
{
    // Используем время с 1 января 1970 года в секундах как случайное зерно
    const unsigned seed = unsigned(std::time(nullptr));
    generator.engine.seed(seed);
}

// Генерирует индекс в диапазоне [0, size)
size_t getRandomIndex(PRNG &generator, size_t size)
{
    // Создаём распределение
    std::uniform_int_distribution<size_t> distribution(0, size);
    // Вычисляем псевдослучайное число: вызовем распределение как функцию,
    //  передав генератор произвольных целых чисел как аргумент.
    return distribution(generator.engine);
}

std::string GameStateToString(GameState gameState)
{
    switch (gameState)
    {
    case GameState::init:
        return "init";
    case GameState::gameOver:
        return "gameOver";
    case GameState::moveBall:
        return "moveBall";
    case GameState::newGame:
        return "newGame";
    case GameState::selectBall:
        return "selectBall";
    case GameState::selectCell:
        return "selectCell";
    case GameState::deleteLines:
        return "deleteLines";
    case GameState::appearNewBalls:
        return "appearNewBalls";
    case GameState::stop:
        return "stop";
    case GameState::wait:
        return "wait";
    default:
        assert(!"This is not possible");
        return "";
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

void printVector(const std::vector<Cell> &inVector)
{
    for (int i = 0; i < inVector.size(); i++)
    {
        //cout << gameGrid[i][j].x << "," << gameGrid[i][j].y << " ";
        std::cout << inVector[i].x << ",  " << inVector[i].y << "; ";
    }
}

void printList(const std::list<Cell> &inList)
{
    for (Cell n : inList)
        std::cout << n.x << ",  " << n.y << "; ";
    std::cout << "\n";
}

sf::Vector2i getCellPositionWhenMousePressed(sf::Vector2i mousePosition)
{
    return {(mousePosition.x - game::OFFSET_FIELD.x) / game::CELL_WIDTH,
            (mousePosition.y - game::OFFSET_FIELD.y) / game::CELL_WIDTH};
}

void gameOver(sf::RenderWindow &window)
{
    sf::Font font;
    font.loadFromFile("../fonts/arial.ttf");
    sf::Text textGameOver("Game over!\n Press Tab to continue", font, 32);
    textGameOver.setStyle(sf::Text::Bold);
    sf::RectangleShape rectangle;
    rectangle.setSize(sf::Vector2f(game::CELL_WIDTH * game::MAX_FIELD_SIZE, game::CELL_WIDTH * game::MAX_FIELD_SIZE));
    rectangle.setFillColor(sf::Color(0, 0, 0, 150));
    rectangle.setPosition(game::OFFSET_FIELD.x, game::OFFSET_FIELD.y);
    textGameOver.setFillColor(sf::Color::Red);
    textGameOver.setPosition(game::OFFSET_FIELD.x + game::CELL_WIDTH * 2, game::OFFSET_FIELD.y + game::MAX_FIELD_SIZE / 2 * game::CELL_WIDTH);
    window.draw(rectangle);
    window.draw(textGameOver);
}