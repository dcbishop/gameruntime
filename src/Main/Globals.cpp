#include "../Main/Globals.hpp"

#include "../Main/Replicant.hpp"
#include "../Main/BaseClass.hpp"

#include "../Debug/console.h"

Globals globals;

ClassList Globals::classes_list_;
ShareList Globals::shared_list_;
ReplicantMap Globals::dirty_list_;

const string Globals::build_date = _BUILD_DATE;
const string Globals::git_version = _GIT_VERSION;
const string Globals::git_sha1 = _GIT_SHA1;

void Globals::addDirty(Replicant* shared) {
   dirty_list_.insert( make_pair(shared->getSharedId(), shared) );
}
