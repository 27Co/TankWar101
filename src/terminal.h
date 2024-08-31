#ifndef TERMINAL_H
#define TERMINAL_H

#include <getopt.h>

#include <cstring>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

#include "AI.h"
#include "Entity.h"
#include "Field.h"
#include "Global.h"

const std::array<std::string, 3> modeName = {{"PVP", "PVE", "DEMO"}};

extern "C" {
const struct option longOptions[] = {
    {"help", no_argument, nullptr, 'h'},
    {"log-file", required_argument, nullptr, 'l'},
    {"mode", required_argument, nullptr, 'm'},
    {"initial-life", required_argument, nullptr, 'p'},
    {nullptr, 0, nullptr, 0}};

int set_opts(int& mode, std::string& fileName, int argc, char** argv);
}

void print_help(const std::string& argv);

int print_intro(bool isDEMO = false);

int loop(Game& game, std::ofstream& fout, int mode);

std::string get_directions(Game game, int mode);

int turn_all(Game game, std::string directions, std::ofstream& fout);

void new_buffer();
void restore_buffer();
void clear_screen();

#endif  // TERMINAL_H
