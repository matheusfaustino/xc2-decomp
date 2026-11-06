#pragma once
#include <stddef.h>

#include "mm_math/mm_vec3.h"
#include "mm_math/mm_vec4.h"
#include "mm_mtl/fixed_vector.h"
#include "ptlib/renderer/particle_renderer_back_buffer.h"
#include "ptlib/renderer/particle_renderer_shader.h"
#include "types.h"

namespace grlib {
class CGLibDepthBuffer;
class CGLibRenderTarget;
class CGLibTextureBuffer;
} // namespace grlib

namespace ptlib {

class StaticAllocObj;
struct DrDrawWorkInfoEF;
struct FxParam;

namespace emitter {
class EmitterManager;
class EmitterObj;
class ParticleObj;
} // namespace emitter

namespace material {
class MaterialBase;
} // namespace material

namespace renderer {

// Provenance: the name comes from the mangled game symbols of RenderGrObj::draw_type1_setup and the tail draw functions.
// Evidence: renderEmitter keeps it at sp+0x60 with the next local at sp+0x80.
struct DrawTailWork {
  emitter::ParticleObj *unknown00; // Inferred role: first particle of the tail; original member name unknown.
  int unknown08;                   // Inferred role: tail particle index; original member name unknown.
  u8 _0c[0x10 - 0x0c];
  f32 unknown10;  // Inferred role: copied from the emitter argument; original member name unknown.
  u32 unknown14;  // Inferred role: copied from the emitter argument; original member name unknown.
  int unknown18;  // Inferred role: tail color mode; original member name unknown.
  bool unknown1c; // Inferred role: selects the z component of the top particle; original member name unknown.
};

static_assert(offsetof(DrawTailWork, unknown08) == 0x08, "");
static_assert(offsetof(DrawTailWork, unknown10) == 0x10, "");
static_assert(offsetof(DrawTailWork, unknown14) == 0x14, "");
static_assert(offsetof(DrawTailWork, unknown18) == 0x18, "");
static_assert(offsetof(DrawTailWork, unknown1c) == 0x1c, "");
static_assert(sizeof(DrawTailWork) == 0x20, "");

// Provenance: the name comes from the mangled game symbol of RenderGrObj::draw_common_atr_wpn_tail.
// Evidence: renderEmitter fills two of them per tail particle, 0x60 bytes apart.
struct WpnTailShdVtxLayout {
  f32 unknown00[3]; // Inferred role: position; original member name unknown.
  u32 unknown0c;    // Inferred role: index in the compute particle buffer; original member name unknown.
  f32 unknown10[3]; // Inferred role: copied from the particle; original member name unknown.
  f32 unknown1c;    // Inferred role: copied from the particle; original member name unknown.
  f32 unknown20[4]; // Inferred role: rotation; original member name unknown.
  f32 unknown30[3]; // Inferred role: copied from the particle; original member name unknown.
  f32 unknown3c;    // Inferred role: tail coordinate; original member name unknown.
  f32 unknown40[3]; // Inferred role: tail color; original member name unknown.
  u8 _4c[0x50 - 0x4c];
  f32 unknown50; // Inferred role: draw parameter; original member name unknown.
  f32 unknown54; // Inferred role: draw parameter; original member name unknown.
  f32 unknown58; // Inferred role: draw size; original member name unknown.
  f32 unknown5c; // Inferred role: draw size; original member name unknown.
};

static_assert(offsetof(WpnTailShdVtxLayout, unknown0c) == 0x0c, "");
static_assert(offsetof(WpnTailShdVtxLayout, unknown20) == 0x20, "");
static_assert(offsetof(WpnTailShdVtxLayout, unknown3c) == 0x3c, "");
static_assert(offsetof(WpnTailShdVtxLayout, unknown50) == 0x50, "");
static_assert(offsetof(WpnTailShdVtxLayout, unknown5c) == 0x5c, "");
static_assert(sizeof(WpnTailShdVtxLayout) == 0x60, "");

namespace sort {

struct EmitterSort {
  bool unknown00; // Inferred role: opaque; original member name unknown.
  u8 unknown01;   // Inferred role: priority; original member name unknown.
  f32 unknown04;  // Inferred role: depth; original member name unknown.
  bool unknown08; // Inferred role: capture; original member name unknown.
  u8 _09[0x10 - 0x09];
  emitter::EmitterObj *unknown10; // Inferred role: emitter; original member name unknown.
};

static_assert(offsetof(EmitterSort, unknown04) == 0x04, "");
static_assert(offsetof(EmitterSort, unknown08) == 0x08, "");
static_assert(offsetof(EmitterSort, unknown10) == 0x10, "");
static_assert(sizeof(EmitterSort) == 0x18, "");

struct Compare {
  bool operator()(const EmitterSort &sort, const EmitterSort &other) const {
    if (sort.unknown01 < other.unknown01) {
      return true;
    }
    if (sort.unknown01 == other.unknown01) {
      if (sort.unknown00 > other.unknown00) {
        return true;
      }
      if (sort.unknown00 == other.unknown00 && sort.unknown04 < other.unknown04) {
        return true;
      }
    }
    return false;
  }
};

} // namespace sort

class RenderGrObj {
public:
  static const u32 EMITTER_SORT_MAX = 4000;

  RenderGrObj();
  ~RenderGrObj();
  void term();
  void clear();
  BackBuffer *getBackBuffer();
  grlib::CGLibTextureBuffer *getDummyTex();
  grlib::CGLibRenderTarget *getReduceBuffer(u32 index);
  void fxcore_setup_target();
  material::MaterialBase *tex_loadmem(int key, void *data, int size, int format, u8 *userData, bool isResident, void *owner);
  void tex_freemem(int key, void *owner);
  void tex_freemem(void *material, void *owner);
  void fxcore_setup_target_core(int &width, int &height, int &subWidth, int &subHeight);
  void setEffectRenderTarget(grlib::CGLibRenderTarget *target, grlib::CGLibDepthBuffer *depth, grlib::CGLibRenderTarget *subTarget1, grlib::CGLibRenderTarget *subTarget2);
  void fxcore_setup_target_update(int index);
  void draw_capture();
  u32 changeShader(u32 type, bool isSimple);
  void myGX2SetBlendControl(int type);
  void set_alpha_mode(int type, bool isSimple);
  void *lockStream(emitter::EmitterObj *emitter);
  void unlockStream(emitter::EmitterObj *emitter, int count);
  void draw_common_uconst(f32 *uniform, emitter::EmitterObj *emitter);
  void draw_common_uparam(f32 *uniform, u32 *flags, emitter::EmitterObj *emitter);
  bool draw_type1_setup(emitter::EmitterObj *emitter, DrawTailWork *work);
  void draw_common_atr_wpn_tail(WpnTailShdVtxLayout *layout, mm::Vec3 &position, emitter::ParticleObj *particle, emitter::ParticleObj *top, f32 alpha, u32 drawType, DrawTailWork *work);
  void draw_common_atr_normal(f32 *uniform, emitter::ParticleObj *particle, f32 alpha, u32 drawType, f32 scaleX, f32 scaleY);
  void draw_common_atr_strip(f32 *uniform, emitter::ParticleObj *particle, emitter::ParticleObj *top, f32 alpha, u32 drawType, DrawTailWork *work, mm::Vec3 position, mm::Vec3 direction, f32 scaleX,
                             f32 scaleY);
  void draw_common_atr_screen(f32 *uniform, emitter::ParticleObj *particle, f32 alpha, u32 drawType, f32 scaleX, f32 scaleY);
  void draw_common_atr_string(f32 *uniform, emitter::ParticleObj *particle, emitter::ParticleObj *top, f32 alpha, u32 drawType, DrawTailWork *work, f32 scaleX, f32 scaleY);
  void draw(emitter::EmitterManager &emitterManager, FxParam &fxParam, int viewIndex, int isBlur);
  void prioritySort(emitter::EmitterManager &emitterManager, int prioIdx, FxParam &fxParam, mm::mtl::FixedVector<sort::EmitterSort, EMITTER_SORT_MAX> &sorts);

  BackBuffer unknown00; // Inferred role: back buffers; original member name unknown.
  u8 _30[0x40 - 0x30];
  void *unknown40; // Inferred role: cleared by the constructor; original member name unknown.
  void *unknown48; // Inferred role: cleared by the constructor; original member name unknown.
  void *unknown50; // Inferred role: cleared by the constructor; original member name unknown.
  u8 _58[0x60 - 0x58];
  mm::Vec3 unknown60; // Inferred role: reset by the constructor; original member name unknown.
  void *unknown70;    // Inferred role: cleared by the constructor; original member name unknown.
  u8 _78[0x80 - 0x78];
  mm::Vec4 unknown80;      // Inferred role: render target size ratios; original member name unknown.
  mm::Vec4 unknown90;      // Inferred role: render target size; original member name unknown.
  mm::Vec4 unknowna0;      // Inferred role: sub render target size; original member name unknown.
  ShaderManager unknownb0; // Inferred role: shaders; original member name unknown.
  u8 _250[0x258 - 0x250];
  const DrDrawWorkInfoEF *unknown258; // Inferred role: draw work info; original member name unknown.
  void *unknown260;                   // Inferred role: cleared by the constructor; original member name unknown.
  StaticAllocObj *unknown268;         // Inferred role: allocator; original member name unknown.
  s32 unknown270;                     // Inferred role: render target state; original member name unknown.
  u8 _274[0x280 - 0x274];
};

static_assert(offsetof(RenderGrObj, unknown40) == 0x40, "");
static_assert(offsetof(RenderGrObj, unknown50) == 0x50, "");
static_assert(offsetof(RenderGrObj, unknown60) == 0x60, "");
static_assert(offsetof(RenderGrObj, unknown70) == 0x70, "");
static_assert(offsetof(RenderGrObj, unknown80) == 0x80, "");
static_assert(offsetof(RenderGrObj, unknown90) == 0x90, "");
static_assert(offsetof(RenderGrObj, unknowna0) == 0xa0, "");
static_assert(offsetof(RenderGrObj, unknownb0) == 0xb0, "");
static_assert(offsetof(RenderGrObj, unknown258) == 0x258, "");
static_assert(offsetof(RenderGrObj, unknown260) == 0x260, "");
static_assert(offsetof(RenderGrObj, unknown268) == 0x268, "");
static_assert(offsetof(RenderGrObj, unknown270) == 0x270, "");
static_assert(sizeof(RenderGrObj) == 0x280, "");

} // namespace renderer
} // namespace ptlib
