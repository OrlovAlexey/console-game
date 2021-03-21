#pragma once

#include <string>

class Unit {
protected:
    int power;
    int shield;
    std::string name;
    std::string country;
    int hp;
public:
    int get_hp() const {
        return hp;
    }

    int get_shield() const {
        return shield;
    }

    int healing(int heal) {
        hp += heal;
    }

    int attack() const {
        return power;
    }

    void take_damage(int d) {
        if (shield == 0) {
            hp -= d;
            return;
        }
        if (shield - d <= 0) {
            shield = 0;
        } else {
            shield -= d;
        }
    }
};
