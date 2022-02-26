#pragma once

#include <vector>
#include "ball.h"
#include "field.h"

bool findPathInGrid(std::vector<std::vector<Cell>> &gameGrid, Cell &startBall, Cell &endCell);
