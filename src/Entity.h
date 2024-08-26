#ifndef ENTITY_H
#define ENTITY_H

#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

#include "Global.h"

const std::array<int, 4> dx = {1, 0, -1, 0};
const std::array<int, 4> dy = {0, 1, 0, -1};
class Entity {
   private:
    int x, y;
    Face face;

   public:
    Entity(int x0, int y0, Face face0);
    ~Entity();

    void info() const;
    int getx() const;
    int gety() const;
    void move(int step);
    int probex(int step = 1, Direction direction = Direction::forward) const;
    int probey(int step = 1, Direction direction = Direction::forward) const;
    // int probey(int step = 1) const;
    Face getFace() const;
    void turn(Direction direction);
    bool inField(int step = 1) const;
};

class Bullet : public Entity {
   private:
    int id;

   public:
    Bullet(int x0, int y0, Face face0, int id);
    ~Bullet();
    int getid() const;
};

class Tank : public Entity {
   private:
    int hp, id;
    static int ID;

   public:
    Tank(int x0, int y0, Face face0, int hp0);
    ~Tank();
    void deduct(int dhp);
    int gethp() const;
    int getid() const;
    Bullet* shot();
};

using Game = struct _Game {
    Field field;
    int round = 1;
    std::vector<Tank*> Tanks_p;
    std::vector<Bullet*> Bullets_p;
};

void delete_bullets(std::vector<Bullet*>& Bullets_p);

void delete_tanks(std::vector<Tank*>& Tanks_p);

void move_all(const std::vector<Tank*>& Tanks_p,
              std::vector<Bullet*>& Bullets_p);

void shot_all(const std::vector<Tank*>& Tanks_p,
              std::vector<Bullet*>& Bullets_p);

bool check_collision(std::vector<Tank*>& Tanks_p,
                     std::vector<Bullet*>& Bullets_p, std::ofstream& fout);

bool check_gas(const std::vector<Tank*>& Tanks_p, int round,
               std::ofstream& fout);

std::vector<Tank*> init_Tanks(int initialHP = 5, int num = TANK_NUM);

void tank_info(const std::vector<Tank*>& Tanks_p, std::ofstream& fout);

EntityStatus get_XYZ(const std::vector<Tank*>& Entities_p);

EntityStatus get_XYZ(const std::vector<Bullet*>& Entities_p);

IDs get_winnerID(const std::vector<Tank*>& Tanks_p);

void print_winner(const IDs& winnerID, int mode);

IDs get_OopsID(const std::vector<Tank*>& Tanks_p, const IDs& winnerID);

#endif  // ENTITY_H
