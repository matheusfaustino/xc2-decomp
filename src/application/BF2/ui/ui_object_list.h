#pragma once
#include <stddef.h>

#include "mm_mtl/intrusive_list.h"
#include "types.h"
#include "ui/ui_object.h" // IWYU pragma: keep

namespace ui {

class UIObjectList {
public:
  UIObjectList();
  virtual ~UIObjectList();
  void clear();
  void update();

  u32 regist(UIObject *object, const char *name);
  void release(UIObject *object);
  void release(const char *name);
  u32 count(const char *name, u32 start);
  u32 search(const char *name);
  u32 searchDuplicate(const char *name, u16 index);
  u32 countActive();
  void releaseObject(UIObject *object);
  void apply();
  void reloadText();

  mm::mtl::IntrusiveList<UIObject> unknown08; // Inferred role: objects; original member name unknown.
  bool unknown20;                             // Inferred role: initialized; original member name unknown.
  bool unknown21;                             // Inferred role: needs update; original member name unknown.
};

static_assert(offsetof(UIObjectList, unknown08) == 0x08, "");
static_assert(offsetof(UIObjectList, unknown20) == 0x20, "");
static_assert(offsetof(UIObjectList, unknown21) == 0x21, "");
static_assert(sizeof(UIObjectList) == 0x28, "");

} // namespace ui
