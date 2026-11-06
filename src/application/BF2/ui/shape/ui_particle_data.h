#pragma once
#include <stddef.h>

#include "types.h"

namespace ui {

class UIParticleData {
public:
  UIParticleData();
  virtual ~UIParticleData();
  void deserialize(const void *data, u32 size);

  u8 unknown08[0x64]; // Inferred role: particle data; original member names unknown.
  f32 unknown6c;      // Inferred role: particle count component; original member name unknown.
  f32 unknown70;      // Inferred role: particle count component; original member name unknown.
  f32 unknown74;      // Inferred role: particle count factor; original member name unknown.
  u8 unknown78[0x30]; // Inferred role: particle data; original member names unknown.
};

static_assert(offsetof(UIParticleData, unknown08) == 0x08, "");
static_assert(offsetof(UIParticleData, unknown6c) == 0x6c, "");
static_assert(offsetof(UIParticleData, unknown74) == 0x74, "");
static_assert(sizeof(UIParticleData) == 0xa8, "");

} // namespace ui
