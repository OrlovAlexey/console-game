#include <iostream>
#include "include.h"

int main() {
    NormalSoldierFactory fact;
    Russia::Soldier* s = new Russia::Soldier();
//    Russia::Soldier* s = dynamic_cast<Russia::Soldier*>(fact.CreateUnit());
    std::cout << s->get_hp() << " " << s->power;
    return 0;
}
