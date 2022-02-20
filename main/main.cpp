#include <SFML/Graphics.hpp>
#include <vector>
#include <list>
#include <sstream>
#include <iostream>
#include <cassert>
#include <string>

#include <time.h>

#include "gameconst.h"
#include "field.h"
#include "ball.h"

using namespace sf;
using namespace std;
using namespace game;

enum class BallColor
{
    RED,
    PINK,
    ORANGE,
    YELLOW,
    PURPLE,
    LIME,
    DEEP_SKY_BLUE,
};

// void onMouseButtonPressed(const sf::Event::MouseButtonPressed &event, sf::Vector2f &mouseClickPosition)
// {
//     mouseClickPosition = {float(event.x), float(event.y)};
// }

// bool checkBorder();
// bool isEmptyCell();
// void findPath();
// void restorePath();
// void checkLines();
// void getRandomColor();

// void getScore();
// void gameOver(Event event);

#include <SFML/Graphics.hpp>

void handleEvents(sf::RenderWindow &window);
bool checkOutOfBorder(int x, int y, int fieldSize);
std::string GameStateToString(GameState gameState);
// void update(sf::Clock &clock, GameScene &scene);
// void render(sf::RenderWindow &window, const GameScene &scene);

int main()
{
    int appearBalls = 3;
    list<Cell> appearList;
    sf::ContextSettings settings;
    settings.antialiasingLevel = ANTIALIASING_LEVEL;
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGTH), "Lines");
    window.setFramerateLimit(MAX_FPS);

    vector<vector<Cell>> gameGrid(MAX_FIELD_SIZE, vector<Cell>(MAX_FIELD_SIZE));
    initGameGrid(gameGrid);

    GameState state = GameState::init;
    Cell startCell;
    Cell endCell;
    Texture texture;
    texture.loadFromFile("../img/cell.png");
    // Создание спрайта
    Sprite sprite(texture);

    sf::Vector2f mouseClickPosition;

    generateAppearList(appearList, appearBalls);
    arrangeBallsRandomly(appearList, gameGrid);
    print2Vector(gameGrid);

    sf::Event event;

    while (window.isOpen())
    {
        Vector2i pos = Mouse::getPosition(window);
        int x = (pos.x - OFFSET_FIELD.x) / CELL_WIDTH;
        int y = (pos.y - OFFSET_FIELD.y) / CELL_WIDTH;

        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:
                // Получаем нажатую клавишу - выполняем соответствующее действие
                if (event.key.code == sf::Keyboard::Escape)
                    window.close();
                break;
            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == Mouse::Left)
                {
                    if (checkOutOfBorder(x, y, MAX_FIELD_SIZE))
                    {
                        if (state == GameState::init)
                        {
                            std::cout << "Mouse pressed in " << x << ", " << y << "\n";
                            setSelectedBall(gameGrid[y][x]);
                            state = GameState::ballSelected;
                            startCell = gameGrid[y][x];
                            std::cout << GameStateToString(state) << ", "
                                      << gameGrid[y][x].color << ", "
                                      << gameGrid[y][x].empty << ", "
                                      << gameGrid[y][x].x << ", "
                                      << gameGrid[y][x].y << ","
                                      << gameGrid[y][x].selected
                                      << "\n";
                        }
                        else if (state == GameState::ballSelected)
                        {
                            setSelectedBall(gameGrid[y][x]);
                            endCell = gameGrid[y][x];
                            state = GameState::ballMove;
                            std::cout << "After ballSelected: " << GameStateToString(state) << ", "
                                      << gameGrid[y][x].color << ", "
                                      << gameGrid[y][x].empty << ", "
                                      << gameGrid[y][x].x << ", "
                                      << gameGrid[y][x].y << ","
                                      << gameGrid[y][x].selected
                                      << "\n";
                        }
                        else
                            break;
                    }
                }
                break;
            default:
                break;
            }
        }
        // Выполняем необходимые действия по отрисовке
        window.clear(sf::Color::White);
        drawFields(window, sprite);
        drawBalls(window, sprite, gameGrid);

        window.display();
    }

    return 0;
}

bool checkOutOfBorder(int x, int y, int fieldSize)
{
    if (x >= 0 && x < fieldSize && y >= 0 && y < fieldSize)
        return true;
    else
        return false;
}

void handleEvents(sf::RenderWindow &window)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        switch (event.type)
        {
        case sf::Event::Closed:
            window.close();
            break;
        case sf::Event::KeyPressed:
            // Получаем нажатую клавишу - выполняем соответствующее действие
            if (event.key.code == sf::Keyboard::Escape)
                window.close();
            break;
        default:
            break;
        }
    }
}

// void update(sf::Clock &clock, GameScene &scene)
// {
//     const float elapsedTime = clock.getElapsedTime().asSeconds();
//     clock.restart();
//     updateGameScene(scene, elapsedTime);
// }

// void render(sf::RenderWindow &window, const GameScene &scene)
// {
//     window.clear();
//     drawGameScene(window, scene);
//     window.display();
// }

std::string GameStateToString(GameState gameState)
{
    switch (gameState)
    {
    case GameState::init:
        return "init";
    case GameState::ballMove:
        return "ballMove";
    case GameState::ballSelected:
        return "ballSelected";
    case GameState::lineStrip:
        return "lineStrip";
    case GameState::nextBalls:
        return "nextBalls";
    case GameState::stop:
        return "stop";
    case GameState::wait:
        return "wait";
    default:
        assert(!"This is not possible");
        return "";
    }
}
