// Original file name from the game string C:/Monolithsoft/BF2/prog/libs/wndlib/src/wndlib/wnd_probe.cpp.
#include "wnd/wnd_probe.h"
#include "mm/mm_std_base.h"
#include "mm_mtl/vector.h" // IWYU pragma: keep
#include "wnd/wnd.h"       // IWYU pragma: keep
#include "wnd/wnd_wind.h"

namespace wnd {
struct UpdateInfo;
} // namespace wnd

template void mm::mtl::Vector<wnd::LocalWindHitCache>::push_back(const wnd::LocalWindHitCache &);
template void mm::mtl::Vector<wnd::LocalWindHitCache>::expand(size_t, size_t);

namespace wnd {

WindProbe::WindProbe(const ProbeCreateInfo &info, u32 seed, mm::mtl::IAllocator *allocator)
    : WindObject(unknown002_1, info), unknown88(seed), unknown180(nullptr), unknown188(nullptr), unknown190(allocator), m_pOverlappingCache(nullptr) {
  unknown190.reserve(info.unknown14, 16);
}

WindProbe::~WindProbe() {
  // The game asserts that the overlapping cache was released before the probe.
#line 38
  MM_ASSERT(m_pOverlappingCache == nullptr);
}

void WindProbe::bindGlobalWind(const GlobalWind &wind) {
  unknown90.setNode(&wind);
  unknown180 = &wind;
}

void WindProbe::unbindGlobalWind() {
  if (unknown180 == unknown90.unknownd0) {
    unknown90.setNode(nullptr);
  }
  unknown180 = nullptr;
}

void WindProbe::setAreaWind(const AreaWind *wind) {
  if (unknown188 == wind) {
    return;
  }
  if (wind != nullptr) {
    unknown90.changeNode(&wind->unknown90, wind->unknown260);
  } else {
    unknown90.changeNode(unknown180, unknown188->unknown260);
  }
  unknown188 = wind;
}

void WindProbe::calcGlobalWindPower(const UpdateInfo &info) { unknown90.update(info); }

} // namespace wnd
