#pragma once
#include <stddef.h>

#include "mm_math/mm_col4.h"
#include "mm_math/mm_mat44.h"
#include "mm_math/mm_vec3.h"
#include "types.h"

namespace effect {

// Layout inferred from the stores that fw::EffectManager::setParam, setMatrix and create make to a stack
// SendParam before es_set_param (1.0.0: 0x7100172368, 0x710017242c, 0x71001725fc, 0x71001726cc, 0x7100172ad4).
// Member names are unknown.
struct alignas(16) SendParam {
  SendParam() : unknown00(mm::Mat44::identity) {
    unknown40 = mm::Col4(1.0f, 1.0f, 1.0f, 1.0f);
    unknown50 = 0;
    unknown60.y = 1.0f;
    unknown60.z = 1.0f;
    unknown60.x = 1.0f;
    unknowna3 = 1;
    unknown70 = 0;
    unknown78 = 0;
    unknowna2 = 0;
    unknowna4 = 0;
    unknowna5 = 0;
    unknown80 = -1.0f;
    unknown84 = 0.0f;
    unknowna0 = 0xffff;
    unknowna6 = 0;
    unknown90 = -1.0f;
    unknown94 = -1.0f;
    unknown88 = -1.0f;
    unknown8c = -1.0f;
    unknown98 = -1.0f;
    unknown9c = 0;
    unknowna8 = 0;
  }

  mm::Mat44 unknown00; // Inferred role: matrix; original member name unknown.
  mm::Col4 unknown40;  // Inferred role: color; original member name unknown.
  u64 unknown50;       // Role and original member name unknown.
  u8 _58[0x60 - 0x58];
  mm::Vec3 unknown60; // Inferred role: scale; original member name unknown.
  u64 unknown70;      // Role and original member name unknown.
  u32 unknown78;      // Role and original member name unknown.
  u8 _7c[0x80 - 0x7c];
  f32 unknown80; // Role and original member name unknown.
  f32 unknown84; // Inferred role: scale or speed; original member name unknown.
  f32 unknown88; // Role and original member name unknown.
  f32 unknown8c; // Role and original member name unknown.
  f32 unknown90; // Role and original member name unknown.
  f32 unknown94; // Role and original member name unknown.
  f32 unknown98; // Role and original member name unknown.
  u32 unknown9c; // Inferred role: category, an integer copied to the emitter; original member name unknown.
  u16 unknowna0; // Role and original member name unknown.
  u8 unknowna2;  // Role and original member name unknown.
  u8 unknowna3;  // Role and original member name unknown.
  u8 unknowna4;  // Role and original member name unknown.
  u8 unknowna5;  // Role and original member name unknown.
  u16 unknowna6; // Role and original member name unknown.
  u8 unknowna8;  // Role and original member name unknown.
};

static_assert(offsetof(SendParam, unknown40) == 0x40, "");
static_assert(offsetof(SendParam, unknown50) == 0x50, "");
static_assert(offsetof(SendParam, unknown60) == 0x60, "");
static_assert(offsetof(SendParam, unknown70) == 0x70, "");
static_assert(offsetof(SendParam, unknown80) == 0x80, "");
static_assert(offsetof(SendParam, unknown98) == 0x98, "");
static_assert(offsetof(SendParam, unknowna0) == 0xa0, "");
static_assert(offsetof(SendParam, unknowna5) == 0xa5, "");
static_assert(offsetof(SendParam, unknowna8) == 0xa8, "");
static_assert(sizeof(SendParam) == 0xb0, "");

} // namespace effect
