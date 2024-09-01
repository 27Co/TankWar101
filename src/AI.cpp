#include "AI.h"

#include <iostream>
#include <vector>

/**
 * @brief: AI that can make a decision
 * @param game: ongoing game
 * @param which: which tank to control
 * @param aggressive: whether AI is in aggressive mode
 */
Direction oneAI(const Game& game, size_t which, bool aggressive) {
    if (which >= TANK_NUM) {
        std::cout << "Error calling oneAI: index out of range" << std::endl;
        return Direction::none;
    }

    // tiles that bullets can reach in the next round
    std::vector<std::array<size_t, 2>> dangerZone;
    for (auto bullet_p : game.Bullets_p) {
        dangerZone.push_back({bullet_p->getx(), bullet_p->gety()});
        if (bullet_p->inField()) {
            dangerZone.push_back(
                {static_cast<size_t>(bullet_p->probex(bullet_p->getFace())),
                 static_cast<size_t>(bullet_p->probey(bullet_p->getFace()))});
        }
        if (bullet_p->inField(2)) {
            dangerZone.push_back(
                {static_cast<size_t>(bullet_p->probex(bullet_p->getFace(), 2)),
                 static_cast<size_t>(
                     bullet_p->probey(bullet_p->getFace(), 2))});
        }
    }

    std::vector<Direction> safeDirections;
    for (auto d : directions) {
        // safeDir should be valid (inField)
        if (!game.Tanks_p[which]->inField()) {
            continue;
        }
        bool safe = true;
        size_t newX = static_cast<size_t>(game.Tanks_p[which]->probex(d));
        size_t newY = static_cast<size_t>(game.Tanks_p[which]->probey(d));
        for (auto bulletPos : dangerZone) {
            if (newX == bulletPos[0] && newY == bulletPos[1]) {
                safe = false;
                break;
            }
        }
        size_t gasWidth = INITIAL_GAS_WIDTH + game.round / SHRINK_RATE;
        if (newX < gasWidth || newX >= FIELD_SIZE_DISPLAY - gasWidth ||
            newY < gasWidth || newY >= FIELD_SIZE_DISPLAY - gasWidth) {
            safe = false;
        }
        if (safe) {
            safeDirections.push_back(d);
        }
    }

    if (!aggressive && safeDirections.size() > 0) {
        std::srand(static_cast<unsigned int>(time(nullptr)));
        return safeDirections[static_cast<size_t>(std::rand()) %
                              safeDirections.size()];
    }

    size_t another = (which == 0) ? 1 : 0;

    Direction targetDir;
    double distance2 = pow(FIELD_SIZE_DISPLAY, 2) * 2;
    if (safeDirections.size() > 0) {
        for (auto d : safeDirections) {
            double newDistance2 =
                pow(game.Tanks_p[which]->probex(d) -
                        static_cast<int>(game.Tanks_p[another]->getx()),
                    2) +
                pow(game.Tanks_p[which]->probey(d) -
                        static_cast<int>(game.Tanks_p[another]->gety()),
                    2);
            if (newDistance2 < distance2) {
                targetDir = d;
                distance2 = newDistance2;
            }
        }
    } else {
        targetDir = directions[static_cast<size_t>(std::rand()) %
                               safeDirections.size()];
    }
    return targetDir;
}
