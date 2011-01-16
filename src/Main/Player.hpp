#include "../Main/BaseClass.hpp"
#include "../Main/Replicant.hpp"

class Player: public BaseClass, public Properties {
   public:
      virtual inline const string& getClassName() const { return class_name_; }
      Player():playerName_("Unnamed"), health_(100){}
      inline const string& getPlayerName() { return playerName_; }
      inline const int& getPlayerHealth() { return health_; }

   private:
      static const string class_name_;
      string playerName_;
      int health_;
};
