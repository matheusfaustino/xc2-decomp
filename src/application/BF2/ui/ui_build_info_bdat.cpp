#include "ui/ui_build_info_bdat.h"

#include "bdat.h"
#include "ui/ui_util.h"

namespace ui {

UIBuildInfoBdat::UIBuildInfoBdat(const char *name) : m_name(name) { Bdat::getFP(m_name); }

UIBuildInfoBdat::~UIBuildInfoBdat() { ; }

const char *UIBuildInfoBdat::getName() const { return m_name; }

u32 UIBuildInfoBdat::getObjectNum() const { return Bdat::getIdCount(Bdat::getFP(m_name)); }

void UIBuildInfoBdat::getObjectData(u32 index, ObjectData &data) const {
  u8 *bdat = Bdat::getFP(m_name);
  // The original reads the message bdat before the object columns and does not use the result.
  UIUtil::getBdatFP(UI_BDAT_MENU_MS);
  u32 id = Bdat::getIdTop(bdat) + index;
  data = ObjectData();
  data.unknown30 = Bdat::getVal(bdat, "type", id).valueU8;
  if (Bdat::getMember(bdat, "name") != nullptr) {
    data.unknown00 = Bdat::getVal(bdat, "name", id).valueString;
  }
  if (Bdat::getMember(bdat, "parent") != nullptr) {
    data.unknown08 = Bdat::getVal(bdat, "parent", id).valueString;
  }
  if (Bdat::getMember(bdat, "attach_layout") != nullptr) {
    data.unknown10 = Bdat::getVal(bdat, "attach_layout", id).valueString;
  }
  if (Bdat::getMember(bdat, "offs_x") != nullptr && Bdat::getMember(bdat, "offs_y") != nullptr) {
    data.unknown32 = Bdat::getVal(bdat, "offs_x", id).valueU16;
    data.unknown34 = Bdat::getVal(bdat, "offs_y", id).valueU16;
  }
  if (Bdat::getMember(bdat, "priority") != nullptr) {
    data.unknown48 = Bdat::getVal(bdat, "priority", id).valueU16;
  }
  if (Bdat::getMember(bdat, "def_undisp") != nullptr) {
    data.unknown4b = Bdat::getVal(bdat, "def_undisp", id).valueU8 == 1;
  }
  if (Bdat::getMember(bdat, "resource") != nullptr) {
    u32 resource = Bdat::getVal(bdat, "resource", id).valueU32;
    if (resource != 0) {
      switch (data.unknown30) {
      case 2:
      case 6:
        data.unknown28 = Bdat::getVal(UIUtil::getBdatFP(UI_BDAT_RES_IMAGE), "file", resource).valueString;
        break;
      case 3:
      case 7:
        data.unknown28 = Bdat::getVal(UIUtil::getBdatFP(UI_BDAT_RES_LAYOUT), "file", resource).valueString;
        break;
      case 8:
        data.unknown28 = Bdat::getVal(UIUtil::getBdatFP(UI_BDAT_RES_FONT), "file", resource).valueString;
        break;
      case 9:
        data.unknown28 = Bdat::getVal(UIUtil::getBdatFP(UI_BDAT_RES_FONT_STYLE), "file", resource).valueString;
        data.unknown4c = true;
        break;
      case 10:
        data.unknown28 = Bdat::getVal(UIUtil::getBdatFP(UI_BDAT_RES_MSPROJ), "file", resource).valueString;
        break;
      }
    }
  }
  if (Bdat::getMember(bdat, "text_id") != nullptr) {
    u32 textId = Bdat::getVal(bdat, "text_id", id).valueU32;
    if (textId != 0) {
      data.unknown84 = textId;
    }
    if (Bdat::getMember(bdat, "text_align_h") != nullptr) {
      data.unknown88 = Bdat::getVal(bdat, "text_align_h", id).valueU8;
    }
    if (Bdat::getMember(bdat, "text_align_v") != nullptr) {
      data.unknown89 = Bdat::getVal(bdat, "text_align_v", id).valueU8;
    }
    if (Bdat::getMember(bdat, "text_prop") != nullptr) {
      data.unknown8a = Bdat::getVal(bdat, "text_prop", id).valueU8;
    }
  }
  if (Bdat::getMember(bdat, "input_type") != nullptr) {
    if (Bdat::getMember(bdat, "input_id") != nullptr) {
      data.unknown60 = Bdat::getVal(bdat, "input_type", id).valueU8;
      data.unknown62 = Bdat::getVal(bdat, "input_id", id).valueU16;
    }
  }
}

} // namespace ui
