#pragma once

#include <random>
#include <string>
#include "ball.h"

struct PRNG
{
    std::mt19937 engine;
};

void initGenerator(PRNG &generator);
size_t getRandomIndex(PRNG &generator, size_t size);
std::string GameStateToString(GameState gameState);