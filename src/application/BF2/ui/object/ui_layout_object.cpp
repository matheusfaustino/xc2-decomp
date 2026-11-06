#include "ui/object/ui_layout_object.h"

#include "layer/layer_acc_layout_obj.h"
#include "layer/layer_acc_manager.h"
#include "mm_math/mm_rect.h"
#include "mm_math/mm_vec2.h"
#include "mm_mtl/bitflag.h"
#include "mm_mtl/fixstr.h"
#include "mm_mtl/intrusive_list.h"
#include "mm_mtl/rtti.h"
#include "ui/layout/ui_layout_data.h"
#include "ui/ui_displayinfo.h"
#include "ui/ui_object_base.h"
#include "ui/ui_object_list.h"
#include "ui/ui_transform.h"

namespace ui {

UILayoutObject::~UILayoutObject() {
  if (unknown110 != nullptr) {
    unknown110->release();
    unknown110 = nullptr;
  }
  if (unknown108 != nullptr) {
    delete unknown108;
    unknown108 = nullptr;
  }
}

void UILayoutObject::update() {
  u32 flags = unknown98.unknown001(0);
  bool term;
  if ((flags & (1 << UI_OBJECT_FLAG_DUPLICATE)) != 0 && getParentPtr() != nullptr) {
    term = getParentPtr()->unknown98.test(UI_OBJECT_FLAG_TERM);
  } else {
    term = (flags & (1 << UI_OBJECT_FLAG_TERM)) != 0;
  }
  if (!term && unknown98.test(UI_OBJECT_FLAG_SUSPEND)) {
    return;
  }
  UIObject::update();
  if (unknown108 == nullptr || unknown108->unknown20) {
    return;
  }
  unknown108->update();
  if (unknowndc == 0) {
    return;
  }
  if (unknown20.unknown24 || unknown108->unknown21) {
    updateSize();
  }
}

const mm::mtl::RTTI *UILayoutObject::getRTTI() const { return &m_rtti; }

void UILayoutObject::setLayoutData(UILayoutData *layoutData) {
  unknown110 = layoutData;
  layoutData->addRef();
}

void UILayoutObject::setLayoutData(UILayoutObject *layoutObject) {
  unknown110 = layoutObject->unknown110;
  unknown110->addRef();
}

bool UILayoutObject::isTermWithChild() const {
  if (unknown108 == nullptr) {
    return isTerm();
  }
  if (!isTerm()) {
    return false;
  }
  return unknown108->unknown20;
}

void UILayoutObject::reloadText() {
  if (unknown108 != nullptr) {
    unknown108->reloadText();
  }
}

u32 UILayoutObject::addChild(u32 parent, UIObject *child) {
  u32 handle = unknown108->regist(child, child->unknowna0.c_str());
  if (handle != 0) {
    child->setParent(parent);
  }
  return handle;
}

void UILayoutObject::setInitialize() {
  UIObjectBase::setInitialize();
  if (unknown108 != nullptr) {
    unknown108->unknown20 = false;
  }
}

void UILayoutObject::getDisplayOffset(u16 mode, mm::Pnt<s16> &offset) const {
  offset = unknown118;
  if (mode == UI_DISPLAY_MODE_CENTER) {
    offset.x += static_cast<s16>(static_cast<s32>(static_cast<f32>(unknown50.unknown2c) * unknown20.unknown10.x * 0.5f));
    offset.y += static_cast<s16>(static_cast<s32>(static_cast<f32>(unknown50.unknown2e) * unknown20.unknown10.y * 0.5f));
  }
}

u32 UILayoutObject::countActiveChild() const {
  if (unknown108 == nullptr || unknown98.test(UI_OBJECT_FLAG_SUSPEND)) {
    return 0;
  }
  u32 count = unknown108->countActive();
  UIObjectList *list = unknown108;
  for (UIObject *it = list->unknown08.begin().get(); it != list->unknown08.end().get(); it = static_cast<UIObject *>(it->unknown00)) {
    if (it->unknownd8 != UI_OBJECT_TYPE_LAYOUT) {
      continue;
    }
    UILayoutObject *child = it->getRTTI()->isKindOf(&UILayoutObject::m_rtti) ? static_cast<UILayoutObject *>(it) : nullptr;
    count += child->countActiveChild();
  }
  return count;
}

void UILayoutObject::setViewport(const mm::Rect<s16> &viewport) {
  layer::LayerAccLayoutObj acc(unknownc8);
  layer::LayerAccManager manager;
  acc.setViewPort(static_cast<f32>(viewport.x), static_cast<f32>(viewport.y), static_cast<f32>(viewport.width), static_cast<f32>(viewport.height));
  acc.setViewPortMode(true);
}

} // namespace ui
