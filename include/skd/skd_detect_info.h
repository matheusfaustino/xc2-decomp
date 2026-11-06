#pragma once
#include <stddef.h>

#include "mm/mm_std_base.h"
#include "mm_math/mm_vec3.h"
#include "types.h"

namespace skd {

// Hit information filled by the detect jobs; the buffer below holds them and the apply job averages the enabled ones.
// Layout evidence: executeDetectSphereJob (0x7100982db4) writes the world position at 0x10 and the hit flag at 0x20,
// addDetectSphereJob (0x7100982eac) writes the node index at 0x0, and DetectInfoBuffer entries are 0x30 bytes apart.
struct DetectInfo {
  u16 unknown00; // Inferred role: node index; original member name unknown.
  u8 _02[0x0e];
  mm::Vec3 unknown10; // Inferred role: world position; original member name unknown.
  bool unknown20;     // Inferred role: enabled; original member name unknown.
  u8 _21[0x0f];
};

static_assert(offsetof(DetectInfo, unknown10) == 0x10, "");
static_assert(offsetof(DetectInfo, unknown20) == 0x20, "");
static_assert(sizeof(DetectInfo) == 0x30, "");

// DetectInfo registry shared by the detect and apply jobs. Layout evidence: addDetectSphereJob appends at
// buffer + count * 0x30 and increments the count at 0x18000; the assertion "SIZE > index" (skd_detect_info.h) caps it.
struct DetectInfoBuffer {
  static const u32 SIZE = 0x800;

  // Inferred helper: appends an entry; original name unknown. The "SIZE > m_size" assertion text comes from the game.
  DetectInfo *unknown001() {
    MM_ASSERT(SIZE > m_size);
    return &unknown00[m_size++];
  }

  DetectInfo unknown00[SIZE];
  u32 m_size; // Name from the "SIZE > m_size" assertion in skd_detect_info.h.
};

static_assert(offsetof(DetectInfoBuffer, m_size) == 0x18000, "");

} // namespace skd
