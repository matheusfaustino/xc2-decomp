#include "ui/ctrl/ui_ctrl_cursor_color.h"

#include "framework/memory/framework_memory.h"
#include "mm_mtl/reference_object.h"
#include "ui/list/ui_select_obj_list.h"
#include "ui/ui_util.h"

namespace ui {

UICtrlCursorColor::UICtrlCursorColor(u32 object) : UICtrlObject(object), unknown30(), unknown50(nullptr), unknown58(0), unknown5c(0) {}

UICtrlCursorColor::~UICtrlCursorColor() { mm::mtl::safeRelease(unknown50); }

UICtrlObject *UICtrlCursorColor::create(u32 object, const CursorSetting &setting, UISelectObjList *list) {
  if (list == nullptr) {
    return nullptr;
  }
  list->addRef();
#line 47
  UICtrlCursorColor *ctrl = new (UIUtil::getProgAllocator(), 16, __FILE__, __LINE__) UICtrlCursorColor(object);
  if (ctrl == nullptr) {
    return nullptr;
  }
  ctrl->unknown30 = setting;
  ctrl->unknown50 = list;
  ctrl->unknown58 = list->getIndex(false);
  ctrl->unknown5c = object;
  return ctrl;
}

void UICtrlCursorColor::open(s32 index) {
  if (unknown50 != nullptr) {
    unknown58 = index;
    unknown50->setIndex(index);
  }
}

void UICtrlCursorColor::setDisp(bool disp) { inputChange(disp ? unknown50->getIndex(false) : unknown50->getSize() + 1); }

void UICtrlCursorColor::setIndex(u32 index) {
  if (unknown50 != nullptr) {
    unknown50->setIndex(index);
  }
}

void UICtrlCursorColor::update(const fw::UpdateInfo &info) {
  if (unknown50 != nullptr && unknown50->getSize() != 0) {
    s32 index = unknown50->getIndex(false);
    if (index != unknown58) {
      unknown58 = index;
    }
  }
}

} // namespace ui
