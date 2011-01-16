#ifndef GAMERUNTIME_CREATURE_HPP_
#define GAMERUNTIME_CREATURE_HPP_

#include "../Main/PropertyClass.hpp"
// TODO:DEBUG: A class for testing PropertyClass remove me

class Creature {
   public:
   Creature():
      health(health_s, 100),
      x(x_s, 0.0f),
      y(y_s, 0.0f),
      z(z_s, 0.0f)
   {

   }
   
   PropertyClass<int> health;
   PropertyClass<float> x;
   PropertyClass<float> y;
   PropertyClass<float> z;
   
   static const string health_s;
   static const string x_s;
   static const string y_s;
   static const string z_s;
};

const string Creature::health_s = "health";
const string Creature::x_s = "x";
const string Creature::y_s = "y";
const string Creature::z_s = "z";

#endif /* CREATURE */
