#pragma once
#include <stddef.h>

#include "mm_math/mm_pnt.h"
#include "mm_math/mm_rect.h"
#include "types.h"
#include "ui/ui_object.h"

namespace mm {
namespace mtl {
class RTTI;
} // namespace mtl
} // namespace mm

namespace ui {

class UILayoutData;
class UIObjectList;

class UILayoutObject : public UIObject {
public:
  static mm::mtl::RTTI m_rtti;

  virtual ~UILayoutObject();
  virtual void update();
  virtual const mm::mtl::RTTI *getRTTI() const;
  virtual void setInitialize();
  virtual void getDisplayOffset(u16 mode, mm::Pnt<s16> &offset) const;
  void setLayoutData(UILayoutData *layoutData);
  void setLayoutData(UILayoutObject *layoutObject);
  bool isTermWithChild() const;
  void reloadText();
  u32 addChild(u32 parent, UIObject *child);
  u32 countChild(const char *name, u32 length) const;
  u32 countActiveChild() const;
  u32 searchChild(const char *name) const;
  u32 searchDuplicatedChild(const char *name, u16 index) const;
  void setViewport(const mm::Rect<s16> &viewport);
  void updateSize();

  UIObjectList *unknown108; // Inferred role: child object list; original member name unknown.
  UILayoutData *unknown110; // Inferred role: layout data; original member name unknown.
  mm::Pnt<s16> unknown118;  // Inferred role: display offset; original member name unknown.
};

static_assert(offsetof(UILayoutObject, unknown108) == 0x108, "");
static_assert(offsetof(UILayoutObject, unknown110) == 0x110, "");
static_assert(offsetof(UILayoutObject, unknown118) == 0x118, "");
static_assert(sizeof(UILayoutObject) == 0x120, "");

} // namespace ui
