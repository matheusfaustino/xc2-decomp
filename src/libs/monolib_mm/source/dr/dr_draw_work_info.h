#pragma once
#include <stddef.h>

#include "mm_math/mm_mat44.h"
#include "mm_math/mm_vec3.h"
#include "types.h"

namespace ml {

class IDrDrawWorkInfo {
public:
  u8 _000[0x90 - 0x000];
  void *unknown90[2]; // Inferred role: shadow textures; original member name unknown.
  u8 _a0[0xb0 - 0x0a0];
  mm::Vec3 unknownb0; // Inferred role: camera position; original member name unknown.
  u8 _c0[0x170 - 0x0c0];
  f32 unknown170; // Inferred role: cloud density; original member name unknown.
  u8 _174[0x1d4 - 0x174];
  f32 unknown1d4[24]; // Inferred role: fog table; original member name unknown.
  u8 _234[0x420 - 0x234];
  mm::Mat44 unknown420[3]; // Inferred role: shadow matrices; original member name unknown.
  f32 unknown4e0[3];       // Inferred role: shadow offsets; original member name unknown.
  f32 unknown4ec;          // Inferred role: original member name unknown.
  u8 _4f0[0x538 - 0x4f0];
  f32 unknown538; // Inferred role: fog start; original member name unknown.
  f32 unknown53c; // Inferred role: fog end; original member name unknown.
  f32 unknown540; // Inferred role: fog density; original member name unknown.
  u8 _544[0x550 - 0x544];
  u32 unknown550; // Inferred role: screen width; original member name unknown.
  u32 unknown554; // Inferred role: screen height; original member name unknown.
  u8 _558[0x560 - 0x558];
  int unknown560; // Inferred role: loop; original member name unknown.
  u8 _564[0x580 - 0x564];
};

static_assert(offsetof(IDrDrawWorkInfo, unknown90) == 0x90, "");
static_assert(offsetof(IDrDrawWorkInfo, unknownb0) == 0xb0, "");
static_assert(offsetof(IDrDrawWorkInfo, unknown170) == 0x170, "");
static_assert(offsetof(IDrDrawWorkInfo, unknown1d4) == 0x1d4, "");
static_assert(offsetof(IDrDrawWorkInfo, unknown420) == 0x420, "");
static_assert(offsetof(IDrDrawWorkInfo, unknown4e0) == 0x4e0, "");
static_assert(offsetof(IDrDrawWorkInfo, unknown4ec) == 0x4ec, "");
static_assert(offsetof(IDrDrawWorkInfo, unknown538) == 0x538, "");
static_assert(offsetof(IDrDrawWorkInfo, unknown550) == 0x550, "");
static_assert(offsetof(IDrDrawWorkInfo, unknown560) == 0x560, "");
static_assert(sizeof(IDrDrawWorkInfo) == 0x580, "");

} // namespace ml
