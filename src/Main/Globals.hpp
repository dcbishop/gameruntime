#ifndef GLOBALS_HPP_
#define GLOBALS_HPP_

#include "../Main/ObjectList.hpp"
#include "../Main/ShareList.hpp"
//#include "../Main/Replicant.hpp"
#include "../Main/SharedId.hpp"

class Replicant;

typedef unordered_map<SharedId, Replicant*> ReplicantMap;

class Globals {
      public:
         ClassList* getClassList() { return &classes_list_; }
         ShareList* getShareList() { return &shared_list_; }
         ReplicantMap* getDirtyList() { return &dirty_list_; }

         inline void Register(BaseClass* baseclass) {
            classes_list_.Register(baseclass);
         }

         inline void Register(HasSharedId* shared) {
            shared_list_.Register(shared);
         }
         void addDirty(Replicant* shared);

         static const string build_date;
         static const string git_version;
         static const string git_sha1;

      private:
         static ClassList classes_list_;
         static ShareList shared_list_;
         static ReplicantMap dirty_list_;
};



extern Globals globals;

#endif /* GLOBALS_HPP_ */
