#include <iostream>

#include "Field.h"
#include "terminal.h"

int main(int argc, char* argv[]) {
    int initialHP = 5;
    int mode = 0;
    std::string fileName = "tankwar.log";
    if (set_opts(mode, fileName, argc, argv) != 0) {
        return 0;
    }

    std::ofstream fout;
    fout.open(fileName);

    new_buffer();
    clear_screen();
    struct termios tty, ttyTmp;
    tcgetattr(STDIN_FILENO, &tty);
    ttyTmp = tty;
    ttyTmp.c_lflag &= static_cast<unsigned int>(~(ICANON | ECHO));
    tcsetattr(STDIN_FILENO, TCSANOW, &ttyTmp);

    if (print_intro() == 1) {
        restore_buffer();
        tcsetattr(STDIN_FILENO, TCSANOW, &tty);
        return 0;
    }

    Game game;
    init_field(game.field);
    game.Tanks_p = init_Tanks(initialHP);
    std::cout << std::endl << "TankWar started ";
    std::cout << "(Game mode: " << modeName[static_cast<size_t>(mode)] << ", ";
    std::cout << "Initial hp: " << initialHP << ")" << std::endl;
    fout << "TankWar started ";
    fout << "(Game mode: " << modeName[static_cast<size_t>(mode)] << ", ";
    fout << "Initial hp: " << initialHP << ")" << std::endl;

    int status;
    while ((status = loop(game, fout, mode)) == 0);

    if (status == 1) {
        clear_screen();
        IDs winnerID = get_winnerID(game.Tanks_p);
        fout << "get_winnerID done" << std::endl;
        IDs OopsID = get_OopsID(game.Tanks_p, winnerID);
        fout << "get_OopsID done" << std::endl;
        print_field(game.field, get_XYFI(game.Bullets_p),
                    get_XYFI(game.Tanks_p), OopsID);
        fout << "endgame field printed" << std::endl;
        print_winner(winnerID, mode);
        fout << "winners announced" << std::endl;
    } else {
        std::cout << "Exited Game" << std::endl;
        fout << "Exited Game" << std::endl;
    }

    delete_bullets(game.Bullets_p);
    fout << "bullets deleted" << std::endl;
    delete_tanks(game.Tanks_p);
    fout << "tanks deleted" << std::endl;
    fout.close();
    std::cout << "Done. See you next time!" << std::endl;
    std::cout << "Press any key to quit" << std::flush;
    status = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &tty);
    restore_buffer();

    return 0;
}
