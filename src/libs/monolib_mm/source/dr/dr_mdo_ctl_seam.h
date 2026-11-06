#pragma once
#include <stddef.h>

#include "dr/dr_mdl_obj.h"
#include "dr/dr_res_mdo_inst_anm_obj.h"
#include "dr/dr_res_mdo_seam_coli.h"
#include "dr/dr_res_mdo_seam_mem_man.h"
#include "dr/dr_res_mdo_seam_parm_read.h"
#include "dr/dr_res_mdo_seam_read.h"
#include "dr/dr_res_mdo_seam_res.h"
#include "dr/dr_res_mdo_seam_wrk_base.h"
#include "dr/dr_res_mdo_seam_wrk_env.h"
#include "dr/dr_res_mdo_seam_wrk_inst.h"
#include "dr/dr_res_mdo_seam_wrk_light.h"
#include "dr/dr_res_mdo_seam_wrk_low.h"
#include "dr/dr_res_mdo_seam_wrk_mpf.h"
#include "mm_math/mm_mat44.h"
#include "mm_mtl/fixed_vector.h"
#include "mm_mtl/fixstr.h"
#include "types.h"

namespace cloud {
class CloudMdlDataObj;
} // namespace cloud

namespace grlib {
class CGLibVtxInstans;
} // namespace grlib

namespace ml {
class DrCalcClipMan;
class DrCalcClipObj;
class DrCalcCpPlaneObj;
class DrCalcViewWork;
class DrEnvColiObj;
class DrEnvironmentObj;
class DrGlobalIBLPack;
class DrMdoCtlSeamInstMap;
class DrResEtcTexList;
class DrResMdoInstMdlInfo;
class DrResMdoPosAddList;
class DrResMdoSortSeamAll;
class DrResMdoSortSeamEnvMap;
struct MdoWrkMatRes;

struct MdsSeamOfsTexTblData {
  u32 unknown00; // Inferred role: texture data offset; original member name unknown.
  s32 unknown04; // Inferred role: texture size; original member name unknown.
  u32 unknown08; // Inferred role: height data offset; original member name unknown.
  u32 unknown0c; // Inferred role: height texture size; original member name unknown.
  u32 unknown10; // Role and original member name unknown.
};

static_assert(offsetof(MdsSeamOfsTexTblData, unknown0c) == 0x0c, "");
static_assert(sizeof(MdsSeamOfsTexTblData) == 0x14, "");

// Inferred role: model data seam etc pos data; original type name unknown.
struct unknown058 {
  u32 unknown00;    // Inferred role: object name offset; original member name unknown.
  u32 unknown04;    // Inferred role: eff name offset; original member name unknown.
  f32 unknown08;    // Inferred role: range; original member name unknown.
  u32 unknown0c;    // Inferred role: matrix max; original member name unknown.
  u32 unknown10;    // Inferred role: matrix offset; original member name unknown.
  f32 unknown14[2]; // Inferred role: near clips; original member name unknown.
  f32 unknown1c[2]; // Inferred role: far clips; original member name unknown.
  u32 unknown24;    // Inferred role: groups offset; original member name unknown.
};

static_assert(offsetof(unknown058, unknown10) == 0x10, "");
static_assert(offsetof(unknown058, unknown14) == 0x14, "");
static_assert(offsetof(unknown058, unknown1c) == 0x1c, "");
static_assert(offsetof(unknown058, unknown24) == 0x24, "");

// Inferred role: seam etc pos; original type name unknown.
struct unknown059 {
  const unknown058 *unknown00; // Inferred role: data; original member name unknown.
  bool unknown08;              // Inferred role: always disp; original member name unknown.
};

static_assert(sizeof(unknown059) == 0x10, "");

// Inferred role: seam etc pos list; original type name unknown.
struct unknown060 {
  unknown060() : unknown00(nullptr), unknown08(0) {}

  unknown059 *unknown00; // Inferred role: objs; original member name unknown.
  u32 unknown08;         // Inferred role: object max; original member name unknown.
};

// Inferred role: seam texture list table entry; original type name unknown.
struct unknown136 {
  u32 unknown00; // Inferred role: texture list data offset; original member name unknown.
  s32 unknown04; // Inferred role: texture list data size; original member name unknown.
};

static_assert(sizeof(unknown136) == 0x8, "");

// Inferred role: seam resource header; original type name unknown.
struct unknown061 {
  u8 _00[0x18];
  u32 m_seamBaseMdlMax;
  u8 _1c[0x20 - 0x1c];
  u32 m_seamInstMdlMax;
  u8 _24[0x28 - 0x24];
  u32 m_seamColiMax;
  u8 _2c[0x44 - 0x2c];
  u32 m_seamShaderMax;
  u8 _48[0x50 - 0x48];
  u32 m_seamVertexMax;
  u8 _54[0x58 - 0x54];
  u32 m_seamTextureMax;
  u32 unknown5c; // Inferred role: offset of the offset texture table; original member name unknown.
  u8 _60[0x64 - 0x60];
  u32 m_seamMpfMdlMax;
  u8 _68[0x6c - 0x68];
  u32 m_seamEtcPosDataMax;
  u8 _70[0x74 - 0x70];
  u32 m_seamMpfPosDataMax;
  u8 _78[0x84 - 0x78];
  u32 unknown84; // Inferred role: offset of the environment data; original member name unknown.
  u8 _88[0x94 - 0x88];
  u32 unknown94; // Inferred role: offset of the texture list table; original member name unknown.
  u8 _98[0xa8 - 0x98];
  u32 unknowna8; // Inferred role: offset of the clip data; original member name unknown.
  u8 _ac[0xe8 - 0xac];
  u32 unknowne8; // Inferred role: offset of the cloud model data; original member name unknown.
};

static_assert(offsetof(unknown061, m_seamBaseMdlMax) == 0x18, "");
static_assert(offsetof(unknown061, m_seamInstMdlMax) == 0x20, "");
static_assert(offsetof(unknown061, m_seamColiMax) == 0x28, "");
static_assert(offsetof(unknown061, m_seamShaderMax) == 0x44, "");
static_assert(offsetof(unknown061, m_seamVertexMax) == 0x50, "");
static_assert(offsetof(unknown061, m_seamTextureMax) == 0x58, "");
static_assert(offsetof(unknown061, m_seamMpfMdlMax) == 0x64, "");
static_assert(offsetof(unknown061, m_seamEtcPosDataMax) == 0x6c, "");
static_assert(offsetof(unknown061, m_seamMpfPosDataMax) == 0x74, "");
static_assert(offsetof(unknown061, unknown94) == 0x94, "");
static_assert(offsetof(unknown061, unknown5c) == 0x5c, "");
static_assert(offsetof(unknown061, unknown84) == 0x84, "");
static_assert(offsetof(unknown061, unknowna8) == 0xa8, "");
static_assert(offsetof(unknown061, unknowne8) == 0xe8, "");

struct MdoArgSeamUpdate {
  DrCalcViewWork *unknown00;             // Inferred role: view work; original member name unknown.
  u16 *unknown08;                        // Inferred role: vertex use flags; original member name unknown.
  DrResMdoSeamVertex *unknown10;         // Inferred role: vertices; original member name unknown.
  u16 *unknown18;                        // Inferred role: texture use flags; original member name unknown.
  DrResMdoSeamTexture *unknown20;        // Inferred role: textures; original member name unknown.
  u16 *unknown28;                        // Inferred role: texture list use flags; original member name unknown.
  DrResMdoSeamTexList *unknown30;        // Inferred role: texture lists; original member name unknown.
  const MdsSeamOfsTexTblData *unknown38; // Inferred role: offset texture table; original member name unknown.
  u32 unknown40;                         // Inferred role: seam flags; original member name unknown.
  grlib::CGLibVtxInstans *unknown48;     // Inferred role: instance buffer; original member name unknown.
  DrResMdoInstAnmObj *unknown50;         // Inferred role: inst anm object; original member name unknown.
  DrCalcClipMan *unknown58;              // Inferred role: clip manager; original member name unknown.
  DrMdoCtlSeamInstMap *unknown60;        // Inferred role: inst map controller, set by the inst map update; original member name unknown.
};

static_assert(offsetof(MdoArgSeamUpdate, unknown38) == 0x38, "");
static_assert(offsetof(MdoArgSeamUpdate, unknown40) == 0x40, "");
static_assert(offsetof(MdoArgSeamUpdate, unknown48) == 0x48, "");
static_assert(offsetof(MdoArgSeamUpdate, unknown58) == 0x58, "");
static_assert(sizeof(MdoArgSeamUpdate) == 0x68, "");

// Inferred role: seam read call; original type name unknown.
struct unknown064 {
  u8 _00[0x28];
};

static_assert(sizeof(unknown064) == 0x28, "");

// Inferred role: global IBL entry of the seam data; original type name unknown.
struct unknown096 {
  u8 _00[0x08];
  u32 unknown08; // Inferred role: offset of the IBL data from the list; original member name unknown.
  u32 unknown0c; // Inferred role: size of the IBL data; original member name unknown.
  u8 _10[0x28 - 0x10];
};

static_assert(offsetof(unknown096, unknown08) == 0x08, "");
static_assert(offsetof(unknown096, unknown0c) == 0x0c, "");
static_assert(sizeof(unknown096) == 0x28, "");

// Inferred role: global IBL list of the seam data; original type name unknown.
struct unknown097 {
  s32 unknown00; // Inferred role: entry count; original member name unknown.
  u32 unknown04; // Inferred role: offset of the entries; original member name unknown.
};

static_assert(offsetof(unknown097, unknown04) == 0x04, "");

// Inferred role: doubly linked list of seam objects (unknown08 next, unknown10 prev); original type name unknown.
template <typename T> struct unknown093 {
  unknown093() : unknown00(nullptr), unknown08(nullptr) {}

  T *unknown00; // Inferred role: first object; original member name unknown.
  T *unknown08; // Inferred role: last object; original member name unknown.
};

// Inferred role: seam object pool with a free list and a used list; original type name unknown.
template <typename T> struct unknown094 {
  unknown094() : unknown20(nullptr), unknown28(0), unknown2c(0), unknown30(0) {}
  ~unknown094() { unknown003(); }

  // Inferred helper: moves the first free object to the end of the used list; original name unknown.
  T *unknown001() {
    T *obj = unknown00.unknown00;
    if (obj != nullptr) {
      T *next = obj->unknown08;
      if (next == nullptr) {
        unknown00.unknown00 = nullptr;
        unknown00.unknown08 = nullptr;
      } else {
        unknown00.unknown00 = next;
        next->unknown10 = nullptr;
      }
      if (unknown10.unknown08 != nullptr) {
        unknown10.unknown08->unknown08 = obj;
      }
      if (unknown10.unknown00 == nullptr) {
        unknown10.unknown00 = obj;
      }
      obj->unknown10 = unknown10.unknown08;
      obj->unknown08 = nullptr;
      unknown10.unknown08 = obj;
      unknown2c--;
      unknown28++;
    }
    return obj;
  }

  // Inferred helper: moves a used object to the end of the free list; original name unknown.
  void unknown002(T *obj) {
    T *next = obj->unknown08;
    T *prev = obj->unknown10;
    if (next == nullptr) {
      if (prev != nullptr) {
        prev->unknown08 = nullptr;
      }
    } else {
      if (prev != nullptr) {
        prev->unknown08 = next;
      }
      next->unknown10 = prev;
    }
    if (unknown10.unknown08 == obj) {
      unknown10.unknown08 = prev;
    }
    if (unknown10.unknown00 == obj) {
      unknown10.unknown00 = next;
      if (next != nullptr) {
        next->unknown10 = nullptr;
      }
    }
    if (unknown00.unknown08 == nullptr) {
      unknown00.unknown00 = obj;
      unknown00.unknown08 = obj;
      obj->unknown10 = nullptr;
    } else {
      unknown00.unknown08->unknown08 = obj;
      obj->unknown10 = unknown00.unknown08;
      unknown00.unknown08 = obj;
    }
    obj->unknown08 = nullptr;
    unknown2c++;
    unknown28--;
  }

  // Inferred helper: destroys the objects and empties the pool, as the release of the seam data does; original name unknown.
  void unknown003() {
    for (int i = 0; i < unknown30; i++) {
      unknown20[i].~T();
    }
    unknown30 = 0;
    unknown20 = nullptr;
    unknown28 = 0;
    unknown2c = 0;
    unknown10.unknown00 = nullptr;
    unknown10.unknown08 = nullptr;
    unknown00.unknown00 = nullptr;
    unknown00.unknown08 = nullptr;
  }

  unknown093<T> unknown00; // Inferred role: free objects; original member name unknown.
  unknown093<T> unknown10; // Inferred role: used objects; original member name unknown.
  T *unknown20;            // Inferred role: objects; original member name unknown.
  s32 unknown28;           // Inferred role: used object count; original member name unknown.
  s32 unknown2c;           // Inferred role: free object count; original member name unknown.
  s32 unknown30;           // Inferred role: object count; original member name unknown.
};

static_assert(offsetof(unknown094<DrResMdoSeamTexture>, unknown10) == 0x10, "");
static_assert(offsetof(unknown094<DrResMdoSeamTexture>, unknown28) == 0x28, "");
static_assert(sizeof(unknown094<DrResMdoSeamTexture>) == 0x38, "");

class DrMdoCtlSeam : public DrMdlObj {
public:
  DrMdoCtlSeam(int viewNum);
  void *getColiListTbl();
  u32 getColiListMax();
  DrResMdoSeamColi *getColiRes(int index);
  u32 isRead(int index) const;
  void setErrCode(int code, int param);
  static int calcSemaAlignment(u32 size, u32 alignment);
  void seamZPrePassBegin();
  void seamZPrePassEnd();
  void seamThicknessPassBegin(bool isFirst);
  void seamThicknessPassEnd(bool value);
  void calcSeamMapRender(int renderType, u32 flags);
  DrResMdoSeamTexture *createFreeSeamTexture();
  DrResMdoSeamTexList *createFreeSeamTexList();
  DrResMdoSeamVertex *createFreeSeamVertex();
  void releaseFreeSeamTexture(DrResMdoSeamTexture *texture, bool isAll);
  void releaseFreeSeamMemObj(DrResMdoSeamMemObj *mem);
  void releaseFreeSeamTexList(DrResMdoSeamTexList *texList, bool isAll);
  void releaseFreeSeamVertex(DrResMdoSeamVertex *vertex, bool isAll);
  DrResMdoSeamMemObj *createFreeSeamMemObj4K(u32 size);
  DrResMdoSeamMemObj *createFreeSeamMemObjBack4K(u32 size);
  DrResMdoSeamColi *createFreeSeamCollision();
  void setupSeamMapHead(const void *data, const char *path);
  void releaseSeam();
  void resourceRelease();
  bool calcBegin();
  void calcEnd();
  void calcFastUpdate();
  void updateTexutreSeam();
  void updateVertexSeam();

  u8 _190[0x1a8 - 0x190];
  unknown061 *unknown1a8;                                  // Inferred role: seam resource header; original member name unknown.
  unknown094<DrResMdoSeamTexture> unknown1b0;              // Inferred role: texture pool; original member name unknown.
  unknown094<DrResMdoSeamTexList> unknown1e8;              // Inferred role: texture list pool; original member name unknown.
  unknown094<DrResMdoSeamVertex> unknown220;               // Inferred role: vertex pool; original member name unknown.
  unknown094<DrResMdoSeamColi> unknown258;                 // Inferred role: collision pool; original member name unknown.
  unknown094<DrResMdoSeamWrkBase> unknown290;              // Inferred role: base map pool; original member name unknown.
  unknown094<DrResMdoSeamWrkLow> unknown2c8;               // Inferred role: low map pool; original member name unknown.
  unknown094<DrResMdoSeamWrkInst> unknown300;              // Inferred role: inst map pool; original member name unknown.
  unknown094<DrResMdoSeamWrkMpf> unknown338;               // Inferred role: mpf map pool; original member name unknown.
  unknown094<DrResMdoSeamWrkEnv> unknown370;               // Inferred role: env map pool; original member name unknown.
  unknown094<DrResMdoSeamWrkLight> unknown3a8;             // Inferred role: light pool; original member name unknown.
  void *unknown3e0;                                        // Inferred role: unknown, cleared by the constructor; original member name unknown.
  DrResMdoSortSeamAll *unknown3e8;                         // Inferred role: sort object of the maps; original member name unknown.
  DrResMdoSortSeamEnvMap *unknown3f0;                      // Inferred role: sort object of the env maps; original member name unknown.
  DrEnvironmentObj *unknown3f8;                            // Inferred role: environment object; original member name unknown.
  DrEnvColiObj *unknown400;                                // Inferred role: environment collision object; original member name unknown.
  DrCalcClipObj *unknown408;                               // Inferred role: clip object; original member name unknown.
  DrCalcClipMan *unknown410;                               // Inferred role: clip manager; original member name unknown.
  DrCalcCpPlaneObj *unknown418;                            // Inferred role: clip plane object; original member name unknown.
  mm::mtl::FixedVector<DrGlobalIBLPack *, 32> unknown420;  // Inferred role: global IBL packs; original member name unknown.
  mm::mtl::FixedVector<DrEnvironmentObj *, 32> unknown528; // Inferred role: environment objects; original member name unknown.
  DrResMdoPosAddList *unknown630;                          // Inferred role: position add list; original member name unknown.
  DrResMdoInstMdlInfo *unknown638;                         // Inferred role: instance model info; original member name unknown.
  void *unknown640[2];                                     // Inferred role: collision list tables per buffer; original member name unknown.
  u32 unknown650[2];                                       // Inferred role: collision list max per buffer; original member name unknown.
  DrResEtcTexList *unknown658;                             // Inferred role: texture list; original member name unknown.
  u8 _660[0x698 - 0x660];
  DrResMdoInstAnmObj unknown698; // Inferred role: inst anm object; original member name unknown.
  unknown060 unknown6c8;         // Inferred role: etc pos list; original member name unknown.
  const unknown097 *unknown6d8;  // Inferred role: global IBL list; original member name unknown.
  DrResMdoSeamMemMan unknown6e0; // Inferred role: seam memory manager; original member name unknown.
  u8 _b28[0xb30 - 0xb28];
  cloud::CloudMdlDataObj *unknownb30; // Inferred role: cloud model data; original member name unknown.
  MdoWrkMatRes *unknownb38;           // Inferred role: saved mat res; original member name unknown.
  void *unknownb40;                   // Inferred role: memory pool buffer; original member name unknown.
  u8 _b48[0xb50 - 0xb48];
  u32 *unknownb50; // Inferred role: base map read bits; original member name unknown.
  u8 _b58[0xb60 - 0xb58];
  u32 *unknownb60; // Inferred role: base map render bits; original member name unknown.
  u8 _b68[0xb70 - 0xb68];
  u32 *unknownb70; // Inferred role: low map read bits; original member name unknown.
  u8 _b78[0xb80 - 0xb78];
  u32 *unknownb80; // Inferred role: inst map read bits; original member name unknown.
  u8 _b88[0xb90 - 0xb88];
  u32 *unknownb90; // Inferred role: mpf map read bits; original member name unknown.
  u8 _b98[0xba0 - 0xb98];
  u32 *unknownba0; // Inferred role: env map read bits; original member name unknown.
  u8 _ba8[0xbc0 - 0xba8];
  u32 *unknownbc0; // Inferred role: light read bits; original member name unknown.
  u8 _bc8[0xbd0 - 0xbc8];
  u16 *unknownbd0; // Inferred role: vertex use flags per seam index; original member name unknown.
  u8 _bd8[0xbe0 - 0xbd8];
  u16 *unknownbe0; // Inferred role: texture use flags per seam index; original member name unknown.
  u8 _be8[0xbf0 - 0xbe8];
  u16 *unknownbf0; // Inferred role: texture list use flags per seam index; original member name unknown.
  u8 _bf8[0xc00 - 0xbf8];
  s16 unknownc00; // Inferred role: base map read count; original member name unknown.
  s16 unknownc02; // Inferred role: low map read count; original member name unknown.
  s16 unknownc04; // Inferred role: inst map read count; original member name unknown.
  s16 unknownc06; // Inferred role: mpf map read count; original member name unknown.
  s16 unknownc08; // Inferred role: env map read count; original member name unknown.
  s16 unknownc0a; // Inferred role: unknown counter, cleared by the constructor; original member name unknown.
  s16 unknownc0c; // Inferred role: light read count; original member name unknown.
  s16 unknownc0e; // Inferred role: unknown counter, cleared by the constructor; original member name unknown.
  s16 unknownc10; // Inferred role: unknown counter, cleared by the constructor; original member name unknown.
  s16 unknownc12; // Inferred role: unknown counter, cleared by the constructor; original member name unknown.
  s16 unknownc14; // Inferred role: unknown counter, cleared by the constructor; original member name unknown.
  s16 unknownc16; // Inferred role: unknown counter, cleared by the constructor; original member name unknown.
  s16 unknownc18; // Inferred role: unknown counter, cleared by the constructor; original member name unknown.
  s16 unknownc1a; // Inferred role: unknown counter, cleared by the constructor; original member name unknown.
  s16 unknownc1c; // Inferred role: unknown counter, cleared by the constructor; original member name unknown.
  s16 unknownc1e; // Inferred role: unknown counter, cleared by the constructor; original member name unknown.
  s16 unknownc20; // Inferred role: unknown counter, cleared by the constructor; original member name unknown.
  s16 unknownc22; // Inferred role: unknown counter, cleared by the constructor; original member name unknown.
  s16 unknownc24; // Inferred role: unknown counter, cleared by the constructor; original member name unknown.
  s16 unknownc26; // Inferred role: unknown counter, cleared by the constructor; original member name unknown.
  s16 unknownc28; // Inferred role: unknown counter, cleared by the constructor; original member name unknown.
  s16 unknownc2a; // Inferred role: frame count set to 30 when calcEnd sees seam flag 0x200000; original member name unknown.
  u8 _c2c[0xc2e - 0xc2c];
  s8 unknownc2e;  // Inferred role: unknown counter, cleared by the constructor; original member name unknown.
  s8 unknownc2f;  // Inferred role: unknown counter, cleared by the constructor; original member name unknown.
  s16 unknownc30; // Inferred role: unknown counter, cleared by the constructor; original member name unknown.
  u8 _c32[0xc34 - 0xc32];
  s32 unknownc34; // Inferred role: unknown size, cleared by the constructor; original member name unknown.
  s32 unknownc38; // Inferred role: unknown size, cleared by the constructor; original member name unknown.
  s32 unknownc3c; // Inferred role: unknown size, cleared by the constructor; original member name unknown.
  s32 unknownc40; // Inferred role: base map object count; original member name unknown.
  s32 unknownc44; // Inferred role: low map object count; original member name unknown.
  s32 unknownc48; // Inferred role: inst map memory size; original member name unknown.
  s32 unknownc4c; // Inferred role: mpf map memory size; original member name unknown.
  s32 unknownc50; // Inferred role: env map object count; original member name unknown.
  s32 unknownc54; // Inferred role: unknown size, cleared by the constructor; original member name unknown.
  s32 unknownc58; // Inferred role: unknown size, cleared by the constructor; original member name unknown.
  s32 unknownc5c; // Inferred role: unknown size, cleared by the constructor; original member name unknown.
  s32 unknownc60; // Inferred role: unknown size, cleared by the constructor; original member name unknown.
  s32 unknownc64; // Inferred role: unknown size, cleared by the constructor; original member name unknown.
  s32 unknownc68; // Inferred role: unknown size, cleared by the constructor; original member name unknown.
  s32 unknownc6c; // Inferred role: light memory size; original member name unknown.
  s32 unknownc70; // Inferred role: unknown size, cleared by the constructor; original member name unknown.
  s32 unknownc74; // Inferred role: unknown size, cleared by the constructor; original member name unknown.
  u32 unknownc78; // Inferred role: unknown flags, cleared by the constructor; original member name unknown.
  u32 unknownc7c; // Inferred role: seam flags; original member name unknown.
  u32 unknownc80; // Inferred role: unknown flags, cleared by the constructor; original member name unknown.
  u32 unknownc84; // Inferred role: unknown flags, cleared by the constructor; original member name unknown.
  u32 unknownc88; // Inferred role: unknown, 1 after construction; original member name unknown.
  u64 unknownc90; // Inferred role: base map shadow cache mask; original member name unknown.
  u64 unknownc98; // Inferred role: inst map shadow cache mask; original member name unknown.
  s16 unknownca0; // Inferred role: base map counter; original member name unknown.
  s16 unknownca2; // Inferred role: base map counter; original member name unknown.
  s16 unknownca4; // Inferred role: inst map counter; original member name unknown.
  s16 unknownca6; // Inferred role: inst map counter; original member name unknown.
  u8 _ca8[0xcac - 0xca8];
  s16 unknowncac;    // Inferred role: error code write index; original member name unknown.
  s16 unknowncae[4]; // Inferred role: error codes; original member name unknown.
  u8 _cb6[0xcb8 - 0xcb6];
  u32 unknowncb8[4];               // Inferred role: error parameters; original member name unknown.
  bool unknowncc8;                 // Inferred role: unknown state, cleared by the constructor; original member name unknown.
  DrResMdoSeamRead unknowncd0;     // Inferred role: seam reader; original member name unknown.
  DrResMdoSeamParmRead unknownd30; // Inferred role: seam read parameters; original member name unknown.
  unknown064 unknown1370[8];       // Inferred role: seam read calls; original member name unknown.
  u32 unknown14b0;                 // Inferred role: unknown, cleared by the constructor; original member name unknown.
  u8 _14b4[0x14c0 - 0x14b4];
  u64 unknown14c0; // Inferred role: unknown, cleared by the constructor; original member name unknown.
  u32 unknown14c8; // Inferred role: unknown, cleared by the constructor; original member name unknown.
  u8 _14cc[0x14d0 - 0x14cc];
  mm::Mat44 unknown14d0;            // Inferred role: world view projection matrix; original member name unknown.
  mm::Mat44 unknown1510;            // Inferred role: world view matrix; original member name unknown.
  mm::mtl::FixStr<128> unknown1550; // Inferred role: seam file path; original member name unknown.
};

static_assert(offsetof(DrMdoCtlSeam, unknown1a8) == 0x1a8, "");
static_assert(offsetof(DrMdoCtlSeam, unknown1b0) == 0x1b0, "");
static_assert(offsetof(DrMdoCtlSeam, unknown1e8) == 0x1e8, "");
static_assert(offsetof(DrMdoCtlSeam, unknown220) == 0x220, "");
static_assert(offsetof(DrMdoCtlSeam, unknown290) == 0x290, "");
static_assert(offsetof(DrMdoCtlSeam, unknown2c8) == 0x2c8, "");
static_assert(offsetof(DrMdoCtlSeam, unknown300) == 0x300, "");
static_assert(offsetof(DrMdoCtlSeam, unknown370) == 0x370, "");
static_assert(offsetof(DrMdoCtlSeam, unknown3a8) == 0x3a8, "");
static_assert(offsetof(DrMdoCtlSeam, unknown258) == 0x258, "");
static_assert(offsetof(DrMdoCtlSeam, unknown338) == 0x338, "");
static_assert(offsetof(DrMdoCtlSeam, unknownbc0) == 0xbc0, "");
static_assert(offsetof(DrMdoCtlSeam, unknownc0c) == 0xc0c, "");
static_assert(offsetof(DrMdoCtlSeam, unknownc6c) == 0xc6c, "");
static_assert(offsetof(DrMdoCtlSeam, unknownb90) == 0xb90, "");
static_assert(offsetof(DrMdoCtlSeam, unknownc06) == 0xc06, "");
static_assert(offsetof(DrMdoCtlSeam, unknownc4c) == 0xc4c, "");
static_assert(offsetof(DrMdoCtlSeam, unknown640) == 0x640, "");
static_assert(offsetof(DrMdoCtlSeam, unknownc40) == 0xc40, "");
static_assert(offsetof(DrMdoCtlSeam, unknownc44) == 0xc44, "");
static_assert(offsetof(DrMdoCtlSeam, unknownc50) == 0xc50, "");
static_assert(offsetof(DrMdoCtlSeam, unknown650) == 0x650, "");
static_assert(offsetof(DrMdoCtlSeam, unknowncac) == 0xcac, "");
static_assert(offsetof(DrMdoCtlSeam, unknowncae) == 0xcae, "");
static_assert(offsetof(DrMdoCtlSeam, unknowncb8) == 0xcb8, "");
static_assert(offsetof(DrMdoCtlSeam, unknown3e0) == 0x3e0, "");
static_assert(offsetof(DrMdoCtlSeam, unknown420) == 0x420, "");
static_assert(offsetof(DrMdoCtlSeam, unknown528) == 0x528, "");
static_assert(offsetof(DrMdoCtlSeam, unknown630) == 0x630, "");
static_assert(offsetof(DrMdoCtlSeam, unknown658) == 0x658, "");
static_assert(offsetof(DrMdoCtlSeam, unknown6d8) == 0x6d8, "");
static_assert(offsetof(DrMdoCtlSeam, unknownb30) == 0xb30, "");
static_assert(offsetof(DrMdoCtlSeam, unknownb40) == 0xb40, "");
static_assert(offsetof(DrMdoCtlSeam, unknownc2e) == 0xc2e, "");
static_assert(offsetof(DrMdoCtlSeam, unknownc34) == 0xc34, "");
static_assert(offsetof(DrMdoCtlSeam, unknownc88) == 0xc88, "");
static_assert(offsetof(DrMdoCtlSeam, unknownc90) == 0xc90, "");
static_assert(offsetof(DrMdoCtlSeam, unknownca0) == 0xca0, "");
static_assert(offsetof(DrMdoCtlSeam, unknowncc8) == 0xcc8, "");
static_assert(offsetof(DrMdoCtlSeam, unknowncd0) == 0xcd0, "");
static_assert(offsetof(DrMdoCtlSeam, unknownd30) == 0xd30, "");
static_assert(offsetof(DrMdoCtlSeam, unknown14b0) == 0x14b0, "");
static_assert(offsetof(DrMdoCtlSeam, unknown14c0) == 0x14c0, "");
static_assert(offsetof(DrMdoCtlSeam, unknown14d0) == 0x14d0, "");
static_assert(offsetof(DrMdoCtlSeam, unknown1510) == 0x1510, "");
static_assert(offsetof(DrMdoCtlSeam, unknown1550) == 0x1550, "");
static_assert(offsetof(DrMdoCtlSeam, unknown698) == 0x698, "");
static_assert(offsetof(DrMdoCtlSeam, unknown6c8) == 0x6c8, "");
static_assert(offsetof(DrMdoCtlSeam, unknownb38) == 0xb38, "");
static_assert(offsetof(DrMdoCtlSeam, unknown6e0) == 0x6e0, "");
static_assert(offsetof(DrMdoCtlSeam, unknownbd0) == 0xbd0, "");
static_assert(offsetof(DrMdoCtlSeam, unknownbe0) == 0xbe0, "");
static_assert(offsetof(DrMdoCtlSeam, unknownbf0) == 0xbf0, "");
static_assert(offsetof(DrMdoCtlSeam, unknownb50) == 0xb50, "");
static_assert(offsetof(DrMdoCtlSeam, unknownb60) == 0xb60, "");
static_assert(offsetof(DrMdoCtlSeam, unknownb70) == 0xb70, "");
static_assert(offsetof(DrMdoCtlSeam, unknownb80) == 0xb80, "");
static_assert(offsetof(DrMdoCtlSeam, unknownba0) == 0xba0, "");
static_assert(offsetof(DrMdoCtlSeam, unknownc00) == 0xc00, "");
static_assert(offsetof(DrMdoCtlSeam, unknownc08) == 0xc08, "");
static_assert(offsetof(DrMdoCtlSeam, unknownc48) == 0xc48, "");
static_assert(offsetof(DrMdoCtlSeam, unknownc7c) == 0xc7c, "");

} // namespace ml
