#pragma once
#include <stddef.h>

#include "mm_math/mm_vec3.h"
#include "types.h"

namespace wnd {

// Passed to the wind nodes by WindWorld::update: the accumulated time, the frame delta and the debug flag.
struct UpdateInfo {
  f32 unknown00;  // Inferred role: time; original member name unknown.
  f32 unknown04;  // Inferred role: delta time; original member name unknown.
  bool unknown08; // Inferred role: debug mode; original member name unknown.
};

static_assert(sizeof(UpdateInfo) == 0x0c, "");

class WindNode {
public:
  WindNode() {
    for (int i = 0; i < 12; i++) {
      unknown10[i] = mm::Vec3::zero;
    }
  }

  virtual void update(const UpdateInfo &info) = 0;

  mm::Vec3 unknown10[12]; // Inferred role: wind per probe index; original member name and size unknown.
};

static_assert(offsetof(WindNode, unknown10) == 0x10, "");
static_assert(sizeof(WindNode) == 0xd0, "");

} // namespace wnd
