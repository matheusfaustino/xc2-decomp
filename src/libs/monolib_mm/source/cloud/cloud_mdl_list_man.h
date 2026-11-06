#pragma once
#include <stddef.h>

namespace cloud {

class CloudMdlParts;

class CloudMdlListMan {
public:
  CloudMdlListMan();
  ~CloudMdlListMan();
  void releaseAll();
  void update();
  void calcZSort();
  void changeObj(CloudMdlParts *oldObj, CloudMdlParts *newObj);
  CloudMdlParts *createObj();
  void releaseObj(CloudMdlParts *&obj);

  CloudMdlParts *unknown00; // Inferred role: first part; original member name unknown.
  CloudMdlParts *unknown08; // Inferred role: last part; original member name unknown.
};

static_assert(offsetof(CloudMdlListMan, unknown08) == 0x08, "");
static_assert(sizeof(CloudMdlListMan) == 0x10, "");

} // namespace cloud
