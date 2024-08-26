#ifndef FIELD_H
#define FIELD_H

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <vector>

#include "Entity.h"
#include "Global.h"

// first four for different faces, last three for Bulletn, Gas, Empty
const std::array<char, 7> icons = {'V', '>', 'A', '<', '*', '~', ' '};

void init_field(Field& field);

void print_field(Field& field, const EntityStatus& BulletsXYZ,
                 const EntityStatus& TanksXYZ, IDs OopsID = {{0}});

// bool isTank(Field field, int x, int y, const EntityStatus& tanksXYZ);
void colorTank(Field& field, int x, int y, const EntityStatus& tanksXYZ,
               IDs winnerID);

void colorBullet(int x, int y, const EntityStatus& BulletXYZ);

void update_field(Field& field, const EntityStatus& tanksXYZ,
                  const EntityStatus& bulletsXYZ, int round);

#endif  // FIELD_H