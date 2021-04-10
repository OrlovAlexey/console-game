
#pragma once
#include "AbstractUnit.h"
#include "AbstractUnit.cpp"
#include "AbstractCountry.h"

class Russia : public Country {
public:
    Russia() {
        hp = 5000;
    }
struct Unit1 : public Unit {
        const int recovery_time = 1;
        Unit1() {
            name = "Soldier";
            hp = 100;
            power = 100;
            shield = 20;
            country = "Russia";
            symbol = 's';
        }
    };
    Unit* get_new_unit1() override {
        return new Unit1();
    }

    struct Unit2 : public Unit {
        const int recovery_time = 2;
        Unit2() {
            name = "Sniper";
            hp = 50;
            power = 150;
            shield = 0;
            country = "Russia";
            symbol = 'S';
        }
    };
    Unit* get_new_unit2() override {
        return new Unit2();
    }

    struct Unit3 : public Unit {
        const int recovery_time = 3;
        Unit3() {
            name = "HeavySoldier";
            hp = 150;
            power = 50;
            shield = 80;
            country = "Russia";
            symbol = 'H';
        }
    };
    Unit* get_new_unit3() override {
        return new Unit3();
    }

    struct Unit4 : public Unit {
        const int recovery_time = 10;
        Unit4() {
            name = "Tank";
            hp = 300;
            power = 150;
            shield = 150;
            country = "Russia";
            symbol = 'T';
        }
    };
    Unit* get_new_unit4() override {
        return new Unit4();
    }

    struct Unit5 : public Unit {
        const int recovery_time = 25;
        Unit5() {
            name = "Artillery";
            hp = 100;
            power = 500;
            shield = 100;
            country = "Russia";
            symbol = 'M';
            printf("Topol-M out!\n");
        }
    };
    Unit* get_new_unit5() override {
        return new Unit5();
    }

};



class China : public Country {
public:
    China() {
        hp = 6000;
    }
    struct Unit1 : public Unit {
        const int recovery_time = 1;
        Unit1() {
            name = "Soldier";
            hp = 100;
            power = 100;
            shield = 0;
            country = "China";
            symbol = 's';
        }
    };
    Unit* get_new_unit1() override {
        return new Unit1();
    }

    struct Unit2 : public Unit {
        const int recovery_time = 2;
        Unit2() {
            name = "KungfuMaster";
            hp = 100;
            power = 150;
            shield = 0;
            country = "China";
            symbol = 'K';
        }
    };
    Unit* get_new_unit2() override {
        return new Unit2();
    }

    struct Unit3 : public Unit {
        const int recovery_time = 3;
        Unit3() {
            name = "Jeep";
            hp = 100;
            power = 100;
            shield = 100;
            country = "China";
            symbol = 'J';
        }
    };
    Unit* get_new_unit3() override {
        return new Unit3();
    }

    struct Unit4 : public Unit {
        const int recovery_time = 10;
        Unit4() {
            name = "SubMarine";
            hp = 300;
            power = 250;
            shield = 0;
            country = "China";
            symbol = 'P';
        }
    };
    Unit* get_new_unit4() override {
        return new Unit4();
    }

    struct Unit5 : public Unit {
        const int recovery_time = 25;
        Unit5() {
            name = "Dictator";
            hp = 20;
            power = 20;
            shield = 50;
            country = "China";
            symbol = 'X';
            printf("Leader XI out!\n");
            for (int i = 0; i < 7; ++i)
                spawn_soldier();//todo
        }
        Unit* spawn_soldier() {
            return reinterpret_cast<Unit*>(new Unit1());
        }
    };
    Unit* get_new_unit5() override {
        return new Unit5();
    }

};


class India : public Country {
public:
    India() {
        hp = 4500;
    }
    struct Unit1 : public Unit {
        const int recovery_time = 1;
        Unit1() {
            name = "Soldier";
            hp = 100;
            power = 110;
            shield = 10;
            country = "India";
            symbol = 's';
        }
    };
    Unit* get_new_unit1() override {
        return new Unit1();
    }

    struct Unit2 : public Unit {
        const int recovery_time = 2;
        Unit2() {
            name = "Archer";
            hp = 30;
            power = 150;
            shield = 10;
            country = "India";
            symbol = 'A';
        }
    };
    Unit* get_new_unit2() override {
        return new Unit2();
    }

    struct Unit3 : public Unit {
        const int recovery_time = 3;
        Unit3() {
            name = "Priest";
            hp = 100;
            power = 20;
            shield = 20;
            country = "India";
            symbol = 'P';
        }
        void healing(Unit* u) {
            u->healing(10);
        }
    };
    Unit* get_new_unit3() override {
        return new Unit3();
    }

    struct Unit4 : public Unit {
        const int recovery_time = 10;
        Unit4() {
            name = "Elephant";
            hp = 400;
            power = 150;
            shield = 50;
            country = "India";
            symbol = 'E';
        }
    };
    Unit* get_new_unit4() override {
        return new Unit4();
    }

    struct Unit5 : public Unit {
        const int recovery_time = 25;
        Unit5() {
            name = "God";
            hp = 500;
            power = 300;
            shield = 0;
            country = "India";
            symbol = 'G';
            printf("Buddha out!\n");
        }
    };
    Unit* get_new_unit5() override {
        return new Unit5();
    }
};