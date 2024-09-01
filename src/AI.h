#ifndef AI_H
#define AI_H

#include <math.h>
#include <time.h>

#include <cstdlib>

#include "Entity.h"
#include "Global.h"

const std::array<Direction, 5> directions = {{Direction::down, Direction::right,
                                              Direction::up, Direction::left,
                                              Direction::none}};

Direction oneAI(const Game& game, size_t which, bool aggressive = true);

#endif  // AI_H
