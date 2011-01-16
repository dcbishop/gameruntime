#ifndef OBJECTLIST_HPP_
#define OBJECTLIST_HPP_

#include <string>
#include <unordered_map>
using namespace std;

#include "SharedId.hpp"
class BaseClass;

typedef unordered_map<SharedId, BaseClass*> ClassesMap; 

class ClassList {
   public:
      void Register(BaseClass* classes);

      void DebugPrint();

   private:
      ClassesMap classes_;
};



#endif /* OBJECTLIST_HPP_ */
