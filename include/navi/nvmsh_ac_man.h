#pragma once
#include <stddef.h>

#include "types.h"

namespace mtl {
class ALLOC_HANDLE;
} // namespace mtl

namespace navi {

class NvmshAcObjBase;
class NvmshResMan;

// Intrusive list of navigation mesh agent objects.
template <typename T> class TNaviListMan {
public:
  T *unknown00; // Inferred role: head; original member name unknown.
  T *unknown08; // Inferred role: tail; original member name unknown.
};

static_assert(offsetof(TNaviListMan<NvmshAcObjBase>, unknown00) == 0x00, "");
static_assert(offsetof(TNaviListMan<NvmshAcObjBase>, unknown08) == 0x08, "");
static_assert(sizeof(TNaviListMan<NvmshAcObjBase>) == 0x10, "");

// Owns, updates and releases the navigation mesh agent objects.
class NvmshAcMan {
public:
  void tail();
  void beginUpdate();
  void releaseObj(NvmshAcObjBase *object);
  void update(NvmshResMan *resMan);
  NvmshAcObjBase *createObj(mtl::ALLOC_HANDLE handle, NvmshResMan *resMan);

  u8 _00[0x808];
  TNaviListMan<NvmshAcObjBase> unknown808; // Inferred role: update list; original member name unknown.
  u8 _818[0x840 - 0x818];
  u16 unknown840; // Inferred role: inverse of unknown842, set by beginUpdate; original member name unknown.
  u16 unknown842; // Inferred role: flipped by tail; original member name unknown.
};

static_assert(offsetof(NvmshAcMan, unknown808) == 0x808, "");
static_assert(offsetof(NvmshAcMan, unknown840) == 0x840, "");
static_assert(offsetof(NvmshAcMan, unknown842) == 0x842, "");
static_assert(sizeof(NvmshAcMan) == 0x848, "");

} // namespace navi
