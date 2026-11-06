#include "gf/menu/object/gf_menu_obj_damage_buff.h"

#include "ui/event/ui_event_data.h"
#include "ui/ui_object_acc.h"

namespace gf {

// Inferred role: call id of the damage buff; original name unknown.
constexpr u32 unknown001 = 0x2d;

void GfMenuObjDamageBuff::finalize() { GfMenuObject::finalize(); }

void GfMenuObjDamageBuff::close(u32 handle) {
  ui::UIObjectAcc root(getAttachedObject(unknown001));
  if (root.isValid()) {
    if (!root.isTerm()) {
      ui::UIObjectAcc acc(handle);
      if (acc.isValid()) {
        if (acc.isDuplicate()) {
          ui::EventData event;
          event.unknown00 = 0x22;
          event.unknown08_u16 = ui::UIObjectAcc(handle).getDuplicateIndex();
          sendEvent(root.unknown10, event, 0.0f);
        }
      }
    }
  }
}

void GfMenuObjDamageBuff::update() { GfMenuObject::update(); }

} // namespace gf
