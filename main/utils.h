#pragma once
#include <random>

struct PRNG
{
    std::mt19937 engine;
};

void initGenerator(PRNG &generator);
size_t getRandomIndex(PRNG &generator, size_t size);
