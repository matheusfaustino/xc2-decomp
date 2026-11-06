#include "ui/ui_group.h"

#include "framework/memory/framework_memory.h"
#include "framework/resource/common_resource.h"
#include "mm_mtl/bitflag.h"
#include "mm_mtl/intrusive_list.h"
#include "ui/event/ui_event_data.h"
#include "ui/event/ui_event_manager.h"
#include "ui/input/ui_input_layer.h"
#include "ui/input/ui_input_manager.h"
#include "ui/object/ui_layout_object.h"
#include "ui/ui_displayinfo.h"
#include "ui/ui_object.h"
#include "ui/ui_object_base.h"
#include "ui/ui_object_list.h"
#include "ui/ui_resource.h"
#include "ui/ui_util.h"

namespace ui {

UIGroup::UIGroup() : unknown08(nullptr), unknown10(), unknown38(), unknown60(0) {}

UIGroup::~UIGroup() { clean(); }

void UIGroup::clean() {
  if (unknown10.capacity() != 0) {
    for (RootObjInfo *it = unknown10.begin(); it != unknown10.end(); ++it) {
      if (it->unknown50 != nullptr) {
        it->unknown50->release();
        it->unknown50 = nullptr;
      }
      releaseInput(it->unknown44);
    }
    unknown10.destroy();
  }
  if (unknown38.capacity() != 0) {
    // The game never advances the iterator in this loop and clears unknown10 again below instead of unknown38.
    for (InputInfo *it = unknown38.begin(); it != unknown38.end();) {
      if (it->unknown00 != nullptr) {
        it->unknown00->release();
        it->unknown00 = nullptr;
      }
    }
    unknown10.destroy();
  }
  if (unknown08 != nullptr) {
    delete unknown08;
    unknown08 = nullptr;
  }
  unknown60 = 0;
}

void UIGroup::setup(u32 size) {
  if (size == 0) {
    return;
  }
  if (unknown08 != nullptr) {
    delete unknown08;
    unknown08 = nullptr;
  }
// __LINE__ is the line argument of the operator new call.
#line 60
  unknown08 = new (UIUtil::getProgAllocator(), 16, __FILE__, __LINE__) UIObjectList;
  unknown10.setAllocator(UIUtil::getProgAllocator());
  unknown10.reserve(size);
  unknown38.setAllocator(UIUtil::getProgAllocator());
  unknown38.reserve(size);
}

void UIGroup::disp(bool disp) {
  if (disp) {
    unknown60 &= ~4;
  } else {
    unknown60 |= 4;
  }
}

bool UIGroup::hasObject() const { return unknown10.size() != 0; }

int UIGroup::registObject(UIObject *object, const char *name, u16 layer, UIResource *resource) {
  if (unknown08 == nullptr) {
    return 0;
  }

  RootObjInfo info;
  info.unknown44 = unknown08->regist(object, name);
  if (info.unknown44 == 0) {
    return 0;
  }

  info.unknown00 = name;
  info.unknown48 = layer;
  info.unknown50 = resource;
  info.unknown5a = false;
  info.unknown5b = false;
  info.unknown58 = 0;
  resource->addRef();
  unknown10.push_back(info);
  return info.unknown44;
}

void UIGroup::applyObject() {
  if (unknown08 == nullptr || (unknown60 & 2) != 0) {
    return;
  }

  if ((unknown60 & 4) != 0) {
    for (RootObjInfo *it = unknown10.begin(); it != unknown10.end(); ++it) {
      UIObject *object = UIUtil::getObject(it->unknown44);
      it->unknown5c = object->unknown50.unknown08;
      object->unknown50.setDisp(false);
    }
  }

  unknown08->apply();

  if ((unknown60 & 4) != 0) {
    for (RootObjInfo *it = unknown10.begin(); it != unknown10.end(); ++it) {
      UIObject *object = UIUtil::getObject(it->unknown44);
      object->unknown50.setDisp(it->unknown5c);
    }
  }
}

u32 UIGroup::searchObject(const char *name) { return unknown08 != nullptr ? unknown08->search(name) : 0; }

bool UIGroup::isActiveLayer(u16 layer) {
  for (RootObjInfo *it = unknown10.begin(); it != unknown10.end(); ++it) {
    if (it->unknown48 == layer) {
      return true;
    }
  }
  return false;
}

UIGroup::RootObjInfo *UIGroup::getRootInfo(u32 handle) {
  for (RootObjInfo *it = unknown10.begin(); it != unknown10.end(); ++it) {
    if (it->unknown44 == handle) {
      return it;
    }
  }
  return nullptr;
}

bool UIGroup::isAssignedLayer(const char *name, u32 layer) {
  u32 handle = searchObject(name);
  if (handle == 0) {
    return false;
  }
  RootObjInfo *info = getRootInfo(handle);
  if (info == nullptr) {
    return false;
  }
  return info->unknown48 == layer;
}

void UIGroup::initializeObject(const char *name) {
  u32 handle = searchObject(name);
  if (handle == 0) {
    return;
  }
  RootObjInfo *info = getRootInfo(handle);
  if (info == nullptr) {
    return;
  }
  readyReInitialize(handle);
  info->unknown5a = false;
  info->unknown5b = false;
}

void UIGroup::readyTerminate(u32 handle) {
  UIObject *object = UIUtil::getObject(handle);
  if (object->unknown98.test(UI_OBJECT_FLAG_LISTENER)) {
    UIEventManager::EventInfo info;
    info.unknown08.unknown00 = 2;
    UIEventManager *eventManager = UIEventManager::get();
    info.unknown00 = handle;
    eventManager->pushObjectEvent(info);
  } else {
    object->setTerm();
  }
  if (object->unknownd8 == UI_OBJECT_TYPE_LAYOUT) {
    UIObjectList *list = UIUtil::getLayoutObject(handle)->unknown108;
    for (mm::mtl::IntrusiveList<UIObject>::iterator it = list->unknown08.begin(); it != list->unknown08.end(); ++it) {
      readyTerminate(it.get()->unknowne8);
    }
  }
}

void UIGroup::terminateObject(const char *name, bool isImmediate) {
  u32 handle = searchObject(name);
  if (handle == 0) {
    return;
  }
  RootObjInfo *info = getRootInfo(handle);
  if (info == nullptr) {
    return;
  }
  releaseInput(handle);
  readyTerminate(handle);
  info->unknown5a = true;
  info->unknown5b = isImmediate;
}

void UIGroup::releaseObject(const char *name) {
  u32 handle = searchObject(name);
  if (handle == 0) {
    return;
  }
  RootObjInfo *info = getRootInfo(handle);
  if (info == nullptr) {
    return;
  }
  info->unknown5b = true;
}

void UIGroup::terminateObjectAll(bool isImmediate) {
  for (RootObjInfo *it = unknown10.begin(); it != unknown10.end(); ++it) {
    releaseInput(it->unknown44);
    readyTerminate(it->unknown44);
    it->unknown5a = true;
    it->unknown5b = isImmediate;
  }
}

void UIGroup::attachInput(const char *name, UIInputLayer *input) {
  if (unknown08 == nullptr) {
    return;
  }
  u32 object = unknown08->search(name);
  if (object == 0) {
    return;
  }
  UIObject *inputObject = UIUtil::getObject(input->unknown40);
  mm::mtl::FixStr<64> inputName;
  inputName.format("%s_%s", name, inputObject->unknowna0.c_str());
  u32 handle = UIInputManager::get()->registInputLayer(input, inputName.c_str());
  if (handle == 0) {
    return;
  }
  input->addRef();
  InputInfo info;
  info.unknown00 = input;
  info.unknown08 = handle;
  info.unknown0c = object;
  unknown38.push_back(info);
}

void UIGroup::suspend(bool suspend) {
  // The game sets bit 0 in both cases; only a suspension also sets bit 1.
  if (suspend) {
    unknown60 |= 3;
  } else {
    unknown60 |= 1;
  }
  for (InputInfo *it = unknown38.begin(); it != unknown38.end(); ++it) {
    if (it->unknown08 != 0) {
      UIInputManager::get()->releaseInputLayer(it->unknown08);
      it->unknown08 = 0;
    }
  }
}

void UIGroup::resume() {
  unknown60 &= ~3;
  for (InputInfo *it = unknown38.begin(); it != unknown38.end(); ++it) {
    if (it->unknown08 == 0) {
      it->unknown08 = UIInputManager::get()->registInputLayer(it->unknown00, getRootInfo(it->unknown0c)->unknown00.c_str());
    }
  }
}

u32 UIGroup::getLoadedResourceSize() {
  u32 size = 0;
  for (RootObjInfo *it = unknown10.begin(); it != unknown10.end(); ++it) {
    UIResource *resource = it->unknown50;
    for (u32 i = 0; i < resource->unknown30.size(); ++i) {
      size += resource->unknown30[i]->getAssetSize();
    }
  }
  return size;
}

void UIGroup::releaseInput(u32 handle) {
  for (InputInfo *it = unknown38.begin(); it != unknown38.end();) {
    if (it->unknown0c == handle) {
      if (it->unknown08 != 0) {
        UIInputManager::get()->releaseInputLayer(it->unknown08);
      }
      if (it->unknown00 != nullptr) {
        it->unknown00->release();
        it->unknown00 = nullptr;
      }
      it = unknown38.erase(it);
    } else {
      ++it;
    }
  }
}

void UIGroup::reloadText() {
  for (RootObjInfo *it = unknown10.begin(); it != unknown10.end(); ++it) {
    UIUtil::getLayoutObject(it->unknown44)->reloadText();
  }
}

} // namespace ui
