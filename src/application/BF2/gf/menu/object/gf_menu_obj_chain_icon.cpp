#include "gf/menu/object/gf_menu_obj_chain_icon.h"

#include "gf/menu/object/gf_menu_obj_util.h"
#include "ui/event/ui_event_data.h"
#include "ui/ui_object_acc.h"

namespace gf {

// Inferred role: call id of the chain icon; original name unknown.
constexpr u32 unknown001 = 0x41;

void GfMenuObjChainIcon::finalize() { GfMenuObject::finalize(); }

void GfMenuObjChainIcon::update() { GfMenuObject::update(); }

void GfMenuObjChainIcon::close(u32 handle) {
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

void GfMenuObjChainIcon::setupScreen(u32 handle, int, int, int, int) { GfMenuObjUtil::setChildDisp(handle, "LAY_main", true); }

} // namespace gf
