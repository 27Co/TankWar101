#ifndef AI_H
#define AI_H

#include <math.h>
#include <time.h>

#include <chrono>
#include <cstdlib>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "Entity.h"
#include "Global.h"

using namespace ::std;

const std::array<char, 3> directions_char = {{'d', 'w', 'a'}};

const std::array<Direction, 3> directions_check = {
    {Direction::forward, Direction::t_left, Direction::t_right}};

// string defendingAI(Game game, char command);
// string attackingAI(Game game, char command);
// string AI(Game game, char command);
// char firstAI(Game game);

char oneAI(const Game& game, int which, bool aggressive = true);

#endif  // AI_H
