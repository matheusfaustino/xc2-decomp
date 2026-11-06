// Original file name from the game string C:/Monolithsoft/BF2/prog/libs/wndlib/src/wndlib/wind/wnd_blender.cpp.
#include "wnd/wnd_node.h"
#include "wnd/wnd_wind.h" // IWYU pragma: keep

namespace wnd {

void WindBlender::setNode(const WindNode *node) {
  unknownd0 = node;
  unknownd8 = nullptr;
  unknowne4 = 0.0f;
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
