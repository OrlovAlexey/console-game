
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
    Unit* operator[] (unsigned int index) {
        switch (index) {
            case 0:
                return get_new_unit1();
            case 1:
                return get_new_unit2();
            case 2:
                return get_new_unit3();
            case 3:
                return get_new_unit4();
            case 4:
                return get_new_unit5();
        }
    }
    int take_damage(int dam) {
        hp -= dam;
        return hp;
    }
};

