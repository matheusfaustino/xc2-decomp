#include "cloud/cloud_mdl_list_man.h"

#include "cloud/cloud_man.h"
#include "cloud/cloud_mdl_parts.h"
#include "mtl/alloc_handle.h"

namespace cloud {

CloudMdlListMan::CloudMdlListMan() : unknown00(nullptr), unknown08(nullptr) {}

CloudMdlListMan::~CloudMdlListMan() { releaseAll(); }

void CloudMdlListMan::releaseAll() {
  CloudMdlParts *object = unknown00;
  while (object != nullptr) {
    CloudMdlParts *next = object->unknownc0;
    delete object;
    object = next;
  }
  unknown00 = nullptr;
  unknown08 = nullptr;
}

void CloudMdlListMan::update() {}

void CloudMdlListMan::changeObj(CloudMdlParts *oldObj, CloudMdlParts *newObj) {
  CloudMdlParts *prev = oldObj->unknownc8;
  CloudMdlParts *next = newObj->unknownc0;
  if (prev != nullptr) {
    prev->unknownc0 = newObj;
  }
  newObj->unknownc8 = prev;
  if (next != nullptr) {
    next->unknownc8 = oldObj;
  }
  oldObj->unknownc0 = next;
  oldObj->unknownc8 = newObj;
  newObj->unknownc0 = oldObj;
}

CloudMdlParts *CloudMdlListMan::createObj() {
  CloudMdlParts *object = new (CloudMan::get()->unknown10) CloudMdlParts();
  if (unknown08 != nullptr) {
    unknown08->unknownc0 = object;
    object->unknownc8 = unknown08;
  } else {
    unknown00 = object;
  }
  unknown08 = object;
  return object;
}

void CloudMdlListMan::releaseObj(CloudMdlParts *&obj) {
  CloudMdlParts *next = obj->unknownc0;
  CloudMdlParts *prev = obj->unknownc8;
  if (prev == nullptr) {
    if (next == nullptr) {
      unknown00 = nullptr;
      unknown08 = nullptr;
    } else {
      unknown00 = next;
      next->unknownc8 = nullptr;
    }
  } else if (next == nullptr) {
    unknown08 = prev;
    prev->unknownc0 = nullptr;
  } else {
    prev->unknownc0 = next;
    obj->unknownc0->unknownc8 = obj->unknownc8;
  }
  if (obj != nullptr) {
    delete obj;
    obj = nullptr;
  }
}

} // namespace cloud
