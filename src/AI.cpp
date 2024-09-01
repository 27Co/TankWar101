#include "AI.h"

/**
 * @brief: AI that can make a decision
 * @param game: ongoing game
 * @param which: which tank to control
 * @param aggressive: whether AI is in aggressive mode
 */
Direction oneAI(const Game& game, size_t which, bool aggressive) {
    if (which >= TANK_NUM) {
        std::cout << "Error calling oneAI: index out of range" << std::endl;
        return Direction::forward;
    }

    // tiles that bullets can reach in the next round
    std::vector<std::array<int, 2>> dangerZone;
    for (auto bullet_p : game.Bullets_p) {
        dangerZone.push_back({bullet_p->getx(), bullet_p->gety()});
        dangerZone.push_back({bullet_p->probex(1), bullet_p->probey(1)});
        dangerZone.push_back({bullet_p->probex(2), bullet_p->probey(2)});
    }

    std::vector<Direction> safeDirections;
    for (auto d : directions_check) {
        bool safe = true;
        int newX = game.Tanks_p[which]->probex(1, d);
        int newY = game.Tanks_p[which]->probey(1, d);
        for (auto bulletPos : dangerZone) {
            if (newX == bulletPos[0] && newY == bulletPos[1]) {
                safe = false;
                break;
            }
        }
        int gasWidth = INITIAL_GAS_WIDTH + game.round / SHRINK_RATE;
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
        return directions_d[static_cast<size_t>(
            safeDirections[std::rand() % safeDirections.size()])];
    }

    int another = (which == 0) ? 1 : 0;

    Direction enemyDirection = Direction::forward;
    double distance2 = pow(FIELD_SIZE_DISPLAY, 2) * 2;
    // std::cout << "Safe directions: " << std::endl;
    for (auto d : safeDirections) {
        // std::cout << static_cast<int>(d);
        double newDistance2 = pow(game.Tanks_p[which]->probex(1, d) -
                                      game.Tanks_p[another]->getx(),
                                  2) +
                              pow(game.Tanks_p[which]->probey(1, d) -
                                      game.Tanks_p[another]->gety(),
                                  2);
        if (newDistance2 < distance2) {
            enemyDirection = d;
            distance2 = newDistance2;
        }
    }
    return directions_d[static_cast<size_t>(enemyDirection)];
}
