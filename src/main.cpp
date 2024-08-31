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
    if (print_intro(mode == 2) == 1) {
        std::cout << "\033[?1049l" << std::flush;
        return 0;
    }

    Game game;
    init_field(game.field);
    game.Tanks_p = init_Tanks(initialHP);
    std::cout << std::endl << "TankWar started ";
    std::cout << "(Game mode: " << modeName[mode] << ", ";
    std::cout << "Initial hp: " << initialHP << ")" << std::endl;
    fout << "TankWar started ";
    fout << "(Game mode: " << modeName[mode] << ", ";
    fout << "Initial hp: " << initialHP << ")" << std::endl;

    int status;
    while ((status = loop(game, fout, mode)) == 0);

    if (status == 1) {
        IDs winnerID = get_winnerID(game.Tanks_p);
        fout << "get_winnerID done" << std::endl;
        IDs OopsID = get_OopsID(game.Tanks_p, winnerID);
        fout << "get_OopsID done" << std::endl;
        print_field(game.field, get_XYZ(game.Bullets_p), get_XYZ(game.Tanks_p),
                    OopsID);
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
    std::cout << "Exit in 3 second" << std::flush;
    std::this_thread::sleep_for(std::chrono::seconds(3));
    restore_buffer();

    return 0;
}
