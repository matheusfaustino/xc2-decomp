#include "gf/menu/object/gf_menu_obj_chapter_telop.h"

#include "gf/gf_allocator.h"
#include "gf/menu/object/gf_menu_obj_util.h"
#include "ui/event/ui_event_data.h"
#include "ui/event/ui_event_manager.h"
#include "ui/ui_object_acc.h"

namespace gf {

// Inferred role: call id of the chapter telop; original name unknown.
constexpr u32 unknown001 = 0x44;

GfMenuObjChapterTelop::GfMenuObjChapterTelop(u32 handle) : GfMenuObject(handle), unknown28(this) {}

GfMenuObjChapterTelop::~GfMenuObjChapterTelop() {}

void GfMenuObjChapterTelop::initialize() {
  ui::UIObjectAcc root(unknown18);
  ui::UIObjectAcc main(root.unknown10, "LAY_main");
  if (main.isValid()) {
// __LINE__ must be 92 as in the original file: it is the line argument of operator new.
#line 92
    ListListener *listener = new (getAllocator(ALLOCATOR_DEFAULT), __FILE__, __LINE__, 8) ListListener(this);
    listener->addRef();
    ui::UIEventManager::get()->registObjectListener(main.unknown10, listener);
    if (listener != nullptr) {
      listener->release();
    }
    attachExternalCall(unknown001, main.unknown10);
    hookAnimationEvent(main.unknown10, main.searchChild("IMG_begin_track"));
    hookAnimationEvent(main.unknown10, main.searchChild("IMG_begin_title"));
    hookAnimationEvent(main.unknown10, main.searchChild("IMG_end_telop"));
  }
  GfMenuObject::initialize();
}

void GfMenuObjChapterTelop::finalize() { GfMenuObject::finalize(); }

void GfMenuObjChapterTelop::close(u16 title) {
  u32 handle = getAttachedObject(unknown001);
  if (ui::UIObjectAcc(handle).isValid()) {
    if (!ui::UIObjectAcc(handle).isTerm()) {
      ui::EventData event;
      event.unknown00 = 0x22;
      event.unknown08_u16 = title;
      sendEvent(handle, event, 0.0f);
    }
  }
}

void GfMenuObjChapterTelop::update() {
  unknown28.unknown003();
  GfMenuObject::update();
}

bool GfMenuObjChapterTelop::seqWaitStreamBegin() {
  ui::UIObjectAcc track(unknown18, "IMG_begin_track");
  ui::UIObjectAcc title(unknown18, "IMG_begin_title");
  if (track.isStreamImage() && track.isStreaming()) {
    return false;
  }
  if (title.isStreamImage() && title.isStreaming()) {
    return false;
  }
  track.setDisp(true);
  track.startAnimation("start", false);
  title.setDisp(true);
  title.startAnimation("start", false);
  return true;
}

bool GfMenuObjChapterTelop::seqOutChapterEnd() {
  ui::UIObjectAcc acc(unknown18, "LAY_end_telop");
  GfMenuObjUtil::CtrlParam param;
  param.unknown08 = 2;
  param.unknown00 = 1.0f;
  param.unknown14 = 10;
  GfMenuObjUtil::startCtrl(unknown18, param, "LAY_end_telop");
  unknown28.unknown002(&GfMenuObjChapterTelop::seqWaitChapterEnd);
  return true;
}

bool GfMenuObjChapterTelop::seqWaitStreamEnd() {
  ui::UIObjectAcc acc(unknown18, "IMG_end_telop");
  if (acc.isStreamImage() && acc.isStreaming()) {
    return false;
  }
  acc.startAnimation("start", false);
  return true;
}

bool GfMenuObjChapterTelop::seqWaitChapterEnd() {
  ui::UIObjectAcc acc(unknown18, "LAY_end_telop");
  if (GfMenuObjUtil::isBusyCtrl(GfMenuObjUtil::getCtrlID(acc.unknown10))) {
    return false;
  }
  ui::UIObjectAcc main(unknown18, "LAY_main");
  termCallContext(main.unknown10);
  return true;
}

} // namespace gf
