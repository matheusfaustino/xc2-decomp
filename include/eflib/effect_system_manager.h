#pragma once
#include <stddef.h>

#include "mm_mtl/fixed_vector.h"
#include "ptlib/forcefield/particle_force_field.h"
#include "types.h"

namespace mtl {
class ALLOC_HANDLE;
} // namespace mtl

namespace ptlib {
struct DrDrawWorkInfoEF;
} // namespace ptlib

namespace effect {

class IEffectCallBack;
struct SendParam;

class SystemManager {
public:
  struct alignas(16) EsCommand {
    u8 _00[0xf0 - 0x00];
  };

  using CommandList = mm::mtl::FixedVector<EsCommand, 0x400>;

  static SystemManager *get();
  static SystemManager *&getPtr();
  static bool isValid();
  static void testRender(const ptlib::DrDrawWorkInfoEF *info);
  SystemManager();
  virtual ~SystemManager();
  static SystemManager *create(const ptlib::DrDrawWorkInfoEF *info);
  void Init(const ptlib::DrDrawWorkInfoEF *info);
  static void destroy();
  void Term();
  static void setUserFuncClear();
  static void setUserAllocFunc(void *(*allocSys)(void *, u32, u32, const char *, u32), void (*freeSys)(void *, void *), void *(*allocRes)(void *, u32, u32, const char *, u32),
                               void (*freeRes)(void *, void *), mtl::ALLOC_HANDLE (*getSysHandle)(), mtl::ALLOC_HANDLE (*getResHandle)());
  static void setUserFileFunc(void *(*read)(const char *, int &), void (*readEnd)(void *));
  static void setUserOutputString(void (*outputDebug)(const char *), void (*outputWarn)(const char *));
  static void setUserDrFunc(void *(*createMdl)(const void *, const char *, int, const void *, int), void (*deleteMdl)(void *), void *(*createLgt)(u32, int), void (*deleteLgt)(u32, void *),
                            void (*setMdlPos)(void *, void *), void (*setMdlMatrix)(void *, void *), void (*setMdlColor)(void *, void *), void (*setMdlAlpha)(void *, f32),
                            void (*setMdlSpd)(void *, void *), void (*setLgtPos)(u32, void *, void *), void (*setLgtMatrix)(u32, void *, void *), void (*setLgtParam)(u32, void *, void *),
                            void (*debugRenderer)(void *), void *(*createGrp)(int, int), void (*destroyGrp)(void *));
  void syncRenderBegin();
  void syncRenderEnd();
  void syncUpdateBegin();
  void syncUpdateEnd();
  void setIBLSaturation(f32 saturation);
  void setDirScale(f32 scale);
  void setSpecScale(f32 scale);
  void setResponsiveAA(f32 responsiveAA);
  void setAllDisp(bool isDisp);
  void setDisableCollision(bool isDisable);
  void updateCam(const ptlib::DrDrawWorkInfoEF *info);
  void MainDraw(const ptlib::DrDrawWorkInfoEF *info, int viewIndex, bool isBlur);
  void MainLoop(const ptlib::DrDrawWorkInfoEF *info);
  void es_command_core();
  void es_pause(bool isPause);
  void es_pack_free_(u64 pack);
  void es_pack_free(u64 pack);
  void es_pack_regist(const char *name, void *data, u32 size);
  void es_pack_regist(void *data, u32 size);
  u64 es_regist(void *data, u32 size);
  void *es_regist(const char *name, void *data, u32 size);
  void es_free(void *data);
  u32 es_create(void *data, bool isLoop);
  void es_set_param_pos_only(u32 handle, const SendParam *param);
  void es_set_param_pos_only2(u32 handle, const SendParam *param);
  void es_set_category(u32 handle, u32 category);
  void es_set_param(u32 handle, const SendParam *param);
  void es_set_alpha(u32 handle, f32 alpha);
  void es_set_speed(u32 handle, f32 speed);
  void es_set_viewnum(u32 handle, int viewnum);
  u32 es_create(void *data, const SendParam *param, bool isLoop);
  u32 es_create(void *data, const SendParam *param, const IEffectCallBack *callback, int unknown, int category, bool isLoop);
  void es_set_param(u32 handle, const SendParam *param, const IEffectCallBack *callback, int unknown, int category);
  void es_set_param_cas(u32 handle, const IEffectCallBack *callback, const SendParam *param);
  void es_set_param_cas(u32 handle, const IEffectCallBack *callback, const char *name, int unknown, f32 value, int category);
  CommandList *getCommand(u32 index);
  f32 getGlobalSpeed();
  void es_set_disp(u32 handle, int isDisp);
  void es_fade_core(void *data, u32 handle, int frame);
  void es_remove_cas(void *callback);
  int es_isalive(u32 handle);
  int es_getDeadFlag(u32 handle);
  bool es_is_loop(void *data) const;
  bool es_contais_cam_axis(void *data);
  const char *es_get_name(void *data);
  void es_clearall(bool isForce);
  void log();
  CommandList *getCommandPtr(u32 index);
  void setGlobalSpeed(f32 speed);
  void setCategorySpd(u32 category, f32 speed);
  void memoryDump();
  void begin();
  void end();
  void tail(bool flag);
  void apply(const ptlib::DrDrawWorkInfoEF *info);
  void opacityRender(const ptlib::DrDrawWorkInfoEF *info);
  void alphaRender(const ptlib::DrDrawWorkInfoEF *info);
  void lastRender(const ptlib::DrDrawWorkInfoEF *info);
  void finalRender(const ptlib::DrDrawWorkInfoEF *info);
  void blurRender(const ptlib::DrDrawWorkInfoEF *info);
  void getNum(u32 &emitterNum, u32 &particleNum, u32 &drawNum);
  void getNumDetail(u32 &emitterNum, u32 &particleNum, u32 &drawNum, u32 &workNum);
  u32 getEmitterForm(u32 handle);

  u8 _08[0x09 - 0x08];
  bool unknown09;                                          // Inferred role: update; original member name unknown.
  bool unknown0a;                                          // Inferred role: draw; original member name unknown.
  bool unknown0b;                                          // Inferred role: disp emitter; original member name unknown.
  bool unknown0c;                                          // Inferred role: disp emitter set; original member name unknown.
  mm::mtl::FixedVector<ptlib::AddForceParam, 8> unknown10; // Inferred role: forces; original member name unknown.
  CommandList unknown220[2];                               // Inferred role: commands; original member name unknown.
  f32 unknown78240[2];                                     // Inferred role: global speed; original member name unknown.
  bool unknown78248;                                       // Inferred role: clear force; original member name unknown.
  u8 _78249;
  bool unknown7824a; // Inferred role: disp particle texture; original member name unknown.
  bool unknown7824b; // Inferred role: work texture; original member name unknown.
  u8 unknown7824c;   // Inferred role: command buffer; original member name unknown.
  u8 unknown7824d;   // Inferred role: write buffer; original member name unknown.
  u8 unknown7824e;   // Inferred role: frame; original member name unknown.
  u8 unknown7824f;   // Inferred role: clear count; original member name unknown.
};

static_assert(offsetof(SystemManager, unknown09) == 0x09, "");
static_assert(offsetof(SystemManager, unknown0b) == 0x0b, "");
static_assert(offsetof(SystemManager, unknown10) == 0x10, "");
static_assert(offsetof(SystemManager, unknown220) == 0x220, "");
static_assert(offsetof(SystemManager, unknown78240) == 0x78240, "");
static_assert(offsetof(SystemManager, unknown7824a) == 0x7824a, "");
static_assert(offsetof(SystemManager, unknown7824d) == 0x7824d, "");
static_assert(offsetof(SystemManager, unknown7824f) == 0x7824f, "");
static_assert(sizeof(SystemManager) == 0x78250, "");

} // namespace effect
