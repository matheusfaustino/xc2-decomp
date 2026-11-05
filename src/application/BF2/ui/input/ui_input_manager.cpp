#include "ui/input/ui_input_manager.h"

#include "mm_math/mm_pnt.h"
#include "mm_math/mm_rect.h"
#include "mm_mtl/bitflag.h"
#include "ui/input/ui_input_layer.h"
#include "ui/input/ui_input_pad.h"
#include "ui/input/ui_input_pointer.h"
#include "ui/ui_object.h"
#include "ui/ui_util.h"

namespace ui {

void UIInputManager::enable() { unknown24 = true; }

void UIInputManager::disable() { unknown24 = false; }

UIInputManager::InputLayerInfo *UIInputManager::getInputInfo(u32 input) {
  for (mm::mtl::IntrusiveList<InputLayerInfo>::iterator it = unknown08.begin(); it != unknown08.end(); ++it) {
    if (it->unknown18 == input) {
      return it.get();
    }
  }
  return nullptr;
}

UIInputLayer *UIInputManager::getInputLayer(u32 input) {
  if (input == 0) {
    return nullptr;
  }
  InputLayerInfo *info = getInputInfo(input);
  return info != nullptr ? info->unknown10 : nullptr;
}

void UIInputManager::enableInput(u32 input) {
  if (input == 0) {
    return;
  }
  InputLayerInfo *info = getInputInfo(input);
  if (info != nullptr) {
    info->unknown20 = true;
  }
}

void UIInputManager::disableInput(u32 input) {
  if (input == 0) {
    return;
  }
  InputLayerInfo *info = getInputInfo(input);
  if (info != nullptr) {
    info->unknown20 = false;
  }
}

bool UIInputManager::isEnableInput(u32 input) {
  if (input == 0) {
    return false;
  }
  InputLayerInfo *info = getInputInfo(input);
  return info != nullptr && info->unknown20;
}

bool UIInputManager::migrateInput(u32 input, u32 handle) {
  if (input == 0) {
    return false;
  }
  InputLayerInfo *info = getInputInfo(input);
  if (info == nullptr) {
    return false;
  }
  info->unknown1c = handle;
  if (handle != 0) {
    UIUtil::getObject(handle)->unknown98.set(2);
  }
  return true;
}

void UIInputManager::setSelectionChecker(u32 input, IUIInputSelectionChecker *checker) {
  for (mm::mtl::IntrusiveList<InputLayerInfo>::iterator it = unknown08.begin(); it != unknown08.end(); ++it) {
    if (it->unknown18 == input) {
      it->unknown10->unknown1d8 = checker;
      return;
    }
  }
}

void UIInputManager::setInputActionEnable(u32 input, u32 action, bool enable) {
  for (mm::mtl::IntrusiveList<InputLayerInfo>::iterator it = unknown08.begin(); it != unknown08.end(); ++it) {
    if (it->unknown18 == input) {
      it->unknown10->setActionEnable(static_cast<u16>(action), enable);
      return;
    }
  }
}

void UIInputManager::setInputSelectEnable(u32 input, bool enable) {
  for (mm::mtl::IntrusiveList<InputLayerInfo>::iterator it = unknown08.begin(); it != unknown08.end(); ++it) {
    if (it->unknown18 == input) {
      it->unknown10->unknown4c = enable;
      return;
    }
  }
}

void UIInputManager::setPadLineSelection(u32 input, u32 line) {
  UIInputPad *pad = unknown001(input);
  if (pad == nullptr) {
    return;
  }
  pad->setLineSelectionMax(line);
  return;
}

void UIInputManager::setPadLineSelectionLoop(u32 input, bool loop) {
  UIInputPad *pad = unknown001(input);
  if (pad == nullptr) {
    return;
  }
  pad->setLineSelectLoop(loop);
  return;
}

void UIInputManager::setPadPageSize(u32 input, u32 size) {
  UIInputPad *pad = unknown001(input);
  if (pad == nullptr) {
    return;
  }
  pad->setPageSize(size);
  return;
}

void UIInputManager::setPadLoopSetting(u32 input, bool loop) {
  UIInputPad *pad = unknown001(input);
  if (pad == nullptr) {
    return;
  }
  pad->setLoop(loop);
  return;
}

void UIInputManager::setupPointerSelection(u32 input, const UIInputPointer::SelectionSetup &setup) {
  UIInputPointer *pointer = unknown002(input);
  if (pointer == nullptr) {
    return;
  }
  pointer->setupPointerSelection(setup);
  return;
}

void UIInputManager::changePointerPos(u32 input, const mm::Pnt<s16> &pos) {
  UIInputPointer *pointer = unknown002(input);
  if (pointer == nullptr) {
    return;
  }
  pointer->changePointerPos(pos);
  return;
}

void UIInputManager::changeRegionScale(u32 input, f32 scale) {
  UIInputPointer *pointer = unknown002(input);
  if (pointer == nullptr) {
    return;
  }
  pointer->changeRegionScale(scale);
  return;
}

void UIInputManager::changeRegionRect(u32 input, const mm::Rect<s16> &rect) {
  UIInputPointer *pointer = unknown002(input);
  if (pointer == nullptr) {
    return;
  }
  pointer->changeRegionRect(rect);
  return;
}

u32 UIInputManager::searchInputHandle(u32 handle) {
  if (handle == 0) {
    return 0;
  }
  UIObject *object = UIUtil::getObject(handle);
  if (object == nullptr || !object->unknown98.test(2)) {
    return 0;
  }
  for (mm::mtl::IntrusiveList<InputLayerInfo>::iterator it = unknown08.begin(); it != unknown08.end(); ++it) {
    if (it->unknown10->unknown40 == handle) {
      return it->unknown18;
    }
    if (it->unknown1c == handle) {
      return it->unknown18;
    }
  }
  return 0;
}

void UIInputManager::releaseInputLayer(u32 handle) {
  InputLayerInfo *info = unknown08.begin().get();
  InputLayerInfo *end = unknown08.end().get();
  while (info != end) {
    if (info->unknown18 == handle) {
      if (UIUtil::isValid(info->unknown10->unknown40)) {
        UIUtil::getObject(info->unknown10->unknown40)->unknown98.set(2, false);
      }
      if (info->unknown10 != nullptr) {
        info->unknown10->release();
        info->unknown10 = nullptr;
      }
      unknown08.erase(mm::mtl::IntrusiveList<InputLayerInfo>::iterator(info));
      delete info;
      return;
    }
    info = static_cast<InputLayerInfo *>(info->unknown00);
  }
}

} // namespace ui
