// Original file name from the game string C:/Monolithsoft/BF2/prog/libs/wndlib/src/wndlib/wind/wnd_area_wind.cpp.
#include "mm/mm_std_base.h"
#include "wnd/wnd_object.h"
#include "wnd/wnd_probe.h" // IWYU pragma: keep
#include "wnd/wnd_wind.h"

namespace wnd {
struct UpdateInfo;

AreaWind::AreaWind(const AreaWindCreateInfo &info) : WindObject(unknown002_2, info), unknown260(info.unknown20) {
#line 22
  MM_ASSERT(info.m_pResWind);
  unknown90.unknownd0 = info.m_pResWind;
  unknown268.m_pOwner = this;
}

void AreaWind::update(const UpdateInfo &info) { unknown90.update(info); }

} // namespace wnd
