#pragma once
#include <stddef.h>

#include "mm_math/mm_pnt.h"
#include "types.h"
#include "ui/ctrl/ui_ctrl_curve.h"
#include "ui/ctrl/ui_ctrl_object.h"
#include "ui/ctrl/ui_ctrl_simple.h"

namespace ui {

class UISelectObjList;

class UICtrlRevolveSelect : public UICtrlObject {
public:
  struct RevolveSetting {
    RevolveSetting()
        : unknown00(0), unknown02(0), unknown03(0), unknown04(0.0f), unknown0c(0), unknown14(0), unknown18(0.5f), unknown1c(0), unknown20(0), unknown24(CURVE_TYPE_LINEAR),
          unknown28(CURVE_TYPE_LINEAR), unknown2c(0.5f) {}

    u16 unknown00;          // Inferred role: angle range; original member name unknown.
    u8 unknown02;           // Inferred role: layout type; original member name unknown.
    u8 unknown03;           // Inferred role: reverse; original member name unknown.
    f32 unknown04;          // Inferred role: roll time; original member name unknown.
    mm::Pnt<s16> unknown08; // Inferred role: center position; original member name unknown.
    u32 unknown0c;          // Inferred role: radius; original member name unknown.
    f32 unknown10;          // Inferred role: move time; original member name unknown.
    u16 unknown14;          // Inferred role: start angle; original member name unknown.
    f32 unknown18;          // Inferred role: scale rate; original member name unknown.
    s32 unknown1c;          // Inferred role: depth radius; original member name unknown.
    u32 unknown20;          // Inferred role: priority base; original member name unknown.
    CURVE_TYPE unknown24;   // Inferred role: roll curve; original member name unknown.
    CURVE_TYPE unknown28;   // Inferred role: move curve; original member name unknown.
    f32 unknown2c;          // Inferred role: dark rate; original member name unknown.
  };

  UICtrlRevolveSelect(u32 object);
  virtual ~UICtrlRevolveSelect();

  void reset();
  f32 getAngleStep();
  void resetAngelRange(u16 range);
  void getCenterPos(mm::Pnt<s16> &pos) const;
  bool isRolling() const;
  bool isMoving() const;

  RevolveSetting unknown2c;   // Inferred role: setting; original member name unknown.
  u32 unknown5c;              // Inferred role: current angle; original member name unknown.
  UISelectObjList *unknown60; // Inferred role: list; original member name unknown.
  s32 unknown68;              // Inferred role: index; original member name unknown.
  f32 unknown6c;              // Inferred role: target angle; original member name unknown.
  f32 unknown70;              // Inferred role: angle; original member name unknown.
  UICtrlSimple<1> unknown78;  // Inferred role: roll; original member name unknown.
  mm::Pnt<s16> unknowna0;     // Inferred role: center offset; original member name unknown.
  UICtrlSimple<2> unknowna8;  // Inferred role: move; original member name unknown.
  u32 unknownd8;              // Inferred role: rotating object; original member name unknown.
};

static_assert(offsetof(UICtrlRevolveSelect, unknown2c) == 0x2c, "");
static_assert(offsetof(UICtrlRevolveSelect, unknown5c) == 0x5c, "");
static_assert(offsetof(UICtrlRevolveSelect, unknown60) == 0x60, "");
static_assert(offsetof(UICtrlRevolveSelect, unknown68) == 0x68, "");
static_assert(offsetof(UICtrlRevolveSelect, unknown6c) == 0x6c, "");
static_assert(offsetof(UICtrlRevolveSelect, unknown70) == 0x70, "");
static_assert(offsetof(UICtrlRevolveSelect, unknown78) == 0x78, "");
static_assert(offsetof(UICtrlRevolveSelect, unknowna0) == 0xa0, "");
static_assert(offsetof(UICtrlRevolveSelect, unknowna8) == 0xa8, "");
static_assert(offsetof(UICtrlRevolveSelect, unknownd8) == 0xd8, "");
static_assert(sizeof(UICtrlRevolveSelect) == 0xe0, "");

} // namespace ui
