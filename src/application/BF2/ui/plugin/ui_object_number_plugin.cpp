#include "ui/plugin/ui_object_number_plugin.h"

#include <string.h>

#include "framework/lang_util.h"
#include "framework/memory/framework_memory.h"
#include "mm_mtl/reference_object.h"
#include "mm_mtl/rtti.h"
#include "ui/ctrl/ui_ctrl_manager.h"
#include "ui/plugin/ui_object_plugin_util.h"
#include "ui/ui_object.h"
#include "ui/ui_object_acc.h"
#include "ui/ui_object_plugin.h"
#include "ui/ui_util.h"

namespace ui {

UIObjectNumberPlugin::UIObjectNumberPlugin() : unknown7c(0), unknown80(0), unknown84(9999), unknown88(TYPE_NONE), unknown89(false), unknown8a(false), unknown90(nullptr), unknowne0(0.0f) {}

UIObjectNumberPlugin::~UIObjectNumberPlugin() {
  if (unknownc0.ctrl != nullptr) {
    UICtrlManager::get()->delUpdater(unknownc0);
  }
  unknownc0.ctrl = nullptr;
}

UIObjectNumberPlugin *UIObjectNumberPlugin::create(u32 handle) {
// __LINE__ is the line argument of the operator new call.
#line 56
  UIObjectNumberPlugin *plugin = new (UIUtil::getProgAllocator(), 16, __FILE__, __LINE__) UIObjectNumberPlugin();
  plugin->addRef();
  UIObjectAcc acc(handle);
  if (acc.isValid()) {
    int max;
    if (acc.isText()) {
      const char *text = acc.getTextPtr();
      plugin->unknown90 = text;
      if (text == nullptr || !UIUtil::checkNumberTextFormat(text, 1)) {
        plugin->unknown90 = "e%d";
      }
      plugin->unknown88 = strstr(text, "%s") != nullptr ? TYPE_TEXT_STRING : TYPE_TEXT;
      max = 9999;
    } else if (acc.isImage()) {
      max = acc.getImageMax();
      plugin->unknown88 = TYPE_IMAGE;
    } else if (acc.isLayout()) {
      plugin->setDispControl(false);
      max = 9999;
    } else {
      mm::mtl::safeRelease(plugin);
      return plugin;
    }

    if (UIObjectPluginUtil::attachPlugin(handle, plugin)) {
      plugin->unknown84 = max;
      plugin->unknown7c = 0;
      plugin->unknown80 = 0;
      plugin->init(handle);
      return plugin;
    }
  }
  mm::mtl::safeRelease(plugin);
  return plugin;
}

UIObjectNumberPlugin *UIObjectNumberPlugin::createExt(u32 handle, u32 extType) {
// __LINE__ is the line argument of the operator new call.
#line 138
  UIObjectNumberPlugin *plugin = new (UIUtil::getProgAllocator(), 16, __FILE__, __LINE__) UIObjectNumberPlugin();
  plugin->addRef();
  UIObjectAcc acc(handle);
  if (acc.isLayout()) {
    return nullptr;
  }
  if (acc.isValid() && acc.isImage() && UIObjectPluginUtil::attachPlugin(handle, plugin)) {
    acc.setExtType(extType);
    plugin->unknown84 = 9999;
    plugin->unknown7c = 0;
    plugin->unknown80 = 0;
    plugin->init(handle);
    plugin->unknown88 = TYPE_EXT;
    return plugin;
  } else if (plugin != nullptr) {
    plugin->release();
    plugin = nullptr;
  }
  return plugin;
}

UIObjectNumberPlugin *UIObjectNumberPlugin::getPlugin(u32 handle) {
  UIObject *object = UIUtil::getObject(handle);
  if (object == nullptr) {
    return nullptr;
  }
  UIObjectPlugin *plugin = object->unknowne0;
  if (plugin == nullptr) {
    return nullptr;
  }
  return mm::mtl::dynamicCast<UIObjectNumberPlugin>(plugin);
}

void UIObjectNumberPlugin::setMin(int min) {
  if (unknown84 <= min) {
    unknown84 = min + 1;
  }
  if (this->unknown80 != min) {
    this->unknown80 = min;
    unknown18 |= true;
  }
}

void UIObjectNumberPlugin::setMax(int max) {
  if (max <= unknown80) {
    max = unknown80 + 1;
  }
  if (max != this->unknown84) {
    this->unknown84 = max;
    unknown18 |= true;
  }
}

void UIObjectNumberPlugin::setInterpolate(f32 time) {
  unknown8a = true;
  unknowne0 = time;
}

void UIObjectNumberPlugin::reload(UIObjectAcc &acc) {
  if (unknown88 != TYPE_TEXT && unknown88 != TYPE_TEXT_STRING) {
    return;
  }

  const char *text = acc.getTextPtr();
  unknown90 = text;
  if (text == nullptr || !UIUtil::checkNumberTextFormat(text, 1)) {
    unknown90 = "e%d";
  }
  unknown88 = strstr(text, "%s") != nullptr ? TYPE_TEXT_STRING : TYPE_TEXT;
  unknown18 |= true;
}

void UIObjectNumberPlugin::refreshMoneySeparetor() { s_MoneySeparetor = UIUtil::getMoneySeparetor(fw::LangUtil::getLangType()); }

const mm::mtl::RTTI *UIObjectNumberPlugin::getRTTI() const { return &m_rtti; }

bool UIObjectNumberPlugin::isNeedUpdate(UIObjectAcc &acc) { return (unknown18 & 1) || unknownc0.ctrl != nullptr || acc.isDisp() != unknown1d || unknown58.ctrl != nullptr || m_followObjHandle != 0; }

} // namespace ui
