#ifndef BASECLASS_HPP_
#define BASECLASS_HPP_

#include <string>
#include <typeinfo>

using namespace std;

#include "../Debug/console.h"
#include "../Main/SmartPointer.hpp"
#include "../Main/Globals.hpp"
#include "../Main/ClassName.hpp"
#include "../Main/SharedId.hpp"
#include "../Main/UniqueId.hpp"

class ObjectList;

class BaseClass: public HasSharedId, public HasClassName, public HasUniqueId {
   public:  
      virtual inline const string& getClassName() const { return class_name_; }
      virtual void dummy(){}
      
      BaseClass();  
   private:
      static const string class_name_;    
};

#endif /* BASECLASS_HPP_ */
