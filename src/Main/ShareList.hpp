#ifndef GAMERUNTIME_SHAREDLIST_HPP_
#define GAMERUNTIME_SHAREDLIST_HPP_

#include <unordered_map>
using namespace std;

typedef unordered_map<SharedId, HasSharedIdPtr> ShareMap;
class ShareList {
   public:
      void Register(HasSharedIdPtr shared) {
         shares_.insert( make_pair(shared->getSharedId(), shared));
      };
      
      HasSharedIdPtr get(SharedId id) {
         return shares_.find(id)->second;
      }

   private:
      ShareMap shares_;
};

#endif /* GAMERUNTIME_SHAREDLIST_HPP_ */
