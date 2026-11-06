#include "ptlib/renderer/particle_renderer_obj.h"

#include <algorithm>
#include <string.h>

#include "grlib/cglib_color_ctl.h"
#include "grlib/cglib_depth_ctl.h"
#include "grlib/cglib_render_target.h"
#include "mm/mm_std_base.h"
#include "mm_math/mm_col4.h"
#include "mm_math/mm_mat44.h"
#include "mm_math/mm_quat.h"
#include "mm_math/mm_vec3.h"
#include "mm_math/mm_vec3_na.h"
#include "mm_mtl/fixed_vector.h"
#include "ptlib/emitter/particle_emitter.h"
#include "ptlib/emitter/particle_emitter_ext.h"
#include "ptlib/emitter/particle_emitter_manager.h"
#include "ptlib/emitter/particle_emitter_param.h"
#include "ptlib/emitter/particle_emitter_particle.h"
#include "ptlib/material/particle_material_manager.h"
#include "ptlib/particle_system.h"
#include "ptlib/renderer/screen_render_manager.h"
// clang-format off
#include "particle_renderer_back_buffer.cpp"
#include "particle_renderer_shader.cpp"
#include "ptlib/particle_intrusive_list.h"
#include "ptlib/particle_math.h"
// clang-format on

namespace ptlib {
namespace renderer {

constexpr int ARG_DRAW_TYPE_NONE = 5;
constexpr int PRIORITY_MAX = 0xff;

namespace {
// Inferred role: blend functions selected by the packed blend type; original name unknown.
const GrlBlendFunc unknown003[11] = {GRL_BLEND_FUNC_ZERO,
                                     GRL_BLEND_FUNC_ONE,
                                     GRL_BLEND_FUNC_SRC_COLOR,
                                     GRL_BLEND_FUNC_ONE_MINUS_SRC_COLOR,
                                     GRL_BLEND_FUNC_SRC_ALPHA,
                                     GRL_BLEND_FUNC_ONE_MINUS_SRC_ALPHA,
                                     GRL_BLEND_FUNC_UNKNOWN_7,
                                     GRL_BLEND_FUNC_UNKNOWN_8,
                                     GRL_BLEND_FUNC_UNKNOWN_9,
                                     GRL_BLEND_FUNC_ONE_MINUS_DST_COLOR,
                                     GRL_BLEND_FUNC_UNKNOWN_11};
// Inferred role: blend combines selected by the packed blend type; original name unknown.
const GrlBlendCombine unknown004[5] = {GRL_BLEND_COMBINE_ADD, GRL_BLEND_COMBINE_SUBTRACT, GRL_BLEND_COMBINE_MIN, GRL_BLEND_COMBINE_MAX, GRL_BLEND_COMBINE_UNKNOWN_3};
// Inferred role: writes the position, the compute index and the transform of a particle, shared by the screen and string attributes; original name unknown.
// Inferred role: packed blend type selected by an alpha mode; original name unknown.
const u8 unknown006[32] = {0x00, 0x02, 0x01, 0x03, 0x02, 0x01, 0x06, 0x05, 0x07, 0x08, 0x06, 0x05, 0x07, 0x04, 0x00, 0x20,
                           0x40, 0x60, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00};
inline void unknown005(f32 *uniform, emitter::ParticleObj *particle) {
  memcpy(&uniform[0], &particle->unknown10, sizeof(mm::Vec3NA));
  reinterpret_cast<u32 *>(uniform)[3] = particle->unknown148;
  memcpy(&uniform[4], &particle->unknown94, sizeof(mm::Vec3NA));
  uniform[7] = particle->unknownc8;
  memcpy(&uniform[8], &particle->unknown60, sizeof(mm::Quat));
  memcpy(&uniform[12], &particle->unknowna0, sizeof(mm::Vec3NA));
}
} // namespace

void RenderGrObj::fxcore_setup_target_update(int index) {
  if (index == 0) {
    grlib::CGLibRenderTarget *target = unknown258->unknown000;
    int width = target->unknownbc;
    int height = target->unknownc0;
    if (unknown00.unknown18->unknownbc != width / 2 || unknown00.unknown18->unknownc0 != height / 2) {
      unknown00.setTextureAll(width, height, 0.0f, 0.0f, unknown258->unknown1d0);
    }
  }
  unknown00.bufferClear();
}

void RenderGrObj::draw_capture() {
  if (unknown258->unknown060 != nullptr && screen::ScreenRenderManager::isValid()) {
    screen::ScreenRenderManager *screenRenderManager = screen::ScreenRenderManager::getPtr();
    grlib::CGLibDepthCtl::setDepthControl(0, 0, GRL_COMPARE_ALWAYS);
    grlib::CGLibColorCtl::setBlendOffControl(GRL_LOGIC_TYPE_COPY);
    unknown258->unknown060->setRenderTarget();
    screenRenderManager->RenderCapScreen(unknown258, unknown258->unknown000->unknown70, unknown258->unknown068->unknown70);
    *unknown258->unknown570 = 1;
    grlib::CGLibColorCtl::setBlendControl(GRL_BLEND_FUNC_SRC_ALPHA, GRL_BLEND_FUNC_ONE_MINUS_SRC_ALPHA, GRL_BLEND_COMBINE_ADD, GRL_LOGIC_TYPE_COPY);
    unknown270 = 0;
  }
}

u32 RenderGrObj::changeShader(u32 type, bool isSimple) {
  if (isSimple) {
    switch (type) {
    case 3:
      type = 4;
      break;
    case 6:
      type = 5;
      break;
    case 8:
      type = 7;
      break;
    }
  }
  return type;
}

void RenderGrObj::set_alpha_mode(int type, bool isSimple) {
  if (isSimple) {
    grlib::CGLibDepthCtl::setDepthControl(1, 0, GRL_COMPARE_LEQUAL);
  } else {
    grlib::CGLibDepthCtl::setDepthControl(0, 0, GRL_COMPARE_ALWAYS);
  }
  if ((type & 0x9f) == 0) {
    return;
  }
  u32 mode = type & 0xff9f;
  if (mode <= 0x7f) {
    mode = unknown006[mode];
  }
  myGX2SetBlendControl(mode);
}

void RenderGrObj::myGX2SetBlendControl(int type) {
  if (type >= 0x80) {
    // The original indexes the 11-entry function table and the 5-entry combine table with 4-bit fields, so it reads past their ends (original bug, reproduced).
    GrlBlendFunc source = unknown003[(type >> 8) & 0xf];
    GrlBlendFunc destination = unknown003[(type >> 12) & 0xf];
    GrlBlendCombine combine = unknown004[type & 0xf];
    grlib::CGLibColorCtl::setBlendControl(source, destination, combine, source, destination, combine, GRL_LOGIC_TYPE_COPY);
    return;
  }
  switch (type) {
  case 0:
    grlib::CGLibColorCtl::setBlendOffControl(GRL_LOGIC_TYPE_COPY);
    break;
  case 1:
  case 5:
    grlib::CGLibColorCtl::setBlendControl(
        GRL_BLEND_FUNC_SRC_ALPHA, GRL_BLEND_FUNC_ONE_MINUS_SRC_ALPHA, GRL_BLEND_COMBINE_ADD, GRL_BLEND_FUNC_ZERO, GRL_BLEND_FUNC_ONE_MINUS_SRC_ALPHA, GRL_BLEND_COMBINE_ADD, GRL_LOGIC_TYPE_COPY);
    break;
  case 2:
    grlib::CGLibColorCtl::setBlendControl(GRL_BLEND_FUNC_SRC_ALPHA, GRL_BLEND_FUNC_ONE, GRL_BLEND_COMBINE_ADD, GRL_BLEND_FUNC_ZERO, GRL_BLEND_FUNC_ONE, GRL_BLEND_COMBINE_ADD, GRL_LOGIC_TYPE_COPY);
    break;
  case 3:
    grlib::CGLibColorCtl::setBlendControl(
        GRL_BLEND_FUNC_SRC_ALPHA, GRL_BLEND_FUNC_ONE, GRL_BLEND_COMBINE_UNKNOWN_3, GRL_BLEND_FUNC_ZERO, GRL_BLEND_FUNC_ONE, GRL_BLEND_COMBINE_UNKNOWN_3, GRL_LOGIC_TYPE_COPY);
    break;
  case 4:
    grlib::CGLibColorCtl::setBlendControl(
        GRL_BLEND_FUNC_ONE_MINUS_DST_COLOR, GRL_BLEND_FUNC_ONE, GRL_BLEND_COMBINE_ADD, GRL_BLEND_FUNC_ONE_MINUS_DST_COLOR, GRL_BLEND_FUNC_ONE, GRL_BLEND_COMBINE_ADD, GRL_LOGIC_TYPE_COPY);
    break;
  case 6:
    grlib::CGLibColorCtl::setBlendControl(GRL_BLEND_FUNC_SRC_ALPHA, GRL_BLEND_FUNC_ONE, GRL_BLEND_COMBINE_ADD, GRL_BLEND_FUNC_ONE, GRL_BLEND_FUNC_ONE, GRL_BLEND_COMBINE_ADD, GRL_LOGIC_TYPE_COPY);
    break;
  case 7:
    grlib::CGLibColorCtl::setBlendControl(
        GRL_BLEND_FUNC_ONE, GRL_BLEND_FUNC_SRC_ALPHA, GRL_BLEND_COMBINE_ADD, GRL_BLEND_FUNC_ZERO, GRL_BLEND_FUNC_SRC_ALPHA, GRL_BLEND_COMBINE_ADD, GRL_LOGIC_TYPE_COPY);
    break;
  case 8:
    grlib::CGLibColorCtl::setBlendControl(
        GRL_BLEND_FUNC_ONE, GRL_BLEND_FUNC_SRC_ALPHA, GRL_BLEND_COMBINE_UNKNOWN_3, GRL_BLEND_FUNC_ZERO, GRL_BLEND_FUNC_SRC_ALPHA, GRL_BLEND_COMBINE_UNKNOWN_3, GRL_LOGIC_TYPE_COPY);
    // The original has no break here and asserts after setting the blend (original bug, reproduced).
  default:
#line 1570
    MM_ASSERT(0);
    break;
  }
}

RenderGrObj::RenderGrObj() : unknown40(nullptr), unknown48(nullptr), unknown50(nullptr), unknown60(mm::Vec3::zero), unknown70(nullptr) {
  clear();
  unknown80.x = 0.0f;
  unknown80.y = 0.0f;
  unknown80.z = 0.0f;
  unknown80.w = 0.0f;
  unknown90.x = 0.0f;
  unknown90.y = 0.0f;
  unknown90.z = 0.0f;
  unknown90.w = 0.0f;
  unknowna0.x = 0.0f;
  unknowna0.y = 0.0f;
  unknowna0.z = 0.0f;
  unknowna0.w = 0.0f;
  unknown260 = nullptr;
  unknown270 = 0;
}

RenderGrObj::~RenderGrObj() { term(); }

void RenderGrObj::clear() { unknown00.unknown002(); }

BackBuffer *RenderGrObj::getBackBuffer() { return &unknown00; }

grlib::CGLibTextureBuffer *RenderGrObj::getDummyTex() { return unknown00.unknown20->unknown70; }

grlib::CGLibRenderTarget *RenderGrObj::getReduceBuffer(u32 index) { return unknown258->unknown060; }

void RenderGrObj::fxcore_setup_target() {
  grlib::CGLibRenderTarget *target = unknown258->unknown000;
  grlib::CGLibRenderTarget *subTarget = unknown258->unknown068;
  unknown00.createAll(
      unknown268, static_cast<s32>(target->unknownbc), static_cast<s32>(target->unknownc0), static_cast<s32>(subTarget->unknownbc), static_cast<s32>(subTarget->unknownc0), unknown258->unknown1d0);
}

material::MaterialBase *RenderGrObj::tex_loadmem(int key, void *data, int size, int format, u8 *userData, bool isResident, void *owner) {
  material::MaterialBase *material = material::MaterialManager::getPtr()->create(key, material::eMaterialType_UNKNOWN_0, data, size, false, userData, owner, format);
  if (material != nullptr && isResident) {
    material->unknown3c = true;
  }
  return material;
}

void RenderGrObj::tex_freemem(int key, void *owner) {
  int materialKey = key;
  material::MaterialManager::getPtr()->destroyMateriaByKey(materialKey, owner);
}

void RenderGrObj::tex_freemem(void *material, void *owner) { material::MaterialManager::getPtr()->destroyMateriaByKey(material, owner); }

void RenderGrObj::fxcore_setup_target_core(int &width, int &height, int &subWidth, int &subHeight) {
  width = unknown258->unknown000->unknownbc;
  height = unknown258->unknown000->unknownc0;
  subWidth = unknown258->unknown068->unknownbc;
  subHeight = unknown258->unknown068->unknownc0;
}

void RenderGrObj::setEffectRenderTarget(grlib::CGLibRenderTarget *target, grlib::CGLibDepthBuffer *depth, grlib::CGLibRenderTarget *subTarget1, grlib::CGLibRenderTarget *subTarget2) {
  if (subTarget1 == nullptr) {
    if (depth == nullptr) {
      if (subTarget2 != nullptr) {
        target->setRenderTarget(*subTarget2);
        return;
      }
      target->setRenderTarget();
      return;
    }
    if (subTarget2 != nullptr) {
      target->setRenderTarget(*depth, *subTarget2);
      return;
    }
    target->setRenderTarget(*depth);
    return;
  }
  if (depth == nullptr) {
    if (subTarget2 != nullptr) {
      target->setRenderTarget(*subTarget1, *subTarget2);
      return;
    }
    target->setRenderTarget(*subTarget1);
    return;
  }
  if (subTarget2 != nullptr) {
    target->setRenderTarget(*depth, *subTarget1, *subTarget2);
    return;
  }
  target->setRenderTarget(*depth, *subTarget1);
}

void *RenderGrObj::lockStream(emitter::EmitterObj *emitter) {
  if (emitter != nullptr) {
    return emitter->lockStream(unknown258);
  }
  return nullptr;
}

void RenderGrObj::unlockStream(emitter::EmitterObj *emitter, int count) {
  if (emitter != nullptr) {
    emitter->unlockStream(unknown258, count);
  }
}

void RenderGrObj::term() {
  unknown00.unknown001();
  unknownb0.term();
}

bool RenderGrObj::draw_type1_setup(emitter::EmitterObj *emitter, DrawTailWork *work) {
  work->unknown00 = emitter::EmitterObj::move_type1_sort(nullptr, emitter);
  if (work->unknown00 == nullptr) {
    work->unknown00 = emitter->unknown40.m_size != 0 ? &*emitter->unknown40.begin() : nullptr;
    if (work->unknown00 == nullptr) {
      return true;
    }
  }
  work->unknown08 = 0;
  work->unknown14 = 0;
  work->unknown18 = 0;
  emitter::EmitterArg *arg = emitter->unknown98;
  work->unknown14 = arg->unknown106;
  work->unknown10 = arg->unknown120;
  return false;
}

void RenderGrObj::draw_common_atr_wpn_tail(WpnTailShdVtxLayout *layout, mm::Vec3 &position, emitter::ParticleObj *particle, emitter::ParticleObj *top, f32 alpha, u32 drawType, DrawTailWork *work) {
  layout->unknown00[0] = position.x;
  layout->unknown00[1] = position.y;
  layout->unknown00[2] = position.z;
  layout->unknown0c = particle->unknown148;
  memcpy(layout->unknown10, &particle->unknown94, sizeof(mm::Vec3NA));
  layout->unknown1c = particle->unknownc8;
  memcpy(layout->unknown20, &particle->unknown60, sizeof(mm::Quat));
  memcpy(layout->unknown30, &particle->unknowna0, sizeof(mm::Vec3NA));
  layout->unknown3c = 1.0f - (!work->unknown1c ? top->unknown70.y : top->unknown70.z);
  switch (work->unknown18) {
  case 0:
    layout->unknown40[0] = particle->unknown70.x;
    layout->unknown40[1] = particle->unknown70.y;
    layout->unknown40[2] = particle->unknown70.z;
    break;
  case 1:
    memset(layout->unknown40, 0, sizeof(layout->unknown40));
    break;
  case 2:
    layout->unknown40[0] = particle->unknown70.x;
    layout->unknown40[1] = 0.0f;
    layout->unknown40[2] = 0.0f;
    break;
  }
  layout->unknown50 = particle->getExtParam().unknown17c;
  layout->unknown54 = particle->getExtParam().unknown180;
  switch (drawType) {
  case 1:
  case 3:
  case 4:
  case 6:
    layout->unknown58 = particle->getExtParam().unknownf8 / static_cast<f32>(unknown258->unknown550);
    layout->unknown5c = -particle->getExtParam().unknownf8 / static_cast<f32>(unknown258->unknown554);
    break;
  case 2:
  case 5:
    layout->unknown58 = particle->getExtParam().unknownf8;
    layout->unknown5c = particle->getExtParam().unknown22c;
    break;
  }
}

void RenderGrObj::draw_common_atr_normal(f32 *uniform, emitter::ParticleObj *particle, f32 alpha, u32 drawType, f32 scaleX, f32 scaleY) {
  uniform[0] = particle->unknown10.x + particle->unknown50.x;
  uniform[1] = particle->unknown10.y + particle->unknown50.y;
  uniform[2] = particle->unknown10.z + particle->unknown50.z;
  reinterpret_cast<u32 *>(uniform)[3] = particle->unknown148;
  uniform[4] = particle->unknown94.x;
  uniform[5] = particle->unknown94.y;
  uniform[6] = particle->unknown94.z + particle->unknown14c;
  uniform[7] = particle->unknownc8;
  memcpy(&uniform[8], &particle->unknown60, sizeof(mm::Quat));
  memcpy(&uniform[12], &particle->unknowna0, sizeof(mm::Vec3NA));
  uniform[15] = particle->getExtParam().unknownd0;
  memcpy(&uniform[16], &particle->unknown70, sizeof(mm::Vec3NA));
  uniform[19] = particle->getExtParam().unknownf4;
  memcpy(&uniform[20], &particle->getExtParam().unknownc0, sizeof(vec2f));
  memcpy(&uniform[22], &particle->getExtParam().unknownc8, sizeof(vec2f));
  memcpy(&uniform[24], &particle->getExtParam().unknowne4, sizeof(vec2f));
  memcpy(&uniform[26], &particle->getExtParam().unknownec, sizeof(vec2f));
  uniform[28] = particle->getExtParam().unknown17c;
  uniform[29] = particle->getExtParam().unknown180;
  switch (drawType) {
  case 1:
  case 9:
  case 11:
  case 12:
    uniform[30] = 0.0f;
    uniform[31] = 0.0f;
    break;
  case 4:
  case 5:
  case 7:
    uniform[30] = particle->getExtParam().unknownf8;
    uniform[31] = particle->getExtParam().unknown22c;
    break;
  case 6:
  case 8:
    uniform[30] = particle->getExtParam().unknownf8;
    uniform[31] = 1.0f;
    break;
  case 10:
  case 15:
    uniform[30] = 0.0f;
    uniform[31] = 0.0f;
    uniform[32] = particle->getExtParam().unknown108_f32[0];
    uniform[33] = particle->getExtParam().unknown108_f32[1];
    uniform[34] = particle->getExtParam().unknown108_f32[2];
    uniform[35] = particle->getExtParam().unknown108_f32[6];
    uniform[36] = particle->getExtParam().unknown108_f32[3];
    uniform[37] = particle->getExtParam().unknown108_f32[4];
    uniform[38] = particle->getExtParam().unknownf8;
    uniform[39] = 1.0f;
    break;
  case 14:
    uniform[30] = particle->getExtParam().unknownf8;
    uniform[31] = 0.0f;
    uniform[32] = particle->getExtParam().unknownd4;
    uniform[33] = particle->getExtParam().unknownd8;
    uniform[34] = particle->getExtParam().unknowndc;
    uniform[35] = particle->getExtParam().unknowne0;
    break;
  default:
    uniform[30] = particle->getExtParam().unknownf8;
    uniform[31] = 0.0f;
    break;
  }
}

void RenderGrObj::draw_common_atr_strip(f32 *uniform, emitter::ParticleObj *particle, emitter::ParticleObj *top, f32 alpha, u32 drawType, DrawTailWork *work, mm::Vec3 position, mm::Vec3 direction,
                                        f32 scaleX, f32 scaleY) {
  memcpy(&uniform[0], &particle->unknown10, sizeof(mm::Vec3NA));
  reinterpret_cast<u32 *>(uniform)[3] = particle->unknown148;
  if (particle->unknownc8 < -2.5f) {
    memcpy(&uniform[4], &position, sizeof(mm::Vec3NA));
    uniform[7] = particle->unknownc8;
    memcpy(&uniform[8], &direction, sizeof(mm::Vec3NA));
  } else {
    memcpy(&uniform[4], &particle->unknown94, sizeof(mm::Vec3NA));
    uniform[7] = particle->unknownc8;
    memcpy(&uniform[8], &particle->unknown60, sizeof(mm::Quat));
  }
  memcpy(&uniform[12], &particle->unknowna0, sizeof(mm::Vec3NA));
  switch (work->unknown18) {
  case 0:
    uniform[16] = particle->unknown70.x;
    uniform[17] = particle->unknown70.y;
    uniform[18] = particle->unknown70.z;
    break;
  case 1:
    memset(&uniform[16], 0, sizeof(mm::Vec3NA));
    break;
  case 2:
    uniform[16] = particle->unknown70.x;
    uniform[17] = 0.0f;
    uniform[18] = 0.0f;
    break;
  case 3:
    uniform[16] = uniform[17] = uniform[18] = particle->getExtParam().getEmParent()->unknown98->unknown11c;
    break;
  }
  if (work->unknown1c) {
    uniform[15] = top->unknown70.z;
  } else {
    uniform[15] = top->unknown70.y;
  }
  uniform[19] = static_cast<f32>(work->unknown08) * top->unknownc4;
  uniform[20] = particle->getExtParam().unknown17c;
  uniform[21] = particle->getExtParam().unknown180;
  uniform[22] = particle->getExtParam().unknownf8 / static_cast<f32>(unknown258->unknown550);
  uniform[23] = -particle->getExtParam().unknownf8 / static_cast<f32>(unknown258->unknown554);
}

void RenderGrObj::draw_common_atr_screen(f32 *uniform, emitter::ParticleObj *particle, f32 alpha, u32 drawType, f32 scaleX, f32 scaleY) {
  unknown005(uniform, particle);
  uniform[15] = particle->getExtParam().unknownd0;
  memcpy(&uniform[16], &particle->unknown70, sizeof(mm::Vec3NA));
  uniform[19] = particle->getExtParam().unknownf4;
}

void RenderGrObj::draw_common_atr_string(f32 *uniform, emitter::ParticleObj *particle, emitter::ParticleObj *top, f32 alpha, u32 drawType, DrawTailWork *work, f32 scaleX, f32 scaleY) {
  unknown005(uniform, particle);
  switch (work->unknown18) {
  case 0:
    uniform[16] = particle->unknown70.x;
    uniform[17] = particle->unknown70.y;
    uniform[18] = particle->unknown70.z;
    break;
  case 1:
    memset(&uniform[16], 0, sizeof(mm::Vec3NA));
    break;
  case 2:
    uniform[16] = particle->unknown70.x;
    uniform[17] = 0.0f;
    uniform[18] = 0.0f;
    break;
  }
  if (work->unknown1c) {
    uniform[15] = top->unknown70.z;
  } else {
    uniform[15] = top->unknown70.y;
  }
  uniform[19] = static_cast<f32>(work->unknown08) * top->unknownc4;
  uniform[20] = particle->getExtParam().unknown17c;
  uniform[21] = particle->getExtParam().unknown180;
  uniform[22] = particle->getExtParam().unknownf8 / static_cast<f32>(unknown258->unknown550);
  uniform[23] = -particle->getExtParam().unknownf8 / static_cast<f32>(unknown258->unknown554);
}

void RenderGrObj::draw_common_uconst(f32 *uniform, emitter::EmitterObj *emitter) {
  emitter::EmitterArg *arg = emitter->unknown98;
  if (arg == nullptr) {
    return;
  }
  f32 value94 = arg->unknown94;
  f32 value128 = arg->unknown128;
  if (emitter->unknown260 == 0xff) {
    value128 = -3.0f;
  }
  f32 value120 = arg->unknown120;
  f32 valuee0 = arg->unknowne0;
  f32 fade = emitter->calc_fade();
  uniform[0] = unknown80.x;
  uniform[1] = unknown80.y;
  uniform[2] = unknown80.z;
  uniform[3] = value94;
  uniform[4] = 0.0f;
  uniform[5] = 0.0f;
  uniform[6] = 0.0f;
  uniform[7] = 0.0f;
  uniform[8] = 0.0f;
  uniform[9] = 0.0f;
  uniform[10] = 0.0f;
  uniform[11] = 0.0f;
  uniform[12] = valuee0;
  uniform[13] = value128;
  uniform[14] = value120;
  uniform[15] = fade;
}

void RenderGrObj::draw_common_uparam(f32 *uniform, u32 *flags, emitter::EmitterObj *emitter) {
  flags[0] = 0;
  flags[1] = 0;
  flags[2] = 0;
  flags[3] = 0;
  emitter::EmitterArg *arg = emitter->unknown98;
  f32 value11c = arg->unknown11c;
  f32 value128 = arg->unknown128;
  if (arg->unknown104 == 0 || value11c < 0.0f) {
    value11c = 1000.0f;
  }
  if (ParticleManager::get()->getFxParam()->unknown12 != 0 && value128 > 0.0f) {
    value128 = 0.0f;
  }
  if (emitter->unknown260 == 0xff) {
    value128 = -3.0f;
  }
  f32 mode = (arg->unknown104 & 0xf0) == 0x90 || arg->unknown104 == 0xd ? 1.0f : 0.0f;
  uniform[4] = uniform[5] = uniform[6] = arg->unknown12c;
  f32 rate = arg->unknown130;
  f32 scale = 1.0f / unknown258->unknown544;
  uniform[7] = scale + rate * (1.0f - scale);
  if ((arg->unknown104 & 0xfe) == 4) {
    uniform[8] = arg->unknown134;
  } else {
    uniform[8] = 0.0f;
  }
  uniform[9] = arg->unknown124;
  uniform[10] = arg->unknown138;
  uniform[11] = (arg->unknown108 & 4) != 0 ? 1.0f : 0.0f;
  f32 fade = emitter->calc_fade();
  uniform[0] = value11c;
  uniform[1] = value128;
  uniform[2] = mode;
  uniform[3] = fade;
  if (emitter->m_param.unknown77 != 0) {
    *flags |= 1;
  }
  if (arg->unknown108 & 0x10) {
    *flags |= 2;
  }
  if (arg->unknown108 & 0x20) {
    *flags |= 4;
  }
}

void RenderGrObj::prioritySort(emitter::EmitterManager &emitterManager, int prioIdx, FxParam &fxParam, mm::mtl::FixedVector<sort::EmitterSort, EMITTER_SORT_MAX> &sorts) {
  const CamParam &camParam = ParticleManager::get()->getCamParam();
  if (!ParticleManager::get()->unknown168) {
    return;
  }
  for (emitter::EmitterObj *emitter = emitterManager.unknown1750; emitter != nullptr; emitter = emitter->unknownc8) {
    if (emitter->isRenderCheck() && emitter->unknown274 == prioIdx && emitter->m_param.unknown71 && emitter->m_param.unknown40.a > 0.0f && emitter->unknown98 != nullptr &&
        emitter->unknown98->unknown100 != ARG_DRAW_TYPE_NONE) {
      const mm::Mat44 &view = camParam.m_viewMatrix;
      sort::EmitterSort entry;
      entry.unknown00 = emitter->unknown98->unknownfc == 0;
      int priority = emitter->unknown260 + emitter->m_param.unknown70;
      if (priority < 0) {
        priority = 0;
      }
      if (priority > PRIORITY_MAX) {
        priority = PRIORITY_MAX;
      }
      entry.unknown01 = priority;
      entry.unknown04 = emitter->m_param.unknown00.unknown30.x * view.rows[0][2] + emitter->m_param.unknown00.unknown30.y * view.rows[1][2] + emitter->m_param.unknown00.unknown30.z * view.rows[2][2] +
                        view.rows[3][2];
      entry.unknown08 = false;
      entry.unknown10 = emitter;
      sorts.push_back(entry);
    }
  }
  std::stable_sort(sorts.begin(), sorts.end(), sort::Compare());
}

} // namespace renderer
} // namespace ptlib
