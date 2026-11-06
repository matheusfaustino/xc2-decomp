#pragma once
#include <stddef.h>

#include "mm_math/mm_pnt.h"
#include "types.h"
#include "ui/ctrl/ui_ctrl_curve.h"
#include "ui/ctrl/ui_ctrl_object.h"
#include "ui/ctrl/ui_ctrl_simple.h"

namespace ui {

class UISelectObjList;

class UICtrlCursorSelect : public UICtrlObject {
public:
  struct CursorSetting {
    CursorSetting() : unknown00(0.0f), unknown04(CURVE_TYPE_LINEAR), unknown08(nullptr), unknown10(nullptr), unknown18(1), unknown1a(0), unknown1c(0), unknown1e(0), unknown1f(0), unknown20(true) {}

    void setType(int type);

    f32 unknown00;         // Inferred role: move time; original member name unknown.
    CURVE_TYPE unknown04;  // Inferred role: move curve; original member name unknown.
    const char *unknown08; // Inferred role: animation at the end of the move; original member name unknown.
    const char *unknown10; // Inferred role: animation at the start of the move; original member name unknown.
    u16 unknown18;         // Inferred role: dock type; original member name unknown.
    s16 unknown1a;         // Inferred role: offset x; original member name unknown.
    s16 unknown1c;         // Inferred role: offset y; original member name unknown.
    u8 unknown1e;          // Inferred role: x axis fixed; original member name unknown.
    u8 unknown1f;          // Inferred role: y axis fixed; original member name unknown.
    bool unknown20;        // Inferred role: global; original member name unknown.
  };

  UICtrlCursorSelect(u32 object);
  virtual ~UICtrlCursorSelect();

  void setIndex(u32 index);
  bool inputChange(u32 index);
  void setSelIndex(u32 index, bool immediate);
  void setDisp(bool disp);
  void setAddColor(const char *name);
  void setMulColor(const char *name);
  void setAlpha(f32 alpha);

  CursorSetting unknown30;    // Inferred role: setting; original member name unknown.
  UISelectObjList *unknown58; // Inferred role: list; original member name unknown.
  s32 unknown60;              // Inferred role: index; original member name unknown.
  UICtrlSimple<2> unknown68;  // Inferred role: move; original member name unknown.
  u32 unknown98;              // Inferred role: cursor object; original member name unknown.
  mm::Pnt<s16> unknown9c;     // Inferred role: cursor position; original member name unknown.
  bool unknowna0;             // Inferred role: disp; original member name unknown.
};

static_assert(offsetof(UICtrlCursorSelect::CursorSetting, unknown18) == 0x18, "");
static_assert(offsetof(UICtrlCursorSelect::CursorSetting, unknown20) == 0x20, "");
static_assert(offsetof(UICtrlCursorSelect, unknown30) == 0x30, "");
static_assert(offsetof(UICtrlCursorSelect, unknown58) == 0x58, "");
static_assert(offsetof(UICtrlCursorSelect, unknown60) == 0x60, "");
static_assert(offsetof(UICtrlCursorSelect, unknown68) == 0x68, "");
static_assert(offsetof(UICtrlCursorSelect, unknown98) == 0x98, "");
static_assert(offsetof(UICtrlCursorSelect, unknown9c) == 0x9c, "");
static_assert(offsetof(UICtrlCursorSelect, unknowna0) == 0xa0, "");
static_assert(sizeof(UICtrlCursorSelect) == 0xa8, "");

} // namespace ui
