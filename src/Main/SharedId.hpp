#ifndef GAMERUNTIME_SHAREDID_HPP_
#define GAMERUNTIME_SHAREDID_HPP_

#include <cstdlib>

typedef unsigned int SharedId;
class HasSharedId {
   public:
      HasSharedId(const SharedId& shared_id): shared_id_(shared_id) {};
      HasSharedId() {
         shared_id_ = rand();
      }
      const SharedId& getSharedId() const { return shared_id_; }
      void setSharedId(const SharedId& shared_id) { shared_id_ = shared_id; } 

   private:
      SharedId shared_id_;
      
};
typedef HasSharedId* HasSharedIdPtr;

#endif /* GAMERUNTIME_SHAREDID_HPP_ */
