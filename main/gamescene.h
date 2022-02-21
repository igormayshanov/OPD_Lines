#pragma once

#include <SFML/Graphics.hpp>
#include "ball.h"
#include "field.h"
struct GameScene
{
    Cell ball;
    GameState gameState;
    unsigned scoreCount;

    sf::Font arial;
    sf::RectangleShape gameOverBackground;
    sf::Text gameOverLabel;
};

void initializeGameScene(GameScene &scene, const sf::Vector2f &sceneSize);
void updateGameScene(GameScene &scene, float elapsedTime);
std::string getGameSceneWindowTitle(const GameScene &scene);
void drawGameScene(sf::RenderWindow &window, const GameScene &scene);
void destroyGameScene(GameScene &scene);