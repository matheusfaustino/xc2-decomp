#include "ui/ui_manager.h"

#include "bdat.h"
#include "framework/memory/framework_memory.h"
#include "mm_math/mm_col4.h"
#include "mm_mtl/fixstr.h"
#include "ui/event/ui_event_data.h"
#include "ui/event/ui_event_manager.h"
#include "ui/factory/ui_obj_factory.h"
#include "ui/motion/ui_motion_library.h"
#include "ui/plugin/ui_object_number_plugin.h"
#include "ui/text/ui_text_manager.h"
#include "ui/timer/ui_timer_manager.h"
#include "ui/ui_color_library.h"
#include "ui/ui_uio_library.h"
#include "ui/ui_util.h"

namespace ui {

namespace {

// Inferred role: calls an action for each sheet name of a created layer; original helper name unknown.
template <typename Action> void unknown001(UIManager &manager, u32 layer, Action action) {
  if (!manager.isLayerCreated(layer)) {
    return;
  }
  u8 *bdat = UIUtil::getBdatFP(UI_BDAT_LAYER);
  if (bdat == nullptr) {
    return;
  }

  u8 group = Bdat::getVal(bdat, "group", layer).valueU8;
  int memberSize = Bdat::getMemberSize(bdat);
  int sheetCount = (memberSize - 1) / 2;
  mm::mtl::FixStr<32> member;
  if (memberSize >= 3) {
    for (u32 i = 0; i < static_cast<u32>(sheetCount); i++) {
      member.format("sheet%02d", i + 1);
      const char *sheet = Bdat::getVal(bdat, member.c_str(), layer).valueString;
      if (sheet != nullptr && *sheet != '\0') {
        action(group, sheet);
      }
    }
  }
}

} // namespace

void UIManager::terminate() {
  for (int i = 0; i < GROUP_MAX; i++) {
    unknown08[i].terminateObjectAll(true);
  }
}

bool UIManager::isLayerCreated(u32 layer) {
  for (LayerInfo *it = unknown348.begin(); it != unknown348.end(); ++it) {
    if (it->unknown02 == layer) {
      return true;
    }
  }
  return false;
}

u32 UIManager::searchObject(const char *name, u16 group) {
  if (name != nullptr && group < GROUP_MAX && *name != '\0') {
    return unknown08[group].searchObject(name);
  }
  return 0;
}

bool UIManager::preloadLayer(u32 layer) {
  u8 *bdat = UIUtil::getBdatFP(UI_BDAT_LAYER);
  if (bdat == nullptr) {
    return false;
  }

  Bdat::getVal(bdat, "group", layer);
  int memberSize = Bdat::getMemberSize(bdat);
  int sheetCount = (memberSize - 1) / 2;
  mm::mtl::FixStr<32> member;
  if (memberSize >= 3) {
    for (u32 i = 0; i < static_cast<u32>(sheetCount); i++) {
      member.format("sheet%02d", i + 1);
      if (Bdat::getMember(bdat, member.c_str()) != nullptr) {
        const char *sheet = Bdat::getVal(bdat, member.c_str(), layer).valueString;
        if (sheet != nullptr && *sheet != '\0') {
          UIObjFactory::get()->registPreloadResource(1, sheet);
        }
      }
    }
  }
  return true;
}

void UIManager::initializeLayer(u32 layer) {
  unknown001(*this, layer, [this](u8 group, const char *sheet) { unknown08[group].initializeObject(sheet); });
}

void UIManager::terminateLayer(u32 layer, bool isImmediate) {
  unknown001(*this, layer, [this, layer, isImmediate](u8 group, const char *sheet) {
    if (unknown08[group].isAssignedLayer(sheet, layer)) {
      unknown08[group].terminateObject(sheet, isImmediate);
    }
  });
}

void UIManager::releaseLayer(u32 layer) {
  unknown001(*this, layer, [this](u8 group, const char *sheet) { unknown08[group].releaseObject(sheet); });
}

void UIManager::terminateObject(const char *name, u8 group, bool isImmediate) {
  if (name != nullptr && group < GROUP_MAX && *name != '\0') {
    unknown08[group].terminateObject(name, isImmediate);
  }
}

void UIManager::requestObjectEvent(u32 handle, const EventData &event, f32 delay) {
  if (UIUtil::isValid(handle)) {
    UIEventManager::EventInfo info;
    info.unknown00 = handle;
    info.unknown08 = event;
    info.unknown04 = delay;
    UIEventManager::get()->pushObjectEvent(info);
  }
}

void UIManager::requestGameEvent(u32 handle, const EventData &event) {
  if (UIUtil::isValid(handle)) {
    UIEventManager::EventInfo info;
    info.unknown00 = handle;
    info.unknown08 = event;
    UIEventManager::get()->pushGameEvent(info);
  }
}

void UIManager::setGameEventLisenter(UIGameListener *listener) { UIEventManager::get()->setGameListener(listener); }

void UIManager::setUpdateCallback(UIUpdateCallback *callback) { m_updateCallback = callback; }

void UIManager::registObjectTimer(u32 handle, u16 id, u64 time, bool repeat) { UITimerManager::get()->registTimer(handle, id, time, repeat); }

void UIManager::releaseObjectTimer(u32 handle) {
  u32 timer = UITimerManager::get()->getTimerHandle(handle);
  if (timer != 0) {
    UITimerManager::get()->releaseTimer(timer);
  }
}

void UIManager::setDisp(u16 group, bool disp) {
  if (group < GROUP_MAX) {
    unknown08[group].disp(disp);
  }
}

bool UIManager::isLoading() const { return UIObjFactory::get()->isLoading(); }

void UIManager::reloadText() {
  UIObjectNumberPlugin::refreshMoneySeparetor();
  UITextManager::get()->reloadAll();
  for (int i = 0; i < GROUP_MAX; i++) {
    unknown08[i].reloadText();
  }
}

u32 UIManager::getLoadedResourceSize() {
  u32 size = 0;
  for (int i = 0; i < GROUP_MAX; i++) {
    size += unknown08[i].getLoadedResourceSize();
  }
  return size;
}

bool UIManager::isTerminated() const {
  for (int i = 0; i < GROUP_MAX; i++) {
    if (unknown08[i].hasObject()) {
      return false;
    }
  }
  return true;
}

void UIManager::resetData() {
  UIUtil::initBdatFP();
  if (UIColorLibrary::get() == nullptr) {
// __LINE__ is the line argument of the operator new call.
#line 282
    new (UIUtil::getProgAllocator(), 16, __FILE__, __LINE__) UIColorLibrary();
  }
  u8 *colorBdat = UIUtil::getBdatFP(UI_BDAT_COLOR_LIST);
  if (colorBdat != nullptr) {
    for (int i = 0; i < Bdat::getIdCount(colorBdat); i++) {
      int id = Bdat::getIdTop(colorBdat) + i;
      const char *name = Bdat::getVal(colorBdat, "name", id).valueString;
      if (name != nullptr && *name != '\0') {
        mm::Col4 color;
        color.r = static_cast<u32>(Bdat::getVal(colorBdat, "col_r", id).valueU8) / 255.0f;
        color.g = static_cast<u32>(Bdat::getVal(colorBdat, "col_g", id).valueU8) / 255.0f;
        color.b = static_cast<u32>(Bdat::getVal(colorBdat, "col_b", id).valueU8) / 255.0f;
        color.a = 1.0f;
        UIColorLibrary::get()->add(name, color);
      }
    }
  }

  UIMotionLibrary::get()->reset();
  u8 *motionBdat = UIUtil::getBdatFP(UI_BDAT_RES_MOTION);
  if (motionBdat != nullptr) {
    for (int id = 1; id <= Bdat::getIdCount(motionBdat); id++) {
      const char *file = Bdat::getVal(motionBdat, "file", id).valueString;
      UIMotionLibrary::get()->add(file);
    }
  }

  UIUioLibrary::get()->reset();
  u8 *uioBdat = UIUtil::getBdatFP(UI_BDAT_RES_UIO);
  if (uioBdat != nullptr) {
    for (int id = Bdat::getIdTop(uioBdat); id <= Bdat::getIdEnd(uioBdat); id++) {
      const char *file = Bdat::getVal(uioBdat, "file", id).valueString;
      UIUioLibrary::get()->add(file);
    }
  }
}

} // namespace ui
