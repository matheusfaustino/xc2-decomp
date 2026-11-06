// Original file name from the game string C:/Monolithsoft/BF2/prog/libs/wndlib/src/wndlib/wind/wnd_local_wind.cpp.
#include "mm_math/mm_vec3.h"
#include "types.h"
#include "wnd/wnd_object.h"
#include "wnd/wnd_probe.h" // IWYU pragma: keep

namespace wnd {

LocalWind::LocalWind(const LocalWindCreateInfo &info) : WindObject(unknown002_0, info), unknown88(info.unknown14), unknown8c(info.unknown18), unknown90(info.unknown1c) { unknown98.m_pOwner = this; }

f32 LocalWind::calcDampingDistance(f32 distance) const {
  f32 ratio = (distance - unknown8c) / (unknown90 - unknown8c);
  f32 damping = 1.0f - ratio;
  return damping < 0.0f ? 0.0f : (damping > 1.0f ? 1.0f : damping);
}

void LocalWind::calcEffectiveWindFormCache(const HitCache &cache, mm::Vec3 *wind) const {
  f32 damping = calcDampingDistance(cache.unknown08);
  *wind = cache.unknown10 * unknown88 * damping;
}

} // namespace wnd
