
#pragma once
#include "AbstractUnit.h"

//#define NUMBER_OF_UNITS 5

class Country {
public:
    int hp;
    virtual Unit* get_new_unit1() = 0;
    virtual Unit* get_new_unit2() = 0;
    virtual Unit* get_new_unit3() = 0;
    virtual Unit* get_new_unit4() = 0;
    virtual Unit* get_new_unit5() = 0;
    int take_damage(int dam) {
        hp -= dam;
        return hp;
    }
};

