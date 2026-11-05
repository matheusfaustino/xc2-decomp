#include "ui/ctrl/ui_ctrl_revolve_select.h"

#include "mm_mtl/reference_object.h"
#include "ui/list/ui_select_obj_list.h"

namespace ui {

UICtrlRevolveSelect::UICtrlRevolveSelect(u32 object)
    : UICtrlObject(object), unknown2c(), unknown5c(0), unknown60(nullptr), unknown68(0), unknown6c(0.0f), unknown70(0.0f), unknown78(), unknowna0(0, 0), unknowna8(), unknownd8(0) {}

UICtrlRevolveSelect::~UICtrlRevolveSelect() { mm::mtl::safeRelease(unknown60); }

f32 UICtrlRevolveSelect::getAngleStep() {
  u32 count = unknown60->getSize();
  u16 range = unknown2c.unknown00;
  f32 step;
  if (count < 2) {
    step = range;
  } else if (range == 360) {
    step = static_cast<f32>(range) / static_cast<f32>(unknown60->getSize());
  } else {
    step = static_cast<f32>(range) / static_cast<f32>(unknown60->getSize() - 1);
  }
  return unknown2c.unknown03 == 0 ? -step : step;
}

void UICtrlRevolveSelect::reset() {
  int index = unknown60->getIndex(false);
  if (unknown60->getSize() != 0) {
    f32 angle = getAngleStep();
    if (index >= 0) {
      unknown6c = -(index * angle);
      unknown70 = -(index * angle);
    }
    unknown68 = index;
  }
}

void UICtrlRevolveSelect::resetAngelRange(u16 range) { unknown2c.unknown00 = range; }

void UICtrlRevolveSelect::getCenterPos(mm::Pnt<s16> &pos) const {
  pos.x = unknown2c.unknown08.x + unknowna0.x;
  pos.y = unknown2c.unknown08.y + unknowna0.y;
}

bool UICtrlRevolveSelect::isRolling() const { return unknown78.isBusy(); }

bool UICtrlRevolveSelect::isMoving() const { return unknowna8.isBusy(); }

} // namespace ui
