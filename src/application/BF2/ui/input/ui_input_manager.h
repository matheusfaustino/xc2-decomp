#pragma once
#include <stddef.h>

#include "mm_math/mm_pnt.h"
#include "mm_math/mm_rect.h"
#include "mm_mtl/intrusive_list.h"
#include "mm_mtl/singleton.h"
#include "types.h"
#include "ui/input/ui_input_layer.h"
#include "ui/input/ui_input_pointer.h"

namespace ui {
class UIInputPad;

class UIInputManager : public mm::mtl::PtrSingleton<UIInputManager> {
public:
  struct InputLayerInfo : public mm::mtl::IntrusiveListNode<InputLayerInfo> {
    UIInputLayer *unknown10; // Inferred role: layer; original member name unknown.
    u32 unknown18;           // Inferred role: handle; original member name unknown.
    u32 unknown1c;           // Inferred role: owner handle; original member name unknown.
    bool unknown20;          // Inferred role: enabled; original member name unknown.
    u8 _21[0x28 - 0x21];
  };

  void enable();
  void disable();
  void setupInputSelection(u32 input, int index, u32 size);
  void sendEvent(InputLayerInfo &info);
  void resetInputSelection(u32 input);
  void setSelectionChecker(u32 input, IUIInputSelectionChecker *checker);
  void setInputActionEnable(u32 input, u32 action, bool enable);
  u32 registInputLayer(UIInputLayer *layer, const char *name);
  void releaseInputLayer(u32 handle);
  void setInputSelectEnable(u32 input, bool enable);
  u32 searchInputHandle(u32 handle);
  bool migrateInput(u32 input, u32 handle);
  void enableInput(u32 input);
  void disableInput(u32 input);
  void setPadLineSelection(u32 input, u32 line);
  void setPadLineSelectionLoop(u32 input, bool loop);
  void setPadPageSize(u32 input, u32 size);
  void setPadLoopSetting(u32 input, bool loop);
  void setupPointerSelection(u32 input, const UIInputPointer::SelectionSetup &setup);
  void changePointerPos(u32 input, const mm::Pnt<s16> &pos);
  void changeRegionScale(u32 input, f32 scale);
  void changeRegionRect(u32 input, const mm::Rect<s16> &rect);
  InputLayerInfo *getInputInfo(u32 input);
  UIInputLayer *getInputLayer(u32 input);
  bool isEnableInput(u32 input);

  // Inferred role: finds the pad layer for an input; original helper name unknown.
  UIInputPad *unknown001(u32 input) {
    InputLayerInfo *info = getInputInfo(input);
    return info != nullptr ? info->unknown10->castUIInputPad() : nullptr;
  }

  // Inferred role: finds the pointer layer for an input; original helper name unknown.
  UIInputPointer *unknown002(u32 input) {
    InputLayerInfo *info = getInputInfo(input);
    return info != nullptr ? info->unknown10->castUIInputPointer() : nullptr;
  }

  mm::mtl::IntrusiveList<InputLayerInfo> unknown08; // Inferred role: input layers; original member name unknown.
  u32 unknown20;                                    // Role and original member name unknown.
  bool unknown24;                                   // Inferred role: enabled; original member name unknown.
};

static_assert(offsetof(UIInputManager::InputLayerInfo, unknown10) == 0x10, "");
static_assert(offsetof(UIInputManager::InputLayerInfo, unknown18) == 0x18, "");
static_assert(offsetof(UIInputManager::InputLayerInfo, unknown1c) == 0x1c, "");
static_assert(offsetof(UIInputManager::InputLayerInfo, unknown20) == 0x20, "");
static_assert(sizeof(UIInputManager::InputLayerInfo) == 0x28, "");
static_assert(offsetof(UIInputManager, unknown08) == 0x08, "");
static_assert(offsetof(UIInputManager, unknown20) == 0x20, "");
static_assert(offsetof(UIInputManager, unknown24) == 0x24, "");
static_assert(sizeof(UIInputManager) == 0x28, "");

} // namespace ui
