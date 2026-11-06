#pragma once
#include <stddef.h>

#include "mm_math/mm_pnt.h"
#include "mm_mtl/array.h"
#include "types.h"
#include "ui/ctrl/ui_ctrl_curve.h"
#include "ui/ctrl/ui_ctrl_object.h"
#include "ui/ctrl/ui_ctrl_simple.h"

namespace ui {

class UISelectObjList;

class UICtrlSlideSelect : public UICtrlObject {
public:
  struct SlideSetting {
    SlideSetting() : unknown00(0.0f), unknown04(CURVE_TYPE_LINEAR), unknown08(1), unknown14(1.0f), unknown18(1.5f) {}

    f32 unknown00;          // Inferred role: slide time; original member name unknown.
    CURVE_TYPE unknown04;   // Inferred role: slide curve; original member name unknown.
    u32 unknown08;          // Inferred role: visible count; original member name unknown.
    mm::Pnt<s16> unknown0c; // Inferred role: origin; original member name unknown.
    mm::Pnt<s16> unknown10; // Inferred role: step; original member name unknown.
    f32 unknown14;          // Inferred role: normal scale; original member name unknown.
    f32 unknown18;          // Inferred role: selected scale; original member name unknown.
  };

  UICtrlSlideSelect(u32 object);
  static UICtrlSlideSelect *create(u32 object, const SlideSetting &setting, UISelectObjList *list);
  virtual ~UICtrlSlideSelect();

  void updateObjectPos();
  void setSelIndex(int index, int prev, int next, bool immediate);
  virtual void update(const fw::UpdateInfo &info);

  SlideSetting unknown2c;                    // Inferred role: setting; original member name unknown.
  UISelectObjList *unknown48;                // Inferred role: list; original member name unknown.
  s32 unknown50;                             // Inferred role: index; original member name unknown.
  mm::mtl::Array<UICtrlSimple<4>> unknown58; // Inferred role: slots; original member name unknown.
};

static_assert(sizeof(UICtrlSlideSelect::SlideSetting) == 0x1c, "");
static_assert(offsetof(UICtrlSlideSelect, unknown2c) == 0x2c, "");
static_assert(offsetof(UICtrlSlideSelect, unknown48) == 0x48, "");
static_assert(offsetof(UICtrlSlideSelect, unknown50) == 0x50, "");
static_assert(offsetof(UICtrlSlideSelect, unknown58) == 0x58, "");
static_assert(sizeof(UICtrlSlideSelect) == 0x70, "");

} // namespace ui
