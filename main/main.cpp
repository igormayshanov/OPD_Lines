#include <SFML/Graphics.hpp>
#include <vector>
#include <list>
#include <sstream>
#include <iostream>
#include <string>

#include <time.h>

#include "gameconst.h"
#include "field.h"
#include "ball.h"
#include "findPath.h"

using namespace sf;
using namespace std;
using namespace game;

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

void handleEvents(sf::RenderWindow &window, sf::Event &event, sf::Vector2i &mouseClickPosition);
sf::Vector2i getCellPositionWhenMousePressed(int x, int y);
bool checkOutOfBorder(int x, int y, int fieldSize);
std::string GameStateToString(GameState gameState);

void updateGame(const sf::Vector2i &mouseClickPosition, GameState &state, vector<vector<Cell>> &gameGrid, std::list<Cell> appearList, Cell &startBall, Cell &endBall); //обновляем игровое поле
// void update(sf::Clock &clock, GameScene &scene);
// void render(sf::RenderWindow &window, const GameScene &scene);

int main()
{

    std::list<Cell> appearList;
    sf::ContextSettings settings;
    settings.antialiasingLevel = ANTIALIASING_LEVEL;
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGTH), "Lines");
    window.setFramerateLimit(MAX_FPS);

    std::vector<std::vector<Cell>> gameGrid(MAX_FIELD_SIZE, vector<Cell>(MAX_FIELD_SIZE));
    initGameGrid(gameGrid);

    GameState gameState = GameState::init;
    Cell startBall;
    Cell endBall;
    sf::Texture texture;
    texture.loadFromFile("../img/cell.png");
    sf::Sprite sprite(texture);

    sf::Vector2i mouseClickPosition;

    generateAppearList(appearList);
    arrangeBallsRandomly(appearList, gameGrid);
    print2Vector(gameGrid);
    gameState = GameState::wait;

    sf::Event event;

    while (window.isOpen())
    {
        handleEvents(window, event, mouseClickPosition);
        updateGame(mouseClickPosition, gameState, gameGrid, appearList, startBall, endBall);
        window.clear(sf::Color::White);
        drawFields(window, sprite);
        drawBalls(window, sprite, gameGrid);
        window.display();
    }
    return 0;
}

void handleEvents(sf::RenderWindow &window, sf::Event &event, sf::Vector2i &mouseClickPosition)
{
    while (window.pollEvent(event))
    {
        switch (event.type)
        {
        case sf::Event::Closed:
            window.close();
            break;
        case sf::Event::KeyPressed:
            if (event.key.code == sf::Keyboard::Escape)
                window.close();
            break;
        case sf::Event::MouseButtonPressed:
            if ((event.mouseButton.button == Mouse::Left) && (event.mouseButton.x > game::OFFSET_FIELD.x) && (event.mouseButton.y > game::OFFSET_FIELD.y))
            {
                sf::Vector2i temp = getCellPositionWhenMousePressed(event.mouseButton.x, event.mouseButton.y);
                if (checkOutOfBorder(temp.x, temp.y))
                {
                    mouseClickPosition = temp;
                }
            }
            break;
        default:
            break;
        }
    }
}

sf::Vector2i getCellPositionWhenMousePressed(int x, int y)
{
    return {(x - game::OFFSET_FIELD.x) / CELL_WIDTH,
            (y - game::OFFSET_FIELD.y) / CELL_WIDTH};
}

void updateGame(const sf::Vector2i &mouseClickPosition, GameState &state, std::vector<std::vector<Cell>> &gameGrid, std::list<Cell> appearList, Cell &startBall, Cell &endBall)
{
    switch (state)
    {
    case GameState::init:
        break;
    case GameState::wait:
    {
        if (!gameGrid[mouseClickPosition.y][mouseClickPosition.x].empty)
        {
            std::cout << "StartCell x: " << mouseClickPosition.x << std::endl;
            std::cout << "StartCell y: " << mouseClickPosition.y << std::endl;
            setSelectedBall(gameGrid[mouseClickPosition.y][mouseClickPosition.x]);
            startBall = gameGrid[mouseClickPosition.y][mouseClickPosition.x];
            state = GameState::ballSelected;
            std::cout << GameStateToString(state) << std::endl;
        }
    }
    break;
    case GameState::ballSelected:
    {
        if (gameGrid[mouseClickPosition.y][mouseClickPosition.x].empty)
        {
            std::cout << "endCell x: " << mouseClickPosition.x << std::endl;
            std::cout << "endCell y: " << mouseClickPosition.y << std::endl;
            setSelectedBall(gameGrid[mouseClickPosition.y][mouseClickPosition.x]);
            endBall = gameGrid[mouseClickPosition.y][mouseClickPosition.x];
            state = GameState::ballMove;
            std::cout << GameStateToString(state) << std::endl;
        }
    }
    break;
    case GameState::ballMove:
    {
        std::cout << "Ok, I am in ballMove case!!!" << std::endl;
        if (findPathInGrid(gameGrid, startBall, endBall))
            state = GameState::stripBalls;
        else
        {
            state = GameState::ballSelected;
            std::cout << "No path" << std::endl;
        }

        std::cout << GameStateToString(state) << std::endl;
    }
    break;
    case GameState::nextBalls:
    {
        if (isGameGridFull(gameGrid))
        {
            state = GameState::stop;
        }
        generateAppearList(appearList);
        //add a function to display the list
        arrangeBallsRandomly(appearList, gameGrid);
        state = GameState::wait;
        std::cout << GameStateToString(state) << std::endl;
    }
    break;
    case GameState::stripBalls:
    {
        state = GameState::wait;
        std::cout << GameStateToString(state) << std::endl;
    }
    break;
    case GameState::stop:
        std::cout << GameStateToString(state) << std::endl;
        break;
        // default:
        //     brake;
    }
}

// init,
//     wait,
//     ballSelected,
//     ballMove,
//     nextBalls,
//     stripBalls,
//     stop,
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
