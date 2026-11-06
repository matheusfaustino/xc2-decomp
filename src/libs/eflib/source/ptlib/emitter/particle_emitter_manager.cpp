#include "ptlib/emitter/particle_emitter_manager.h"

#include <math.h>
#include <string.h>

#include "dr/dr_gpu_check_base.h"
#include "grlib/cglib_color_ctl.h"
#include "grlib/cglib_ctl_shader.h"
#include "grlib/cglib_ctl_texture.h"
#include "grlib/cglib_ctl_vertex.h"
#include "grlib/cglib_dbl_uniform_block.h"
#include "grlib/cglib_dec_shader.h"
#include "grlib/cglib_depth_ctl.h"
#include "grlib/cglib_polygon.h"
#include "grlib/cglib_shader.h"
#include "mm/mm_std_base.h"
#include "mm_math/mm_col3.h"
#include "mm_math/mm_col4.h"
#include "mm_math/mm_vec2.h"
#include "mm_math/mm_vec3.h"
#include "mm_math/mm_vec3_na.h"
#include "mm_mtl/fixed_vector.h"
#include "ptlib/emitter/compute/particle_emitter_compute.h"
#include "ptlib/emitter/particle_emitter.h"
#include "ptlib/emitter/particle_emitter_ext.h"
#include "ptlib/emitter/particle_emitter_param.h"
#include "ptlib/emitter/particle_emitter_particle.h"
#include "ptlib/emitter/particle_emitter_set_node.h"
#include "ptlib/forcefield/particle_force_field.h"
#include "ptlib/math/particle_keyframe_ip.h"
#include "ptlib/particle_math.h"
#include "ptlib/particle_system.h"
#include "ptlib/renderer/particle_renderer_eff_state.h"
#include "ptlib/renderer/particle_renderer_manager.h"
#include "ptlib/renderer/particle_renderer_obj.h"
#include "ptlib/renderer/particle_renderer_shader.h"
#include "ptlib/renderer/particle_renderer_stream.h"

namespace ptlib {

namespace emitter {

namespace {
EmitterManager *s_instance;
} // namespace

int EmitterObj::getParentParticleState() const {
  if (unknown80 == nullptr) {
    return 0;
  }
  if (unknown80->m_extParam->m_emParent != nullptr) {
    if (unknown88 != unknown80->unknown138) {
      return 2;
    }
    return 1;
  }
  return 2;
}

void EmitterObj::emitter_dead(u8 value) {
  unknown188 = value;
  for (IntrusiveList<ParticleObj>::iterator it = unknown40.begin(); it != unknown40.end(); ++it) {
    if (it->unknownc0 == 0x7fff) {
      it->unknownc0 = 0;
    }
  }
}

f32 EmitterObj::getCateSpd() {
  EmitterManager *manager = EmitterManager::getPtr();
  return manager->unknown1700[m_param.unknown90];
}

int EmitterObj::move_func(f32 speed, int isMove) {
#line 1933 "ptlib/emitter/particle_emitter.cpp"
  MM_ASSERT(m_move_type < MOVE_TYPE_MAX);
  switch (m_move_type) {
  case MOVE_TYPE_UNKNOWN_1:
    return move01_type0(speed, isMove);
  case MOVE_TYPE_UNKNOWN_2:
    return move01_type1(speed, isMove);
  case MOVE_TYPE_UNKNOWN_3:
    move01_type9(speed, isMove);
    break;
  case MOVE_TYPE_UNKNOWN_4:
    return move01_type_model(speed, isMove);
  case MOVE_TYPE_UNKNOWN_5:
    return move01_type5(speed, isMove);
  case MOVE_TYPE_UNKNOWN_6:
    return move01_type12(speed, isMove);
  }
  return 0;
}

EmitterManager *EmitterManager::get() {
#line 21 "ptlib/emitter/particle_emitter_manager.cpp"
  MM_ASSERT(getPtr());
  return getPtr();
}

EmitterManager *&EmitterManager::getPtr() { return s_instance; }

void EmitterManager::moveAfter() {}

void EmitterObj::remove_particle_free_tbl(ParticleObj *particle) {
  unknown40.erase(particle);
  particle->particle_clear_union();
  EmitterManager::get()->addFreeParticleFront(particle);
}

void EmitterObj::releaseParticleAll() {
  IntrusiveList<ParticleObj>::iterator it = unknown40.begin();
  while (it != unknown40.end()) {
    ParticleObj &particle = *it;
    unknown40.erase(it);
    EmitterManager::get()->addFreeParticleFront(&particle);
    it = unknown40.begin();
  }
}

void EmitterObj::emitter_set_param_sub_pos_only2(EmitterParam *param) {
  if (unknown80 == nullptr) {
    m_param.unknown00.unknown30 = mm::Vec3(param->unknown00.unknown30);
  }
  ParticleManager::get()->update_model_spd(this, 1.0f);
}

void EmitterObj::emitter_set_param_sub(EmitterParam *param) {
  u32 type;
  if (unknown80 == nullptr) {
    memcpy(&m_param.unknown00, param, sizeof(unknown001));
    if (unknown98 == nullptr) {
      return;
    }
    type = unknown98->unknown109;
    if (type == 0xff) {
      m_param.unknown40 = param->unknown40;
    } else if (type - 1 < 0xe) {
      m_param.unknown60 = param->unknown60;
    }
    m_param.unknown6c = param->unknown6c;
    m_param.unknown50 = param->unknown50;
    m_param.unknown70 = param->unknown70;
    f32 radius = param->unknown68;
    if (radius > 0.0f && m_pGpuCheck == nullptr) {
      mm::Vec3NA start = unknown98->unknown14;
      mm::Vec3NA end = unknown98->unknown44;
      f32 dy = start.y - end.y;
      f32 dx = start.x - end.x;
      f32 dz = start.z - end.z;
      unknown31c = sqrtf(dx * dx + dy * dy + dz * dz);
      unknown31c = (unknown31c + (unknown98->unknown9c + unknown98->unknowna0)) * 1.5f;
      const DrDrawWorkInfoEF *info = renderer::RenderManager::get()->getRenderGrObj()->unknown258;
      if (unknown31c < radius) {
        unknown318 = radius;
      } else {
        unknown318 = unknown31c;
      }
      m_pGpuCheck = ml::DrGpuCheckBase::createEffGpuCheckObj(info->unknown560);
    }
  } else {
    type = unknown98->unknown109;
  }
  if (type == 0xff) {
    m_param.unknown40 = param->unknown40;
  }
  m_param.unknown71 = param->unknown71;
  m_param.unknown72 = param->unknown72;
  m_param.unknown73 = param->unknown73;
  if (param->speed >= 0.0f) {
    m_param.speed = param->speed;
  }
  if (param->unknown90 != -1) {
    m_param.unknown90 = param->unknown90;
  }
  m_param.unknown7c = param->unknown7c;
  m_param.unknown74 = param->unknown74;
  m_param.unknown76 = param->unknown76;
  m_param.unknown77 = param->unknown77;
  m_param.unknown80 = param->unknown80;
  m_param.unknown84 = param->unknown84;
  m_param.unknown88 = param->unknown88;
  m_param.unknown8c = param->unknown8c;
  m_param.unknown94 = param->unknown94;
}

IntrusiveList<EmitterObj> &EmitterManager::getEmitterActiveList() { return unknown17a8; }

IntrusiveList<EmitterObj>::iterator EmitterManager::getEmitterActiveIt(EmitterObj *emitter) { return unknown17a8.find(emitter); }

EmitterObj *EmitterManager::getEmitterFree() {
  if (unknown17c0.m_size == 0) {
    return nullptr;
  }
  IntrusiveList<EmitterObj>::iterator it = unknown17c0.begin();
  EmitterObj &emitter = *it;
  unknown17c0.erase(it);
  return &emitter;
}

void EmitterManager::addEmitterFree(EmitterObj *emitter) {
  if (emitter->unknown60 != nullptr) {
    emitter->unknown60->release();
  }
  emitter->releaseParticleAll();
  unknown17c0.push_back(emitter);
}

void EmitterManager::removeEmitterActive(EmitterObj *emitter) {
  if (emitter->unknown60 != nullptr) {
    emitter->unknown60->release();
  }
  emitter->releaseParticleAll();
  unknown17a8.erase(getEmitterActiveIt(emitter));
}

void EmitterManager::addEmitterActive(EmitterObj *emitter) { unknown17a8.push_back(emitter); }

void EmitterManager::addEmitterActiveAfterParent(EmitterObj *emitter, EmitterObj *parent) { unknown17a8.push_back(emitter); }

ParticleObj *EmitterManager::particleAlloc(EmitterObj *emitter, bool isFront) {
  ParticleObj *et = getFreeParticle();
  if (et != nullptr) {
    if (isFront) {
      emitter->unknown40.push_front(et);
    } else {
      emitter->unknown40.push_back(et);
    }
    u8 isModel = 1;
    if (emitter->unknown98 == nullptr || emitter->unknown98->unknown00 != 4) {
      isModel = 0;
    }
    et->unknown13e = isModel;
    u8 isLight = 1;
    if (emitter->unknown98 == nullptr || emitter->unknown98->unknown00 != 5) {
      isLight = 0;
    }
    et->unknown144 = isLight;
    et->initUtilObj();
    et->unknown20 = et->unknown10;
    et->getExtParam().m_emParent = emitter;
#line 893
    MM_ASSERT(et->getExtParam().getEmParent());
  }
  return et;
}

ParticleObj *EmitterManager::getFreeParticle() {
  if (unknown17d8.m_size < 1) {
    return nullptr;
  }
  IntrusiveList<ParticleObj>::iterator it = unknown17d8.begin();
  if (it == unknown17d8.end()) {
    return nullptr;
  }
  ParticleObj *particle = it.get();
  unknown17d8.erase(it);
  particle->unknown001();
  return particle;
}

void EmitterManager::addFreeParticleFront(ParticleObj *particle) { unknown17d8.push_front(particle); }

void EmitterManager::addFreeParticleBack(ParticleObj *particle) { unknown17d8.push_back(particle); }

void EmitterManager::emitter_set_param_pos_only(u32 handle, EmitterParam *param) {
  for (IntrusiveList<EmitterObj>::iterator it = unknown17a8.begin(); it != unknown17a8.end(); ++it) {
    if (it->unknown264 == handle) {
      if (it->unknown80 == nullptr) {
        memcpy(&it->m_param.unknown00, param, sizeof(unknown001));
      }
      ParticleManager::get()->update_model_spd(it.get(), 1.0f);
    }
  }
}

void EmitterManager::emitter_set_param_pos_only2(u32 handle, EmitterParam *param) {
  for (IntrusiveList<EmitterObj>::iterator it = unknown17a8.begin(); it != unknown17a8.end(); ++it) {
    if (it->unknown264 == handle) {
      it->emitter_set_param_sub_pos_only2(param);
    }
  }
}

void EmitterManager::emitter_set_param(u32 handle, EmitterParam *param) {
  for (IntrusiveList<EmitterObj>::iterator it = unknown17a8.begin(); it != unknown17a8.end(); ++it) {
    if (it->unknown264 == handle) {
      it->emitter_set_param_sub(param);
    }
  }
  f32 skip = param->unknown7c;
  if (skip >= 1.0f) {
    int count = static_cast<int>(skip * 0.5f);
    if (count < 1) {
      count = 1;
    } else {
      skip = 2.0f;
    }
    for (int i = 0; i < count; i++) {
      emitter_update(handle, skip, false);
    }
  }
}

void EmitterManager::emitter_set_param(FxHeader *header, EmitterParam *param) {
  for (IntrusiveList<EmitterObj>::iterator it = unknown17a8.begin(); it != unknown17a8.end(); ++it) {
    if (it->unknown58 == header) {
      it->emitter_set_param_sub(param);
    }
  }
}

void EmitterManager::emitter_set_disp(u32 handle, int isDisp) {
  for (IntrusiveList<EmitterObj>::iterator it = unknown17a8.begin(); it != unknown17a8.end(); ++it) {
    if (it->unknown264 == handle) {
      it->m_param.unknown71 = isDisp;
    }
  }
}

void EmitterManager::emitter_set_alpha(u32 handle, f32 alpha) {
  for (IntrusiveList<EmitterObj>::iterator it = unknown17a8.begin(); it != unknown17a8.end(); ++it) {
    if (it->unknown264 == handle) {
      it->m_param.unknown40.a = alpha;
    }
  }
}

void EmitterManager::emitter_set_category(u32 handle, u32 category) {
  for (IntrusiveList<EmitterObj>::iterator it = unknown17a8.begin(); it != unknown17a8.end(); ++it) {
    if (it->unknown264 == handle) {
      it->m_param.unknown90 = category;
    }
  }
}

void EmitterManager::emitter_set_nowait(u32 handle, int isNoWait) {
  for (IntrusiveList<EmitterObj>::iterator it = unknown17a8.begin(); it != unknown17a8.end(); ++it) {
    if (it->unknown264 == handle) {
      it->m_param.unknown72 = isNoWait;
    }
  }
}

void EmitterManager::emitter_set_viewnum(u32 handle, int viewnum) {
  for (IntrusiveList<EmitterObj>::iterator it = unknown17a8.begin(); it != unknown17a8.end(); ++it) {
    if (it->unknown264 == handle) {
      it->unknown274 = viewnum;
      it->unknown90 = ParticleManager::getGrp(viewnum & 0xff);
    }
  }
}

void EmitterManager::emitter_set_link(FxHeader *header) {
  if (unknown1808.m_size < 1) {
#line 908
    MM_ASSERT(0);
    return;
  }
  IntrusiveList<EmitterSetNode>::iterator it = unknown1808.begin();
  EmitterSetNode &node = *it;
  unknown1808.erase(it);
  node.unknown18 = header;
  unknown17f0.push_back(&node);
}

ParticleObj *EmitterManager::createCommon(EmitterObj *emitter) {
  ParticleObj *particle;
  if (emitter->unknown261_0 == 1) {
    particle = particleAlloc(emitter, false);
  } else {
    particle = particleAlloc(emitter, true);
  }
  if (particle != nullptr) {
    mm::Vec3 goal;
    EmitterObj::calc_start2goal01(emitter, &particle->unknown10, &goal);
    particle->unknown30 = goal;
    particle->unknownc2 = 0;
    particle->unknown20 = particle->unknown10;
    particle->unknown40 = mm::Vec3::zero;
    particle->unknown50 = mm::Vec3::zero;
    particle->getExtParam().unknown108 = nullptr;
    particle->getExtParam().unknown110 = nullptr;
    particle->getExtParam().unknown118 = nullptr;
    particle->getExtParam().unknown120 = nullptr;
    particle->getExtParam().unknown110 = nullptr;
  }
  return particle;
}

void EmitterManager::emitter_create_local_common_update_kfinit(EmitterObj *emitter, ParticleObj *particle, EmitterArg *arg) {
  if (arg->unknown00 == 4) {
    ParticleManager::get()->update_model(particle, emitter, arg);
  }
  if (arg->unknown00 == 5) {
    ParticleManager::get()->update_plight(particle, emitter);
  }
  memset(particle->getExtParam().unknown190, 0xff, sizeof(particle->getExtParam().unknown190));
  memset(particle->getExtParam().unknown214, 0, sizeof(particle->getExtParam().unknown214));
}

void EmitterManager::emitter_create_local_common_life(EmitterObj *emitter, ParticleObj *particle, EmitterArg *arg) {
  u16 base = arg->unknown8e;
  u32 life;
  f32 rate;
  if (base == 0) {
    life = 0x7fff;
    rate = 1.0f;
  } else {
    f32 range = arg->unknown90;
    f32 random = math::unknown001(emitter->unknown268);
    life = static_cast<u32>(range * random) + base;
    f32 total = arg->unknown8c;
    rate = total / static_cast<f32>(life);
  }
  particle->unknownc0 = life;
  particle->unknownbc = rate;
  particle->unknownb8 = 1.0f;
  particle->unknownc3 = 0;
}

void EmitterManager::emitter_set_casparam(u32 handle, const void *model, EmitterParam *param) {
  for (IntrusiveList<EmitterObj>::iterator it = unknown17a8.begin(); it != unknown17a8.end(); ++it) {
    if (it->unknown264 == handle && it->unknown80 == nullptr) {
      it->unknownc0->unknown00 = model;
      it->unknownc0->unknown70[0] = 0;
      it->emitter_set_param_sub(param);
    }
  }
}

void *EmitterManager::createFunc(EmitterObj *emitter, ParticleObj *particle, f32 frame, int param) {
  if (particle->unknownd4_int == 1) {
    return create_local01(emitter, particle, frame, param);
  }
  efAstMsg("0", "create func set error !!!");
  return nullptr;
}

void EmitterManager::emitter_set_casname(u32 handle, const void *model, const char *name, int index, f32 frame, int flags) {
  if (name != nullptr && strlen(name) >= 0x10) {
    efAstMsg("0", "emitter_set_casname:too long name[%s]", name);
  }
  for (IntrusiveList<EmitterObj>::iterator it = unknown17a8.begin(); it != unknown17a8.end(); ++it) {
    if (it->unknown264 == handle && it->unknown80 == nullptr) {
      if (model == nullptr) {
        it->unknownc0->unknown00 = nullptr;
        it->unknownc0->unknown70[0] = 0;
      } else {
        it->unknownc0->unknown00 = model;
        it->m_param.unknown6c = frame;
        it->m_param.unknown73 = flags;
        if (name != nullptr) {
          strcpy(it->unknownc0->unknown70, name);
        }
      }
    }
  }
}

bool EmitterManager::emitter_set_check(FxHeader *header) {
  for (IntrusiveList<EmitterSetNode>::iterator it = unknown17f0.begin(); it != unknown17f0.end(); ++it) {
    if (header != nullptr) {
      return true;
    }
  }
  return false;
}

void EmitterManager::emitter_pack_free(u64 pack) {
  int count = 0;
  for (IntrusiveList<EmitterSetNode>::iterator it = unknown17f0.begin(); it != unknown17f0.end(); ++it) {
    if (it->unknown18->unknown48 == pack) {
      count++;
    }
  }
  if (count > 0) {
    do {
      for (IntrusiveList<EmitterSetNode>::iterator it = unknown17f0.begin(); it != unknown17f0.end(); ++it) {
        if (it->unknown18->unknown48 == pack) {
          emitter_set_free(it->unknown18, 1);
          count--;
          break;
        }
      }
    } while (count > 0);
  }
}

void EmitterManager::emitter_update_sub(CamParam &camParam, mm::mtl::FixedVector<EmitterObj *, 2001> &freeEmitters, u32 handle, bool isNoWait, f32 speed,
                                        mm::mtl::FixedVector<EmitterObj *, 2001> &updateEmitters, bool isUnknown) {
  for (IntrusiveList<EmitterObj>::iterator it = unknown17a8.begin(); it != unknown17a8.end(); ++it) {
    EmitterObj *emitter = it.get();
    if (emitter->unknown274 != camParam.unknown12c) {
      continue;
    }
    if (emitter->unknown26c_4) {
      emitter->unknown10 = 0;
      freeEmitters.push_back(emitter);
      continue;
    }
    if (handle != 0 ? emitter->unknown264 != handle : emitter->unknown264 == 0) {
      continue;
    }
    if (isNoWait && emitter->m_param.unknown72 == 0) {
      continue;
    }
    bool isWait;
    do {
      isWait = false;
      switch (emitter->getParentParticleState()) {
      case 2:
        emitter->emitter_dead(0);
        break;
      case 1:
        if (static_cast<s16>(emitter->unknown80->unknownc0) < 2) {
          emitter->emitter_dead(0);
        } else if (emitter->unknown261_4 != 0 && emitter->unknown80->unknown13f == 0) {
          // Game bug, reproduced: the wait moves to the next emitter without testing the end of the list nor its view, free, handle and no-wait filters.
          ++it;
          emitter = it.get();
          isWait = true;
        }
        break;
      }
    } while (isWait);
    if (!emitter->unknown26c_0 && emitter->m_param.speed * speed * emitter->getCateSpd() <= 0.0f) {
      continue;
    }
    emitter->unknown268 = emitter->unknown268 * 0x41c64e6d + 0x3039;
    updateEmitters.push_back(emitter);
  }
}

EmitterObj *EmitterManager::emitter_free_core(EmitterObj *emitter) {
  if (emitter->unknown98 == nullptr) {
    efMsg("free type error");
  } else {
    switch (emitter->unknown98->unknown00) {
    case 4:
      for (IntrusiveList<ParticleObj>::iterator it = emitter->unknown40.begin(); it != emitter->unknown40.end(); ++it) {
        (*it).particle_clear_union();
      }
      break;
    case 5:
      for (IntrusiveList<ParticleObj>::iterator it = emitter->unknown40.begin(); it != emitter->unknown40.end(); ++it) {
        *it;
      }
      for (IntrusiveList<ParticleObj>::iterator it = emitter->unknown40.begin(); it != emitter->unknown40.end(); ++it) {
        (*it).particle_clear_union();
      }
      break;
    default:
      for (IntrusiveList<ParticleObj>::iterator it = emitter->unknown40.begin(); it != emitter->unknown40.end(); ++it) {
        (*it).particle_clear_union();
      }
      break;
    }
  }
  if (emitter->unknown90 != nullptr) {
    emitter->unknown90 = nullptr;
  }
  EmitterObj *next = static_cast<EmitterObj *>(emitter->unknown00);
  if (emitter->m_pGpuCheck != nullptr) {
    emitter->m_pGpuCheck->release();
    emitter->m_pGpuCheck = nullptr;
  }
  emitter->unknown98 = nullptr;
  removeEmitterActive(emitter);
  addEmitterFree(emitter);
  return next;
}

void EmitterManager::updateFeedback(mm::mtl::FixedVector<EmitterObj *, 2001> &emitters, f32 speed) {
  forcefield::FfManager *ffManager = forcefield::FfManager::getPtr();
  ffManager->update(speed);
  for (EmitterObj **it = emitters.begin(); it != emitters.end(); ++it) {
    EmitterObj *emitter = *it;
    if (emitter->isRenderCheck()) {
      for (IntrusiveList<ParticleObj>::iterator particle = emitter->unknown40.begin(); particle != emitter->unknown40.end(); ++particle) {
        if (emitter->unknown274 == 0) {
          ffManager->moveForce(particle.get());
        }
      }
    }
  }
}

void EmitterManager::move() {
  EmitterObj *lastTop = nullptr;
  EmitterObj *lastSecond = nullptr;
  EmitterObj *lastThird = nullptr;
  for (IntrusiveList<EmitterObj>::iterator it = unknown17a8.begin(); it != unknown17a8.end(); ++it) {
    EmitterObj *emitter = it.get();
    EmitterArg *arg = emitter->unknown98;
    bool isRender = emitter->isRenderCheck();
    if (arg != nullptr && isRender && ((arg->unknown00 & 0xfe) == 4 || (emitter->m_param.unknown71 != 0 && 0.0f < emitter->m_param.unknown40.a))) {
      switch (arg->unknown00) {
      case 0:
      case 1:
      case 9:
        if (lastTop == nullptr) {
          unknown1750 = emitter;
        } else {
          lastTop->unknownc8 = emitter;
        }
        lastTop = emitter;
        break;
      case 2:
        if (lastSecond == nullptr) {
          unknown1760 = emitter;
        } else {
          lastSecond->unknownc8 = emitter;
        }
        lastSecond = emitter;
        break;
      case 4:
      case 5:
        if (lastThird == nullptr) {
          unknown1758 = emitter;
        } else {
          lastThird->unknownc8 = emitter;
        }
        lastThird = emitter;
        break;
      case 0xc:
        if (lastTop != nullptr) {
          lastTop->unknownc8 = emitter;
        } else {
          unknown1750 = emitter;
        }
        lastTop = emitter;
        break;
      }
      if (lastTop == nullptr) {
        unknown1750 = nullptr;
      } else {
        lastTop->unknownc8 = nullptr;
      }
      if (lastSecond == nullptr) {
        unknown1760 = nullptr;
      } else {
        lastSecond->unknownc8 = nullptr;
      }
      if (lastThird == nullptr) {
        unknown1758 = nullptr;
      } else {
        lastThird->unknownc8 = nullptr;
      }
      unknown1768 = nullptr;
    }
  }
}

void EmitterManager::emitter_set_speed(u32 handle, f32 speed) {
  for (IntrusiveList<EmitterObj>::iterator it = unknown17a8.begin(); it != unknown17a8.end(); ++it) {
    if (handle == 0 || it->unknown264 == handle) {
      it->m_param.speed = speed;
    }
  }
}

bool EmitterObj::isRenderCheck() const {
  if (m_pGpuCheck == nullptr || m_pGpuCheck->isRender()) {
    return true;
  }
  return unknown18c < 60.0f;
}

// The game executable compiles ComputeManager::calculateParticle in the translation unit of the EmitterManager functions, where EmitterManager::getPtr is inlined.
void ComputeManager::calculateParticle() {
  grlib::CGLibColorCtl::setBlendOffControl(GRL_LOGIC_TYPE_COPY);
  grlib::CGLibColorCtl::setColorBufferOn();
  grlib::CGLibDepthCtl::setDepthControl(0, 0, GRL_COMPARE_ALWAYS);
  grlib::CGLibPolygon::setCullingOff();
  EmitterManager *emitterManager = EmitterManager::getPtr();
  renderer::RenderGrObj *renderGrObj = renderer::RenderManager::get()->getRenderGrObj();
  const DrDrawWorkInfoEF *info = renderGrObj->unknown258;
  grlib::CGLibDecShader *decShader = renderGrObj->unknownb0.getShaderByIndex(renderer::EFF_SHD_TYPE_UNKNOWN_13);
  grlib::CGLibCtlShader::setShader(*decShader, renderer::EffStateManager::get()->unknown4f0);
  u32 shadowLocation1 = (decShader->unknown38 + decShader->unknown58)->getTextureLocation("shw1");
  u32 shadowLocation2 = (decShader->unknown38 + decShader->unknown58)->getTextureLocation("shw2");
  u32 shadowLocation3 = (decShader->unknown38 + decShader->unknown58)->getTextureLocation("shw3");
  grlib::CGLibCtlTexture::setTexture(*info->unknown090, renderer::EffStateManager::get()->unknown360, shadowLocation1);
  grlib::CGLibCtlTexture::setTexture(*info->unknown098, renderer::EffStateManager::get()->unknown360, shadowLocation2);
  grlib::CGLibCtlTexture::setTexture(*info->unknown0a0, renderer::EffStateManager::get()->unknown360, shadowLocation3);
  u32 commonBuffOffset = (decShader->unknown38 + decShader->unknown58)->getUniformBlockOfs("CommonBuff");
  u32 readBlockOffset = (decShader->unknown38 + decShader->unknown58)->getUniformBlockOfs("ReadBlock");
  unknown108[unknown208].unknown08.setUniformBlock(commonBuffOffset);
  int uniformBlockSize = ComputeUniform::s_nUniformBlockSize[0];
  bool isRasterizerSet = false;
  for (IntrusiveList<EmitterObj>::iterator it = emitterManager->getEmitterActiveList().begin(); it != emitterManager->getEmitterActiveList().end(); ++it) {
    if (it->isRenderCheck() && it->unknown27c != 0) {
      if (!isRasterizerSet) {
        grlib::CGLibPolygon::setRasterizer(true);
      }
      unknown48.unknown08.setUniformBlock(readBlockOffset, it->unknown27e << 9, 0x200);
      grlib::CGLibCtlVertex::setUniformVertex(unknown08.unknown08, uniformBlockSize, it->unknown27a * uniformBlockSize, 0, 0);
      unknown08.unknown08.setStorageBuffer(0);
      grlib::CGLibCtlVertex::renderVertex(GRL_PRIMITIVE_POINTS, it->unknown27c);
      isRasterizerSet = true;
    }
  }
  if (isRasterizerSet) {
    grlib::CGLibPolygon::setRasterizer(false);
    grlib::CGLibCtlShader::setMemoryBarrierBit(1);
  }
}

} // namespace emitter
} // namespace ptlib
