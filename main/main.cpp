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

void handleEvents(sf::RenderWindow &window, sf::Event &event, GameState &gameState, sf::Vector2i &mouseClickPosition);
void updateGame(sf::Vector2i &mouseClickPosition, GameState &state, std::vector<std::vector<Cell>> &gameGrid, std::list<Cell> &appearList, std::list<Cell> &lineToDelete, Cell &startBall, Cell &endCell, int &gameScore);

int main()
{
    std::list<Cell> lineToDelete;
    lineToDelete.clear();
    int gameScore = 0;

    sf::Font font;
    font.loadFromFile("../fonts/arial.ttf");
    sf::Text gameScoreText("0", font, 40);
    gameScoreText.setFillColor(Color::Red);
    gameScoreText.setStyle(sf::Text::Bold);
    gameScoreText.setPosition(240, 30);
    sf::Text info("Esc - exit, Tab - new game", font, 25);
    info.setFillColor(sf::Color::Red);
    info.setStyle(Text::Bold);
    info.setPosition(game::OFFSET_FIELD.x, 460);

    std::vector<std::vector<Cell>> gameGrid(MAX_FIELD_SIZE, vector<Cell>(MAX_FIELD_SIZE));
    initGameGrid(gameGrid);

    GameState gameState = GameState::init;
    Cell startBall;
    Cell endCell;
    sf::Texture texture;
    texture.loadFromFile("../img/cell.png");
    sf::Sprite sprite(texture);

    sf::Vector2i mouseClickPosition;

    std::list<Cell> appearList;
    generateAppearList(appearList);
    std::cout << "Print appearList" << std::endl;
    printList(appearList);
    arrangeBallsRandomly(appearList, gameGrid);
    appearList.clear();
    printList(appearList);
    print2Vector(gameGrid);

    sf::ContextSettings settings;
    settings.antialiasingLevel = ANTIALIASING_LEVEL;
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGTH), "Lines");
    window.setFramerateLimit(MAX_FPS);
    sf::Event event;

    while (window.isOpen())
    {
        window.clear(sf::Color::White);
        handleEvents(window, event, gameState, mouseClickPosition);
        updateGame(mouseClickPosition, gameState, gameGrid, appearList, lineToDelete, startBall, endCell, gameScore);
        window.draw(info);
        drawAppearListField(window, sprite);
        showAppearList(window, sprite, appearList);
        drawFields(window, sprite);
        drawBalls(window, sprite, gameGrid);
        drawScore(window, gameScore, gameScoreText);
        if (gameState == GameState::gameOver)
            gameOver(window);
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
            if (event.key.code == sf::Keyboard::Tab)
                gameState = GameState::newGame;
            break;
        case sf::Event::MouseButtonPressed:
            if ((event.mouseButton.button == Mouse::Left) && (event.mouseButton.x > game::OFFSET_FIELD.x) && (event.mouseButton.y > game::OFFSET_FIELD.y))
            {
                sf::Vector2i temp = getCellPositionWhenMousePressed({event.mouseButton.x, event.mouseButton.y});
                if (checkOutOfBorder(temp.x, temp.y))
                {
                    mouseClickPosition = temp;
                    if (gameState != GameState::selectCell && gameState != GameState::gameOver)
                        gameState = GameState::wait;
                }
            }
            break;
        default:
            break;
        }
    }
}

void updateGame(sf::Vector2i &mouseClickPosition, GameState &state, std::vector<std::vector<Cell>> &gameGrid, std::list<Cell> &appearList, std::list<Cell> &lineToDelete, Cell &startBall, Cell &endCell, int &gameScore)
{
    std::list<Cell>::iterator iterator;
    switch (state)
    {
    case GameState::init:
    {
        generateAppearList(appearList);
        state = GameState::stop;
    }
    break;
    case GameState::newGame:
    {
        initGameGrid(gameGrid);
        gameScore = 0;
        arrangeBallsRandomly(appearList, gameGrid);
        appearList.clear();
        generateAppearList(appearList);
        state = GameState::stop;
    }
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
            state = GameState::gameOver;
        }
        else
        {
            arrangeBallsRandomly(appearList, gameGrid);
            appearList.clear();
            generateAppearList(appearList);
            state = GameState::stop;
        }
    }
    break;
    case GameState::deleteLines:
    {
        std::cout << "endCell OK = " << endCell.x << ", " << endCell.y << std::endl;
        print2Vector(gameGrid);
        if (checkLines(gameGrid[endCell.y][endCell.x], gameGrid, lineToDelete))
        {
            Cell delBall;
            gameScore += getScore(lineToDelete);
            iterator = lineToDelete.begin();
            while (iterator != lineToDelete.end())
            {
                delBall = *iterator;
                initEmptyCell(gameGrid[delBall.y][delBall.x]);
                ++iterator;
            }
            std::cout << "gameScore= " << gameScore << "  del Line Size= " << lineToDelete.size() << std::endl;
            state = GameState::stop;
            printList(lineToDelete);
        }
        else
        {
            state = GameState::appearNewBalls;
        }
        std::cout << GameStateToString(state) << std::endl;
    }
    break;
    case GameState::gameOver:
        break;
    case GameState::stop:
        break;
    default:
        break;
    }
}