#include <iostream>
#include <random>
#include <ctime>
#include "utils.h"

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
