#include "gf/menu/object/gf_menu_obj_salvage.h"

#include "gf/gf_allocator.h"
#include "gf/menu/gf_menu_types.h"
#include "gf/menu/object/gf_menu_obj_util.h"
#include "gf/sound/gf_sound.h"
#include "ui/event/ui_event_data.h"
#include "ui/event/ui_event_manager.h"
#include "ui/ui_object_acc.h"

namespace gf {

// Inferred role: rating animation of each result rank; original name unknown.
static const char *const unknown001[] = {"rank_1", "rank_2", "rank_3", "rank_4"};

// Inferred role: call id of the salvage result; original name unknown.
constexpr u32 unknown002 = 0x53;

GfMenuObjSalvage::GfMenuObjSalvage(u32 handle) : GfMenuObject(handle), unknown24(0), unknown28(this), unknown2b8(false) {}

GfMenuObjSalvage::~GfMenuObjSalvage() {}

void GfMenuObjSalvage::initialize() {
  ui::UIObjectAcc main(unknown18, "LAY_main");
  if (main.isValid()) {
    attachExternalCall(unknown002, main.unknown10);
    {
      ui::UIObjectAcc result(main.unknown10, "LAY_result");
// __LINE__ must be 78 as in the original file: it is the line argument of operator new.
#line 78
      ResultListener *listener = new (getAllocator(ALLOCATOR_DEFAULT), __FILE__, __LINE__, 8) ResultListener(this);
      listener->addRef();
      ui::UIEventManager::get()->registObjectListener(main.searchChild("LAY_result"), listener);
      if (listener != nullptr) {
        listener->release();
      }
    }
    GfMenuObject::initialize();
  }
}

void GfMenuObjSalvage::finalize() { GfMenuObject::finalize(); }

void GfMenuObjSalvage::openResult(GfMenuCallContext *context, const ResultParam &param) {
  ui::UIObjectAcc root(getAttachedObject(unknown002));
  if (GfMenuObjUtil::checkCCStart(context)) {
    ui::UIObjectAcc result(root.searchChild("LAY_result"));
    ui::EventData event;
    event.unknown00 = 0x21;
    event.unknown08_u16 = param.unknown00;
    sendEvent(result.unknown10, event, 0.0f);
    startCallContext(context, unknown002, result.unknown10);
  }
}

void GfMenuObjSalvage::update() {
  unknown28.unknown003();
  GfMenuObject::update();
}

void GfMenuObjSalvage::ResultListener::reciveEvent(const ui::EventData &event, u32 handle) {
  ui::UIObjectAcc acc(handle);
  if (event.unknown00 == 1) {
    acc.setDisp(false);
    unknown0c = -1;
  } else if (event.unknown00 == GfMenuTypes::unknown003_33) {
    unknown20->unknown24 = static_cast<u16>(event.unknown08);
    unknown20->unknown28.unknown005();
    unknown20->unknown28.unknown002(&GfMenuObjSalvage::seqResultAppear);
    unknown20->unknown2b8 = false;
  } else {
    ui::UIObjectListener::reciveEvent(event, handle);
  }
}

bool GfMenuObjSalvage::seqResultAppear() {
  ui::UIObjectAcc rating(unknown18, "IMG_result_rating");
  bool isEnd;
  if (!unknown2b8) {
    ui::UIObjectAcc result(unknown18, "LAY_result");
    result.setDisp(true);
    GfMenuObjUtil::CtrlParam param;
    param.unknown004("fade_in_r");
    GfMenuObjUtil::startCtrl(result.unknown10, param, "LAY_result_index");
    rating.startAnimation(unknown001[unknown24], false);
    unknown2b8 = true;
    GfMenuObjUtil::playJingle(15);
    isEnd = false;
  } else if (!rating.isPlayingAnimation()) {
    unknown2b8 = false;
    unknown28.unknown002(&GfMenuObjSalvage::seqResultLeave);
    isEnd = true;
  } else {
    isEnd = false;
  }
  return isEnd;
}

bool GfMenuObjSalvage::seqResultLeave() {
  ui::UIObjectAcc acc(unknown18, "LAY_result");
  bool isEnd;
  if (!unknown2b8) {
    GfMenuObjUtil::CtrlParam param;
    param.unknown004("fade_out_l");
    param.unknown14 = 10;
    GfMenuObjUtil::startCtrl(acc.unknown10, param, nullptr);
    isEnd = false;
    unknown2b8 = true;
  } else if (!GfMenuObjUtil::isBusyCtrl(GfMenuObjUtil::getCtrlID(acc.unknown10))) {
    unknown28.unknown002(&GfMenuObjSalvage::seqResultEnd);
    isEnd = true;
  } else {
    isEnd = false;
  }
  return isEnd;
}

bool GfMenuObjSalvage::seqResultEnd() {
  ui::UIObjectAcc acc(unknown18, "LAY_result");
  termCallContext(acc.unknown10);
  return true;
}

} // namespace gf
