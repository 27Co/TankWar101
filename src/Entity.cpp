#include "Entity.h"

Entity::Entity(int x0, int y0, Face face0) : x(x0), y(y0), face(face0) {}

Entity::~Entity() = default;

void Entity::info() const {
    std::cout << "Location: " << x << ", " << y;
    std::cout << " Facing " << faceName[static_cast<int>(face)] << std::endl;
}

int Entity::getx() const { return this->x; }

int Entity::gety() const { return this->y; }

/**
 * @brief: change the coordinates of the tank/bullet according to its face
 * @param speed: number of tile to move per round
 */
void Entity::move(int step) {
    this->x += dx[static_cast<int>(face)] * step;
    this->y += dy[static_cast<int>(face)] * step;
}

/**
 * @brief: predict the entity's position
 * @param step: after "moving" how many steps
 * @param direction: in this direction
 * @return x coordinate after "moving"
 */
int Entity::probex(int step, Direction direction) const {
    int newFace_int =
        static_cast<int>(this->getFace()) + static_cast<int>(direction) - 1;
    Face newFace_temp = static_cast<Face>(
        (newFace_int < 0) ? (newFace_int + 4) : (newFace_int % 4));
    return this->x + dx[static_cast<int>(newFace_temp)] * step;
};

/**
 * @brief: predict the entity's position
 * @param step: after "moving" how many steps
 * @param direction: in this direction
 * @return y coordinate after "moving"
 */
int Entity::probey(int step, Direction direction) const {
    int newFace_int =
        static_cast<int>(this->getFace()) + static_cast<int>(direction) - 1;
    Face newFace_temp = static_cast<Face>(
        (newFace_int < 0) ? (newFace_int + 4) : (newFace_int % 4));
    return this->y + dy[static_cast<int>(newFace_temp)] * step;
};
// int Entity::probey(int step) const {
//     return this->y + dy[static_cast<int>(this->face)] * step;
// };

Face Entity::getFace() const { return this->face; }

/**
 * @brief: modify the face of an entity (should be tank)
 * @param direction: the direction to turn (t_left, forward, t_right)
 */
void Entity::turn(Direction direction) {
    // std::cout << "before: " << this->face << std::endl;
    int newFace_int =
        static_cast<int>(this->getFace()) + static_cast<int>(direction) - 1;
    this->face = static_cast<Face>((newFace_int < 0) ? (newFace_int + 4)
                                                     : (newFace_int % 4));
    // std::cout << "after: " << this->face << std::endl;
}

/**
 * @brief check if the entity is still in the display area
 * @param step: after moving this number of tiles
 */
bool Entity::inField(int step) const {
    if (this->probex(step) < 0 || this->probex(step) >= FIELD_SIZE_DISPLAY ||
        this->probey(step) < 0 || this->probey(step) >= FIELD_SIZE_DISPLAY) {
        return false;
    }
    return true;
}

int Tank::ID = 0;

/**
 * @brief: constructor for class Tank, keep track of the tank ID
 * @param x0, y0, face0, hp0: initial status of the tank
 */
Tank::Tank(int x0, int y0, Face face0, int hp0)
    : Entity(x0, y0, face0), hp(hp0) {
    ID++;
    this->id = ID;
}

Tank::~Tank() = default;

/**
 * @brief: deduct the hp of a tank
 * @param dhp: hp to be deducted
 */
void Tank::deduct(int dhp) {
    std::cout << tankColors[this->id - 1] << "Tank " << this->id << NOCOLOR
              << " hp: " << this->hp;
    this->hp = (this->hp - dhp >= 0) ? this->hp - dhp : 0;
    std::cout << " -> " << this->hp << std::endl;
}

int Tank::gethp() const { return this->hp; }

int Tank::getid() const { return this->id; }

/**
 * @brief: shot a bullet
 * @return a pointer to the newly created Bullet object
 */
Bullet* Tank::shot() {
    auto bullet_p = new Bullet(this->probex(2), this->probey(2),
                               this->getFace(), this->getid());
    return bullet_p;
}

/**
 * @brief: constructor for class Bullet
 * @param x0, y0, face0: initial status of the bullet
 * @param id: id of the tank that generate this bullet
 */
Bullet::Bullet(int x0, int y0, Face face0, int id)
    : Entity(x0, y0, face0), id(id) {}

Bullet::~Bullet() = default;

int Bullet::getid() const { return this->id; }

/**
 * @brief: free the memory allocated for bullets
 * @param Bullets_p: vector containing pointers to all bullets
 */
void delete_bullets(std::vector<Bullet*>& Bullets_p) {
    for (auto& bullet_p : Bullets_p) {
        delete bullet_p;
        bullet_p = nullptr;
    }
    Bullets_p.clear();
}

/**
 * @brief: free the memory allocated for bullets
 * @param Tanks_p: vector containing all pointers to bullets
 */
void delete_tanks(std::vector<Tank*>& Tanks_p) {
    for (auto& tank_p : Tanks_p) {
        delete tank_p;
        tank_p = nullptr;
    }
    Tanks_p.clear();
}

/**
 * @brief: call move() on all tanks and bullets
 * @param Tanks_p, Bullets_p: vectors containing pointers to tanks and bullets
 */
void move_all(const std::vector<Tank*>& Tanks_p,
              std::vector<Bullet*>& Bullets_p) {
    // std::cout << "the beginning of move_all" << std::endl;
    for (auto tank_p : Tanks_p) {
        if (tank_p->Entity::inField(1)) {
            tank_p->Entity::move(1);
        }
    }
    // std::cout << "in move_all: after first for loop" << std::endl;
    for (auto& bullet_p : Bullets_p) {
        if (bullet_p->Entity::inField(2)) {
            // std::cout << "is in field" << std::endl;
            bullet_p->Entity::move(2);
        } else {
            delete bullet_p;
            bullet_p = nullptr;
        }
    }

    for (size_t i = 0, size = Bullets_p.size(); i < size; i++) {
        auto it = std::find(Bullets_p.begin(), Bullets_p.end(), nullptr);
        if (it != Bullets_p.end()) {
            // std::cout << "removed" << std::endl;
            Bullets_p.erase(it);
        }
    }
}

/**
 * @brief: call shot() on all tanks
 * @param Tanks_p, Bullets_p: vectors containing pointers to tanks and bullets
 */
void shot_all(const std::vector<Tank*>& Tanks_p,
              std::vector<Bullet*>& Bullets_p) {
    // std::cout << "check right before shot_all: " << Bullets_p.size()
    //           << std::endl;
    for (auto tank_p : Tanks_p) {
        if (tank_p->Entity::inField(2)) {
            Bullets_p.push_back(tank_p->shot());
        }
    }
}

/**
 * @brief: deduct hp due to collisions (tank&tank and Tank&bullet)
 * @param Tanks_p, Bullets_p: vectors containing pointers to tanks and bullets
 * @param fout: log file handler
 * @return true if a tank is destroyed by collision
 */
bool check_collision(std::vector<Tank*>& Tanks_p,
                     std::vector<Bullet*>& Bullets_p, std::ofstream& fout) {
    // return true if the tanks collide, end of game
    for (size_t i = 0; i < TANK_NUM; i++) {
        for (size_t j = i + 1; j < TANK_NUM; j++) {
            bool oneStep_t =
                Tanks_p[i]->Entity::probex() == Tanks_p[j]->Entity::getx() &&
                Tanks_p[i]->Entity::probey() == Tanks_p[j]->Entity::gety() &&
                Tanks_p[j]->Entity::probex() == Tanks_p[i]->Entity::getx() &&
                Tanks_p[j]->Entity::probey() == Tanks_p[i]->Entity::gety();
            bool twoStep_t =
                Tanks_p[i]->Entity::probex() == Tanks_p[j]->Entity::probex() &&
                Tanks_p[i]->Entity::probey() == Tanks_p[j]->Entity::probey();
            if (oneStep_t || twoStep_t) {
                std::cout << "collision between tanks: " << std::endl;
                std::cout << "Tank " << Tanks_p[i]->getid() << " with hp "
                          << Tanks_p[i]->gethp();
                std::cout << " vs. Tank " << Tanks_p[j]->getid() << " with hp "
                          << Tanks_p[j]->gethp();
                std::cout << std::endl;
                fout << "Tank " << Tanks_p[i]->getid() << " with hp "
                     << Tanks_p[i]->gethp();
                fout << " crashed with Tank " << Tanks_p[j]->getid()
                     << " with hp " << Tanks_p[j]->gethp();
                fout << std::endl;
                return true;
            }
        }
    }
    for (auto& bullet_p : Bullets_p) {
        for (auto tank_p : Tanks_p) {
            bool oneStep_b =
                bullet_p->Entity::probex() == tank_p->Entity::getx() &&
                bullet_p->Entity::probey() == tank_p->Entity::gety() &&
                tank_p->Entity::probex() == bullet_p->Entity::getx() &&
                tank_p->Entity::probey() == bullet_p->Entity::gety();
            bool twoStep_b =
                bullet_p->Entity::probex() == tank_p->Entity::probex() &&
                bullet_p->Entity::probey() == tank_p->Entity::probey();
            bool threeStep_b =
                bullet_p->Entity::probex(2) == tank_p->Entity::probex() &&
                bullet_p->Entity::probey(2) == tank_p->Entity::probey();
            if (oneStep_b || twoStep_b || threeStep_b) {
                // std::cout << "tank hit by bullet" << std::endl;
                fout << "tank hit by bullet, hp -2" << std::endl;
                tank_p->deduct(2);
                delete bullet_p;
                bullet_p = nullptr;
                break;
            }
        }
    }

    for (size_t i = 0, n = Bullets_p.size(); i < n; i++) {
        auto it = std::find(Bullets_p.begin(), Bullets_p.end(), nullptr);
        if (it != Bullets_p.end()) {
            Bullets_p.erase(it);
        }
    }

    for (auto tank_p : Tanks_p) {
        if (tank_p->gethp() <= 0) {
            return true;
        }
    }

    return false;
}

/**
 * @brief: deduct hp due to gas
 * @param Tanks_p: vectors containing pointers to tanks and bullets
 * @param round: current round of the game
 * @param fout: log file handler
 * @return true if a tank is destroyed by the gas
 */
bool check_gas(const std::vector<Tank*>& Tanks_p, int round,
               std::ofstream& fout) {
    int gasWidth = INITIAL_GAS_WIDTH + round / SHRINK_RATE;
    bool tf = false;
    for (auto& tank_p : Tanks_p) {
        // current: check before move
        // if after, use probe()
        if (tank_p->Entity::getx() < gasWidth ||
            tank_p->Entity::getx() >= FIELD_SIZE_DISPLAY - gasWidth ||
            tank_p->Entity::gety() < gasWidth ||
            tank_p->Entity::gety() >= FIELD_SIZE_DISPLAY - gasWidth) {
            tank_p->deduct(1);
            fout << "Tank " << tank_p->getid() << " is outside safezone, hp -1"
                 << std::endl;
            if (tank_p->gethp() <= 0) {
                tf = true;
            }
        }
    }
    return tf;
}

/**
 * @brief: initialize vector containing tank pointers
 * @param initialHP: initialHP of each tank
 * @param num: number of tanks to create
 * @return a vector containing pointers to all tanks
 */
std::vector<Tank*> init_Tanks(int initialHP, int num) {
    auto tank1_p = new Tank(FIELD_SIZE_DISPLAY - INITIAL_GAS_WIDTH - 1,
                            FIELD_SIZE_DISPLAY / 2, Face::up, initialHP);
    auto tank2_p = new Tank(INITIAL_GAS_WIDTH, FIELD_SIZE_DISPLAY / 2 - 1,
                            Face::down, initialHP);
    std::vector<Tank*> Tanks_p = {tank1_p, tank2_p};
    if (num == 4) {
        auto tank3_p = new Tank(FIELD_SIZE_DISPLAY / 2 - 1, INITIAL_GAS_WIDTH,
                                Face::right, initialHP);
        auto tank4_p = new Tank(FIELD_SIZE_DISPLAY / 2,
                                FIELD_SIZE_DISPLAY - INITIAL_GAS_WIDTH - 1,
                                Face::left, initialHP);
        Tanks_p.push_back(tank3_p);
        Tanks_p.push_back(tank4_p);
    }
    return Tanks_p;
}

/**
 * @brief: print tank info
 * @param Tanks_p: vector containing pointers to all tanks
 * @param fout: log file handler
 */
void tank_info(const std::vector<Tank*>& Tanks_p, std::ofstream& fout) {
    for (auto tank_p : Tanks_p) {
        std::cout << tankColors[tank_p->getid() - 1] << "Tank "
                  << tank_p->getid() << NOCOLOR << " hp: " << tank_p->gethp();
        fout << "Tank " << tank_p->getid() << " hp: " << tank_p->gethp();
        std::cout << " facing " << faceName[static_cast<int>(tank_p->getFace())]
                  << std::endl;
        fout << " facing " << faceName[static_cast<int>(tank_p->getFace())]
             << std::endl;
    }
}

/**
 * @brief: get the status of the entity (tank)
 * @param Entities_p: vector containing pointers to entites (tanks)
 * @return: a vector containing (x, y) coordinates
 */
EntityStatus get_XYZ(const std::vector<Tank*>& Entities_p) {
    EntityStatus XYZ;
    for (auto p : Entities_p) {
        std::array<int, 4> tempXYZ = {
            {p->getx(), p->gety(), static_cast<int>(p->getFace()), p->getid()}};
        XYZ.push_back(tempXYZ);
    }
    return XYZ;
}

/**
 * @brief: get the status of the entity (bullet)
 * @param Entities_p: vector containing pointers to entites (bullets)
 * @return: a vector containing (x, y) coordinates
 */
EntityStatus get_XYZ(const std::vector<Bullet*>& Entities_p) {
    EntityStatus XYZ;
    for (auto p : Entities_p) {
        std::array<int, 4> tempXYZ = {
            {p->getx(), p->gety(), static_cast<int>(p->getFace()), p->getid()}};
        XYZ.push_back(tempXYZ);
    }
    return XYZ;
}

/**
 * @brief: get winnerIDs according to hp
 * @param Tanks_p: vector containing tank pointers
 * @return: array of winners' IDs
 */
IDs get_winnerID(const std::vector<Tank*>& Tanks_p) {
    IDs winnerID = {{0}};
    int winnerHP = 0;
    int winnerNum = 0;
    for (auto tank_p : Tanks_p) {
        if (tank_p->gethp() > winnerHP) {
            winnerHP = tank_p->gethp();
            for (int i = 0; i < winnerNum; i++) {
                winnerID[i] = 0;
            }
            winnerNum = 0;
            winnerID[winnerNum] = tank_p->getid();
            winnerNum++;
        } else if (tank_p->gethp() == winnerHP) {
            winnerID[winnerNum] = tank_p->getid();
            winnerNum++;
        }
    }
    return winnerID;
}

/**
 * @brief: get the IDs of the tank who didn't win
 * @param winnerID: IDs of winners
 * @return: array of ids
 */
IDs get_OopsID(const std::vector<Tank*>& Tanks_p, const IDs& winnerID) {
    IDs OopsID = {{0}};
    for (size_t i = 0; i < TANK_NUM; i++) {
        bool isWinner = false;
        for (auto id : winnerID) {
            if (Tanks_p[i]->getid() == id) {
                isWinner = true;
            }
        }
        if (!isWinner) {
            OopsID[i] = Tanks_p[i]->getid();
        }
    }
    return OopsID;
}

/**
 * @brief: print the winner(s) for the game
 * @param winnerID: array containing winnerID(s)
 */
void print_winner(const IDs& winnerID, int mode) {
    int winnerNum = 0;
    for (int i = 0; i < TANK_NUM && winnerID[i] > 0; i++, winnerNum++);
    std::cout << std::endl;
    if (winnerNum == 1) {
        if (mode == 1) {
            if (winnerID[0] == 1) {
                std::cout << "Congratulations, you beat the AI!" << std::endl;
            } else {
                std::cout << "Oops, you are defeated by the AI... Try again!"
                          << std::endl;
            }
            return;
        }
        std::cout << "Congratulations! " << tankColors[winnerID[0] - 1]
                  << "Tank " << winnerID[0] << NOCOLOR
                  << ", you are the winner!" << std::endl;
    } else {
        std::cout << "Great game! Match ended in a draw with winners";
        for (int i = 0; i < winnerNum; i++) {
            std::cout << tankColors[winnerID[i] - 1] << " Tank " << winnerID[i]
                      << NOCOLOR;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}