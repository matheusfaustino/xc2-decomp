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

// Owns, updates and releases the navigation mesh agent objects.
class NvmshAcMan {
public:
  void tail();
  void beginUpdate();
  void releaseObj(NvmshAcObjBase *object);
  void update(NvmshResMan *resMan);
  NvmshAcObjBase *createObj(mtl::ALLOC_HANDLE handle, NvmshResMan *resMan);

  u8 _00[0x808];
  NvmshAcObjBase *unknown808; // Inferred role: head of the update list; original member name unknown.
  NvmshAcObjBase *unknown810; // Inferred role: tail of the update list; original member name unknown.
  u8 _818[0x840 - 0x818];
  u16 unknown840; // Inferred role: copied from unknown842 by beginUpdate; original member name unknown.
  u16 unknown842; // Inferred role: flipped by tail; original member name unknown.
};

static_assert(offsetof(NvmshAcMan, unknown808) == 0x808, "");
static_assert(offsetof(NvmshAcMan, unknown810) == 0x810, "");
static_assert(offsetof(NvmshAcMan, unknown840) == 0x840, "");
static_assert(offsetof(NvmshAcMan, unknown842) == 0x842, "");

} // namespace navi
