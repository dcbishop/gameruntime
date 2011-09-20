#include "../Main/Globals.hpp"

#include "../Main/Replicant.hpp"
#include "../Main/BaseClass.hpp"
#include "../Main/PropertyClass.hpp"

#include "../Debug/console.h"

Globals globals;

ClassList Globals::classes_list_;
ShareList Globals::shared_list_;
PropertyMap Globals::transmission_list_;
//ReplicantMap Globals::dirty_list_;

#ifndef _BUILD_DATE
#define _BUILD_DATE "UNKNOWN"
#endif /* _BUILD_DATE */
#ifndef _GIT_VERSION
#define _GIT_VERSION "UNKNOWN"
#endif /* _GIT_VERSION */
#ifndef _GIT_SHA1
#define _GIT_SHA1 "UNKNOWN"
#endif /* _GIT_SHA1 */

const string Globals::build_date = _BUILD_DATE;
const string Globals::git_version = _GIT_VERSION;
const string Globals::git_sha1 = _GIT_SHA1;

/*void Globals::addDirty(Replicant* shared) {
   dirty_list_.insert( make_pair(shared->getSharedId(), shared) );
}*/

void Globals::addTransmitter(PropertyClassPtr shared) {
   //TODO: Tell all the clients of the new object!
   transmission_list_.insert( make_pair(shared->getSharedId(), shared) );
}

void Globals::debugPrintTransmitters() {
   DEBUG_M("Transmission list.");
   for(PropertyMap::iterator it = transmission_list_.begin(); it!=transmission_list_.end(); it++) {
      DEBUG_M("%d - %s", it->second->getSharedId(), it->second->getName().c_str());
   }
}

void Globals::debugFakeTransmit() {
   DEBUG_M("Transmission list.");
   for(PropertyMap::iterator it = transmission_list_.begin(); it!=transmission_list_.end(); it++) {
      // If it's not market for transmit, skip it...
      if(!(it->second)->getTransmitFlag()) {
         DEBUG_M("Transmit flag not set, skipping...");
         continue;
      } else {
         DEBUG_M("Transmit flag is set...");
      }
      // Remove the transmit flag...
      DEBUG_M("Clearing transmit flag.");
      it->second->setTransmitFlag(false);
      DEBUG_M("SET %d TO %s", it->second->getSharedId(), it->second->toString().c_str());
   }
}
