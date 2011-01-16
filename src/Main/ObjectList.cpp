#include "../Main/ObjectList.hpp"

#include "../Main/BaseClass.hpp"

void ClassList::Register(BaseClass* object) {
   classes_.insert( make_pair(object->getSharedId(), object));
}

void ClassList::DebugPrint() {
   DEBUG_M("SharedId:\t\tClass Name:\tMemory Address:");
   for(ClassesMap::const_iterator it = classes_.begin(); it != classes_.end(); ++it) {
      DEBUG_M("%d\t%s\t%p", it->first, it->second->getClassName().c_str(), it->second);
   }
   std::cout << std::endl; 
}
