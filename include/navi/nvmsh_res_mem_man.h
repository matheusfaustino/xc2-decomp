#pragma once
#include <stddef.h>

#include "types.h"

namespace mtl {
class ALLOC_HANDLE;
} // namespace mtl

namespace navi {

// Element of the free and used lists of NvmshResMemMan.
class NvmshResMemObj {
public:
  NvmshResMemObj();

  NvmshResMemObj *unknown00; // Inferred role: previous element; original member name unknown.
  NvmshResMemObj *unknown08; // Inferred role: next element; original member name unknown.
  u32 unknown10;             // Inferred role: state flags, bit 0 when the block is in use; original member name unknown.
  u8 _14[4];
  void *unknown18; // Inferred role: data pointer; original member name unknown.
  u32 unknown20;   // Inferred role: data size; original member name unknown.
  // The constructor of NvmshResMemMan allocates 19 elements with a 0x50 stride, so the element size is 0x50.
  u8 _24[0x50 - 0x24];
};

static_assert(offsetof(NvmshResMemObj, unknown08) == 0x08, "");
static_assert(offsetof(NvmshResMemObj, unknown10) == 0x10, "");
static_assert(offsetof(NvmshResMemObj, unknown18) == 0x18, "");
static_assert(offsetof(NvmshResMemObj, unknown20) == 0x20, "");
static_assert(sizeof(NvmshResMemObj) == 0x50, "");

// Owns the navigation mesh memory block and the lists of its free and used pieces.
class NvmshResMemMan {
public:
  NvmshResMemMan(u8 *buffer, int size, mtl::ALLOC_HANDLE handle);
  ~NvmshResMemMan();

  static void memLcMove(void *destination, void *source, u32 size);
  NvmshResMemObj *createFreeObj(u32 size);
  void releaseFreeObj(NvmshResMemObj *object);
  void clearMemObj(NvmshResMemObj *object);
  void moveMemoryObject(NvmshResMemObj *destination, NvmshResMemObj *source);
  bool calcPieceMem();

  void *unknown00; // Inferred role: allocated block; original member name unknown.
  NvmshResMemObj *unknown08;
  NvmshResMemObj *unknown10;
  NvmshResMemObj *unknown18; // Inferred role: head of the list of used pieces; original member name unknown.
  NvmshResMemObj *unknown20;
};

static_assert(offsetof(NvmshResMemMan, unknown10) == 0x10, "");
static_assert(offsetof(NvmshResMemMan, unknown18) == 0x18, "");
static_assert(offsetof(NvmshResMemMan, unknown20) == 0x20, "");

} // namespace navi
