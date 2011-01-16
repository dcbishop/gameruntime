#ifndef GAMERUNTIME_SHAREDLIST_HPP_
#define GAMERUNTIME_SHAREDLIST_HPP_

#include <unordered_map>
using namespace std;

typedef unordered_map<SharedId, HasSharedId*> ShareMap;
class ShareList {
   public:
      void Register(HasSharedId* shared) {
         shares_.insert( make_pair(shared->getSharedId(), shared));
      };
      
      HasSharedId* get(SharedId id) {
         return shares_.find(id)->second;
      }

   private:
      ShareMap shares_;
};

#endif /* GAMERUNTIME_SHAREDLIST_HPP_ */
