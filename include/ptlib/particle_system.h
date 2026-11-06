#pragma once
#include <stddef.h>

#include "mm_math/mm_mat44.h"
#include "mtl/alloc_handle.h"
#include "types.h"

namespace grlib {
class CGLibRenderTarget;
class CGLibTextureBuffer;
} // namespace grlib

namespace mm {
struct Col4;
struct Vec3;
} // namespace mm

namespace ptlib {

namespace emitter {
struct EmitterArg;
class EmitterObj;
class ParticleObj;
struct EmitterParam;
} // namespace emitter

namespace renderer {
class RenderManager;
} // namespace renderer

struct DrDrawWorkInfoEF {
  grlib::CGLibRenderTarget *unknown000; // Inferred role: main target; original member name unknown.
  u8 _008[0x018 - 0x008];
  grlib::CGLibRenderTarget *unknown018; // Inferred role: downsampled depth target; original member name unknown.
  grlib::CGLibRenderTarget *unknown020; // Inferred role: original depth target; original member name unknown.
  u8 _028[0x040 - 0x028];
  grlib::CGLibRenderTarget *unknown040; // Inferred role: second color target; original member name unknown.
  u8 _048[0x058 - 0x048];
  grlib::CGLibRenderTarget *unknown058; // Inferred role: third color target; original member name unknown.
  grlib::CGLibRenderTarget *unknown060; // Inferred role: reduce target; original member name unknown.
  grlib::CGLibRenderTarget *unknown068; // Inferred role: sub target; original member name unknown.
  grlib::CGLibRenderTarget *unknown070; // Inferred role: bloom target; original member name unknown.
  u8 _078[0x090 - 0x078];
  grlib::CGLibTextureBuffer *unknown090; // Inferred role: shadow texture 1; original member name unknown.
  grlib::CGLibTextureBuffer *unknown098; // Inferred role: shadow texture 2; original member name unknown.
  grlib::CGLibTextureBuffer *unknown0a0; // Inferred role: shadow texture 3; original member name unknown.
  u8 _0a8[0x1b8 - 0x0a8];
  f32 unknown1b8; // Inferred role: fxaa ratio y; original member name unknown.
  u8 _1bc[0x1c8 - 0x1bc];
  f32 unknown1c8; // Inferred role: fxaa ratio z; original member name unknown.
  u8 _1cc[0x1d0 - 0x1cc];
  u32 unknown1d0; // Inferred role: buffer offset; original member name unknown.
  u8 _1d4[0x544 - 0x1d4];
  f32 unknown544; // Inferred role: resolution scale, its inverse weights the draw parameters; original member name unknown.
  u8 _548[0x550 - 0x548];
  u32 unknown550; // Inferred role: screen width; original member name unknown.
  u32 unknown554; // Inferred role: screen height; original member name unknown.
  u8 _558[0x560 - 0x558];
  u32 unknown560; // Inferred role: view index; original member name unknown.
  u8 _564[0x570 - 0x564];
  u8 *unknown570; // Inferred role: capture done flag; original member name unknown.
};

static_assert(offsetof(DrDrawWorkInfoEF, unknown018) == 0x018, "");
static_assert(offsetof(DrDrawWorkInfoEF, unknown020) == 0x020, "");
static_assert(offsetof(DrDrawWorkInfoEF, unknown040) == 0x040, "");
static_assert(offsetof(DrDrawWorkInfoEF, unknown058) == 0x058, "");
static_assert(offsetof(DrDrawWorkInfoEF, unknown060) == 0x060, "");
static_assert(offsetof(DrDrawWorkInfoEF, unknown068) == 0x068, "");
static_assert(offsetof(DrDrawWorkInfoEF, unknown070) == 0x070, "");
static_assert(offsetof(DrDrawWorkInfoEF, unknown544) == 0x544, "");
static_assert(offsetof(DrDrawWorkInfoEF, unknown090) == 0x090, "");
static_assert(offsetof(DrDrawWorkInfoEF, unknown098) == 0x098, "");
static_assert(offsetof(DrDrawWorkInfoEF, unknown0a0) == 0x0a0, "");
static_assert(offsetof(DrDrawWorkInfoEF, unknown1b8) == 0x1b8, "");
static_assert(offsetof(DrDrawWorkInfoEF, unknown1c8) == 0x1c8, "");
static_assert(offsetof(DrDrawWorkInfoEF, unknown1d0) == 0x1d0, "");
static_assert(offsetof(DrDrawWorkInfoEF, unknown550) == 0x550, "");
static_assert(offsetof(DrDrawWorkInfoEF, unknown554) == 0x554, "");
static_assert(offsetof(DrDrawWorkInfoEF, unknown560) == 0x560, "");
static_assert(offsetof(DrDrawWorkInfoEF, unknown570) == 0x570, "");

namespace emitter {

// Evidence: name from the game symbol of EmitterManager::emitter_alloc01; an entry of the emitter group table of an FxHeader.
struct EmitterGroup {
  u32 unknown00; // Inferred role: start delay copied to the emitter; original member name unknown.
  s16 unknown04; // Original member name unknown.
  u8 unknown06;  // Inferred role: priority copied to the emitter; original member name unknown.
  u8 unknown07;  // Original member name unknown.
  s32 unknown08; // Inferred role: offset of the parent group from this one, -1 for a top level group; original member name unknown.
  u32 unknown0c; // Inferred role: offset of the emitter argument from this group, 0 at the end of the table; original member name unknown.
};

static_assert(offsetof(EmitterGroup, unknown08) == 0x08, "");
static_assert(sizeof(EmitterGroup) == 0x10, "");

// Inferred role: parent of an emitter group, null for a top level group; original name unknown.
inline const EmitterGroup *unknown011(const EmitterGroup *group) {
  if (group->unknown08 == -1) {
    return nullptr;
  }
  return reinterpret_cast<const EmitterGroup *>(reinterpret_cast<const u8 *>(group) + group->unknown08);
}

} // namespace emitter

struct FxHeader {
  u8 _00[0x09 - 0x00];
  u8 unknown09; // Inferred role: loop; original member name unknown.
  u8 unknown0a; // Inferred role: cam axis; original member name unknown.
  u8 _0b[0x14 - 0x0b];
  char unknown14[0x20]; // Inferred role: name; original member name unknown.
  u8 _34[0x48 - 0x34];
  u64 unknown48;                      // Inferred role: pack; original member name unknown.
  emitter::EmitterGroup unknown50[1]; // Inferred role: emitter group table; original member name unknown.
};

static_assert(offsetof(FxHeader, unknown09) == 0x09, "");
static_assert(offsetof(FxHeader, unknown14) == 0x14, "");
static_assert(offsetof(FxHeader, unknown48) == 0x48, "");
static_assert(offsetof(FxHeader, unknown50) == 0x50, "");

struct FxParam {
  // Inferred role: sets the default values of the parameters; original name unknown.
  void unknown001() {
    unknown0d = false;
    unknown0e = false;
    unknown0f = 1;
    unknown10 = false;
    unknown11 = 0;
    unknown12 = 0;
    unknown14 = 1;
  }

  u8 _00[0x0d - 0x00];
  bool unknown0d; // Inferred role: pause; original member name unknown.
  bool unknown0e; // Inferred role: pause request; original member name unknown.
  u8 unknown0f;   // Original member name unknown.
  bool unknown10; // Inferred role: stop; original member name unknown.
  u8 unknown11;   // Original member name unknown.
  u8 unknown12;   // Original member name unknown.
  u8 _13[0x14 - 0x13];
  u32 unknown14; // Original member name unknown.
};

static_assert(offsetof(FxParam, unknown0d) == 0x0d, "");
static_assert(offsetof(FxParam, unknown10) == 0x10, "");
static_assert(offsetof(FxParam, unknown14) == 0x14, "");
static_assert(sizeof(FxParam) == 0x18, "");

// Inferred role: efb section header holding the offset of its data; original name unknown.
struct unknown001 {
  u32 unknown00; // Inferred role: magic "efb0"; original member name unknown.
  u8 _04[0x08 - 0x04];
  u8 unknown08; // Inferred role: section type; original member name unknown.
  u8 _09[0x0c - 0x09];
  u32 unknown0c; // Inferred role: offset of the next section of a pack, from this header; original member name unknown.
  u32 unknown10;
};

static_assert(offsetof(unknown001, unknown08) == 0x08, "");
static_assert(offsetof(unknown001, unknown0c) == 0x0c, "");
static_assert(offsetof(unknown001, unknown10) == 0x10, "");

struct ParseEfbParam {
  u8 *unknown00;         // Inferred role: efb or efs data; original member name unknown.
  const char *unknown08; // Inferred role: name, passed to parseEfb; original member name unknown.
  u8 *unknown10;         // Inferred role: parsed data; original member name unknown.
};

static_assert(offsetof(ParseEfbParam, unknown10) == 0x10, "");

// Inferred role: pointer at an offset from a base, null when the offset is 0; original name unknown.
template <typename T> inline const T *unknown002(const void *base, u32 offset) {
  if (offset == 0) {
    return nullptr;
  }
  return reinterpret_cast<const T *>(static_cast<const u8 *>(base) + offset);
}

template <typename T> inline T *unknown002(void *base, u32 offset) {
  if (offset == 0) {
    return nullptr;
  }
  return reinterpret_cast<T *>(static_cast<u8 *>(base) + offset);
}

struct CamParam {
  mm::Mat44 m_viewMatrix;
  u8 _40[0x12c - 0x40];
  u32 unknown12c; // Inferred role: view index the emitters are updated for; original member name unknown.
};

static_assert(offsetof(CamParam, unknown12c) == 0x12c, "");
static_assert(sizeof(CamParam) == 0x130, "");

class ParticleManager {
public:
  using AllocFunc = void *(*)(void *allocator, u32 size, u32 alignment, const char *file, u32 line);
  using FreeFunc = void (*)(void *allocator, void *ptr);
  using GetHandleFunc = mtl::ALLOC_HANDLE (*)();
  using ReadFunc = void *(*)(const char *path, int &size);
  using ReadEndFunc = void (*)(void *data);
  using OutputStringFunc = void (*)(const char *text);

  // Inferred role: func; original type name unknown.
  struct unknown003 {
    void *(*unknown00)(const void *model, const char *name, int index, const void *texture, int textureIndex); // Inferred role: create mdl; original member name unknown.
    void (*unknown08)(void *model);                                                                            // Inferred role: delete mdl; original member name unknown.
    void *(*unknown10)(u32 type, int index);                                                                   // Inferred role: create lgt; original member name unknown.
    void (*unknown18)(u32 type, void *light);                                                                  // Inferred role: delete lgt; original member name unknown.
    void (*unknown20)(void *model, void *pos);                                                                 // Inferred role: set mdl pos; original member name unknown.
    void (*unknown28)(void *model, void *matrix);                                                              // Inferred role: set mdl matrix; original member name unknown.
    void (*unknown30)(void *model, void *color);                                                               // Inferred role: set mdl color; original member name unknown.
    void (*unknown38)(void *model, f32 alpha);                                                                 // Inferred role: set mdl alpha; original member name unknown.
    void (*unknown40)(void *model, void *speed);                                                               // Inferred role: set mdl spd; original member name unknown.
    void (*unknown48)(u32 type, void *light, void *pos);                                                       // Inferred role: set lgt pos; original member name unknown.
    void (*unknown50)(u32 type, void *light, void *matrix);                                                    // Inferred role: set lgt matrix; original member name unknown.
    void (*unknown58)(u32 type, void *light, void *param);                                                     // Inferred role: set lgt param; original member name unknown.
    void *(*unknown60)(int index, int count);                                                                  // Inferred role: create grp; original member name unknown.
    void (*unknown68)(void *group);                                                                            // Inferred role: destroy grp; original member name unknown.
    void (*unknown70)(void *renderer);                                                                         // Inferred role: debug renderer; original member name unknown.
    u8 _78[0x80 - 0x78];
  };

  static AllocFunc s_pAllocFuncSys;
  static FreeFunc s_pFreeFuncSys;
  static AllocFunc s_pAllocFuncRes;
  static FreeFunc s_pFreeFuncRes;
  static GetHandleFunc s_pGetSysHandle;
  static GetHandleFunc s_pGetResHandle;
  static ReadFunc s_pReadFunc;
  static ReadEndFunc s_pReadEndFunc;
  static OutputStringFunc s_pOutputDebugStringFunc;
  static OutputStringFunc s_pOutputWarnStringFunc;
  static unknown003 s_pDrFunc;

  static bool isValid();
  static ParticleManager *&getPtr();
  static ParticleManager *get();
  static void setUserAllocFunc(AllocFunc allocSys, FreeFunc freeSys, AllocFunc allocRes, FreeFunc freeRes, GetHandleFunc getSysHandle, GetHandleFunc getResHandle);
  static void setUserFileFunc(ReadFunc read, ReadEndFunc readEnd);
  static void setUserOutputString(OutputStringFunc outputDebug, OutputStringFunc outputWarn);
  static void setUserDrFunc(void *(*createMdl)(const void *, const char *, int, const void *, int), void (*deleteMdl)(void *), void *(*createLgt)(u32, int), void (*deleteLgt)(u32, void *),
                            void (*setMdlPos)(void *, void *), void (*setMdlMatrix)(void *, void *), void (*setMdlColor)(void *, void *), void (*setMdlAlpha)(void *, f32),
                            void (*setMdlSpd)(void *, void *), void (*setLgtPos)(u32, void *, void *), void (*setLgtMatrix)(u32, void *, void *), void (*setLgtParam)(u32, void *, void *),
                            void (*debugRenderer)(void *), void *(*createGrp)(int, int), void (*destroyGrp)(void *));
  static void init(const DrDrawWorkInfoEF *info);
  static void *sysAlloc(u32 size, u32 alignment, const char *file, u32 line);
  void clear();
  FxParam *getFxParam();
  void fxcore_setup_target();
  static void *createGrp(int index, int count);
  static void term();
  static void tail();
  static void destroyGrp(void *group);
  static void sysFree(void *ptr);
  static void *resAlloc(u32 size, u32 alignment, const char *file, u32 line);
  static void resFree(void *ptr);
  static mtl::ALLOC_HANDLE getSysHandle();
  static mtl::ALLOC_HANDLE getResHandle();
  static void *readFileBlock(const char *path, int &size);
  static void outputString(const char *text);
  static void outputWarning(const char *text);
  static void *createMdl(const void *model, const char *name, int index, const void *texture, int textureIndex);
  static void deleteMdl(void *model);
  static void setMdlPos(void *model, void *pos);
  static void setMdlMatrix(void *model, void *matrix);
  static void setMdlColor(void *model, void *color);
  static void setMdlAlpha(void *model, f32 alpha);
  static void setMdlSpd(void *model, void *speed);
  static void *createLgt(u32 type, int index);
  static void deleteLgt(u32 type, void *light);
  static void setLgtPos(u32 type, void *light, void *pos);
  static void setLgtMatrix(u32 type, void *light, void *matrix);
  static void setLgtParam(u32 type, void *light, void *param);
  static void debugRenderer(void *renderer);
  static void *getGrp(int index);
  int get_prio_idx(int priority);
  CamParam &getCamParam();
  int convert_key_by_material_index(int key, FxHeader *header) const;
  void parse_efb_type0050(u8 *data);
  void parse_efb_type0051(u8 *data);
  int parse_efb_type0001(u8 *data);
  int parseEfb(ParseEfbParam *param);
  u8 *parseEfb(u8 *data, const char *name, u64 pack);
  u8 *parse_efs(const char *name, void *data, u32 size);
  renderer::RenderManager &getRenderManager();
  u32 emitter_set_create(FxHeader *header, emitter::EmitterParam *param, u32 key, bool option);
  bool emitter_set_check(FxHeader *header);
  void emitter_set_free_all(bool isForce);
  FxHeader *emitter_set_search(int type, const char *name, void *owner, u32 category);
  void emitter_set_param_pos_only(u32 handle, emitter::EmitterParam *param);
  void emitter_set_param_pos_only2(u32 handle, emitter::EmitterParam *param);
  void emitter_set_param(u32 handle, emitter::EmitterParam *param);
  void emitter_set_param(FxHeader *header, emitter::EmitterParam *param);
  void emitter_set_casname(u32 handle, const void *model, const char *name, int index, f32 frame, int flags);
  void emitter_set_casparam(u32 handle, const void *model, emitter::EmitterParam *param);
  void emitter_set_disp(u32 handle, int isDisp);
  void emitter_set_alpha(u32 handle, f32 alpha);
  void emitter_set_speed(u32 handle, f32 speed);
  void emitter_set_category(u32 handle, u32 category);
  void emitter_set_viewnum(u32 handle, int viewnum);
  void emitter_set_nowait(u32 handle, int isNoWait);
  void emitter_set_fadeout(u32 handle, int frame);
  void emitter_set_fadeout(FxHeader *header, int frame);
  void emitter_set_fadeout(void *owner, int frame);
  void set_pause(bool isPause);
  void emitter_pack_free(u64 pack);
  void emitter_set_free(FxHeader *header, int frame);
  void update_model(emitter::ParticleObj *particle, emitter::EmitterObj *emitter, emitter::EmitterArg *arg);
  void update_plight(emitter::ParticleObj *particle, emitter::EmitterObj *emitter);
  void update_model_spd(emitter::EmitterObj *emitter, f32 speed);
  void emitter_alloc01_set_draw(emitter::EmitterObj *emitter, emitter::EmitterArg *arg);
  void update_pause();
  void move();
  void draw(const DrDrawWorkInfoEF *info, int viewIndex, bool isBlur);
  void setDrInfo(const DrDrawWorkInfoEF *info);
  bool testRayGround(mm::Vec3 &outNormal, mm::Vec3 &outPos, const mm::Vec3 &start, const mm::Vec3 &direction, f32 length, f32 backLength);
  bool testLineGround(mm::Vec3 &outNormal, mm::Vec3 &outPos, const mm::Vec3 &start, const mm::Vec3 &end);
  bool testLineGroundColor(mm::Col4 &outColor, const mm::Vec3 &start, const mm::Vec3 &end);
  bool testRayChara(mm::Vec3 &outNormal, mm::Vec3 &outPos, const mm::Vec3 &start, const mm::Vec3 &direction, f32 length, f32 backLength);

  CamParam m_camParam;
  FxParam m_fxParam;
  f32 unknown148; // Inferred role: responsive aa; original member name unknown.
  f32 unknown14c; // Inferred role: spec scale; original member name unknown.
  f32 unknown150; // Inferred role: dir scale; original member name unknown.
  f32 _154;
  f32 unknown158;       // Inferred role: ibl saturation; original member name unknown.
  f32 unknown15c;       // Inferred role: frame count; original member name unknown.
  const u8 *unknown160; // Inferred role: model table; original member name unknown.
  bool unknown168;      // Inferred role: all disp; original member name unknown.
  bool unknown169;      // Original member name unknown.
  u8 _16a[0x170 - 0x16a];
  void *unknown170[4]; // Inferred role: groups; original member name unknown.
};

static_assert(offsetof(ParticleManager, m_camParam) == 0x000, "");
static_assert(offsetof(ParticleManager, m_fxParam) == 0x130, "");
static_assert(offsetof(ParticleManager, unknown148) == 0x148, "");
static_assert(offsetof(ParticleManager, unknown168) == 0x168, "");
static_assert(offsetof(ParticleManager, unknown169) == 0x169, "");
static_assert(offsetof(ParticleManager, unknown15c) == 0x15c, "");
static_assert(offsetof(ParticleManager, unknown160) == 0x160, "");
static_assert(offsetof(ParticleManager, unknown170) == 0x170, "");
static_assert(sizeof(ParticleManager::unknown003) == 0x80, "");

class Utility {
public:
  static u32 conv_rgba(mm::Vec3 *color, f32 alpha);
};

void efAstMsg(const char *cond, const char *format, ...);
void efMsg(const char *format, ...);
void efWng(const char *format, ...);

} // namespace ptlib
