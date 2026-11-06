#pragma once
#include <stddef.h>

#include "types.h"

namespace mm {
struct Vec3;
} // namespace mm

namespace navi {

class NvmshResMemMan;

// Loads and updates the navigation mesh resources and their memory.
class NvmshResMan {
public:
  bool setupResource(const u8 *data, const char *name);
  void releaseResource();
  void tail();
  void resourceNaviUpdate();
  void update();
  void updateMemMove(const mm::Vec3 &position, bool unknown, const mm::Vec3 &previous, int index);

  u8 _00[0x48];
  NvmshResMemMan *unknown48; // Inferred role: memory manager; original member name unknown.
  u8 _50[0x16ea - 0x50];
  u16 unknown16ea; // Inferred role: inverse of unknown16ec; original member name unknown.
  u16 unknown16ec; // Inferred role: flipped by tail; original member name unknown.
};

static_assert(offsetof(NvmshResMan, unknown48) == 0x48, "");
static_assert(offsetof(NvmshResMan, unknown16ea) == 0x16ea, "");
static_assert(offsetof(NvmshResMan, unknown16ec) == 0x16ec, "");
static_assert(sizeof(NvmshResMan) == 0x16f0, "");

} // namespace navi
