#pragma once
#include <stddef.h>

#include "types.h"

namespace ui {

struct EventData {
  EventData() : unknown00(0), unknown08(0), unknown0c(0), unknown10(0), unknown14(0) {}

  u16 unknown00; // Inferred role: type; original member name unknown.
  u8 _02[0x08 - 0x02];
  union {
    u32 unknown08;     // Inferred role: param0; original member name unknown.
    f32 unknown08_f32; // View of the first parameter slot as a float; the repo owner reports GfMenuObjZoneTelop's listener reads it; not matched yet.
    struct {
      u16 unknown08_u16; // Inferred role: param0 low half; original member name unknown.
      u16 unknown0a;     // Inferred role: param0 high half; original member name unknown.
    };
  };
  u32 unknown0c; // Inferred role: param1; original member name unknown.
  u32 unknown10; // Inferred role: param2; original member name unknown.
  u32 unknown14; // Inferred role: param3; original member name unknown.
};

static_assert(offsetof(EventData, unknown08) == 0x08, "");
static_assert(offsetof(EventData, unknown08_f32) == 0x08, "");
static_assert(offsetof(EventData, unknown08_u16) == 0x08, "");
static_assert(offsetof(EventData, unknown0a) == 0x0a, "");
static_assert(offsetof(EventData, unknown0c) == 0x0c, "");
static_assert(offsetof(EventData, unknown10) == 0x10, "");
static_assert(offsetof(EventData, unknown14) == 0x14, "");
static_assert(sizeof(EventData) == 0x18, "");

} // namespace ui
