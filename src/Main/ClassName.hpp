#include <string>
using std::string;

#include "../Main/Globals.hpp"

class HasClassName {
   public:
      virtual inline const string& getClassName() const = 0;
   private:
      void RegisterClassName() {
         globals.getShareList();
      }
};
