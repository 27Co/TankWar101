#ifndef GLOBAL_H
#define GLOBAL_H

#include <array>
#include <string>
#include <vector>

// the first two parameters should differ by a multiply of two
constexpr int FIELD_SIZE_DISPLAY = 26;  // size of the whole field
constexpr int INITIAL_SAFE_ZONE = 20;   // size of initial safe zone (no gas)
constexpr int SHRINK_RATE = 16;   // rounds number for safeZone to shrink once
constexpr int TANK_NUM = 2;       // number of tanks
constexpr int SHOT_INTERVAL = 3;  // number of rounds between two shots

#define INITIAL_GAS_WIDTH (FIELD_SIZE_DISPLAY - INITIAL_SAFE_ZONE) / 2

const std::string NOCOLOR = "\33[0m";
const std::string RED = "\33[31m";
const std::string BRED = "\33[91m";
const std::string GREEN = "\33[32m";
const std::string YELLOW = "\33[33m";
const std::string BYELLOW = "\33[93m";
const std::string BLUE = "\33[34m";
const std::string BBLUE = "\33[94m";
const std::string CYAN = "\33[36m";
const std::string REDB = "\33[41m";
const std::string WHITEB = "\33[47m";

const std::array<std::string, 5> tankColors = {
    {BBLUE, BYELLOW, BRED, CYAN, REDB}};

const std::array<std::string, 4> bulletColors = {{BLUE, YELLOW, BRED, CYAN}};

enum class Face { down, right, up, left };

const std::array<std::string, 4> faceName = {"down", "right", "up", "left"};

enum class Direction { t_right, forward, t_left };

using Object = enum class _object {
    Down,
    Right,
    Up,
    Left,
    Bulletn,
    Gas,
    Empty
};

using Field =
    std::array<std::array<Object, FIELD_SIZE_DISPLAY>, FIELD_SIZE_DISPLAY>;

using EntityStatus = std::vector<std::array<int, 4>>;

using IDs = std::array<int, TANK_NUM>;

#endif  // GLOBAL_H
