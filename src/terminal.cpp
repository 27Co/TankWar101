#include "terminal.h"

/**
 * @brief: handle command line arguments
 */
int set_opts(int& initialHP, int& mode, std::string& fileName, int argc,
             char** argv) {
    int opt;
    while ((opt = getopt_long(argc, argv, "hl:m:p:", longOptions, nullptr)) !=
           -1) {
        switch (opt) {
            case 'h': {
                print_help(argv[0]);
                return -1;
            }
            case 'l': {
                std::ofstream fcheck(optarg);
                if (fcheck.is_open()) {
                    fcheck.close();
                    fileName = optarg;
                    std::cout << "Log file was set to " << fileName
                              << std::endl;
                } else {
                    std::cout << "Invalid file name. Log file is"
                                 "tankwar.log by default"
                              << std::endl;
                }
                break;
            }
            case 'm': {
                if (strcmp(optarg, "PVP") == 0) {
                    std::cout << "You have chosen PVP mode" << std::endl;
                    mode = 0;
                } else if (strcmp(optarg, "PVE") == 0) {
                    std::cout << "You have chosen PVE mode" << std::endl;
                    mode = 1;
                } else if (strcmp(optarg, "DEMO") == 0) {
                    std::cout << "You have chosen DEMO mode" << std::endl;
                    mode = 2;
                } else {
                    std::cout << "Invalid mode name, game mode was set to PVP "
                                 "by default"
                              << std::endl;
                }
                break;
            }
            case 'p': {
                if (optarg[0] >= '1' && optarg[0] <= '9') {
                    initialHP = std::stoi(optarg);
                    std::cout << "Initial hp successfully set to " << initialHP
                              << std::endl;
                } else {
                    std::cout << "Initial hp setup failed: hp should be a "
                                 "positive integer"
                              << std::endl;
                    std::cout << "Initial hp was set to 5 by defult"
                              << std::endl;
                }
                break;
            }
            // case '?':
            //     std::cout << "Error optopt: " << optopt << std::endl;
            //     std::cout << "Error opterr: " << opterr << std::endl;
            //     break;
            default:
                break;
        }
    }
    return 0;
}

/**
 * @brief: print the help message
 * @param arg0: the first argument from command line
 */
void print_help(const std::string& arg0) {
    std::cout << "Usage: " << arg0 << " [options](optional)" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "-h | --help" << std::endl;
    std::cout << "      Print this help message and exit." << std::endl;
    std::cout << "-l <file> | --log-file <file>" << std::endl;
    std::cout << "      Log the game process to a file. (Default: tankwar.log)"
              << std::endl;
    std::cout << "-m <mode> | --mode=<mode>" << std::endl;
    std::cout << "      Specify the game mode (PVP/PVE/DEMO). (Default: PVP)"
              << std::endl;
    std::cout << "-p <point> | --initial-life=<point>" << std::endl;
    std::cout
        << "      Specify the initial life points of the tanks. (Default: 5)"
        << std::endl;
}

/**
 * @brief: print the introduction of the game
 */
int print_intro(bool isDEMO) {
    if (isDEMO) {
        std::cout << std::endl
                  << "AIs will NOT keep moving forward. Just wait and see :)"
                  << std::endl;
    } else {
        std::cout << std::endl << "Tank control:" << std::endl;
        std::cout << "In each turn, a tank can move left (a), right "
                     "(d) or forward (w)"
                  << std::endl;
        std::cout << "You can exit the game by entering q" << std::endl;
        std::cout << "Any other input will be treated as w (forward)"
                  << std::endl;
    }

    char temp;
    std::cout << std::endl;
    std::cout << "Enter q to quit, any other character to continue: ";
    std::cin >> temp;
    if (temp == 'q') {
        return 1;
    }
    return 0;
}

/**
 * @brief: main loop of the game
 * @param game: ongoing game
 * @param fout: handler of the log file
 * @param mode: game mode (0 for PVP, 1 for PVE, 2 for DEMO)
 * @return 0 if the game continues; 1 if the game ends; -1 if players quit
 */
int loop(Game& game, std::ofstream& fout, int mode) {
    std::cout << std::endl << "Round " << game.round << std::endl;
    fout << "Round " << game.round << std::endl;

    update_field(game.field, get_XYZ(game.Tanks_p), get_XYZ(game.Bullets_p),
                 game.round);
    fout << "Field updated" << std::endl;

    print_field(game.field, get_XYZ(game.Bullets_p), get_XYZ(game.Tanks_p));
    fout << "Field printed" << std::endl;
    tank_info(game.Tanks_p, fout);
    std::cout << std::endl;

    std::string directions = get_directions(game, mode);
    fout << "Input directions: " << directions << std::endl;

    if (turn_all(game, directions, fout) != 0) {
        return -1;
    }

    if (game.round % 3 == 1) {
        shot_all(game.Tanks_p, game.Bullets_p);
        fout << "Since this is round " << game.round << " all tanks made a shot"
             << std::endl;
    }

    if (check_collision(game.Tanks_p, game.Bullets_p, fout) ||
        check_gas(game.Tanks_p, game.round, fout)) {
        move_all(game.Tanks_p, game.Bullets_p);
        update_field(game.field, get_XYZ(game.Tanks_p), get_XYZ(game.Bullets_p),
                     game.round);
        fout << "Game ends" << std::endl;
        return 1;
    }

    move_all(game.Tanks_p, game.Bullets_p);
    fout << "All tanks and bullets moved" << std::endl;
    fout << "Heading for next round..." << std::endl << std::endl;

    game.round++;

    return 0;
}

/**
 * @brief: get the input directions from cin or AI
 * @param game: ongoing game
 * @param mode: game mode (0 for PVP, 1 for PVE, 2 for DEMO)
 * @return: the input directions
 */
std::string get_directions(Game game, int mode) {
    std::string directions = "ss";
    switch (mode) {
        case 0:
            for (int i = 0; i < TANK_NUM; i++) {
                std::cout << "Enter a direction for " << tankColors[i]
                          << "tank " << game.Tanks_p[i]->getid() << NOCOLOR
                          << std::endl;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(),
                                '\n');
                std::cin >> directions[i];
            }
            break;
        case 1:
            std::cout << "Enter a direction for Your " << tankColors[0]
                      << "tank" << NOCOLOR << std::endl;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin >> directions[0];
            // directions = AI(game, directions[0]);
            directions[1] = oneAI(game, 1);
            std::cout << "AI's choice: " << std::endl
                      << directions[1] << std::endl;
            break;
        case 2:
            // directions[0] = firstAI(game);
            // directions = AI(game, directions[0]);
            for (int i = 0; i < TANK_NUM; i++) {
                std::cout << "Enter a direction for " << tankColors[i]
                          << "tank " << game.Tanks_p[i]->getid() << NOCOLOR
                          << std::endl;
                directions[i] = oneAI(game, i, i % 2 == 0);
                std::cout << directions[i] << std::endl;
            }
            break;
        default:
            break;
    }
    return directions;
}

/**
 * @brief: turn the tanks according to directions
 * @param game: ongoing game
 * @param directions: directions to turn
 * @param fout: handler of the log file
 * @return: the input directions
 */
int turn_all(Game game, std::string directions, std::ofstream& fout) {
    for (int i = 0; i < TANK_NUM; i++) {
        switch (directions[i]) {
            case 'a':
                game.Tanks_p[i]->Entity::turn(Direction::t_left);
                fout << "Tank " << game.Tanks_p[1]->getid() << " turned left"
                     << std::endl;
                break;
            case 'w':
                game.Tanks_p[i]->Entity::turn(Direction::forward);
                fout << "Tank " << game.Tanks_p[1]->getid() << " didn't turn"
                     << std::endl;
                break;
            case 'd':
                game.Tanks_p[i]->Entity::turn(Direction::t_right);
                fout << "Tank " << game.Tanks_p[1]->getid() << " turned right"
                     << std::endl;
                break;
            case 'q':
                fout << "Player exited the game" << std::endl;
                return -1;

            default:
                break;
        }
    }
    return 0;
}
