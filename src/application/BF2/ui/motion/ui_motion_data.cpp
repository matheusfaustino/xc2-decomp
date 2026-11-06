#include "ui/motion/ui_motion_data.h"

#include <string.h>

namespace ui {

UIMotionData::UIMotionData(mm::mtl::IAllocator *allocator) : unknown58(allocator) {
  unknown08.setAllocator(allocator);
  unknown30.setAllocator(allocator);
}

void UIMotionData::initialize() {}

void UIMotionData::clear() {
  for (u32 i = 0; i < unknown08.size(); i++) {
    if (unknown08[i].unknown00.size() != 0) {
      for (u32 j = 0; j < unknown08[i].unknown00.size(); j++) {
        if (unknown08[i].unknown00[j].capacity() != 0) {
          unknown08[i].unknown00[j].destroy();
        }
      }
      unknown08[i].unknown00.destroy();
    }
  }
  if (unknown08.capacity() != 0) {
    unknown08.destroy();
  }
  if (unknown30.capacity() != 0) {
    unknown30.destroy();
  }
}

u32 UIMotionData::getMotionNum() const { return unknown08.size(); }

u32 UIMotionData::getParamType(const char *name) const { return 0xffff; }

u32 UIMotionData::searchMotion(const char *name) const {
  for (u32 i = 0; i < unknown08.size(); i++) {
    if (unknown08[i].unknown28 < static_cast<u32>(unknown30.size())) {
      if (strcmp(unknown30[unknown08[i].unknown28], name) == 0) {
        return i;
      }
    }
  }
  return static_cast<u32>(-1);
}

f32 UIMotionData::getMotionLength(u32 motion) const {
  if (motion < unknown08.size()) {
    return static_cast<f32>(unknown08[motion].unknown00.size()) * 0.033333335f;
  }
  return 0.0f;
}

bool UIMotionData::getMotionParamDefault(u32 motion, MotionParam &param) const {
  if (motion < unknown08.size()) {
    u32 frame;
    bool result = getMotionParamFrame(motion, 0, frame, param);
    return result && frame == 0;
  }
  return false;
}

const char *UIMotionData::getMotionName(u32 motion) const {
  if (motion >= unknown08.size()) {
    return nullptr;
  }
  return unknown30[unknown08[motion].unknown28];
}

} // namespace ui
