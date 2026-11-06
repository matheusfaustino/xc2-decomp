#include "ui/ctrl/ui_ctrl_manager.h"

#include "mm_math/mm_pnt.h"
#include "mm_math/mm_vec2.h"
#include "mm_mtl/reference_object.h"
#include "ui/ctrl/ui_ctrl_object.h"
#include "ui/event/ui_event_data.h"
#include "ui/event/ui_event_manager.h"
#include "ui/motion/ui_motion_library.h"
#include "ui/ui_object.h"
#include "ui/ui_object_base.h"
#include "ui/ui_util.h"

namespace ui {
class UIMotionData;

UICtrlManager::UICtrlManager() : unknown10() {
  for (u32 i = 0; i < CTRL_SLOT_MAX; i++) {
    unknown10[i].unknown1d8 = nullptr;
    unknown10[i].unknown1d4 = 0;
    unknown10[i].unknown1e8 = 0;
    unknown10[i].unknownf0.stop();
  }
}

void UICtrlManager::releaseCtrlSlot(int slot) {
  if (unknown10[slot].unknown1d8 == nullptr) {
    return;
  }

  unknown10[slot].unknown1d8->unknown98.set(UI_OBJECT_FLAG_CONTROL, false);
  if (unknown10[slot].unknown1e4.test(CTRL_FLAG_TERM)) {
    unknown10[slot].unknown1d8->setTerm();
  }
  unknown10[slot].unknownf0.stop();
  mm::mtl::safeRelease(unknown10[slot].unknown1d8);
  unknown10[slot].unknown1d4 = 0;
  unknown10[slot].unknown1e8 = 0;
}

void UICtrlManager::setDefault(int slot) {
  if (isValidSlot(slot) && unknown10[slot].unknown1d8 != nullptr) {
    unknown10[slot].unknown1d8->unknown20 = unknown10[slot].unknown1f0;
    unknown10[slot].unknown1d8->unknown50 = unknown10[slot].unknown220;
  }
}

void UICtrlManager::stopAll() {
  for (int i = 0; i < CTRL_SLOT_MAX; i++) {
    if (unknown10[i].unknownf0.isBusy()) {
      stopCtrl(i, true, true);
    }
  }
}

void UICtrlManager::stopCtrl(int slot, bool release, bool callback) {
  unknown10[slot].unknownf0.stop();
  if (callback && unknown10[slot].unknown1e8 != 0) {
    UIObject *handler = UIUtil::getObject(unknown10[slot].unknown1e8);
    if (handler != nullptr && handler->unknown98.test(UI_OBJECT_FLAG_LISTENER)) {
      UIEventManager::EventInfo event;
      event.unknown00 = unknown10[slot].unknown1e8;
      event.unknown08.unknown00 = 3;
      UIEventManager::get()->pushObjectEvent(event);
    }
  }
  if (unknown10[slot].unknown1e4.test(CTRL_FLAG_RESTORE)) {
    setDefault(slot);
  }
  if (release) {
    releaseCtrlSlot(slot);
  }
}

int UICtrlManager::reserveCtrlSlot(u32 handle, bool loop) {
  if (!UIUtil::isValid(handle)) {
    return -1;
  }

  for (u32 i = 0; i < CTRL_SLOT_MAX; i++) {
    if (unknown10[i].unknown1d4 == handle) {
      unknown10[i].unknown1e0 = loop;
      return i;
    }
  }

  for (u32 i = 0; i < CTRL_SLOT_MAX; i++) {
    if (unknown10[i].unknown1d8 == nullptr) {
      unknown10[i].unknown1d4 = handle;
      unknown10[i].unknown1e8 = handle;
      unknown10[i].unknown1d8 = UIUtil::getObject(handle);
      unknown10[i].unknown1d8->addRef();
      unknown10[i].unknown178.clear();
      unknown10[i].unknown1d0 = 0.0f;
      unknown10[i].unknown1e0 = loop;
      unknown10[i].unknown1e4.clear();
      unknown10[i].unknown1f0 = unknown10[i].unknown1d8->unknown20;
      unknown10[i].unknown220 = unknown10[i].unknown1d8->unknown50;
      return i;
    }
  }
  return -1;
}

int UICtrlManager::getCtrlSlot(u32 handle) {
  for (u32 i = 0; i < CTRL_SLOT_MAX; i++) {
    if (unknown10[i].unknown1d4 == handle) {
      return i;
    }
  }
  return -1;
}

void UICtrlManager::changeEventHandleObject(int slot, u32 handler) {
  if (slot != -1 && unknown10[slot].unknown1d8 != nullptr) {
    unknown10[slot].unknown1e8 = handler;
  }
}

void UICtrlManager::setDelayTime(int slot, f32 delay) {
  if (unknown10[slot].unknown1d8 != nullptr) {
    unknown10[slot].unknown1d0 = delay;
  }
}

void UICtrlManager::addPositionMethod(int slot, s16 x, s16 y, CURVE_TYPE curve) {
  if (unknown10[slot].unknown1d8 != nullptr) {
    unknown10[slot].unknown00.setup(unknown10[slot].unknown1d8, mm::Pnt<s16>(x, y));
    UICtrl::Method method;
    method.unknown00 = &unknown10[slot].unknown00;
    method.unknown08 = UIUtil::getCurve(curve);
    unknown10[slot].unknown178.push_back(method);
  }
}

void UICtrlManager::addScaleMethod(int slot, f32 x, f32 y, CURVE_TYPE curve) {
  if (unknown10[slot].unknown1d8 != nullptr) {
    alignas(16) mm::Vec2 scale = {x, y};
    unknown10[slot].unknown10.setup(unknown10[slot].unknown1d8, scale);
    UICtrl::Method method;
    method.unknown00 = &unknown10[slot].unknown10;
    method.unknown08 = UIUtil::getCurve(curve);
    unknown10[slot].unknown178.push_back(method);
  }
}

void UICtrlManager::addAlphaMethod(int slot, f32 alpha, CURVE_TYPE curve) {
  if (unknown10[slot].unknown1d8 != nullptr) {
    unknown10[slot].unknown40.setup(unknown10[slot].unknown1d8, alpha);
    UICtrl::Method method;
    method.unknown00 = &unknown10[slot].unknown40;
    method.unknown08 = UIUtil::getCurve(curve);
    unknown10[slot].unknown178.push_back(method);
  }
}

void UICtrlManager::addBrightMethod(int slot, f32 bright, CURVE_TYPE curve) {
  if (unknown10[slot].unknown1d8 != nullptr) {
    unknown10[slot].unknown50.setup(unknown10[slot].unknown1d8, bright);
    UICtrl::Method method;
    method.unknown00 = &unknown10[slot].unknown50;
    method.unknown08 = UIUtil::getCurve(curve);
    unknown10[slot].unknown178.push_back(method);
  }
}

void UICtrlManager::addRotMethod(int slot, f32 rot, CURVE_TYPE curve) {
  if (unknown10[slot].unknown1d8 != nullptr) {
    unknown10[slot].unknown60.setup(unknown10[slot].unknown1d8, rot);
    UICtrl::Method method;
    method.unknown00 = &unknown10[slot].unknown60;
    method.unknown08 = UIUtil::getCurve(curve);
    unknown10[slot].unknown178.push_back(method);
  }
}

void UICtrlManager::addMulColorMethod(int slot, const mm::Col3 &color, CURVE_TYPE curve) {
  if (unknown10[slot].unknown1d8 != nullptr) {
    unknown10[slot].unknown70.setup(unknown10[slot].unknown1d8, color);
    UICtrl::Method method;
    method.unknown00 = &unknown10[slot].unknown70;
    method.unknown08 = UIUtil::getCurve(curve);
    unknown10[slot].unknown178.push_back(method);
  }
}

void UICtrlManager::addAddColorMethod(int slot, const mm::Col3 &color, CURVE_TYPE curve) {
  if (unknown10[slot].unknown1d8 != nullptr) {
    unknown10[slot].unknown90.setup(unknown10[slot].unknown1d8, color);
    UICtrl::Method method;
    method.unknown00 = &unknown10[slot].unknown90;
    method.unknown08 = UIUtil::getCurve(curve);
    unknown10[slot].unknown178.push_back(method);
  }
}

void UICtrlManager::addMotionMethod(int slot, const char *motion, const char *name) {
  if (unknown10[slot].unknown1d8 == nullptr) {
    return;
  }

  UIMotionData *motionData = UIMotionLibrary::get()->find(motion);
  if (motionData != nullptr) {
    unknown10[slot].unknownb0.setup(unknown10[slot].unknown1d8, motionData, name);
    UICtrl::Method method;
    method.unknown00 = &unknown10[slot].unknownb0;
    method.unknown08 = UIUtil::getCurve(CURVE_TYPE_LINEAR);
    unknown10[slot].unknown178.push_back(method);
  }
}

void UICtrlManager::startCtrl(int slot, f32 duration, u32 flags, u32 id) {
  if (unknown10[slot].unknown1d8 == nullptr) {
    return;
  }
  unknown10[slot].unknownf0.start(unknown10[slot].unknown1d8, duration, unknown10[slot].unknown178, unknown10[slot].unknown1e0);
  unknown10[slot].unknown178.clear();
  unknown10[slot].unknown1e4.unknown002(flags);
  unknown10[slot].unknown1d8->unknown98.set(UI_OBJECT_FLAG_CONTROL);
  unknown10[slot].unknown1ec = id;
}

void UICtrlManager::getDefaultTransfom(int slot, UITransform &transform) {
  if (isValidSlot(slot) && unknown10[slot].unknown1d8 != nullptr) {
    transform = unknown10[slot].unknown1f0;
  }
}

void UICtrlManager::getDefaultDisplayInfo(int slot, UIDisplayInfo &displayInfo) {
  if (isValidSlot(slot) && unknown10[slot].unknown1d8 != nullptr) {
    displayInfo = unknown10[slot].unknown220;
  }
}

void UICtrlManager::setSpeedFactor(int slot, f32 speedFactor) {
  if (isValidSlot(slot) && unknown10[slot].unknown1d8 != nullptr) {
    unknown10[slot].unknownf0.unknown80 = speedFactor;
  }
}

void UICtrlManager::addCtrlObject(UICtrlObject *object) {
  if (object != nullptr) {
    object->addRef();
    unknown13810.push_back(object);
  }
}

void UICtrlManager::addUpdater(IUICtrlUpdater &updater) {
  if (!updater.intoNode()) {
    unknown13828.push_back(&updater);
  }
}

void UICtrlManager::delUpdater(IUICtrlUpdater &updater) {
  if (updater.intoNode()) {
    unknown13828.erase(&updater);
  }
}

bool UICtrlManager::isBusyCtrl(int slot) {
  bool busy = false;
  if (isValidSlot(slot) && unknown10[slot].unknown1d8 != nullptr) {
    busy = unknown10[slot].unknownf0.isBusy();
  }
  return busy;
}

} // namespace ui
