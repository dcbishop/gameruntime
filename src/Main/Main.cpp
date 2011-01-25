#include <iostream>
using namespace std;

#include "Globals.hpp"
#include "BaseClass.hpp"
#include "Player.hpp"
#include "Creature.hpp"
#include "World.hpp"

#include "../GameServer/GameServer.hpp"

#include "../Debug/console.h"

int main(int argc, char* argv[]) {
   LOG("Starting %s, version %s, built %s...", argv[0], globals.git_version.c_str(), globals.build_date.c_str());
   /*BaseClass test;
   Player testPlayer;   
   globals.getClassList()->DebugPrint();*/
   
   /*globals.debugPrintTransmitters();
   Creature dragon;
   globals.debugPrintTransmitters();
   dragon.health.set(100);
   DEBUG_M("Health is %s", dragon.health.toString().c_str());
   globals.debugPrintTransmitters();
   globals.debugFakeTransmit();
   globals.debugFakeTransmit();
   globals.debugFakeTransmit();
   globals.debugFakeTransmit();*/
  
   World world;
   world.setStringId("The_World");
   
   Area area1;
   area1.setStringId("Area_01");
   Area area2;
   area2.setStringId("Area_02");
   
   world.addArea(&area1);
   world.addArea(&area2);
   
   Player player1;
   Player player2;
   area1.addObject(&player1);
   area2.addObject(&player2);
   
   float x = 0.0f;
   float speed = 0.01f;
   int direction = 1;

   GameServer game_server_;
   if(argc > 2) {
      game_server_.addConnection(argv[1], argv[2]);
   }

   while(true) {
      game_server_.recieve();
      sleep(0.1);
      x+=speed*direction;
      player1.setX(x);
      if(x > 10.0f) {
         direction = -1;
      }
      if(x < -10.0f) {
         direction = 1;
      }
      game_server_.transmit();
   }
   

      
   LOG("Finished.");
}
