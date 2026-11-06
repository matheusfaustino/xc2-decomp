#pragma once
#include <stddef.h>

#include "types.h"
#include "wnd/wnd_node.h"

namespace mm {
struct Vec3;
} // namespace mm

namespace wnd {

struct ResConpoundWind;

class WindBlender : public WindNode {
public:
  WindBlender();
  void update(const UpdateInfo &info) override;
  void setNode(const WindNode *node);
  void changeNode(const WindNode *node, f32 time);

  const WindNode *unknownd0; // Inferred role: node; original member name unknown.
  const WindNode *unknownd8; // Inferred role: previous node, blended out; original member name unknown.
  f32 unknowne0;             // Inferred role: blend ratio; original member name unknown.
  f32 unknowne4;             // Inferred role: blend time; original member name unknown.
};

static_assert(offsetof(WindBlender, unknownd0) == 0xd0, "");
static_assert(offsetof(WindBlender, unknownd8) == 0xd8, "");
static_assert(offsetof(WindBlender, unknowne0) == 0xe0, "");
static_assert(offsetof(WindBlender, unknowne4) == 0xe4, "");
static_assert(sizeof(WindBlender) == 0xf0, "");

class ConpoundWind : public WindNode {
public:
  ConpoundWind();
  void update(const UpdateInfo &info) override;
  void calcWind(f32 time, u32 index, mm::Vec3 *wind);

  const ResConpoundWind *unknownd0; // Inferred role: wind resource, written by GlobalWindManager::setup; original member name unknown.
  u8 _d8[0x1d0 - 0xd8];
};

static_assert(offsetof(ConpoundWind, unknownd0) == 0xd0, "");
static_assert(sizeof(ConpoundWind) == 0x1d0, "");

class GlobalWind : public WindBlender {
public:
  GlobalWind() : unknowne8(nullptr), unknownf0(nullptr) {}
  void update(const UpdateInfo &info) override;

  WindNode *unknowne8; // Inferred role: current node; original member name unknown.
  WindNode *unknownf0; // Inferred role: debug node; original member name unknown.
  u8 _f8[0x100 - 0xf8];
};

static_assert(offsetof(GlobalWind, unknowne8) == 0xe8, "");
static_assert(offsetof(GlobalWind, unknownf0) == 0xf0, "");
static_assert(sizeof(GlobalWind) == 0x100, "");

} // namespace wnd
