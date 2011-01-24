#include "../Main/BaseClass.hpp"
#include "../Main/Replicant.hpp"
#include "../Main/Creature.hpp"

class Player: public Creature {
   public:
      virtual inline const string& getClassName() const { return class_name_; }
      Player():playerName_("Unnamed"){}
      inline const string& getPlayerName() { return playerName_; }

   private:
      static const string class_name_;
      string playerName_;
};
