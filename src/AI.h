#ifndef AI_H
#define AI_H

#include <math.h>
#include <time.h>

#include <cstdlib>
#include <iostream>
#include <vector>

#include "Entity.h"
#include "Global.h"

const std::array<Direction, 3> directions_d = {
    {Direction::t_right, Direction::forward, Direction::t_left}};

const std::array<Direction, 3> directions_check = {
    {Direction::forward, Direction::t_left, Direction::t_right}};

Direction oneAI(const Game& game, size_t which, bool aggressive = true);

#endif  // AI_H
