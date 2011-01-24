#ifndef GAMERUNTIME_StringId_HPP_
#define GAMERUNTIME_StringId_HPP_

#include <cstdlib>
#include <unordered_map>
using namespace std;
#include <uuid/uuid.h>

class ContainsStringIds;
typedef ContainsStringIds* ContainsStringIdsPtr ;

typedef string StringId;
class HasStringId {
   public:
      HasStringId(const StringId& unique_id): unique_id_(unique_id) {};
      HasStringId() {
         // If none is specified, generate a StringId from a UUID
         char uuid_c[37];
         uuid_t uuid;
         uuid_generate(uuid);
         uuid_unparse(uuid, uuid_c);
         unique_id_ = uuid_c;
      }
      const StringId& getStringId() const { return unique_id_; }
      void setStringId(const StringId& unique_id) { unique_id_ = unique_id; } 
      virtual void dummy() const {} // Allow upcast...

   private:
      StringId unique_id_;
      ContainsStringIdsPtr contained_by_;
};
typedef HasStringId* HasStringIdPtr;

class HasStringId;
class ContainsStringIds {
      public:
         void addStringId(HasStringIdPtr string_id) {
            string_ids_.insert(make_pair(string_id->getStringId(), string_id));
         }
         HasStringIdPtr getByStringId(StringId string_id) {
            return string_ids_.find(string_id)->second;
         }
      private:
         unordered_map<StringId, HasStringIdPtr> string_ids_;
};
typedef ContainsStringIds* ContainsStringIdsPtr ;

#endif /* GAMERUNTIME_StringId_HPP_ */
