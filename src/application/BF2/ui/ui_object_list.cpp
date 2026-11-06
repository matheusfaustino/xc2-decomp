#include "ui/ui_object_list.h"

#include "mm_mtl/bitflag.h"
#include "mm_mtl/fixstr.h"
#include "mm_mtl/hash.h"
#include "mm_mtl/rtti.h"
#include "ui/object/ui_layout_object.h"
#include "ui/object/ui_text_object.h"
#include "ui/ui_object_base.h"
#include "ui/ui_util.h"

namespace ui {

UIObjectList::UIObjectList() : unknown20(false), unknown21(false) {}

UIObjectList::~UIObjectList() { clear(); }

u32 UIObjectList::count(const char *name, u32 start) {
  mm::mtl::FixStr<32> formatted;
  formatted.format(name, start);
  u32 hash = mm::mtl::hashStrCrc(formatted.c_str());
  u32 result = 0;
  for (UIObject *it = unknown08.begin().get(); it != unknown08.end().get();) {
    if (it->unknownec == hash) {
      result++;
      formatted.format(name, result + start);
      hash = mm::mtl::hashStrCrc(formatted.c_str());
      it = unknown08.begin().get();
    } else {
      it = static_cast<UIObject *>(it->unknown00);
    }
  }
  return result;
}

void UIObjectList::reloadText() {
  for (mm::mtl::IntrusiveList<UIObject>::iterator it = unknown08.begin(); it != unknown08.end(); ++it) {
    UIObject &object = *it;
    if (object.unknownd8 == UI_OBJECT_TYPE_TEXT) {
      UITextObject *text = object.getRTTI()->isKindOf(&UITextObject::m_rtti) ? static_cast<UITextObject *>(&object) : nullptr;
      text->reloadText();
    } else if (object.unknownd8 == UI_OBJECT_TYPE_LAYOUT) {
      UILayoutObject *layout = object.getRTTI()->isKindOf(&UILayoutObject::m_rtti) ? static_cast<UILayoutObject *>(&object) : nullptr;
      layout->reloadText();
    }
  }
}

u32 UIObjectList::search(const char *name) {
  if (name == nullptr) {
    return 0;
  }
  u32 hash = mm::mtl::hashStrCrc(name);
  for (UIObject *it = unknown08.begin().get(); it != unknown08.end().get(); it = static_cast<UIObject *>(it->unknown00)) {
    if (it->unknownec == hash) {
      return it->unknowne8;
    }
  }
  return 0;
}

u32 UIObjectList::searchDuplicate(const char *name, u16 index) {
  if (name == nullptr) {
    return 0;
  }
  u32 hash = mm::mtl::hashStrCrc(name);
  u32 result = 0;
  for (UIObject *it = unknown08.begin().get(); it != unknown08.end().get(); it = static_cast<UIObject *>(it->unknown00)) {
    if (it->unknown98.test(UI_OBJECT_FLAG_DUPLICATE) && it->unknown9c == index && it->unknownec == hash) {
      result = it->unknowne8;
      break;
    }
  }
  return result;
}

u32 UIObjectList::regist(UIObject *object, const char *name) {
  if (object == nullptr) {
    return 0;
  }
  for (mm::mtl::IntrusiveList<UIObject>::iterator it = unknown08.begin(); it != unknown08.end(); ++it) {
    if (it.get() == object) {
      return 0;
    }
  }
  object->addRef();
  unknown08.push_back(object);
  return object->unknowne8;
}

void UIObjectList::release(UIObject *object) {
  for (mm::mtl::IntrusiveList<UIObject>::iterator it = unknown08.begin(); it != unknown08.end(); ++it) {
    if (it.get() == object) {
      unknown08.erase(it);
      releaseObject(object);
      return;
    }
  }
}

void UIObjectList::release(const char *name) {
  if (name == nullptr) {
    return;
  }
  u32 hash = mm::mtl::hashStrCrc(name);
  for (mm::mtl::IntrusiveList<UIObject>::iterator it = unknown08.begin(); it != unknown08.end(); ++it) {
    UIObject &object = *it;
    if (object.unknownec == hash) {
      unknown08.erase(it);
      UIUtil::releaseObject(object.unknowne8);
      object.release();
      return;
    }
  }
}

void UIObjectList::releaseObject(UIObject *object) {
  UIUtil::releaseObject(object->unknowne8);
  // The game tests the pointer only after reading its handle.
  if (object != nullptr) {
    object->release();
  }
}

void UIObjectList::clear() {
  for (mm::mtl::IntrusiveList<UIObject>::iterator it = unknown08.begin(); it != unknown08.end();) {
    UIObject *object = it.get();
    it = unknown08.erase(it);
    if (object != nullptr) {
      object->setParent(0);
      releaseObject(object);
    }
  }
}

void UIObjectList::apply() {
  for (mm::mtl::IntrusiveList<UIObject>::iterator it = unknown08.begin(); it != unknown08.end(); ++it) {
    UIObject &object = *it;
    object.apply();
    object.resetDirty();
  }
}

u32 UIObjectList::countActive() {
  u32 count = 0;
  for (mm::mtl::IntrusiveList<UIObject>::iterator it = unknown08.begin(); it != unknown08.end(); ++it) {
    if (!it.get()->unknown98.test(UI_OBJECT_FLAG_SUSPEND)) {
      count++;
    }
  }
  return count;
}

} // namespace ui
