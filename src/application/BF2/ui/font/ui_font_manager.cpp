#include "ui/font/ui_font_manager.h"

#include <string.h>

#include "framework/memory/allocator.h"
#include "framework/memory/framework_memory.h"
#include "layer/layer_acc_font_style.h"
#include "layer/layer_base.h"
#include "layer/layer_tag.h"
#include "mm_math/mm_col4.h"
#include "mm_mtl/hash.h"
#include "mm_mtl/reference_object.h"
#include "ms/ms_wrapper.h"
#include "mtl/alloc_handle.h"
#include "ui/ui_build_info_bdat.h"
#include "ui/ui_resource.h"
#include "ui/ui_util.h"

namespace ui {

UIFontManager::UIFontManager() : unknown08(nullptr), m_fontStyle(), unknown68(false), unknown98(nullptr), unknowna0(nullptr) {
  ms::MsWrapper::init(fw::FrameworkMemory::getMem(5));
  unknown70.setAllocator(UIUtil::getProgAllocator());
  unknown70.reserve(8);
}

UIFontManager::~UIFontManager() {
  for (FontRes *it = unknown10.begin(); it != unknown10.end(); ++it) {
    mm::mtl::safeRelease(it->unknown08);
  }
  unknown10.clear();
  unknown70.clear();
  if (unknown70.capacity() != 0) {
    unknown70.destroy();
  }
  mm::mtl::safeRelease(m_fontStyle.unknown08);
  mm::mtl::safeRelease(unknown08);
  mm::mtl::safeRelease(unknown98);
  mm::mtl::safeRelease(unknowna0);
}

void UIFontManager::importMSBP(const void *data, u32 size) {
  layer::LayerAccFontStyle fontStyle(m_fontStyle.unknown08);
  if (data == nullptr || size == 0) {
    return;
  }
  ms::MsWrapper::regist(static_cast<const char *>(data));
  u32 colorNum = ms::MsWrapper::getColorNum();
  if (colorNum != 0) {
    mm::mtl::Vector<layer::LayerStrInfo> *colorInfo = fontStyle.createColorInfo(colorNum);
    for (u32 i = 0; i < colorNum; i++) {
      LMSColor color;
      char name[32];
      if (!ms::MsWrapper::getColorName(i, name, 32)) {
        continue;
      }
      if (!ms::MsWrapper::getColor(i, &color)) {
        continue;
      }
      layer::LayerStrInfo info;
      info.setCol4(name, mm::Col4(color.unknown00 / 255.0f, color.unknown01 / 255.0f, color.unknown02 / 255.0f, color.unknown03 / 255.0f));
      colorInfo->push_back(info);
    }
  }
  for (s32 i = 0; i < ms::MsWrapper::getTagNum(1); i++) {
    if (strcmp(ms::MsWrapper::getTagName(1, i), "icon") != 0) {
      continue;
    }
    s32 iconNum = ms::MsWrapper::getTagParamListItemNum(1, i, 0);
    if (i == -1) {
      break;
    }
    mm::mtl::Vector<layer::LayerStrInfo> *iconInfo = fontStyle.createIconInfo(iconNum);
    if (iconInfo == nullptr) {
      break;
    }
    for (s32 j = 0; j < ms::MsWrapper::getTagParamListItemNum(1, i, 0); j++) {
      layer::LayerStrInfo info;
      info.setS32(ms::MsWrapper::getTagParamListItemName(1, i, 0, j), j);
      iconInfo->push_back(info);
    }
    break;
  }
  ms::MsWrapper::unregist();
}

UIFontManager::FontRes *UIFontManager::getFontRes(u16 id) {
  for (FontRes *it = unknown10.begin(); it != unknown10.end(); ++it) {
    if (it->unknown00 == id) {
      return it;
    }
  }
  return nullptr;
}

UIFontManager::FontRes *UIFontManager::getFontRes(const char *name) {
  u32 hash = mm::mtl::hashStrCrc(name);
  for (FontRes *it = unknown10.begin(); it != unknown10.end(); ++it) {
    if (it->unknown04 == hash) {
      return it;
    }
  }
  return nullptr;
}

UIFontManager::FontRes *UIFontManager::getFontStyle() { return &m_fontStyle; }

UIFontManager::FontRes *UIFontManager::getFontResFromStyle(u16 style) {
  if (m_fontStyle.unknown08 == nullptr) {
    return nullptr;
  }
  layer::LayerAccFontStyle fontStyle(m_fontStyle.unknown08);
  return getFontRes(fontStyle.getFontName(style));
}

void UIFontManager::loadFontSet(const char *name, mm::mtl::IAllocator *allocator) {
  UIBuildInfoBdat buildInfo(name);
// __LINE__ is the line argument of the operator new call.
#line 263
  UIResource *resource = new (fw::Allocator::getProcAllocator(), 16, __FILE__, __LINE__) UIResource(name);
  mm::mtl::safeRelease(unknown08);
  resource->addRef();
  resource->load(&buildInfo, allocator);
  if (!resource->isValid()) {
    mm::mtl::safeRelease(unknown08);
  } else {
    unknown08 = resource;
  }
  unknown68 = false;
}

bool UIFontManager::getFontStyleInfo(u16 style, StyleInfo &info) {
  layer::LayerAccFontStyle fontStyle(m_fontStyle.unknown08);
  if (style >= fontStyle.getStyleDataMax()) {
    return false;
  }
  info.unknown00 = fontStyle.getStyleInfo(style).unknown08->unknown0c;
  info.unknown04 = fontStyle.getStyleInfo(style).unknown08->unknown10;
  return true;
}

bool UIFontManager::findFacePattern(const char *name, u32 &index) {
  u32 hash = mm::mtl::hashStrCrc(name);
  index = 0;
  for (FacePattern *it = unknown70.begin(); it != unknown70.end(); ++it) {
    if (it->unknown00 == hash) {
      index = it->unknown04;
      return true;
    }
  }
  return false;
}

bool UIFontManager::isLoading() const {
  if (unknown08 != nullptr) {
    return unknown08->isLoading();
  }
  return false;
}

} // namespace ui
