#include "terminal.h"

#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

#include "AI.h"
#include "Field.h"

// @brief: handle command line arguments
int set_opts(int& mode, std::string& fileName, int argc, char** argv) {
    int opt;
    while ((opt = getopt_long(argc, argv, "hl:m:", longOptions, nullptr)) !=
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
                if (strcmp(optarg, "PVE") == 0) {
                    std::cout << "You have chosen PVE mode" << std::endl;
                    mode = 0;
                } else if (strcmp(optarg, "DEMO") == 0) {
                    std::cout << "You have chosen DEMO mode" << std::endl;
                    mode = 1;
                } else {
                    std::cout << "Invalid mode name, game mode was set to PVE "
                                 "by default"
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
    std::cout << "      Specify the game mode (PVE/DEMO). (Default: PVE)"
              << std::endl;
    std::cout << "-p <point> | --initial-life=<point>" << std::endl;
    std::cout
        << "      Specify the initial life points of the tanks. (Default: 5)"
        << std::endl;
}

/**
 * @brief: print the introduction of the game
 */
int print_intro() {
    std::cout << "Tank control:" << std::endl;
    std::cout << "Use w, a, s, d or arrow keys to control the tank."
              << std::endl;
    std::cout << "Press space to stay where you are for this round."
              << std::endl;
    std::cout << "Any other input will be treated as space." << std::endl;

    int temp;
    std::cout << std::endl;
    std::cout << "Enter q to quit, any other character to continue: ";
    if ((temp = getchar()) == 'q') {
        return 1;
    }
    return 0;
}

/**
 * @brief: main loop of the game
 * @param game: ongoing game
 * @param fout: handler of the log file
 * @param mode: game mode (0 for PVE, 1 for DEMO)
 * @return: 0 if the game continues; 1 if the game ends
 */
int loop(Game& game, std::ofstream& fout, int mode) {
    clear_screen();
    std::cout << "Round " << game.round << std::endl;
    fout << "Round " << game.round << std::endl;

    update_field(game.field, get_XYFI(game.Tanks_p), get_XYFI(game.Bullets_p),
                 game.round);
    fout << "Field updated" << std::endl;

    print_field(game.field, get_XYFI(game.Bullets_p), get_XYFI(game.Tanks_p));
    fout << "Field printed" << std::endl;
    tank_info(game.Tanks_p, fout);
    std::cout << std::endl;

    turn_tanks(game, mode);

    if (game.round % 3 == 1) {
        shot_all(game.Tanks_p, game.Bullets_p);
        fout << "Since this is round " << game.round << " all tanks made a shot"
             << std::endl;
    }

    if (check_collision(game.Tanks_p, game.Bullets_p, fout) ||
        check_gas(game.Tanks_p, game.round, fout)) {
        move_all(game.Tanks_p, game.Bullets_p);
        update_field(game.field, get_XYFI(game.Tanks_p),
                     get_XYFI(game.Bullets_p), game.round);
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
 * @brief: read direction from input
 * @return: direction
 */
Direction get_direction() {
    int dir = getchar();
    if (dir == 27 && getchar() == 91) {
        dir = getchar();
    }
    switch (dir) {
        case 65:
        case 'w':
            return Direction::up;
        case 66:
        case 's':
            return Direction::down;
        case 67:
        case 'd':
            return Direction::right;
        case 68:
        case 'a':
            return Direction::left;
        default:
            return Direction::none;
    }
}

/**
 * @brief: turn tanks
 * @param game: ongoing game
 * @param mode: game mode (0 for PVE, 1 for DEMO)
 */
void turn_tanks(Game& game, int mode) {
    if (mode == 0) {
        game.Tanks_p[0]->turn(get_direction());
    } else if (mode == 1) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        game.Tanks_p[0]->turn(oneAI(game, 0, false));
    }
    game.Tanks_p[1]->turn(oneAI(game, 1, true));
}

//@brief: open a new buffer
void new_buffer() { std::cout << "\033[?1049h" << std::flush; }

//@brief: restore the original buffer
void restore_buffer() { std::cout << "\033[?1049l" << std::flush; }

//@brief: clear the screen and move the cursor to the top left corner
void clear_screen() { std::cout << "\033[2J\033[1;1H" << std::flush; }
