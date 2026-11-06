#include "framework/wind/wind_manager.h"

#include "arc/arc_data.h"
#include "framework/memory/framework_memory.h"
#include "framework/param/param_builder.h"
#include "framework/render/render_param.h"
#include "framework/resource/common_resource.h"
#include "framework/wind/wind_impluse.h"
#include "framework/wind/wind_probe.h"
#include "fw/fw_param_manager.h"
#include "fw/fw_update_info.h"
#include "mm_math/mm_vec3.h"
#include "mm_mtl/fixed_queue.h"
#include "mm_mtl/reference_object.h"
#include "wnd/wnd_global_wind_manager.h"
#include "wnd/wnd_probe.h"
#include "wnd/wnd_wind.h"

namespace wnd {
struct Query;
} // namespace wnd

namespace fw {

class ProbeDrawer : public mm::mtl::IntrusiveListNode<ProbeDrawer> {};

WindManager::WindManager() : unknown18(nullptr), unknown68(nullptr), unknown80(nullptr) {}

void WindManager::destroyWorld() {
  if (unknown88.get() != nullptr) {
    unknown88 = nullptr;
  }
  if (unknown90.get() != nullptr) {
    unknown90 = nullptr;
  }
  mm::mtl::safeRelease(unknown18);
}

bool WindManager::initialize() {
  if (unknown68 == nullptr) {
    unknown68 = CommonResource::request(FrameworkMemory::getAllocator(5), "/param/wind.prm", 10, false);
    if (unknown68 == nullptr) {
      return false;
    }
  }

  if (!unknown68->valid()) {
    return false;
  }

  arc::CArcData archive(unknown68->getAssetData());
  unknown70 = ParamBuilder::importBinary(archive.getFileAdr("wind.param"), nullptr);
  return true;
}

void WindManager::update(const UpdateInfo &info) {
  int wind = RenderParam::getWindIndex(CAMERA_TYPE_MAIN);
  if (wind != getGlobalWind()) {
    setGlobalWind(wind);
  }

  for (mm::mtl::IntrusiveList<WindProbe>::iterator it = unknown20.begin(); it != unknown20.end(); ++it) {
    it.get()->update();
  }

  {
    mm::mtl::FixedQueue<WindImpluse *, 64> expired;
    for (mm::mtl::IntrusiveList<WindImpluse>::iterator it = unknown50.begin(); it != unknown50.end(); ++it) {
      if (!it.get()->update(info)) {
        expired.push(&*it);
      }
    }

    while (!expired.empty()) {
      expired.front()->release();
      expired.pop_front();
    }
  }

  if (unknown18 != nullptr) {
    unknown18->update(info.unknown04);
  }
}

WindManager::~WindManager() { mm::mtl::safeRelease(unknown68); }

const char *WindManager::getName() const { return "WindManager"; }

bool WindManager::isDebugMode() { return false; }

void WindManager::activateDebugWindow(ml::WinProc *window) {}

int WindManager::getGlobalWind() const {
  if (unknown18 != nullptr) {
    return unknown18->unknown10.unknown140;
  }
  return -1;
}

bool WindManager::setGlobalWind(int wind) {
  if (unknown18 != nullptr) {
    return unknown18->unknown10.setCurrentWind(wind);
  }
  return false;
}

const mm::Vec3 &WindManager::getGlobalWindPower() {
  WindManager *manager = get();
  if (manager == nullptr) {
    return mm::Vec3::zero;
  }
  if (manager->unknown90.get() != nullptr) {
    wnd::WindProbe *probe = manager->unknown90->unknown20;
    return probe->unknown90.unknown10[probe->unknown88];
  }
  if (manager->unknown88.get() != nullptr) {
    return manager->unknown88->unknown20->unknown90.unknown10[0];
  }
  return mm::Vec3::zero;
}

void WindManager::addProbe(WindProbe *probe) {
  unknown20.push_back(probe);
  if (unknown18 != nullptr) {
    unknown18->addProbe(*probe->unknown20);
  }
  if (probe->unknown34) {
    unknown90 = probe;
  }
}

void WindManager::removeProbe(WindProbe *probe) {
  if (unknown90.get() == probe) {
    unknown90 = nullptr;
  }
  if (unknown18 != nullptr) {
    unknown18->removeProbe(*probe->unknown20);
  }
  unknown20.erase(probe);
}

void WindManager::addDebugDrawer(ProbeDrawer &drawer) { unknown38.push_back(&drawer); }

void WindManager::removeDebugDrawer(ProbeDrawer &drawer) { unknown38.erase(&drawer); }

void WindManager::addImpluse(WindImpluse *impluse) {
  unknown18->addLocalWind(*impluse->unknown20);
  unknown50.push_back(impluse);
}

void WindManager::removeImpluse(WindImpluse *impluse) {
  unknown18->removeLocalWind(*impluse->unknown20);
  unknown50.erase(impluse);
}

void WindManager::setDebugMode(bool enable, u32 mask) {
  WindManager *manager = get();
  if (manager != nullptr && manager->unknown18 != nullptr) {
    manager->unknown18->setDebugMode(enable, mask);
  }
}

bool WindManager::query(wnd::Query *query) {
  WindManager *manager = get();
  if (manager != nullptr && manager->unknown18 != nullptr) {
    return manager->unknown18->query(query);
  }
  return false;
}

} // namespace fw
