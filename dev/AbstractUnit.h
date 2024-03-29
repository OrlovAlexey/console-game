#pragma once

#include <string>

class Unit {
protected:
    std::string name;
    int power;
    int shield;
    std::string country;
    char symbol;
    int hp;
    unsigned int recovery_time;
public:
    int get_hp() const {
        return hp;
    }

    int get_shield() const {
        return shield;
    }

    void healing(int heal) {
        hp += heal;
    }

    int attack() const {
        return power;
    }

    char get_symbol() const {
        return symbol;
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

    std::string get_name() const {
        return name;
    }

    unsigned int get_recovery_time() const {
        return recovery_time;
    }
};
