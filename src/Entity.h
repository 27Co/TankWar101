#ifndef ENTITY_H
#define ENTITY_H

#include <fstream>
#include <vector>

#include "Global.h"

const std::array<int, 5> dx = {1, 0, -1, 0, 0};
const std::array<int, 5> dy = {0, 1, 0, -1, 0};
class Entity {
   private:
    size_t x, y;
    Direction face;

   public:
    Entity(size_t x0, size_t y0, Direction face0);
    ~Entity();

    void info() const;
    size_t getx() const;
    size_t gety() const;
    void move(int step);
    int probex(Direction direction, int step = 1) const;
    int probey(Direction direction, int step = 1) const;
    Direction getFace() const;
    void turn(Direction direction);
    bool inField(int step = 1) const;
};

class Bullet : public Entity {
   private:
    size_t id;

   public:
    Bullet(size_t x0, size_t y0, Direction face0, size_t id);
    ~Bullet();
    size_t getid() const;
};

class Tank : public Entity {
   private:
    int hp;
    size_t id;
    static size_t ID;

   public:
    Tank(size_t x0, size_t y0, Direction face0, int hp0);
    ~Tank();
    void deduct(int dhp);
    int gethp() const;
    size_t getid() const;
    Bullet* shot();
};

using Game = struct _Game {
    Field field;
    size_t round = 1;
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

bool check_gas(const std::vector<Tank*>& Tanks_p, size_t round,
               std::ofstream& fout);

std::vector<Tank*> init_Tanks(int initialHP = 5, size_t num = TANK_NUM);

void tank_info(const std::vector<Tank*>& Tanks_p, std::ofstream& fout);

EntityStatus get_XYZ(const std::vector<Tank*>& Entities_p);

EntityStatus get_XYZ(const std::vector<Bullet*>& Entities_p);

IDs get_winnerID(const std::vector<Tank*>& Tanks_p);

void print_winner(const IDs& winnerID, int mode);

IDs get_OopsID(const std::vector<Tank*>& Tanks_p, const IDs& winnerID);

#endif  // ENTITY_H
