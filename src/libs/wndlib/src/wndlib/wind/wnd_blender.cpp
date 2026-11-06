// Original file name from the game string C:/Monolithsoft/BF2/prog/libs/wndlib/src/wndlib/wind/wnd_blender.cpp.
#include <math.h>

#include "mm/mm_std_base.h"
#include "types.h"
#include "wnd/wnd_node.h"
#include "wnd/wnd_wind.h" // IWYU pragma: keep

namespace wnd {

void WindBlender::setNode(const WindNode *node) {
  unknownd0 = node;
  unknownd8 = nullptr;
  unknowne4 = 0.0f;
}

void WindBlender::changeNode(const WindNode *node, f32 time) {
#line 80
  MM_ASSERT(abs(time) > 1.1920928955078125e-07F);
  if (unknownd8 == nullptr) {
    unknownd8 = unknownd0;
  } else if (unknowne0 >= 0.5f) {
    unknownd8 = unknownd0;
  }
  unknownd0 = node;
  unknowne4 = time;
  unknowne0 = 0.0f;
}

void GlobalWind::update(const UpdateInfo &info) {
  if (unknownf0 != nullptr) {
    unknownf0->update(info);
  }
  if (unknowne8 != nullptr) {
    unknowne8->update(info);
  }
  WindBlender::update(info);
  if (unknownf0 == nullptr) {
    unknownf0 = nullptr;
  }
}

} // namespace wnd
