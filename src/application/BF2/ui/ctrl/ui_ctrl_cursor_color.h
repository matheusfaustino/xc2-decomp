#pragma once
#include <stddef.h>

#include "types.h"
#include "ui/ctrl/ui_ctrl_object.h"

namespace ui {

class UISelectObjList;

class UICtrlCursorColor : public UICtrlObject {
public:
  struct CursorSetting {
    CursorSetting() : unknown00(nullptr), unknown08(nullptr), unknown10("txt_normal2"), unknown18("txt_normal2") {}

    const char *unknown00; // Inferred role: cursor child name format; original member name unknown.
    const char *unknown08; // Inferred role: color child name format; original member name unknown.
    const char *unknown10; // Inferred role: normal color name; original member name unknown.
    const char *unknown18; // Inferred role: selected color name; original member name unknown.
  };

  UICtrlCursorColor(u32 object);
  virtual ~UICtrlCursorColor();
  virtual void update(const fw::UpdateInfo &info);

  static UICtrlObject *create(u32 object, const CursorSetting &setting, UISelectObjList *list);
  void open(s32 index);
  bool inputChange(u32 index);
  void setDisp(bool disp);
  void setIndex(u32 index);

  CursorSetting unknown30;    // Inferred role: setting; original member name unknown.
  UISelectObjList *unknown50; // Inferred role: list; original member name unknown.
  s32 unknown58;              // Inferred role: index; original member name unknown.
  u32 unknown5c;              // Inferred role: object; original member name unknown.
};

static_assert(offsetof(UICtrlCursorColor, unknown30) == 0x30, "");
static_assert(offsetof(UICtrlCursorColor, unknown50) == 0x50, "");
static_assert(offsetof(UICtrlCursorColor, unknown58) == 0x58, "");
static_assert(offsetof(UICtrlCursorColor, unknown5c) == 0x5c, "");
static_assert(sizeof(UICtrlCursorColor) == 0x60, "");

} // namespace ui
