// Original file name inferred from the ai manager sources: the game calls AiScriptManager out of line.
#include <string.h>

#include "battle/ai/ai_manager.h"
#include "gf/script/gf_script_manager.h"
#include "mm_mtl/fixed_list.h"
#include "types.h"

namespace fw {
struct UpdateInfo;
} // namespace fw

namespace ai {

void AiScriptManager::_clear() {
  for (u32 i = 0; i < SCRIPT_COUNT; i++) {
    _end(i);
  }
}

void AiScriptManager::_update() {
  for (u32 i = 0; i < SCRIPT_COUNT; i++) {
    switch (unknown08[i].unknown0c) {
    case UNKNOWN001_LOAD:
      _start(i);
      break;
    case UNKNOWN001_WAIT_START:
      _load(i);
      break;
    case UNKNOWN001_DESTROY:
      _end(i);
      break;
    case UNKNOWN001_RELOAD:
      _reload(i);
      break;
    default:
      break;
    }
  }
}

void AiScriptManager::_start(int index) {
  if (gf::GfScriptManager::get() == nullptr) {
    return;
  }
  unknown08[index].unknown0c = UNKNOWN001_WAIT_START;
  gf::GfScriptManager::loadScript(index + gf::SCRIPT_SLOT_AI, unknown08[index].unknown00);
}

void AiScriptManager::_load(int index) {
  if (gf::GfScriptManager::get() == nullptr) {
    return;
  }
  u32 slot = index + gf::SCRIPT_SLOT_AI;
  if (!gf::GfScriptManager::isScriptLoaded(slot)) {
    return;
  }
  if (!gf::GfScriptManager::isScriptActive(slot)) {
    gf::GfScriptManager::startScript(slot);
  }
  unknown08[index].unknown0c = UNKNOWN001_ACTIVE;
}

void AiScriptManager::_reload(int index) {
  if (gf::GfScriptManager::get() == nullptr) {
    return;
  }
  u32 slot = index + gf::SCRIPT_SLOT_AI;
  if (!gf::GfScriptManager::isScriptLoaded(slot)) {
    return;
  }
  gf::GfScriptManager::destroyScript(slot);
  unknown08[index].unknown0c = UNKNOWN001_LOAD;
}

void AiScriptManager::_end(int index) {
  if (gf::GfScriptManager::get() == nullptr) {
    return;
  }
  unknown08[index].unknown00 = nullptr;
  unknown08[index].unknown08 = 0;
  unknown08[index].unknown0c = UNKNOWN001_EMPTY;
  gf::GfScriptManager::destroyScript(index + gf::SCRIPT_SLOT_AI);
}

int AiScriptManager::_getScriptType(int index) { return index + gf::SCRIPT_SLOT_AI; }

int AiScriptManager::_searchData(const char *name) {
  for (int i = 0; i < static_cast<int>(SCRIPT_COUNT); i++) {
    if (unknown08[i].unknown0c != UNKNOWN001_EMPTY && strcmp(unknown08[i].unknown00, name) == 0) {
      return i;
    }
  }
  return -1;
}

int AiScriptManager::_searchEmpty() {
  int index = -1;
  for (int i = 0; i < static_cast<int>(SCRIPT_COUNT); i++) {
    bool isEmpty = unknown08[i].unknown0c == UNKNOWN001_EMPTY;
    index = isEmpty ? i : index;
    if (isEmpty) {
      break;
    }
  }
  return index;
}

void AiScriptDataManager::init() { getPtr()->_init(); }

void AiScriptDataManager::destroy() { getPtr()->unknown08.clear(); }

void AiScriptDataManager::update(const fw::UpdateInfo &) {}

void AiScriptManager::init() {}

void AiScriptManager::destroy() { getPtr()->_clear(); }

void AiScriptManager::update(const fw::UpdateInfo &) { getPtr()->_update(); }

bool AiScriptManager::reload(const char *name) {
  AiScriptManager *manager = getPtr();
  int index = manager->_searchData(name);
  if (index == -1) {
    return false;
  }
  if (manager->unknown08[index].unknown0c != UNKNOWN001_WAIT_START && manager->unknown08[index].unknown0c != UNKNOWN001_ACTIVE) {
    return false;
  }
  manager->unknown08[index].unknown0c = UNKNOWN001_RELOAD;
  return true;
}

} // namespace ai
