#pragma once
#include <stddef.h>

#include "mm_math/mm_vec3.h"
#include "mtl/alloc_handle.h"
#include "types.h"

namespace ml {
class CriticalSection;
class Scn;
class WinProc;
} // namespace ml

namespace navi {

class NvmshAcMan;
class NvmshAcObj;
class NvmshResMan;

class NvmshManager {
public:
  NvmshManager(ml::Scn *scn, mtl::ALLOC_HANDLE handle);
  virtual ~NvmshManager();
  bool setupResource(const void *data, const char *name);
  void releaseResource();
  NvmshAcObj *createObj();
  void releaseObj(NvmshAcObj *object);
  void pause(bool value);
  void pauseHLv(bool pause, bool highLevel);
  void update();
  void memMoveUpdate();
  bool jobUpdate(void *param1, void *param2);
  void setAddNavigation(bool value);
  void setDebugDisp(bool param1, bool param2, bool param3, bool param4, int index);
  void setDebugObjAllDisp(bool value);
  void tail();
  void openDebugWindow(ml::WinProc *window);

  static ml::CriticalSection &getCriticalSection();

  u8 _08[0x20 - 0x08];
  mm::Vec3 unknown20[2]; // Inferred role: positions; original member name unknown.
  mm::Vec3 unknown40;    // Inferred role: previous position; original member name unknown.
  NvmshResMan *unknown50;
  NvmshAcMan *unknown58;
  mtl::ALLOC_HANDLE unknown60; // Inferred role: allocator handle passed to NvmshAcMan::createObj; original member name unknown.
  int unknown64;               // Inferred role: position index; original member name unknown.
  u32 unknown68;               // Inferred role: state flags; original member name unknown.
  int unknown6c;               // Inferred role: passed to NvmshResMan::updateMemMove; original member name unknown.
  u8 _70[0x80 - 0x70];
};

static_assert(offsetof(NvmshManager, unknown20) == 0x20, "");
static_assert(offsetof(NvmshManager, unknown40) == 0x40, "");
static_assert(offsetof(NvmshManager, unknown50) == 0x50, "");
static_assert(offsetof(NvmshManager, unknown58) == 0x58, "");
static_assert(offsetof(NvmshManager, unknown60) == 0x60, "");
static_assert(offsetof(NvmshManager, unknown64) == 0x64, "");
static_assert(offsetof(NvmshManager, unknown68) == 0x68, "");
static_assert(offsetof(NvmshManager, unknown6c) == 0x6c, "");
static_assert(sizeof(NvmshManager) == 0x80, "");

} // namespace navi
