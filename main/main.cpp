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
#include "utils.h"

using namespace sf;
using namespace std;
using namespace game;

// void checkLines();
// void getScore();
// void gameOver(Event event);

void handleEvents(sf::RenderWindow &window, sf::Event &event, GameState &gameState, sf::Vector2i &mouseClickPosition);
sf::Vector2i getCellPositionWhenMousePressed(sf::Vector2i mousePosition);
bool checkOutOfBorder(int x, int y, int fieldSize);
std::string GameStateToString(GameState gameState);

void updateGame(sf::Vector2i &mouseClickPosition, GameState &state, std::vector<std::vector<Cell>> &gameGrid, std::list<Cell> appearList, std::list<Cell> &deletedLine, Cell &startBall, Cell &endCell);

int main()
{
    std::list<Cell> appearList;
    std::list<Cell> deletedLine;
    sf::ContextSettings settings;
    settings.antialiasingLevel = ANTIALIASING_LEVEL;
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGTH), "Lines");
    window.setFramerateLimit(MAX_FPS);

    std::vector<std::vector<Cell>> gameGrid(MAX_FIELD_SIZE, vector<Cell>(MAX_FIELD_SIZE));
    initGameGrid(gameGrid);

    GameState gameState = GameState::init;
    Cell startBall;
    Cell endCell;
    sf::Texture texture;
    texture.loadFromFile("../img/cell.png");
    sf::Sprite sprite(texture);

    sf::Vector2i mouseClickPosition;

    generateAppearList(appearList);
    arrangeBallsRandomly(appearList, gameGrid);
    print2Vector(gameGrid);

    sf::Event event;

    while (window.isOpen())
    {
        handleEvents(window, event, gameState, mouseClickPosition);
        updateGame(mouseClickPosition, gameState, gameGrid, appearList, deletedLine, startBall, endCell);
        window.clear(sf::Color::White);
        drawAppearListField(window, sprite);
        drawFields(window, sprite);
        drawBalls(window, sprite, gameGrid);
        showAppearList(window, sprite, appearList);
        window.display();
    }
    return 0;
}

void handleEvents(sf::RenderWindow &window, sf::Event &event, GameState &gameState, sf::Vector2i &mouseClickPosition)
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
                sf::Vector2i temp = getCellPositionWhenMousePressed({event.mouseButton.x, event.mouseButton.y});
                if (checkOutOfBorder(temp.x, temp.y))
                {
                    mouseClickPosition = temp;
                    if (gameState != GameState::selectCell)
                        gameState = GameState::wait;
                    std::cout << "Click " << GameStateToString(gameState) << std::endl;
                }
            }
            break;
        default:
            break;
        }
    }
}

sf::Vector2i getCellPositionWhenMousePressed(sf::Vector2i mousePosition)
{
    return {(mousePosition.x - game::OFFSET_FIELD.x) / CELL_WIDTH,
            (mousePosition.y - game::OFFSET_FIELD.y) / CELL_WIDTH};
}

void updateGame(sf::Vector2i &mouseClickPosition, GameState &state, std::vector<std::vector<Cell>> &gameGrid, std::list<Cell> appearList, std::list<Cell> &deletedLine, Cell &startBall, Cell &endCell)
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
            std::cout << "StartCell selected status: " << gameGrid[mouseClickPosition.y][mouseClickPosition.x].selected << std::endl;
            setSelectedBall(gameGrid[mouseClickPosition.y][mouseClickPosition.x]);
            startBall = gameGrid[mouseClickPosition.y][mouseClickPosition.x];
            std::cout << "StartCell selected status: " << startBall.selected << std::endl;
            state = GameState::selectCell;
            std::cout << GameStateToString(state) << std::endl;
        }
    }

    break;
    case GameState::selectCell:
    {
        if (gameGrid[mouseClickPosition.y][mouseClickPosition.x].empty)
        {
            std::cout << "endCell x: " << mouseClickPosition.x << std::endl;
            std::cout << "endCell y: " << mouseClickPosition.y << std::endl;
            setSelectedBall(gameGrid[mouseClickPosition.y][mouseClickPosition.x]);
            endCell = gameGrid[mouseClickPosition.y][mouseClickPosition.x];
            state = GameState::moveBall;
            std::cout << GameStateToString(state) << std::endl;
        }
    }

    break;
    case GameState::selectBall:
        break;
    case GameState::moveBall:
    {
        std::cout << "Ok, I am in ballMove case!!!" << std::endl;
        if (findPathInGrid(gameGrid, startBall, endCell))
            state = GameState::deleteLines;
        else
        {
            state = GameState::selectCell;
            std::cout << "No path" << std::endl;
        }

        std::cout << GameStateToString(state) << std::endl;
    }
    break;
    case GameState::appearNewBalls:
    {
        if (isGameGridFull(gameGrid))
        {
            state = GameState::stop;
            std::cout << "Game over!!!" << std::endl;
        }
        else
        {
            generateAppearList(appearList);
            //add a function to display the list
            arrangeBallsRandomly(appearList, gameGrid);
            state = GameState::stop;
        }

        //state = GameState::wait;
        std::cout << GameStateToString(state) << std::endl;
    }
    break;
    case GameState::deleteLines:
    {
        if (checkLines(endCell, gameGrid, deletedLine))
        {
            std::cout << "checkLines OK" << std::endl;
            state = GameState::stop;
            printList(deletedLine);
        }
        else
        {
            std::cout << "checkLines NOT OK" << std::endl;
            state = GameState::appearNewBalls;
        }

        std::cout << GameStateToString(state) << std::endl;
    }
    break;
    case GameState::stop:
        std::cout << GameStateToString(state) << std::endl;
        break;
    default:
        break;
    }
}