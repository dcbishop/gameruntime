#include "../Main/BaseClass.hpp"

#include "../Main/Globals.hpp"

const string BaseClass::class_name_ = "BaseClass";

BaseClass::BaseClass() {
   DEBUG_M("Constructed a %s[%s]", getClassName().c_str(), getStringId().c_str());
   globals.getClassList()->Register(this);
}
