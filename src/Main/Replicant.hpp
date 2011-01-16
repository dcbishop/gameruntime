#ifndef REPLICANT_HPP_
#define REPLICANT_HPP_

#include <string>
#include <set>
using namespace std;

#include "../Main/Globals.hpp"
#include "../Main/SharedId.hpp"
#include "../Main/UniqueId.hpp"

class Replicant: public HasSharedId, public HasUniqueId {
   public:
      void* get() { return ptr_; }
      void set(void* ptr) { ptr_ = ptr; }
      const bool& getDirty() const { return is_dirty_; }
      void setDirty(bool is_dirty);

   private:
      void* ptr_;
      string name_;
      bool is_dirty_;
};

class Properties {
   public:
      void addProperty(void* ptr, const string& name, const int& shared_id);
      bool isDirty_;

   private:
      set<string> properties_;
};

#endif /* REPLICANT */ 
