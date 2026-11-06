// Original file name from the game string C:/Monolithsoft/BF2/prog/application/BF2/battle/ai/ai_manager.cpp.
#include "battle/ai/ai_manager.h"

#include "framework/framework.h"
#include "framework/memory/framework_memory.h"
#include "mm_mtl/fixed_list.h" // IWYU pragma: keep

namespace gf {
struct GF_OBJ_HANDLE;
} // namespace gf

namespace ai {

void AiManager::init() {
  AiScriptDataManager::init();
  AiScriptManager::init();
  gf::GfObjManager::addListener(this, gf::IGfObjListener::OBJEVT_ALL);
}

void AiManager::destroy() {
  gf::GfObjManager::removeListener(this);
  _clear();
  AiScriptDataManager::destroy();
  AiScriptManager::destroy();
}

void AiManager::update(const fw::UpdateInfo &info) {
  for (mm::mtl::FixedList<AiData *, DATA_COUNT>::iterator it = unknown10.begin(); it != unknown10.end(); ++it) {
    (*it)->update(info);
  }
  AiScriptDataManager::update(info);
  AiScriptManager::update(info);
}

void AiManager::regist(gf::GF_OBJ_HANDLE *handle, int id) {
  // __LINE__ must be 61 as in the original file: it is an argument of operator new.
#line 61
  AiData *data = new (fw::Framework::getAllocator(), 0x10, __FILE__, __LINE__) AiData;
  data->setHandle(handle);
  data->setId(id);
  data->init();
  unknown10.push_back(data);
  AiScriptManager::start(AiScriptDataManager::getScriptName(id));
}

AiData *AiManager::getDataFromHandle(gf::GF_OBJ_HANDLE *handle) {
  for (mm::mtl::FixedList<AiData *, DATA_COUNT>::iterator it = unknown10.begin(); it != unknown10.end(); ++it) {
    AiData *data = *it;
    if (data->getHandle() == handle) {
      return data;
    }
  }
  return nullptr;
}

int AiManager::getNumFromId(int id) {
  int num = 0;
  for (mm::mtl::FixedList<AiData *, DATA_COUNT>::iterator it = unknown10.begin(); it != unknown10.end(); ++it) {
    if ((*it)->getId() == id) {
      num++;
    }
  }
  return num;
}

void AiManager::listenerGfObj(OBJEVT event, gf::GF_OBJ_HANDLE *handle, u32) {
  if (event == gf::IGfObjListener::OBJEVT_DESTROY) {
    unregist(handle);
  }
}

} // namespace ai

template mm::mtl::FixedList<ai::AiData *, 128>::iterator mm::mtl::FixedList<ai::AiData *, 128>::erase(mm::mtl::FixedList<ai::AiData *, 128>::iterator it);
