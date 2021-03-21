
#pragma once
#include "AbstractUnit.h"
#include "AbstractCountry.h"

class Russia : public Country {
public:
    Russia() {
        hp = 5000;
    }
    struct Soldier : public Unit {
        const int recovery_time = 1;
        Soldier(){
            hp = 100;
            power = 100;
            shield = 20;
            country = "Russia";
        }
    };
    struct Sniper : public Unit {
        const int recovery_time = 2;
        Sniper(){
            hp = 50;
            power = 150;
            shield = 0;
            country = "Russia";
        }
    };
    struct HeavySoldier : public Unit {
        const int recovery_time = 3;
        HeavySoldier(){
            hp = 150;
            power = 50;
            shield = 80;
            country = "Russia";
        }
    };
    struct Tank : public Unit {
        const int recovery_time = 10;
        Tank(){
            hp = 300;
            power = 150;
            shield = 150;
            country = "Russia";
        }
    };
    struct Artillery : public Unit {
        const int recovery_time = 25;
        Artillery(){
            hp = 100;
            power = 500;
            shield = 100;
            country = "Russia";
            printf("Topol-M out!\n");
        }
    };
};





class China : public Country {
public:
    China() {
        hp = 6000;
    }
    struct Soldier : public Unit {
        const int recovery_time = 1;
        Soldier(){
            hp = 100;
            power = 100;
            shield = 0;
            country = "China";
        }
    };
    struct KungfuMaster : public Unit {
        const int recovery_time = 2;
        KungfuMaster(){
            hp = 100;
            power = 150;
            shield = 0;
            country = "China";
        }
    };
    struct Jeep : public Unit {
        const int recovery_time = 3;
        Jeep(){
            hp = 100;
            power = 100;
            shield = 100;
            country = "China";
        }
    };
    struct Submarine : public Unit {
        const int recovery_time = 10;
        Submarine(){
            hp = 300;
            power = 250;
            shield = 0;
            country = "China";
        }
    };
    struct Dictator : public Unit {
        const int recovery_time = 25;
        Dictator(){
            hp = 20;
            power = 20;
            shield = 50;
            country = "China";
            printf("Leader XI out!\n");
            for (int i = 0; i < 7; ++i)
                spawn_soldier();//todo
        }
        Unit* spawn_soldier() {
            return new Soldier();
        }
    };
};


class India : public Country {
public:
    India() {
        hp = 4500;
    }
    struct Soldier : public Unit {
        const int recovery_time = 1;
        Soldier(){
            hp = 100;
            power = 110;
            shield = 10;
            country = "India";
        }
    };
    struct Archer : public Unit {
        const int recovery_time = 2;
        Archer(){
            hp = 30;
            power = 150;
            shield = 10;
            country = "India";
        }
    };
    struct Priest : public Unit {
        const int recovery_time = 3;
        Priest(){
            hp = 100;
            power = 20;
            shield = 20;
            country = "India";
        }
        void healing(Unit* u){
            u->healing(10);
        }
    };
    struct Elephant : public Unit {
        const int recovery_time = 10;
        Elephant(){
            hp = 400;
            power = 150;
            shield = 50;
            country = "India";
        }
    };
    struct God : public Unit {
        const int recovery_time = 25;
        God(){
            hp = 500;
            power = 300;
            shield = 0;
            country = "India";
            printf("Budha out!\n");
        }
    };
};