#include "ui/input/ui_input_pointer.h"

#include "mm_mtl/reference_object.h"

namespace ui {

UIInputPointer::~UIInputPointer() {
  if (unknown1e8.unknown10 != nullptr) {
    unknown1e8.unknown10->release();
    unknown1e8.unknown10 = nullptr;
  }
}

void UIInputPointer::setupPointerSelection(const SelectionSetup &setup) {
  if (unknown1e8.unknown10 != nullptr) {
    unknown1e8.unknown10->release();
    unknown1e8.unknown10 = nullptr;
  }
  unknown1e8 = setup;
  if (unknown1e8.unknown10 != nullptr) {
    unknown1e8.unknown10->addRef();
  }
  unknown224 = 1.0f;
  unknown204 = mm::Pnt<s16>(0, 0);
}

void UIInputPointer::changePointerPos(const mm::Pnt<s16> &pos) {
  if (unknown204.x != pos.x || unknown204.y != pos.y) {
    unknown242 = pos;
    unknown240 = true;
  }
}

void UIInputPointer::changeRegionScale(f32 scale) {
  if (unknown248 != scale) {
    unknown248 = scale;
    unknown246 = true;
  }
}

void UIInputPointer::changeRegionRect(const mm::Rect<s16> &rect) {
  unknown24e = rect;
  unknown24c = true;
}

const EventData *UIInputPointer::getInputEventExt() { return &unknown228; }

bool UIInputPointer::hasEventExt() const { return unknown228.unknown00 != 0; }

UIInputPointer *UIInputPointer::castUIInputPointer() { return this; }

} // namespace ui
