#ifndef AI_H
#define AI_H

#include <math.h>
#include <time.h>

#include <chrono>
#include <cstdlib>
#include <iostream>
#include <thread>
#include <vector>

#include "Entity.h"
#include "Global.h"

const std::array<char, 3> directions_char = {{'d', 'w', 'a'}};

const std::array<Direction, 3> directions_check = {
    {Direction::forward, Direction::t_left, Direction::t_right}};

char oneAI(const Game& game, int which, bool aggressive = true);

#endif  // AI_H
