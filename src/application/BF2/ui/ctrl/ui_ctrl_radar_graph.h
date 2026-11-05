#pragma once
#include <stddef.h>

#include "mm_mtl/array.h"
#include "types.h"
#include "ui/ctrl/ui_ctrl_object.h"
#include "ui/ctrl/ui_ctrl_simple.h"

namespace ui {

class UIObjectNumberPlugin;

class UICtrlRadarGraph : public UICtrlObject {
public:
  struct Setting {
    u32 unknown00; // Role and original member name unknown.
    u32 unknown04; // Role and original member name unknown.
  };

  struct Value {
    s32 unknown00;                   // Inferred role: current value; original member name unknown.
    UIObjectNumberPlugin *unknown08; // Inferred role: number plugin; original member name unknown.
    UICtrlSimple<1> unknown10;       // Inferred role: interpolation; original member name unknown.
  };

  UICtrlRadarGraph(u32 object);
  virtual ~UICtrlRadarGraph();
  virtual void update(const fw::UpdateInfo &info);
  void setNumberPlugin(u32 index, u32 handle);

  mm::mtl::Array<Value> unknown30; // Inferred role: values; original member name unknown.
  Setting unknown48;               // Inferred role: setting; original member name unknown.
};

static_assert(offsetof(UICtrlRadarGraph::Value, unknown08) == 0x08, "");
static_assert(offsetof(UICtrlRadarGraph::Value, unknown10) == 0x10, "");
static_assert(sizeof(UICtrlRadarGraph::Value) == 0x38, "");
static_assert(offsetof(UICtrlRadarGraph, unknown30) == 0x30, "");
static_assert(offsetof(UICtrlRadarGraph, unknown48) == 0x48, "");
static_assert(sizeof(UICtrlRadarGraph) == 0x50, "");

} // namespace ui
