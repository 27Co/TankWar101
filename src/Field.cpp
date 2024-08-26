#include "Field.h"

/**
 * @brief: initialize the field to Empty
 * @param field: field to be initialize
 */
void init_field(Field& field) {
    std::fill(field.begin()->end(), (field.end() - 1)->end(), Object::Empty);
}

/**
 * @brief: update the field with tanks and bullets
 * @param tanksXYZ: vector containing (x, y) coordinates of tanks
 * @param bulletsXYZ: vector containing (x, y) coordinates of bullets
 * @param round: current round number
 */
void update_field(Field& field, const EntityStatus& tanksXYZ,
                  const EntityStatus& bulletsXYZ, int round) {
    int gasWidth = INITIAL_GAS_WIDTH + round / SHRINK_RATE;
    for (int x = 0; x < FIELD_SIZE_DISPLAY; x++) {
        for (int y = 0; y < FIELD_SIZE_DISPLAY; y++) {
            if (x >= gasWidth && x < FIELD_SIZE_DISPLAY - gasWidth &&
                y >= gasWidth && y < FIELD_SIZE_DISPLAY - gasWidth) {
                field[x][y] = Object::Empty;
            } else {
                field[x][y] = Object::Gas;
            }
        }
    }

    for (auto xyz : tanksXYZ) {
        field[xyz[0]][xyz[1]] = static_cast<Object>(xyz[2]);
    }
    for (auto xyz : bulletsXYZ) {
        field[xyz[0]][xyz[1]] = Object::Bulletn;
    }
}

/**
 * @brief: print the field
 * @param field: field to be printed
 * @param tanksXYZ: a vector containing (x, y) coordinates of tanks
 * @param OopsID: ids of those who didn't win
 */
void print_field(Field& field, const EntityStatus& BulletsXYZ,
                 const EntityStatus& TanksXYZ, IDs OopsID) {
    std::cout << std::endl << "  ";
    for (int y = 0; y < FIELD_SIZE_DISPLAY; y++) {
        std::cout << std::setw(2) << static_cast<char>(y + 'a');
    }
    std::cout << std::endl;
    for (int x = 0; x < FIELD_SIZE_DISPLAY; x++) {
        std::cout << std::setw(2) << x;
        for (int y = 0; y < FIELD_SIZE_DISPLAY; y++) {
            std::cout << " ";
            colorTank(field, x, y, TanksXYZ, OopsID);
            colorBullet(x, y, BulletsXYZ);
            std::cout << icons[static_cast<int>(field[x][y])] << NOCOLOR;
        }
        std::cout << " " << std::endl;
    }
}

/**
 * @brief: check if (x, y) is a tank and color it
 * @param field: field
 * @param x, y: position to be checked
 * @param tanksXYZ: tank positions
 * @param OopsID: ids of those who didn't win
 */
void colorTank(Field& field, int x, int y, const EntityStatus& TanksXYZ,
               IDs OopsID) {
    for (auto tankXYZ : TanksXYZ) {
        if (x == tankXYZ[0] && y == tankXYZ[1]) {
            bool Oops = false;
            for (auto id : OopsID) {
                if (tankXYZ[3] == id) {
                    Oops = true;
                    break;
                }
            }
            if (Oops) {
                std::cout << tankColors[4];
                field[x][y] = Object::Empty;
            } else {
                std::cout << tankColors[tankXYZ[3] - 1];
                if (field[x][y] != static_cast<Object>(tankXYZ[2])) {
                    std::cout << tankColors[4];
                    field[x][y] = Object::Empty;
                } else {
                    std::cout << tankColors[tankXYZ[3] - 1];
                    field[x][y] = static_cast<Object>(tankXYZ[2]);
                }
            }
        }
    }
    return;
}

/**
 * @brief: color the bullets
 */
void colorBullet(int x, int y, const EntityStatus& BulletXYZ) {
    for (auto bulletXYZ : BulletXYZ) {
        if (x == bulletXYZ[0] && y == bulletXYZ[1]) {
            std::cout << bulletColors[bulletXYZ[3] - 1];
        }
    }
    return;
}
