#pragma once
#include <stddef.h>

#include "types.h"

namespace navi {

class NvmshResMemMan;
class NvmshResMemObj;
struct _NvMhAllReadObj;

// Holds the pointers into one navigation mesh resource file.
class NvmshResObj {
public:
  NvmshResObj();
  void releaseNavi();
  void resetNavi();
  void createNavi(const u8 *data);

  u32 unknown00; // Inferred role: flags, bit 0 set once the resource is created; original member name unknown.
  u8 _04[4];
  const u8 *unknown08; // Inferred role: resource data start; original member name unknown.
  u8 _10[0x58 - 0x10];
};

static_assert(offsetof(NvmshResObj, unknown08) == 0x08, "");
static_assert(sizeof(NvmshResObj) == 0x58, "");

// Holds the state of one loaded navigation mesh resource.
class NvmshResData {
public:
  NvmshResData();
  void setReadData(const _NvMhAllReadObj *readData, int offset);
  void releaseCall();
  void releaseNavi(NvmshResMemMan *memMan);

  u32 unknown00; // Inferred role: state flags, bit 2 set when the resource is released; original member name unknown.
  u16 unknown04;
  u8 _06[2];
  const _NvMhAllReadObj *unknown08; // Inferred role: read object; original member name unknown.
  u32 unknown10;
  u8 _14[4];
  void *unknown18; // Inferred role: first memory block; original member name unknown.
  u8 _20[0x48];
  u32 unknown68; // Inferred role: navigation data size; original member name unknown.
  u8 _6c[4];
  void *unknown70; // Inferred role: second memory block; original member name unknown.
  u8 _78[0x48];
  int unknownc0; // Inferred role: read data offset; original member name unknown.
  u8 _c4[4];
  NvmshResMemObj *unknownc8; // Inferred role: allocated memory object; original member name unknown.
};

static_assert(offsetof(NvmshResData, unknown04) == 0x04, "");
static_assert(offsetof(NvmshResData, unknown08) == 0x08, "");
static_assert(offsetof(NvmshResData, unknown10) == 0x10, "");
static_assert(offsetof(NvmshResData, unknown18) == 0x18, "");
static_assert(offsetof(NvmshResData, unknown68) == 0x68, "");
static_assert(offsetof(NvmshResData, unknown70) == 0x70, "");
static_assert(offsetof(NvmshResData, unknownc0) == 0xc0, "");
static_assert(offsetof(NvmshResData, unknownc8) == 0xc8, "");
static_assert(sizeof(NvmshResData) == 0xd0, "");

} // namespace navi
