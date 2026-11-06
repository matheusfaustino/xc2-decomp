#pragma once
#include <stddef.h>

#include "gf/menu/gf_menu_object.h"

#include "mm_math/mm_vec3.h"
#include "types.h"

namespace gf {

class GfMenuObjDamageBuff : public GfMenuObject {
public:
  void initialize() override;
  void finalize() override;
  void update() override;
  static void closeAll();
  struct OpenParam {
    OpenParam() : unknown04(0), unknown05(0), unknown08(0), unknown10(nullptr), unknown18(0), unknown1c(0) {}

    u32 unknown00; // Role and original member name unknown.
    u8 unknown04;  // Role and original member name unknown.
    u8 unknown05;  // Role and original member name unknown.
    u8 _06[0x08 - 0x06];
    u64 unknown08;       // Role and original member name unknown.
    const u8 *unknown10; // Inferred role: bdat; original member name unknown.
    u32 unknown18;       // Inferred role: text id; original member name unknown.
    u32 unknown1c;       // Inferred role: icon id; original member name unknown.
    mm::Vec3 unknown20;  // Inferred role: position; original member name unknown.
  };

  struct OpenUniteParam {
    int unknown00; // Role and original member name unknown.
    int unknown04; // Role and original member name unknown.
    u8 _08[0x10 - 0x08];
    mm::Vec3 unknown10; // Inferred role: position; original member name unknown.
  };

  static void open(const OpenParam &param);
  static void close(u32 handle);
  static void openUniteCombo(const OpenUniteParam &param);
};

static_assert(offsetof(GfMenuObjDamageBuff::OpenParam, unknown10) == 0x10, "");
static_assert(offsetof(GfMenuObjDamageBuff::OpenParam, unknown20) == 0x20, "");
static_assert(sizeof(GfMenuObjDamageBuff::OpenParam) == 0x30, "");
static_assert(offsetof(GfMenuObjDamageBuff::OpenUniteParam, unknown10) == 0x10, "");
static_assert(sizeof(GfMenuObjDamageBuff::OpenUniteParam) == 0x20, "");

} // namespace gf
