#include <new>

#include "eflib/effect_system_manager.h"
#include "mm/mm_std_base.h"
#include "mtl/alloc_handle.h"
#include "ptlib/emitter/particle_emitter_manager.h"
#include "ptlib/forcefield/particle_force_field.h"
#include "ptlib/particle_heap_allocater.h"
#include "ptlib/particle_system.h"
#include "ptlib/renderer/particle_renderer_eff_state.h"
#include "ptlib/renderer/screen_render_manager.h"
#include "types.h"

namespace effect {

namespace {
SystemManager *s_instance;
} // namespace

SystemManager *SystemManager::get() {
  MM_ASSERT(getPtr());
  return getPtr();
}

SystemManager *&SystemManager::getPtr() { return s_instance; }

bool SystemManager::isValid() {
  if (getPtr() != nullptr) {
    if (getPtr()->unknown7824f == 0) {
      return true;
    }
  }
  return false;
}

SystemManager::SystemManager()
    : unknown09(true), unknown0a(true), unknown0b(false), unknown0c(true), unknown78248(false), _78249(0), unknown7824a(false), unknown7824b(false), unknown7824c(0), unknown7824d(0), unknown7824f(0) {
}

SystemManager::~SystemManager() {
  unknown220[0].clear();
  unknown220[1].clear();
}

SystemManager *SystemManager::create(const ptlib::DrDrawWorkInfoEF *info) {
// The allocation records its line number.
#line 551
  getPtr() = static_cast<SystemManager *>(ptlib::ParticleManager::sysAlloc(sizeof(SystemManager), 16, __FILE__, __LINE__));
  new (getPtr()) SystemManager();
  if (getPtr() != nullptr) {
    ptlib::ParticleManager::init(info);
    return getPtr();
  }
  ptlib::efAstMsg("0", "task create error!!");
  return nullptr;
}

void SystemManager::Init(const ptlib::DrDrawWorkInfoEF *info) { ptlib::ParticleManager::init(info); }

void SystemManager::destroy() {
  if (getPtr() != nullptr) {
    getPtr()->Term();
    getPtr()->~SystemManager();
    if (getPtr() != nullptr) {
      ptlib::ParticleManager::sysFree(getPtr());
      getPtr() = nullptr;
    }
  }
  ptlib::screen::ScreenRenderManager::term();
}

void SystemManager::Term() {
  ptlib::ParticleManager::get()->term();
  _78249 = 0;
}

void SystemManager::setUserFuncClear() {
  ptlib::ParticleManager::setUserAllocFunc(nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
  ptlib::ParticleManager::setUserFileFunc(nullptr, nullptr);
  ptlib::ParticleManager::setUserOutputString(nullptr, nullptr);
  ptlib::ParticleManager::setUserDrFunc(nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
}

void SystemManager::setUserAllocFunc(void *(*allocSys)(void *, u32, u32, const char *, u32), void (*freeSys)(void *, void *), void *(*allocRes)(void *, u32, u32, const char *, u32),
                                     void (*freeRes)(void *, void *), mtl::ALLOC_HANDLE (*getSysHandle)(), mtl::ALLOC_HANDLE (*getResHandle)()) {
  ptlib::ParticleManager::setUserAllocFunc(allocSys, freeSys, allocRes, freeRes, getSysHandle, getResHandle);
}

void SystemManager::setUserFileFunc(void *(*read)(const char *, int &), void (*readEnd)(void *)) { ptlib::ParticleManager::setUserFileFunc(read, readEnd); }

void SystemManager::setUserOutputString(void (*outputDebug)(const char *), void (*outputWarn)(const char *)) { ptlib::ParticleManager::setUserOutputString(outputDebug, outputWarn); }

void SystemManager::setUserDrFunc(void *(*createMdl)(const void *, const char *, int, const void *, int), void (*deleteMdl)(void *), void *(*createLgt)(u32, int), void (*deleteLgt)(u32, void *),
                                  void (*setMdlPos)(void *, void *), void (*setMdlMatrix)(void *, void *), void (*setMdlColor)(void *, void *), void (*setMdlAlpha)(void *, f32),
                                  void (*setMdlSpd)(void *, void *), void (*setLgtPos)(u32, void *, void *), void (*setLgtMatrix)(u32, void *, void *), void (*setLgtParam)(u32, void *, void *),
                                  void (*debugRenderer)(void *), void *(*createGrp)(int, int), void (*destroyGrp)(void *)) {
  ptlib::ParticleManager::setUserDrFunc(
      createMdl, deleteMdl, createLgt, deleteLgt, setMdlPos, setMdlMatrix, setMdlColor, setMdlAlpha, setMdlSpd, setLgtPos, setLgtMatrix, setLgtParam, debugRenderer, createGrp, destroyGrp);
}

void SystemManager::es_clearall(bool isForce) {
  if (unknown7824f == 0) {
    unknown7824f = 1;
  }
  unknown78248 = isForce;
}

void SystemManager::begin() { ptlib::renderer::EffStateManager::get()->begin(); }

void SystemManager::memoryDump() { ptlib::HeapAllocaterManager::getPtr(ptlib::HeapAllocaterManager::HEAP_TYPE_UNKNOWN_0)->memoryDump(); }

void SystemManager::syncRenderBegin() {}

void SystemManager::syncRenderEnd() {}

void SystemManager::syncUpdateBegin() {}

void SystemManager::syncUpdateEnd() {}

void SystemManager::setIBLSaturation(f32 saturation) { ptlib::ParticleManager::get()->unknown158 = saturation; }

void SystemManager::setDirScale(f32 scale) { ptlib::ParticleManager::get()->unknown150 = scale; }

void SystemManager::setSpecScale(f32 scale) { ptlib::ParticleManager::get()->unknown14c = scale; }

void SystemManager::setResponsiveAA(f32 responsiveAA) { ptlib::ParticleManager::get()->unknown148 = responsiveAA; }

void SystemManager::setAllDisp(bool isDisp) { ptlib::ParticleManager::get()->unknown168 = isDisp; }

void SystemManager::setDisableCollision(bool isDisable) { ptlib::ParticleManager::get()->unknown168 = isDisable; }

void SystemManager::MainDraw(const ptlib::DrDrawWorkInfoEF *info, int viewIndex, bool isBlur) { ptlib::ParticleManager::get()->draw(info, viewIndex, isBlur); }

void SystemManager::es_pack_free_(u64 pack) { ptlib::ParticleManager::get()->emitter_pack_free(pack); }

void SystemManager::es_free(void *data) {
  if (data != nullptr) {
    ptlib::ParticleManager::get()->emitter_set_free(static_cast<ptlib::FxHeader *>(data), 0);
  }
}

SystemManager::CommandList *SystemManager::getCommand(u32 index) { return index == 0 ? &unknown220[0] : &unknown220[1]; }

f32 SystemManager::getGlobalSpeed() { return unknown78240[unknown7824c]; }

bool SystemManager::es_is_loop(void *data) const { return static_cast<ptlib::FxHeader *>(data)->unknown09 != 0; }

bool SystemManager::es_contais_cam_axis(void *data) { return static_cast<ptlib::FxHeader *>(data)->unknown0a != 0; }

const char *SystemManager::es_get_name(void *data) { return static_cast<ptlib::FxHeader *>(data)->unknown14; }

void SystemManager::log() { ptlib::efMsg("%04x log()", unknown7824e); }

SystemManager::CommandList *SystemManager::getCommandPtr(u32 index) { return index == 0 ? &unknown220[0] : &unknown220[1]; }

void SystemManager::setGlobalSpeed(f32 speed) { unknown78240[unknown7824c == 0] = speed; }

void SystemManager::setCategorySpd(u32 category, f32 speed) { ptlib::emitter::EmitterManager::getPtr()->unknown1700[category] = speed; }

void SystemManager::end() {}

void SystemManager::apply(const ptlib::DrDrawWorkInfoEF *info) {
  if (unknown09) {
    MainLoop(info);
  }
}

void SystemManager::opacityRender(const ptlib::DrDrawWorkInfoEF *info) {}

void SystemManager::lastRender(const ptlib::DrDrawWorkInfoEF *info) {}

void SystemManager::finalRender(const ptlib::DrDrawWorkInfoEF *info) { testRender(info); }

} // namespace effect
