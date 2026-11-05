#include "ui/input/ui_input_switcher.h"

#include "ui/input/ui_input_manager.h"
#include "ui/ui_object_acc.h"
#include "ui/ui_util.h"

namespace ui {

// Inferred role: returns the input handle of a valid object, or 0. Original name unknown.
static u32 unknown001(u32 handle) {
  if (handle == 0 || !UIObjectAcc(handle).isValid()) {
    return 0;
  }
  return UIInputManager::get()->searchInputHandle(handle);
}

UIInputSwitcher::UIInputSwitcher(u32 size) : unknown20(-1) {
  unknown08.setAllocator(UIUtil::getProgAllocator());
  unknown08.reserve(size);
}

UIInputSwitcher::~UIInputSwitcher() {
  if (unknown08.size() != 0) {
    unknown08.destroy();
  }
}

void UIInputSwitcher::clearInputObject() {
  for (u32 i = 0; i < unknown08.size(); i++) {
    unknown08[i] = 0;
  }
}

void UIInputSwitcher::enable(u32 index) {
  if (UIInputManager::get() == nullptr || index >= unknown08.size()) {
    return;
  }

  if (unknown08[index] == 0) {
    return;
  }

  if (unknown20 != -1) {
    u32 input = unknown001(unknown08[unknown20]);
    if (input != 0) {
      UIInputManager::get()->disableInput(input);
    }
  }

  u32 input = unknown001(unknown08[index]);
  if (input != 0) {
    UIInputManager::get()->enableInput(input);
    unknown20 = index;
  }
}

u32 UIInputSwitcher::getInputHandle(u32 handle) const {
  if (handle == 0) {
    return 0;
  }

  if (!UIObjectAcc(handle).isValid()) {
    return 0;
  }

  return UIInputManager::get()->searchInputHandle(handle);
}

u32 UIInputSwitcher::getInputObject(u32 index) {
  if (UIInputManager::get() == nullptr || index >= unknown08.size()) {
    return 0;
  }

  return unknown08[index];
}

void UIInputSwitcher::disable() {
  if (UIInputManager::get() == nullptr) {
    return;
  }

  for (u32 i = 0; i < unknown08.size(); i++) {
    u32 handle = unknown08[i];
    if (handle == 0) {
      continue;
    }

    if (!UIObjectAcc(handle).isValid()) {
      continue;
    }

    u32 input = UIInputManager::get()->searchInputHandle(handle);
    if (input != 0) {
      UIInputManager::get()->disableInput(input);
    }
  }
  unknown20 = -1;
}

bool UIInputSwitcher::isActive(u32) {
  for (u32 i = 0; i < unknown08.size(); i++) {
    if (unknown08[i] == 0) {
      continue;
    }

    if (u32 handle = unknown08[i]) {
      if (UIObjectAcc(handle).isValid()) {
        if (UIInputManager::get()->searchInputHandle(handle) != 0) {
          return true;
        }
      }
    }
  }
  return false;
}

} // namespace ui
