#ifndef FIELD_H
#define FIELD_H

#include "Global.h"

// first four for different faces, last three for Bulletn, Gas, Empty
const std::array<char, 8> icons = {'V', '>', 'A', '<', 'o', '*', '~', ' '};

void init_field(Field& field);

void print_field(Field& field, const EntityStatus& BulletsXYZ,
                 const EntityStatus& TanksXYZ, IDs OopsID = {{0}});

// bool isTank(Field field, int x, int y, const EntityStatus& tanksXYZ);
void colorTank(Field& field, size_t x, size_t y, const EntityStatus& tanksXYZ,
               IDs winnerID);

void colorBullet(size_t x, size_t y, const EntityStatus& BulletXYZ);

void update_field(Field& field, const EntityStatus& tanksXYZ,
                  const EntityStatus& bulletsXYZ, size_t round);

#endif  // FIELD_H
