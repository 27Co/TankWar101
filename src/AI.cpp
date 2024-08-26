#include "AI.h"

/**
 * @brief: AI that can make a decision
 * @param game: ongoing game
 * @param which: which tank to control
 * @param aggressive: whether AI is in aggressive mode
 */
char oneAI(const Game& game, int which, bool aggressive) {
    std::this_thread::sleep_for(std::chrono::milliseconds(800));

    if (which < 0 || which >= TANK_NUM) {
        std::cout << "Error calling oneAI: index out of range" << std::endl;
        return 'e';
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
        return directions_char[static_cast<int>(
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
    return directions_char[static_cast<int>(enemyDirection)];
}

/*
string attackingAI(Game game, char command) {
    // std::cout << "called attacking AI" << std::endl;
    int thexnow = game.Tanks_p[0]->getx();
    int theynow = game.Tanks_p[0]->gety();
    Face facenow = game.Tanks_p[0]->getFace();
    int thexenem = game.Tanks_p[1]->getx();
    int theyenem = game.Tanks_p[1]->gety();
    Face faceenem = game.Tanks_p[1]->getFace();
    int check1, check2;
    bool danger = false;
    int xi, yi;
    for (int i = thexnow - 1; i <= thexnow + 1; i++) {
        for (int j = theynow - 1; j <= theynow + 1; j++) {
            if (game.field[i][j] == Object::Empty) {
                danger = true;
                break;
            }
        }
        if (danger) break;
    }
    if (danger) {
        if ((facenow == Face::down &&
             game.field[thexnow][theynow + 1] == Object::Gas) ||
            (facenow == Face::left &&
             game.field[thexnow - 1][theynow] == Object::Gas) ||
            (facenow == Face::right &&
             game.field[thexnow + 1][theynow] == Object::Gas) ||
            (facenow == Face::up &&
             game.field[thexnow][theynow - 1] == Object::Gas)) {
            check1 = 1;
        } else
            check1 = 0;
    }

    switch (faceenem) {
        case Face::up:
            if (command == 'a')
                faceenem = Face::left;
            else if (command == 'd')
                faceenem = Face::right;
            break;
        case Face::down:
            if (command == 'a')
                faceenem = Face::right;
            else if (command == 'd')
                faceenem = Face::left;
            break;
        case Face::left:
            if (command == 'a')
                faceenem = Face::down;
            else if (command == 'd')
                faceenem = Face::up;
            break;
        case Face::right:
            if (command == 'a')
                faceenem = Face::up;
            else if (command == 'd')
                faceenem = Face::down;
            break;
    }

    switch (faceenem) {
        case Face::up:
            xi = thexenem;
            yi = theyenem - 1;
            break;
        case Face::down:
            xi = thexenem;
            yi = theyenem + 1;
            break;
        case Face::left:
            xi = thexenem - 1;
            yi = theyenem;
            break;
        case Face::right:
            xi = thexenem + 1;
            yi = theyenem;
            break;
    }
    int deltaX = xi - thexnow;
    int deltaY = yi - theynow;

    Face targetDirection;
    if (abs(deltaX) > abs(deltaY)) {
        targetDirection = deltaX > 0 ? Face::right : Face::left;
    } else {
        targetDirection = deltaY > 0 ? Face::down : Face::up;
    }
    switch (targetDirection) {
        case Face::up:
            if ((facenow == Face::up) && (check1 == 1)) {
                if (game.field[thexnow - 1][theynow] == Object::Gas) {
                    check2 = 1;
                } else
                    check2 = 0;
            } else if (facenow == Face::right) {
                check2 = 0;
            } else if (facenow == Face::left) {
                check2 = 1;
            } else if ((facenow == Face::up) && (check1 == 0)) {
                check2 = 2;
            } else {
                check2 = 1;
            }

            break;
        case Face::down:
            if ((facenow == Face::down) && (check1 == 1)) {
                if (game.field[thexnow + 1][theynow] == Object::Gas) {
                    check2 = 1;
                } else
                    check2 = 0;
            } else if (facenow == Face::right) {
                check2 = 1;
            } else if (facenow == Face::left) {
                check2 = 0;
            } else if ((facenow == Face::down) && (check1 == 0)) {
                check2 = 2;
            } else {
                check2 = 1;
            }
            break;
        case Face::left:
            if ((facenow == Face::left) && (check1 == 1)) {
                if (game.field[thexnow][theynow + 1] == Object::Gas) {
                    check2 = 1;
                } else
                    check2 = 0;
            } else if (facenow == Face::down) {
                check2 = 1;
            } else if (facenow == Face::up) {
                check2 = 0;
            } else if ((facenow == Face::left) && (check1 == 0)) {
                check2 = 2;
            } else {
                check2 = 1;
            }
            break;
        case Face::right:
            if ((facenow == Face::right) && (check1 == 1)) {
                if (game.field[thexnow][theynow - 1] == Object::Gas) {
                    check2 = 1;
                } else
                    check2 = 0;
            } else if (facenow == Face::up) {
                check2 = 1;
            } else if (facenow == Face::down) {
                check2 = 0;
            } else if ((facenow == Face::right) && (check1 == 0)) {
                check2 = 2;
            } else {
                check2 = 1;
            }
            break;
    }
    // added default case
    // also, since check2 can only be 0, 1, 2
    // the last else if was changed to else to stop the compiler complaining
    switch (command) {
        case 'q':
            return "qq";
        case 'a':
            if (check2 == 2) {
                return "aw";
            } else if (check2 == 1) {
                return "ad";
            } else {
                return "aa";
            }
            break;
        case 'd':
            if (check2 == 2) {
                return "dw";
            } else if (check2 == 1) {
                return "dd";
            } else {
                return "da";
            }
            break;
        // anything other than a d q will be treated as w (forward)
        default:
            if (check2 == 2) {
                return "ww";
            } else if (check2 == 1) {
                return "wd";
            } else {
                return "wa";
            }
            break;
    }
}

string defendingAI(Game game, char command) {
    // std::cout << "called defending AI" << std::endl;
    int thexnow = game.Tanks_p[0]->getx();
    int theynow = game.Tanks_p[0]->gety();
    Face facenow = game.Tanks_p[0]->getFace();
    int thexenem = game.Tanks_p[1]->getx();
    int theyenem = game.Tanks_p[1]->gety();
    Face faceenem = game.Tanks_p[1]->getFace();
    int check1, check2, check3;
    bool danger = false;
    int xi, yi;
    for (int i = std::max(0, thexnow - 1);
         i < std::min(FIELD_SIZE_DISPLAY, thexnow + 2); i++) {
        for (int j = std::max(0, theynow - 1);
             j < std::min(FIELD_SIZE_DISPLAY, theynow + 2); j++) {
            if (game.field[i][j] == Object::Gas) {
                danger = true;
                break;
            }
        }
        if (danger) break;
    }
    // std::cout << "checkpoint 221" << std::endl;
    if (danger) {
        if ((facenow == Face::down &&
             game.field[thexnow][theynow + 1] == Object::Gas) ||
            (facenow == Face::left &&
             game.field[thexnow - 1][theynow] == Object::Gas) ||
            (facenow == Face::right &&
             game.field[thexnow + 1][theynow] == Object::Gas) ||
            (facenow == Face::up &&
             game.field[thexnow][theynow - 1] == Object::Gas)) {
            check1 = 1;
        } else
            check1 = 0;
    }

    // std::cout << "checkpoint 236" << std::endl;
    switch (faceenem) {
        case Face::up:
            if (command == 'a')
                faceenem = Face::left;
            else if (command == 'd')
                faceenem = Face::right;
            break;
        case Face::down:
            if (command == 'a')
                faceenem = Face::right;
            else if (command == 'd')
                faceenem = Face::left;
            break;
        case Face::left:
            if (command == 'a')
                faceenem = Face::down;
            else if (command == 'd')
                faceenem = Face::up;
            break;
        case Face::right:
            if (command == 'a')
                faceenem = Face::up;
            else if (command == 'd')
                faceenem = Face::down;
            break;
    }

    // std::cout << "checkpoint 265" << std::endl;
    for (int i = std::max(0, thexnow - 1);
         i < std::min(FIELD_SIZE_DISPLAY, thexnow + 2); i++) {
        for (int j = std::max(0, theynow - 1);
             j < std::min(FIELD_SIZE_DISPLAY, theynow + 2); j++) {
            for (size_t k = 0; k < game.Bullets_p.size(); k++) {
                if ((game.Bullets_p[k]->probex(2) == i) &&
                    (game.Bullets_p[k]->probey(2) == j)) {
                    check3 = 1;
                    break;
                } else
                    check3 = 0;
            }
            if (check3 == 1) {
                break;
            }
        }
        if (check3 == 1) {
            break;
        }
    }
    // std::cout << "checkpoint 286" << std::endl;
    switch (faceenem) {
        case Face::up:
            if (command == 'a')
                faceenem = Face::left;
            else if (command == 'd')
                faceenem = Face::right;
            break;
        case Face::down:
            if (command == 'a')
                faceenem = Face::right;
            else if (command == 'd')
                faceenem = Face::left;
            break;
        case Face::left:
            if (command == 'a')
                faceenem = Face::down;
            else if (command == 'd')
                faceenem = Face::up;
            break;
        case Face::right:
            if (command == 'a')
                faceenem = Face::up;
            else if (command == 'd')
                faceenem = Face::down;
            break;
    }

    // std::cout << "checkpoint 313" << std::endl;
    switch (faceenem) {
        case Face::up:
            xi = thexenem;
            yi = theyenem - 1;
            break;
        case Face::down:
            xi = thexenem;
            yi = theyenem + 1;
            break;
        case Face::left:
            xi = thexenem - 1;
            yi = theyenem;
            break;
        case Face::right:
            xi = thexenem + 1;
            yi = theyenem;
            break;
    }
    int deltaX = xi - thexnow;
    int deltaY = yi - theynow;

    Face targetDirection;
    if (abs(deltaX) > abs(deltaY)) {
        targetDirection = deltaX > 0 ? Face::right : Face::left;
    } else {
        targetDirection = deltaY > 0 ? Face::down : Face::up;
    }
    // std::cout << "checkpoint 339" << std::endl;
    if (deltaX + deltaY > 4) {
        switch (targetDirection) {
            case Face::up:
                if ((facenow == Face::up) && (check1 == 1)) {
                    if (game.field[max(thexnow - 1, 0)][min(
                            theynow, FIELD_SIZE_DISPLAY - 1)] == Object::Gas) {
                        check2 = 1;
                    } else
                        check2 = 0;
                } else if (facenow == Face::right) {
                    check2 = 0;
                } else if (facenow == Face::left) {
                    check2 = 1;
                } else if ((facenow == Face::up) && (check1 == 0)) {
                    check2 = 2;
                } else {
                    check2 = 1;
                }

                break;
            case Face::down:
                if ((facenow == Face::down) && (check1 == 1)) {
                    if (game.field[max(thexnow + 1, FIELD_SIZE_DISPLAY - 1)]
                                  [theynow] == Object::Gas) {
                        check2 = 1;
                    } else
                        check2 = 0;
                } else if (facenow == Face::right) {
                    check2 = 1;
                } else if (facenow == Face::left) {
                    check2 = 0;
                } else if ((facenow == Face::down) && (check1 == 0)) {
                    check2 = 2;
                } else {
                    check2 = 1;
                }
                break;
            case Face::left:
                if ((facenow == Face::left) && (check1 == 1)) {
                    if (game.field[thexnow]
                                  [min(theynow + 1, FIELD_SIZE_DISPLAY - 1)] ==
                        Object::Gas) {
                        check2 = 1;
                    } else
                        check2 = 0;
                } else if (facenow == Face::down) {
                    check2 = 1;
                } else if (facenow == Face::up) {
                    check2 = 0;
                } else if ((facenow == Face::left) && (check1 == 0)) {
                    check2 = 2;
                } else {
                    check2 = 1;
                }
                break;
            case Face::right:
                if ((facenow == Face::right) && (check1 == 1)) {
                    if (game.field[thexnow][max(theynow - 1, 0)] ==
                        Object::Gas) {
                        check2 = 1;
                    } else
                        check2 = 0;
                } else if (facenow == Face::up) {
                    check2 = 1;
                } else if (facenow == Face::down) {
                    check2 = 0;
                } else if ((facenow == Face::right) && (check1 == 0)) {
                    check2 = 2;
                } else {
                    check2 = 1;
                }
                break;
        }
        // added default case
        // also, since check2 can only be 0, 1, 2
        // the last else if was changed to else to stop the compiler complaining
        switch (command) {
            case 'q':
                return "qq";
            case 'a':
                if (check2 == 2) {
                    return "aw";
                } else if (check2 == 1) {
                    return "ad";
                } else {
                    return "aa";
                }
                break;
            case 'd':
                if (check2 == 2) {
                    return "dw";
                } else if (check2 == 1) {
                    return "dd";
                } else {
                    return "da";
                }
                break;
            // anything other than a d q will be treated as w (forward)
            default:
                if (check2 == 2) {
                    return "ww";
                } else if (check2 == 1) {
                    return "wd";
                } else {
                    return "wa";
                }
                break;
        }
    } else {
        switch (targetDirection) {
            case Face::up:
                if ((facenow == Face::down) && (check1 == 1) && (check3 == 0)) {
                    if (game.field[max(thexnow - 1, 0)][theynow] ==
                        Object::Gas) {
                        check2 = 1;
                    } else
                        check2 = 0;
                } else if (facenow == Face::left) {
                    check2 = 0;
                } else if (facenow == Face::right) {
                    check2 = 1;
                } else if ((facenow == Face::down) && (check1 == 0)) {
                    check2 = 2;
                } else {
                    check2 = 1;
                }

                break;
            case Face::down:
                if ((facenow == Face::up) && (check1 == 1)) {
                    if (game.field[min(thexnow + 1, FIELD_SIZE_DISPLAY - 1)]
                                  [theynow] == Object::Gas) {
                        check2 = 1;
                    } else
                        check2 = 0;
                } else if (facenow == Face::left) {
                    check2 = 1;
                } else if (facenow == Face::right) {
                    check2 = 0;
                } else if ((facenow == Face::up) && (check1 == 0)) {
                    check2 = 2;
                } else {
                    check2 = 1;
                }
                break;
            case Face::left:
                if ((facenow == Face::right) && (check1 == 1) &&
                    (check3 == 0)) {
                    if (game.field[thexnow]
                                  [min(theynow + 1, FIELD_SIZE_DISPLAY - 1)] ==
                        Object::Gas) {
                        check2 = 1;
                    } else
                        check2 = 0;
                } else if (facenow == Face::up) {
                    check2 = 1;
                } else if (facenow == Face::down) {
                    check2 = 0;
                } else if ((facenow == Face::right) && (check1 == 0) &&
                           (check3 == 0)) {
                    check2 = 2;
                } else {
                    check2 = 1;
                }
                break;
            case Face::right:
                if ((facenow == Face::left) && (check1 == 1) && (check3 == 0)) {
                    if (game.field[thexnow][max(theynow - 1, 0)] ==
                        Object::Gas) {
                        check2 = 1;
                    } else
                        check2 = 0;
                } else if (facenow == Face::down) {
                    check2 = 1;
                } else if (facenow == Face::up) {
                    check2 = 0;
                } else if ((facenow == Face::left) && (check1 == 0) &&
                           (check3 == 0)) {
                    check2 = 2;
                } else {
                    check2 = 1;
                }
                break;
        }
        // std::cout << "checkpoint 408" << std::endl;
        // added default case
        // also, since check2 can only be 0, 1, 2
        // the last else if was changed to else to stop the compiler complaining
        switch (command) {
            case 'q':
                return "qq";
            case 'a':
                if (check2 == 2) {
                    return "aw";
                } else if (check2 == 1) {
                    return "ad";
                } else {
                    return "aa";
                }
                break;
            case 'd':
                if (check2 == 2) {
                    return "dw";
                } else if (check2 == 1) {
                    return "dd";
                } else {
                    return "da";
                }
                break;
            // anything other than a d q will be treated as w (forward)
            default:
                if (check2 == 2) {
                    return "ww";
                } else if (check2 == 1) {
                    return "wd";
                } else {
                    return "wa";
                }
                break;
        }
    }
}

char firstAI(Game game) {
    int thexnow = game.Tanks_p[0]->getx();
    int theynow = game.Tanks_p[0]->gety();
    Face facenow = game.Tanks_p[0]->getFace();
    // int thexenem = game.Tanks_p[1]->getx();
    // int theyenem = game.Tanks_p[1]->gety();
    // Face faceenem = game.Tanks_p[1]->getFace();
    int check1;
    bool danger = false;
    // int xi, yi;
    // std::cout << "checkpoint 471" << std::endl;
    for (int i = std::max(0, thexnow - 1);
         i < std::min(FIELD_SIZE_DISPLAY, thexnow + 2); i++) {
        for (int j = std::max(0, theynow - 1);
             j < std::min(FIELD_SIZE_DISPLAY, theynow + 2); j++) {
            if (game.field[i][j] == Object::Gas) {
                danger = true;
                break;
            }
        }
        if (danger) break;
    }
    // std::cout << "checkpoint 483" << std::endl;
    if (danger) {
        if ((facenow == Face::down &&
             game.field[thexnow][std::min(FIELD_SIZE_DISPLAY, theynow + 1)] ==
                 Object::Gas) ||
            (facenow == Face::left &&
             game.field[std::max(0, thexnow - 1)][theynow] == Object::Gas) ||
            (facenow == Face::right &&
             game.field[std::min(FIELD_SIZE_DISPLAY, theynow + 1)][theynow] ==
                 Object::Gas) ||
            (facenow == Face::up &&
             game.field[thexnow][std::max(0, thexnow - 1)] == Object::Gas)) {
            check1 = 1;
        } else
            check1 = 0;
    }

    // std::cout << "checkpoint 498" << std::endl;
    if (check1) {
        for (size_t i = 0; i < game.Bullets_p.size(); i++) {
            // I don't quite get this, but Tanks_p[i] is out-of-boundary access
            // I changed it to Tanks_p[0] and Tanks_p[1] for now
            // please modify it if this doesn't serve your purpose :)
            if ((game.Bullets_p[i]->probex(2) == game.Tanks_p[0]->probex(1)) &&
                (game.Bullets_p[i]->probey(2) == game.Tanks_p[1]->probey(1)) &&
                (game.Tanks_p[1]->probey(1) == game.Tanks_p[0]->probey(1)) &&
                (game.Tanks_p[1]->probex(1) == game.Tanks_p[0]->probex(1))) {
                return 'a';
            }
        }
        return 'd';
    } else
        return 'w';
}

string AI(Game game, char cmd) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    // std::cout
    //     << "you just need to put in one character and then the ai will deal "
    //        "with the situation and try to beat you!"
    //     << std::endl;
    // std::cout
    //     << "and you need to win through the hard game! enjoy your game! ps. "
    //        "the main has no errors, but the ai is fragile. don't test the "
    //        "stability of ai(or it is satisfied in the main maybe)"
    //     << std::endl;
    if (game.Tanks_p[0]->gethp() > game.Tanks_p[1]->gethp()) {
        // std::cout << "Attacking" << std::endl;
        return attackingAI(game, cmd);
    } else {
        // std::cout << "Defending" << std::endl;
        return defendingAI(game, cmd);
    }
}
*/
