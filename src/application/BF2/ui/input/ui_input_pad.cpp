#include "ui/input/ui_input_pad.h"

#include "framework/memory/framework_memory.h"

namespace ui {

UIInputPad *UIInputPad::create(u32 handle, u32 priority, const PadSetup &setup, mm::mtl::IAllocator *allocator) {
// __LINE__ is the line argument of the operator new call.
#line 113
  return new (allocator, 16, __FILE__, __LINE__) UIInputPad(handle, priority, setup);
}

UIInputPad::~UIInputPad() {
  if (unknown360 != 0) {
    unknown360 = 0;
  }
}

void UIInputPad::setLineSelectionMax(u32 max) {
  if (unknown380 != 0 && unknown384 != 0) {
    u32 count = unknown44 / max;
    if (unknown44 % max != 0) {
      count++;
    }
    if (count > 1) {
      unknown390 = max;
      unknown394 = count;
    }
  }
}

void UIInputPad::setLineSelectLoop(bool loop) { unknown398 = loop; }

void UIInputPad::setPageSize(u32 size) {
  if (unknown388 != 0 && unknown38c != 0) {
    unknown39c = size;
  }
}

void UIInputPad::setLoop(bool loop) {
  unknown368 = loop;
  unknown398 = loop;
}

const EventData *UIInputPad::getInputEventExt() { return &unknown3a8; }

bool UIInputPad::hasEventExt() const { return unknown3a8.unknown00 != 0; }

UIInputPad *UIInputPad::castUIInputPad() { return this; }

} // namespace ui
