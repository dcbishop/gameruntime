#ifndef GLOBALS_HPP_
#define GLOBALS_HPP_

#include "../Main/ObjectList.hpp"
#include "../Main/ShareList.hpp"
//#include "../Main/Replicant.hpp"
#include "../Main/SharedId.hpp"

class Replicant;
class PropertyClass;

typedef unordered_map<SharedId, PropertyClass*> PropertyMap;

class Globals {
      public:
         ClassList* getClassList() { return &classes_list_; }
         ShareList* getShareList() { return &shared_list_; }

         inline void Register(BaseClass* baseclass) {
            classes_list_.Register(baseclass);
         }

         inline void Register(HasSharedId* shared) {
            shared_list_.Register(shared);
         }
         void addTransmitter(PropertyClass* shared);
         
         static const string build_date;
         static const string git_version;
         static const string git_sha1;
         
         void debugPrintTransmitters();
         void debugFakeTransmit();

      private:
         static ClassList classes_list_;
         static ShareList shared_list_;
         static PropertyMap transmission_list_;
};



extern Globals globals;

#endif /* GLOBALS_HPP_ */
