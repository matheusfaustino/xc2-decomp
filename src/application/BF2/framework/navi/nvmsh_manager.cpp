#include "navi/nvmsh_manager.h"

#include "ml/critical_section.h"
#include "navi/nvmsh_ac_man.h"
#include "navi/nvmsh_ac_obj_base.h"
#include "navi/nvmsh_res_man.h"

namespace navi {

bool NvmshManager::setupResource(const void *data, const char *name) { return unknown50->setupResource(static_cast<const u8 *>(data), name); }

void NvmshManager::releaseResource() { unknown50->releaseResource(); }

void NvmshManager::releaseObj(NvmshAcObj *object) { unknown58->releaseObj(static_cast<NvmshAcObjBase *>(object)); }

void NvmshManager::pause(bool value) {
  if (value) {
    unknown68 |= 1;
  } else {
    unknown68 &= ~1u;
  }
}

void NvmshManager::setAddNavigation(bool value) {
  if (value) {
    unknown68 |= 2;
  } else {
    unknown68 &= ~2u;
  }
}

void NvmshManager::pauseHLv(bool pause, bool highLevel) {
  if (pause) {
    unknown68 |= 4;
    if (highLevel) {
      unknown68 |= 8;
    } else {
      unknown68 &= ~8u;
    }
  } else {
    unknown68 &= ~0xcu;
  }
}

bool NvmshManager::jobUpdate(void *param1, void *param2) {
  unknown68 |= 0x20000;
  u32 pauseMask = (unknown68 & 4) == 0 ? 1 : 8;
  if ((unknown68 & pauseMask) == 0) {
    unknown50->update();
    unknown58->update(unknown50);
  }

  unknown68 &= ~0x20000u;
  return false;
}

NvmshAcObj *NvmshManager::createObj() {
  ml::CS lock(getCriticalSection());
  return unknown58->createObj(unknown60, unknown50);
}

ml::CriticalSection &NvmshManager::getCriticalSection() {
  static ml::CriticalSection s_criticalSection;
  return s_criticalSection;
}

void NvmshManager::update() {
  unknown68 |= 0x20000;
  u32 pauseMask = (unknown68 & 4) == 0 ? 1 : 8;
  if ((unknown68 & pauseMask) == 0) {
    unknown50->update();
    unknown58->update(unknown50);
  }
  unknown68 &= ~0x20000u;
}

void NvmshManager::setDebugDisp(bool param1, bool param2, bool param3, bool param4, int index) {}

void NvmshManager::setDebugObjAllDisp(bool value) {}

void NvmshManager::openDebugWindow(ml::WinProc *window) {}

} // namespace navi
