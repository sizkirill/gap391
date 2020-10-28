#include "Grammar/RegularGrammar.h"
#include "Grammar/Weapon.h"
#include <iostream>
#include <ctime>
#include <conio.h>
#include <vld.h>

int main()
{
    RegularGrammar weapons;
    weapons.Init("Assets/Grammars/Weapons.xml");

    std::cout << "Press \'q\' to exit or any key to generate a weapon\n";

    char exit = _getch();
    while (exit != 'q')
    {
        Weapon w = weapons.RunGrammar();
        std::cout << w;
        exit = _getch();
    }
    
    std::cin.get();
    return 0;
}