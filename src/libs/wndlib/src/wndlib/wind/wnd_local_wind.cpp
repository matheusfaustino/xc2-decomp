// Original file name from the game string C:/Monolithsoft/BF2/prog/libs/wndlib/src/wndlib/wind/wnd_local_wind.cpp.
#include "wnd/wnd_object.h"
#include "wnd/wnd_probe.h" // IWYU pragma: keep

namespace wnd {

LocalWind::LocalWind(const LocalWindCreateInfo &info) : WindObject(unknown002_0, info), unknown88(info.unknown14), unknown8c(info.unknown18), unknown90(info.unknown1c) { unknown98.m_pOwner = this; }

} // namespace wnd
