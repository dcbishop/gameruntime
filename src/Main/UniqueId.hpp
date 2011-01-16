#ifndef GAMERUNTIME_UNIQUEID_HPP_
#define GAMERUNTIME_UNIQUEID_HPP_

#include <cstdlib>

#include <uuid/uuid.h>

typedef string UniqueId;
class HasUniqueId {
   public:
      HasUniqueId(const UniqueId& unique_id): unique_id_(unique_id) {};
      HasUniqueId() {
         // If none is specified, generate a UniqueId from a UUID
         char uuid_c[37];
         uuid_t uuid;
         uuid_generate(uuid);
         uuid_unparse(uuid, uuid_c);
         unique_id_ = uuid_c;
      }
      const UniqueId& getUniqueId() const { return unique_id_; }
      void setUniqueId(const UniqueId& unique_id) { unique_id_ = unique_id; } 

   private:
      UniqueId unique_id_;
};

#endif /* GAMERUNTIME_UNIQUEID_HPP_ */
