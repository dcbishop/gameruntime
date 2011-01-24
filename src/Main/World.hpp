#ifndef GAMERUNTIME_WORLD_HPP_
#define GAMERUNTIME_WORLD_HPP_

#include "../Debug/console.h"

class Area: public HasStringId, public ContainsStringIds {
   public:
      void addObject(HasStringIdPtr object) {
         addStringId(object);
      }

      HasStringIdPtr getObject(string string_id) {
         return getByStringId(string_id);
      }
};
typedef Area* AreaPtr;

class World: public HasStringId, public ContainsStringIds {
      public:
         AreaPtr getArea(string string_id) {
            HasStringIdPtr found = getByStringId(string_id);
            return dynamic_cast<AreaPtr>(found);
         }
         
         void addArea(AreaPtr area) {
            DEBUG_M("Adding area '%s' to world '%s'.", area->getStringId().c_str(), this->getStringId().c_str());
            addStringId(area);
         }
};
typedef World* WorldPtr;

#endif /* GAMERUNTIME_WORLD_HPP_ */
