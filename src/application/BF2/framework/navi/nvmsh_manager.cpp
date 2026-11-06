#include "navi/nvmsh_manager.h"

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

void NvmshManager::setDebugDisp(bool param1, bool param2, bool param3, bool param4, int index) {}

void NvmshManager::setDebugObjAllDisp(bool value) {}

void NvmshManager::openDebugWindow(ml::WinProc *window) {}

} // namespace navi
