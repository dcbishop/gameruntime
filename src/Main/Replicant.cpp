#include "../Main/Replicant.hpp"

void Replicant::setDirty(bool is_dirty) {
   is_dirty_ = is_dirty;
   globals.addDirty(this);
}
