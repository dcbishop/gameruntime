#ifndef GAMERUNTIME_CREATURE_HPP_
#define GAMERUNTIME_CREATURE_HPP_

#include "../Main/PropertyClass.hpp"
#include "../Main/StringId.hpp"
#include "../Main/BaseClass.hpp"

class Creature: public BaseClass {
   public:
   Creature():
      health_(health_s, 100),
      x_(x_s, 0.0f),
      y_(y_s, 0.0f),
      z_(z_s, 0.0f)
   {}

      void setHealth(const int& health) {
         health_.set(health);
      }
      const int& getHealth() const {
         return health_.get();
      }
      
      void setX(const float& x) {
         x_.set(x);
      }
      
      void setY(const float& y) {
         y_.set(y);
      }
      
      void setZ(const float& z) {
         z_.set(z);
      }

      const float& getX() const {
         return x_.get();
      }

      const float& getY() const {
         return y_.get();
      }

      const float& getZ() const {
         return z_.get();
      }
            
   private:
      Property<int> health_;
      Property<float> x_;
      Property<float> y_;
      Property<float> z_;
      
      static const string health_s;
      static const string x_s;
      static const string y_s;
      static const string z_s;
};

#endif /* CREATURE */
