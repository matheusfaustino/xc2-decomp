// Original file name from the game string C:/Monolithsoft/BF2/prog/libs/wndlib/src/wndlib/wnd_world.cpp.
#include "mm/mm_std_base.h"
#include "mm_mtl/intrusive_list.h"
#include "mm_mtl/vector.h"
#include "types.h"
#include "wnd/wnd_broad_phaze_interface.h"
#include "wnd/wnd_global_wind_manager.h" // IWYU pragma: keep
#include "wnd/wnd_object.h"
#include "wnd/wnd_probe.h"

namespace wnd {

void WindWorld::setDebugMode(bool enable, u32 mask) {
  unknown1d0 = enable;
  unknown10.setDebugWind(enable, mask);
}

bool WindWorld::addProbe(WindProbe &probe) {
  probe.addRef();
  unknown170.push_back(&probe);
  m_pBroadPhaze->createProxy(probe);
  probe.bindGlobalWind(unknown10.unknown40);
  return true;
}

void WindWorld::removeProbe(WindProbe &probe) {
  probe.setAreaWind(nullptr);
  probe.unbindGlobalWind();
  m_pBroadPhaze->destroyProxy(probe);
  unknown170.erase(&probe);
  probe.unknown190.clear();
  unknown1b8.push_back(&probe.unknown70);
}

bool WindWorld::addAreaWind(AreaWind &wind) {
#line 126
  MM_ASSERT(m_pBroadPhaze);
  bool created = m_pBroadPhaze->addWindObject(wind);
  if (created) {
    wind.addRef();
    unknown188.push_back(&wind.unknown268);
  }
  return created;
}

void WindWorld::removeAreaWind(AreaWind &wind) {
#line 144
  MM_ASSERT(m_pBroadPhaze);
  m_pBroadPhaze->removeWindObject(wind);
  unknown188.erase(&wind.unknown268);
  unknown1b8.push_back(&wind.unknown70);
}

bool WindWorld::addLocalWind(LocalWind &wind) {
#line 91
  MM_ASSERT(m_pBroadPhaze);
  bool created = m_pBroadPhaze->addWindObject(wind);
  if (created) {
    wind.addRef();
    unknown1a0.push_back(&wind.unknown98);
  }
  return created;
}

void WindWorld::removeLocalWind(LocalWind &wind) {
#line 109
  MM_ASSERT(m_pBroadPhaze);
  m_pBroadPhaze->removeWindObject(wind);
  unknown1a0.erase(&wind.unknown98);
  unknown1b8.push_back(&wind.unknown70);
}

} // namespace wnd
