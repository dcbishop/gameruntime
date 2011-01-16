#include <iostream>
using namespace std;

#include "Globals.hpp"
#include "BaseClass.hpp"
#include "Player.hpp"
#include "Creature.hpp"

#include "../Debug/console.h"

int main(int argc, char* argv[]) {
   LOG("Starting %s, version %s, built %s...", argv[0], globals.git_version.c_str(), globals.build_date.c_str());
   /*BaseClass test;
   Player testPlayer;   
   globals.getClassList()->DebugPrint();*/
   Creature dragon;
   LOG("Finished.");
}
