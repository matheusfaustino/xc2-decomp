#pragma once
#include <stddef.h>

#include "dr/dr_mdl_base.h"
#include "grlib/cglib_uniform_array.h"
#include "mm_math/mm_col3.h"
#include "mm_math/mm_mat44.h"
#include "mm_math/mm_vec3.h"
#include "types.h"

namespace grlib {
class CGLibDblUniformBlock;
class CGLibOneUniformBlock;
class CGLibTextureBuffer;
class CGLibTriUniformBlock;
} // namespace grlib

namespace ml {

class DrResMdoMat;
class DrShdMdoExtTblObj;
class DrMan;
class DrMdoSmlShadowObj;
struct MdsMatCharMkTexTbl;
class DrResMdoSampler;
class DrResMdoTexList;
class DrResMdoVertex;
class DrShdMdoDecExtTbl;

struct MdsMatTopHeader {
  u8 _00[0x3c - 0x00];
  u32 unknown3c; // Inferred role: offset of the texture infos; original member name unknown.
};

static_assert(offsetof(MdsMatTopHeader, unknown3c) == 0x3c, "");

struct MdsMatTextureInfo {
  u16 unknown00; // Inferred role: texture index; original member name unknown.
  u16 unknown02; // Inferred role: sampler index of a mipmapped texture; original member name unknown.
  u16 unknown04; // Inferred role: sampler index of a texture without mipmap; original member name unknown.
  u8 _06[0x08 - 0x06];
};

static_assert(sizeof(MdsMatTextureInfo) == 0x08, "");

struct MdsMatInfoHeader {
  // Inferred role: material flag; original type name unknown.
  enum unknown015 {
    unknown015_15 = 1 << 15, // Inferred role: seam material; original name unknown.
    unknown015_16 = 1 << 16, // Inferred role: uv animation; original name unknown.
    unknown015_18 = 1 << 18, // Inferred role: material animation; original name unknown.
  };

  u8 _00[0x04 - 0x00];
  union {
    u32 unknown04; // Inferred role: flags; original member name unknown.
    struct {
      u8 _04;
      u8 unknown05; // Inferred role: flags, bit 7 is the seam material; original member name unknown.
    };
  };
  u8 _08[0x09 - 0x08];
  u8 unknown09; // Inferred role: flags; original member name unknown.
  u8 _0a[0x0c - 0x0a];
  f32 unknown0c; // Inferred role: color r; original member name unknown.
  f32 unknown10; // Inferred role: color g; original member name unknown.
  f32 unknown14; // Inferred role: color b; original member name unknown.
  f32 unknown18; // Inferred role: color a; original member name unknown.
  f32 unknown1c; // Inferred role: alpha test reference; original member name unknown.
  u32 unknown20; // Inferred role: texture index offset; original member name unknown.
  u8 _24[0x30 - 0x24];
  u32 unknown30; // Inferred role: uv animation category; original member name unknown.
  u8 _34[0x38 - 0x34];
  u32 unknown38; // Inferred role: material animation category; original member name unknown.
  u8 _3c[0x40 - 0x3c];
  u32 unknown40; // Inferred role: value offset; original member name unknown.
  u8 _44[0x4c - 0x44];
  u32 unknown4c; // Inferred role: shader table offset; original member name unknown.
  s32 unknown50; // Inferred role: shader table count; original member name unknown.
  u8 _54[0x56 - 0x54];
  u16 unknown56; // Inferred role: avatar map color index; original member name unknown.
  u16 unknown58; // Inferred role: first material callback; original member name unknown.
  u16 unknown5a; // Inferred role: material callback count; original member name unknown.
  u8 _5c[0x62 - 0x5c];
  u16 unknown62; // Inferred role: texture index; original member name unknown.
  u16 unknown64; // Inferred role: time value index; original member name unknown.
  u8 _66[0x74 - 0x66];
};

static_assert(offsetof(MdsMatInfoHeader, unknown04) == 0x04, "");
static_assert(offsetof(MdsMatInfoHeader, unknown05) == 0x05, "");
static_assert(offsetof(MdsMatInfoHeader, unknown09) == 0x09, "");
static_assert(offsetof(MdsMatInfoHeader, unknown0c) == 0x0c, "");
static_assert(offsetof(MdsMatInfoHeader, unknown18) == 0x18, "");
static_assert(offsetof(MdsMatInfoHeader, unknown1c) == 0x1c, "");
static_assert(offsetof(MdsMatInfoHeader, unknown20) == 0x20, "");
static_assert(offsetof(MdsMatInfoHeader, unknown30) == 0x30, "");
static_assert(offsetof(MdsMatInfoHeader, unknown38) == 0x38, "");
static_assert(offsetof(MdsMatInfoHeader, unknown40) == 0x40, "");
static_assert(offsetof(MdsMatInfoHeader, unknown4c) == 0x4c, "");
static_assert(offsetof(MdsMatInfoHeader, unknown50) == 0x50, "");
static_assert(offsetof(MdsMatInfoHeader, unknown56) == 0x56, "");
static_assert(offsetof(MdsMatInfoHeader, unknown58) == 0x58, "");
static_assert(offsetof(MdsMatInfoHeader, unknown5a) == 0x5a, "");
static_assert(offsetof(MdsMatInfoHeader, unknown62) == 0x62, "");
static_assert(offsetof(MdsMatInfoHeader, unknown64) == 0x64, "");

static_assert(sizeof(MdsMatInfoHeader) == 0x74, "");

struct MdsMatShaderTbl {
  u32 unknown00; // Inferred role: technique index; original member name unknown.
  u16 unknown04; // Inferred role: render pass type; original member name unknown.
  u16 unknown06; // Inferred role: material uniform index; original member name unknown.
  u32 unknown08; // Inferred role: flags, bit 0 has uniform values; original member name unknown.
};

static_assert(offsetof(MdsMatShaderTbl, unknown04) == 0x04, "");
static_assert(offsetof(MdsMatShaderTbl, unknown06) == 0x06, "");
static_assert(offsetof(MdsMatShaderTbl, unknown08) == 0x08, "");
static_assert(sizeof(MdsMatShaderTbl) == 0x0c, "");

struct MdsMatTechnique {
  u8 _00[0x10 - 0x00];
  u32 unknown10; // Inferred role: variable table offset; original member name unknown.
  s32 unknown14; // Inferred role: variable count; original member name unknown.
  u8 _18[0x44 - 0x18];
};

static_assert(offsetof(MdsMatTechnique, unknown10) == 0x10, "");
static_assert(offsetof(MdsMatTechnique, unknown14) == 0x14, "");
static_assert(sizeof(MdsMatTechnique) == 0x44, "");

struct unknown091;
struct MdoWrkVanishEffect;

// Inferred role: model data mesh parts info; original type name unknown.
struct unknown020 {
  u8 _00[0x08 - 0x00];
  u8 unknown08; // Inferred role: flags; original member name unknown.
  u8 _09[0x10 - 0x09];
};

static_assert(sizeof(unknown020) == 0x10, "");

// Inferred role: model data mesh header; original type name unknown.
struct unknown022 {
  enum Flag {
    FLAG_UNKNOWN_3 = 1 << 3,
    FLAG_VANISH_EFFECT = 1 << 13,
  };

  u16 unknown00; // Inferred role: flags; original member name unknown.
  u8 _02[0x1c - 0x02];
  u32 unknown1c; // Inferred role: mdl entry offset; original member name unknown.
  u32 unknown20; // Inferred role: mdl entry count; original member name unknown.
  u8 _24[0x30 - 0x24];
  u32 unknown30; // Inferred role: offset of the second table; original member name unknown.
  s32 unknown34; // Inferred role: count of the second table; original member name unknown.
  u8 _38[0x64 - 0x38];
  u32 unknown64; // Inferred role: parts info offset; original member name unknown.
  u8 _68[0x78 - 0x68];
  u32 unknown78; // Inferred role: offset of the node animation table; original member name unknown.
  u8 _7c[0x94 - 0x7c];
  u8 unknown94; // Inferred role: vanish effect num; original member name unknown.
};

static_assert(offsetof(unknown022, unknown1c) == 0x1c, "");
static_assert(offsetof(unknown022, unknown20) == 0x20, "");
static_assert(offsetof(unknown022, unknown30) == 0x30, "");
static_assert(offsetof(unknown022, unknown34) == 0x34, "");
static_assert(offsetof(unknown022, unknown64) == 0x64, "");
static_assert(offsetof(unknown022, unknown78) == 0x78, "");
static_assert(offsetof(unknown022, unknown94) == 0x94, "");

// Inferred role: node animation table header; original type name unknown.
struct unknown001 {
  u32 unknown00; // Inferred role: count; original member name unknown.
  u32 unknown04; // Inferred role: offset of the data; original member name unknown.
};

static_assert(sizeof(unknown001) == 0x08, "");

// Inferred role: node animation work; original type name unknown.
struct unknown002 {
  const unknown001 *unknown00; // Inferred role: table; original member name unknown.
  const unknown001 *unknown08; // Inferred role: table; original member name unknown.
  u32 unknown10;               // Inferred role: count; original member name unknown.
  const u8 *unknown18;         // Inferred role: data; original member name unknown.
  s32 unknown20;               // Inferred role: etc uniform index; original member name unknown.
  u8 _24[0x28 - 0x24];
};

static_assert(offsetof(unknown002, unknown10) == 0x10, "");
static_assert(offsetof(unknown002, unknown20) == 0x20, "");
static_assert(offsetof(unknown002, unknown18) == 0x18, "");
static_assert(sizeof(unknown002) == 0x28, "");

struct _MdoWrkDispCallTemp {
  u8 _00[0x08];
};

static_assert(sizeof(_MdoWrkDispCallTemp) == 0x08, "");

struct MdsMatDispCCData {
  u8 _00[0x0c];
};

static_assert(sizeof(MdsMatDispCCData) == 0x0c, "");

// Inferred role: display call work, followed by one 8 byte slot per entry; original type name unknown.
struct unknown003 {
  s32 unknown00; // Inferred role: entry count; original member name unknown.
  u8 _04[0x08 - 0x04];
  _MdoWrkDispCallTemp *unknown08;    // Inferred role: entries; original member name unknown.
  const MdsMatDispCCData *unknown10; // Inferred role: data; original member name unknown.
};

static_assert(offsetof(unknown003, unknown08) == 0x08, "");
static_assert(offsetof(unknown003, unknown10) == 0x10, "");

// Inferred role: display call table header; original type name unknown.
struct unknown004 {
  u32 unknown00; // Inferred role: entry count; original member name unknown.
  u32 unknown04; // Inferred role: offset of the data; original member name unknown.
};

static_assert(sizeof(unknown004) == 0x08, "");

// Inferred role: fur shader source entry; original type name unknown.
struct unknown005 {
  u32 unknown00;
  u32 unknown04;
  u32 unknown08;
  u32 unknown0c;
  u32 unknown10;
};

static_assert(sizeof(unknown005) == 0x14, "");

// Inferred role: fur shader work entry; original type name unknown.
struct unknown006 {
  u32 unknown00;
  u32 unknown04;
  u32 unknown08;
  u32 unknown0c;
  u32 unknown10;
  u32 unknown14;
};

static_assert(sizeof(unknown006) == 0x18, "");

// Inferred role: fur shader work, followed by the work entries; original type name unknown.
struct unknown007 {
  const u8 *unknown00;         // Inferred role: first data; original member name unknown.
  const unknown005 *unknown08; // Inferred role: source entries; original member name unknown.
  unknown006 *unknown10;       // Inferred role: work entries; original member name unknown.
};

static_assert(offsetof(unknown007, unknown08) == 0x08, "");
static_assert(offsetof(unknown007, unknown10) == 0x10, "");

// Inferred role: fur shader table header; original type name unknown.
struct unknown008 {
  u32 unknown00; // Inferred role: offset of the first data; original member name unknown.
  u32 unknown04; // Inferred role: offset of the source entries; original member name unknown.
  s32 unknown08; // Inferred role: entry count; original member name unknown.
};

static_assert(sizeof(unknown008) == 0x0c, "");

// Inferred role: model work avatar; original type name unknown.
struct unknown023 {
  u32 unknown00; // Inferred role: flags; original member name unknown.
  u8 _04[0x08 - 0x04];
  grlib::CGLibTextureBuffer *unknown08[2]; // Inferred role: base textures; original member name unknown.
  void *unknown18[2];                      // Inferred role: textures; original member name unknown.
  grlib::CGLibTextureBuffer *unknown28;    // Inferred role: eye base texture; original member name unknown.
  void *unknown30;                         // Inferred role: eye texture; original member name unknown.
  bool unknown38;                          // Inferred role: doll mat; original member name unknown.
  u8 _39[0x40 - 0x39];
  grlib::CGLibOneUniformBlock *unknown40; // Inferred role: char colors; original member name unknown.
  grlib::CGLibOneUniformBlock *unknown48; // Inferred role: doll colors; original member name unknown.
};

static_assert(offsetof(unknown023, unknown08) == 0x08, "");
static_assert(offsetof(unknown023, unknown18) == 0x18, "");
static_assert(offsetof(unknown023, unknown28) == 0x28, "");
static_assert(offsetof(unknown023, unknown30) == 0x30, "");
static_assert(offsetof(unknown023, unknown38) == 0x38, "");
static_assert(offsetof(unknown023, unknown40) == 0x40, "");
static_assert(offsetof(unknown023, unknown48) == 0x48, "");

// Inferred role: model work shader parm; original type name unknown.
struct unknown024 {
  mm::Col3 unknown00[4]; // Inferred role: colors; original member name unknown.
  f32 unknown30[1];      // Inferred role: values; original member name unknown.
};

static_assert(offsetof(unknown024, unknown30) == 0x30, "");

// Inferred role: fixed animation data header; original type name unknown.
struct unknown025 {
  u8 _00[0x04 - 0x00];
  u32 unknown04; // Inferred role: offset of the category ranges; original member name unknown.
  u8 _08[0x0c - 0x08];
  u32 unknown0c; // Inferred role: offset of the categories; original member name unknown.
  u8 _10[0x14 - 0x10];
  u32 unknown14;
  u8 _18[0x1c - 0x18];
  u32 unknown1c;
  u8 _20[0x24 - 0x20];
  u32 unknown24;
};

static_assert(offsetof(unknown025, unknown04) == 0x04, "");
static_assert(offsetof(unknown025, unknown0c) == 0x0c, "");
static_assert(offsetof(unknown025, unknown14) == 0x14, "");
static_assert(offsetof(unknown025, unknown1c) == 0x1c, "");
static_assert(offsetof(unknown025, unknown24) == 0x24, "");

// Inferred role: fixed animation category range; original type name unknown.
struct unknown016 {
  u16 unknown00; // Inferred role: first category; original member name unknown.
  u16 unknown02; // Inferred role: category num; original member name unknown.
};

static_assert(sizeof(unknown016) == 0x04, "");

struct MdsMdlInfoHeader {
  u8 _00[0x04 - 0x00];
  u32 unknown04; // Inferred role: render pass flags; original member name unknown.
  u16 unknown08; // Inferred role: vertex bind index; original member name unknown.
  u16 unknown0a; // Inferred role: render index; original member name unknown.
  u8 _0c[0x0e - 0x0c];
  u16 unknown0e; // Inferred role: material index; original member name unknown.
  u8 _10[0x30 - 0x10];
};

static_assert(offsetof(MdsMdlInfoHeader, unknown08) == 0x08, "");
static_assert(offsetof(MdsMdlInfoHeader, unknown0a) == 0x0a, "");
static_assert(offsetof(MdsMdlInfoHeader, unknown0e) == 0x0e, "");

static_assert(sizeof(MdsMdlInfoHeader) == 0x30, "");

// Model data mdl entry, named from DrMdoCtlNom::nomRender(const MdsMdlTopHeader *, DrResChkList *).
struct MdsMdlTopHeader {
  u32 unknown00; // Inferred role: info offset; original member name unknown.
  s32 unknown04; // Inferred role: info count; original member name unknown.
  u8 _08[0x0c - 0x08];
  f32 unknown0c[3]; // Inferred role: aabb max; original member name unknown.
  f32 unknown18[3]; // Inferred role: aabb min; original member name unknown.
  u8 _24[0x44 - 0x24];
};

static_assert(offsetof(MdsMdlTopHeader, unknown0c) == 0x0c, "");
static_assert(offsetof(MdsMdlTopHeader, unknown18) == 0x18, "");
static_assert(sizeof(MdsMdlTopHeader) == 0x44, "");

// Inferred role: flag wave uniform table; original type name unknown.
struct unknown009 {
  const u16 *unknown00; // Inferred role: uniform index per wave; original member name unknown.
  u8 _08[0x18 - 0x08];
  grlib::CGLibUniformArray<grlib::CGLibTriUniformBlock> *unknown18; // Inferred role: uniforms; original member name unknown.
};

static_assert(offsetof(unknown009, unknown18) == 0x18, "");

// Inferred role: material callback table header; original type name unknown.
struct unknown010 {
  u32 unknown00; // Inferred role: offset of the callbacks; original member name unknown.
  u8 _04[0x08 - 0x04];
  u32 unknown08; // Inferred role: offset of the material indices; original member name unknown.
  s32 unknown0c; // Inferred role: material count; original member name unknown.
  u32 unknown10; // Inferred role: offset of the first callback entries; original member name unknown.
  s32 unknown14; // Inferred role: first callback entry count; original member name unknown.
};

static_assert(offsetof(unknown010, unknown08) == 0x08, "");
static_assert(offsetof(unknown010, unknown0c) == 0x0c, "");
static_assert(offsetof(unknown010, unknown10) == 0x10, "");
static_assert(offsetof(unknown010, unknown14) == 0x14, "");
static_assert(sizeof(unknown010) == 0x18, "");

// Inferred role: material callback; original type name unknown.
struct unknown011 {
  u16 unknown00; // Inferred role: callback index; original member name unknown.
  u16 unknown02; // Inferred role: value index; original member name unknown.
};

static_assert(sizeof(unknown011) == 0x04, "");

// Inferred role: first material callback entry; original type name unknown.
struct unknown012 {
  u32 unknown00;      // Inferred role: material index; original member name unknown.
  u32 unknown04 : 16; // Inferred role: callback count; original member name unknown.
  u32 unknown06 : 16; // Inferred role: first callback; original member name unknown.
};

static_assert(sizeof(unknown012) == 0x08, "");

struct MdsFixAnmInf {
  u32 unknown00_0 : 29; // Inferred role: first key; original member name unknown.
  u32 unknown00_29 : 1; // Inferred role: inverted parameter; original member name unknown.
  u32 unknown00_30 : 1; // Inferred role: parameter driven; original member name unknown.
  u32 unknown00_31 : 1; // Inferred role: timer driven; original member name unknown.
  u32 unknown04;        // Inferred role: key num or parameter index; original member name unknown.
  u32 unknown08;        // Inferred role: callback index in the low half, work offset in the high half; original member name unknown.
  u16 unknown0c;        // Inferred role: start frame; original member name unknown.
  u16 unknown0e;        // Inferred role: end frame; original member name unknown.
};

static_assert(offsetof(MdsFixAnmInf, unknown04) == 0x04, "");
static_assert(offsetof(MdsFixAnmInf, unknown08) == 0x08, "");
static_assert(offsetof(MdsFixAnmInf, unknown0c) == 0x0c, "");
static_assert(offsetof(MdsFixAnmInf, unknown0e) == 0x0e, "");
static_assert(sizeof(MdsFixAnmInf) == 0x10, "");

// Inferred role: fixed animation hermite key; original type name unknown.
struct unknown013 {
  f32 unknown00; // Inferred role: out slope; original member name unknown.
  f32 unknown04; // Inferred role: in slope; original member name unknown.
  f32 unknown08; // Inferred role: value; original member name unknown.
  u16 unknown0c; // Inferred role: frame; original member name unknown.
  u8 unknown0e;  // Inferred role: flags, bit 0 is a step key; original member name unknown.
  u8 _0f;
};

static_assert(offsetof(unknown013, unknown0c) == 0x0c, "");
static_assert(offsetof(unknown013, unknown0e) == 0x0e, "");
static_assert(sizeof(unknown013) == 0x10, "");

struct MdoWrkMatRes {
  u8 _000[0x010 - 0x000];
  const u8 *unknown10;         // Inferred role: texture indices; original member name unknown.
  const u8 *unknown18;         // Inferred role: model data; original member name unknown.
  const unknown022 *unknown20; // Inferred role: mesh header; original member name unknown.
  MdsMatInfoHeader *unknown28; // Inferred role: mat data; original member name unknown.
  s32 unknown30;               // Inferred role: mat count; original member name unknown.
  u8 _034[0x038 - 0x034];
  f32 *unknown38;                   // Inferred role: base values; original member name unknown.
  const unknown010 *unknown40;      // Inferred role: material callbacks; original member name unknown.
  const u8 *unknown48;              // Inferred role: avatar map color indices; original member name unknown.
  const u8 *unknown50;              // Inferred role: fixed anim data; original member name unknown.
  const MdsFixAnmInf *unknown58;    // Inferred role: fixed anim infos; original member name unknown.
  const unknown013 *unknown60;      // Inferred role: fixed anim keys; original member name unknown.
  const u8 *unknown68;              // Inferred role: fixed anim arrays; original member name unknown.
  const MdsMatTechnique *unknown70; // Inferred role: techniques; original member name unknown.
  const u8 *unknown78;              // Inferred role: variable tables; original member name unknown.
  u8 _080[0x088 - 0x080];
  DrShdMdoExtTblObj *unknown88;                                     // Inferred role: shader extension tables; original member name unknown.
  grlib::CGLibUniformArray<grlib::CGLibDblUniformBlock> *unknown90; // Inferred role: material uniforms; original member name unknown.
  u8 _098[0x0a0 - 0x098];
  grlib::CGLibUniformArray<grlib::CGLibTriUniformBlock> *unknowna0; // Inferred role: mdl uniforms; original member name unknown.
  u8 _0a8[0x0b0 - 0x0a8];
  grlib::CGLibUniformArray<grlib::CGLibDblUniformBlock> *unknownb0; // Inferred role: vol tex uniforms; original member name unknown.
  grlib::CGLibUniformArray<grlib::CGLibDblUniformBlock> *unknownb8; // Inferred role: rim bloom uniforms; original member name unknown.
  DrResMdoMat *unknownc0;                                           // Inferred role: mat entries; original member name unknown.
  DrResMdoVertex *unknownc8;                                        // Inferred role: vertex; original member name unknown.
  DrResMdoTexList *unknownd0;                                       // Inferred role: texture list; original member name unknown.
  DrResMdoSampler *unknownd8;                                       // Inferred role: samplers; original member name unknown.
  u8 _0e0[0x0e8 - 0x0e0];
  grlib::CGLibTextureBuffer **unknowne8; // Inferred role: textures; original member name unknown.
  s32 unknownf0;                         // Inferred role: texture count; original member name unknown.
  u8 _0f4[0x0f8 - 0x0f4];
  f32 *unknownf8;  // Inferred role: values; original member name unknown.
  f32 *unknown100; // Inferred role: fixed anim frames; original member name unknown.
  u8 _108[0x110 - 0x108];
  unknown023 *unknown110; // Inferred role: avatar; original member name unknown.
  unknown024 *unknown118; // Inferred role: shader parm; original member name unknown.
  u8 _120[0x130 - 0x120];
  unknown003 *unknown130;         // Inferred role: display call; original member name unknown.
  unknown007 *unknown138;         // Inferred role: fur shader; original member name unknown.
  unknown009 *unknown140;         // Inferred role: flag wave uniforms; original member name unknown.
  unknown002 *unknown148;         // Inferred role: node animation; original member name unknown.
  MdoWrkVanishEffect *unknown150; // Inferred role: vanish effects; original member name unknown.
  unknown091 *unknown158;         // Inferred role: rim blooms; original member name unknown.
  u8 _160[0x168 - 0x160];
  const MdsMdlTopHeader *unknown168; // Inferred role: mdl entries; original member name unknown.
  s32 unknown170;                    // Inferred role: mdl entry count; original member name unknown.
  u8 _174[0x178 - 0x174];
  const u8 *unknown178; // Inferred role: second table; original member name unknown.
};

static_assert(offsetof(MdoWrkMatRes, unknown10) == 0x010, "");
static_assert(offsetof(MdoWrkMatRes, unknown50) == 0x050, "");
static_assert(offsetof(MdoWrkMatRes, unknown68) == 0x068, "");
static_assert(offsetof(MdoWrkMatRes, unknown70) == 0x070, "");
static_assert(offsetof(MdoWrkMatRes, unknown78) == 0x078, "");
static_assert(offsetof(MdoWrkMatRes, unknown18) == 0x018, "");
static_assert(offsetof(MdoWrkMatRes, unknown20) == 0x020, "");
static_assert(offsetof(MdoWrkMatRes, unknown28) == 0x028, "");
static_assert(offsetof(MdoWrkMatRes, unknown30) == 0x030, "");
static_assert(offsetof(MdoWrkMatRes, unknown38) == 0x038, "");
static_assert(offsetof(MdoWrkMatRes, unknown40) == 0x040, "");
static_assert(offsetof(MdoWrkMatRes, unknown48) == 0x048, "");
static_assert(offsetof(MdoWrkMatRes, unknown58) == 0x058, "");
static_assert(offsetof(MdoWrkMatRes, unknown60) == 0x060, "");
static_assert(offsetof(MdoWrkMatRes, unknown88) == 0x088, "");
static_assert(offsetof(MdoWrkMatRes, unknown90) == 0x090, "");
static_assert(offsetof(MdoWrkMatRes, unknowna0) == 0x0a0, "");
static_assert(offsetof(MdoWrkMatRes, unknownb0) == 0x0b0, "");
static_assert(offsetof(MdoWrkMatRes, unknownb8) == 0x0b8, "");
static_assert(offsetof(MdoWrkMatRes, unknownc0) == 0x0c0, "");
static_assert(offsetof(MdoWrkMatRes, unknownc8) == 0x0c8, "");
static_assert(offsetof(MdoWrkMatRes, unknownd0) == 0x0d0, "");
static_assert(offsetof(MdoWrkMatRes, unknownd8) == 0x0d8, "");
static_assert(offsetof(MdoWrkMatRes, unknowne8) == 0x0e8, "");
static_assert(offsetof(MdoWrkMatRes, unknownf0) == 0x0f0, "");
static_assert(offsetof(MdoWrkMatRes, unknownf8) == 0x0f8, "");
static_assert(offsetof(MdoWrkMatRes, unknown100) == 0x100, "");
static_assert(offsetof(MdoWrkMatRes, unknown110) == 0x110, "");
static_assert(offsetof(MdoWrkMatRes, unknown118) == 0x118, "");
static_assert(offsetof(MdoWrkMatRes, unknown130) == 0x130, "");
static_assert(offsetof(MdoWrkMatRes, unknown138) == 0x138, "");
static_assert(offsetof(MdoWrkMatRes, unknown140) == 0x140, "");
static_assert(offsetof(MdoWrkMatRes, unknown148) == 0x148, "");
static_assert(offsetof(MdoWrkMatRes, unknown150) == 0x150, "");
static_assert(offsetof(MdoWrkMatRes, unknown158) == 0x158, "");
static_assert(offsetof(MdoWrkMatRes, unknown168) == 0x168, "");
static_assert(offsetof(MdoWrkMatRes, unknown170) == 0x170, "");
static_assert(offsetof(MdoWrkMatRes, unknown178) == 0x178, "");

struct MdoWrkVanishEffect {
  u8 _00[0xa4 - 0x00];
  f32 unknowna4; // Inferred role: ratio; original member name unknown.
  u8 _a8[0xd4 - 0xa8];
  u32 unknownd4; // Inferred role: flags; original member name unknown.
  u8 _d8[0xe0 - 0xd8];
};

static_assert(offsetof(MdoWrkVanishEffect, unknowna4) == 0xa4, "");
static_assert(offsetof(MdoWrkVanishEffect, unknownd4) == 0xd4, "");
static_assert(sizeof(MdoWrkVanishEffect) == 0xe0, "");

class DrMdoMat : public DrMdlBase {
public:
  // Inferred role: draw flag; original type name unknown.
  enum unknown027 {
    unknown027_2 = 1 << 2,    // Inferred role: always tex stm; original name unknown.
    unknown027_4 = 1 << 4,    // Inferred role: write stencil; original name unknown.
    unknown027_6 = 1 << 6,    // Inferred role: flag wave; original name unknown.
    unknown027_8 = 1 << 8,    // Inferred role: registered in the shadow cache update; original name unknown.
    unknown027_10 = 1 << 10,  // Inferred role: simple shadow; original name unknown.
    unknown027_14 = 1 << 14,  // Inferred role: sp alpha mode; original name unknown.
    unknown027_15 = 1 << 15,  // Inferred role: sp blur off; original name unknown.
    unknown027_16 = 1 << 16,  // Inferred role: gpu clip off; original name unknown.
    unknown027_17 = 1 << 17,  // Inferred role: always middle tex stm; original name unknown.
    unknown027_18 = 1 << 18,  // Inferred role: occlusion query off; original name unknown.
    unknown027_20 = 1 << 20,  // Inferred role: express draw mode; original name unknown.
    unknown027_28 = 1 << 28,  // Inferred role: cam check mode; original name unknown.
    unknown027_29 = 1 << 29,  // Inferred role: cam check resl; original name unknown.
    unknown027_30 = 1 << 30,  // Inferred role: debug delete; original name unknown.
    unknown027_31 = 1u << 31, // Inferred role: debug window; original name unknown.
  };

  // Inferred role: mdl flag; original type name unknown.
  enum unknown028 {
    unknown028_0 = 1 << 0,
    unknown028_1 = 1 << 1,
    unknown028_2 = 1 << 2,
    unknown028_3 = 1 << 3,
    unknown028_4 = 1 << 4,
    unknown028_5 = 1 << 5,
    unknown028_6 = 1 << 6,
    unknown028_8 = 1 << 8,
    unknown028_9 = 1 << 9,
    unknown028_20 = 1 << 20,
    unknown028_21 = 1 << 21, // Inferred role: no z pre pass before the opaque render; original name unknown.
    unknown028_15 = 1 << 15, // Inferred role: bloom; original name unknown.
    unknown028_16 = 1 << 16, // Inferred role: alpha bloom; original name unknown.
    unknown028_17 = 1 << 17, // Inferred role: volume; original name unknown.
    unknown028_18 = 1 << 18, // Inferred role: full screen shadow; original name unknown.
    unknown028_25 = 1 << 25,
    unknown028_26 = 1 << 26,
    unknown028_27 = 1 << 27,
  };

  // Inferred role: alpha flag; original type name unknown.
  enum unknown029 {
    unknown029_16 = 1 << 16, // Inferred role: updated; original name unknown.
    unknown029_18 = 1 << 18, // Inferred role: single buffer; original name unknown.
  };

  void initMdlInfo(const u8 *data, MdoWrkMatRes *matRes);
  int getNodeAnmWrkRes(const u8 *data, u8 *work, MdoWrkMatRes *matRes);
  void resetNodeAnmWrkRes(const u8 *data, MdoWrkMatRes *matRes);
  int getDispCallRes(const u8 *data, u32 offset, u8 *work, MdoWrkMatRes *matRes);
  void resetDispCallRes(const u8 *data, u32 offset, MdoWrkMatRes *matRes);
  bool checkCallBackParam(_MdoWrkDispCallTemp *temp, const MdsMatDispCCData *data);
  void updateDispCall();
  bool updateDispEnvCall(MdoWrkMatRes *matRes);
  int getFurShaderRes(const u8 *data, u32 offset, u8 *work, MdoWrkMatRes *matRes);
  void resetFurShaderRes(const u8 *data, u32 offset, MdoWrkMatRes *matRes);
  void releaseMat();
  void releaseBaseMapSeamMat(MdoWrkMatRes *matRes);
  void releaseInstMapSeamMat(MdoWrkMatRes *matRes);
  void releaseMpfMapSeamMat(MdoWrkMatRes *matRes);
  void setDrMan(DrMan *drMan);
  void updateAlphaCall();
  bool isBloomMdl() const;
  bool isAlphaBloomMdl() const;
  u32 getVanishEffectMax() const;
  bool isFullScreenShadowMdl() const;
  MdoWrkVanishEffect *getVanishEffectWrk(u32 index, u32 bufferIndex);
  bool isVolumeMdl() const;
  void setFlagWaveOn(bool isOn);
  void setExtraShaderSetup(const void *shader);
  void releaseExtraShader();
  void matCall_BaseColor(const MdsMatInfoHeader *header, int valueIndex, int matIndex);
  void matCall_BaseVal(const MdsMatInfoHeader *header, int valueIndex, int matIndex);
  void matCall_TransVal(const MdsMatInfoHeader *header, int valueIndex, int matIndex);
  void matCall_CalcColor(const MdsMatInfoHeader *header, int valueIndex, int matIndex);
  void matCall_EmissiveTime(const MdsMatInfoHeader *header, int valueIndex, int matIndex);
  void matCall_RgbTime(const MdsMatInfoHeader *header, int valueIndex, int matIndex);
  void matCall_RTime(const MdsMatInfoHeader *header, int valueIndex, int matIndex);
  void matCall_AccRgb1(const MdsMatInfoHeader *header, int valueIndex, int matIndex);
  void matCall_AccRgb2(const MdsMatInfoHeader *header, int valueIndex, int matIndex);
  void matCall_AccRgb3(const MdsMatInfoHeader *header, int valueIndex, int matIndex);
  void matCall_AccRgb4(const MdsMatInfoHeader *header, int valueIndex, int matIndex);
  void matCall_AccMulRgb1(const MdsMatInfoHeader *header, int valueIndex, int matIndex);
  void matCall_AccMulRgb2(const MdsMatInfoHeader *header, int valueIndex, int matIndex);
  void matCall_AccMulRgb3(const MdsMatInfoHeader *header, int valueIndex, int matIndex);
  void matCall_AccMulRgb4(const MdsMatInfoHeader *header, int valueIndex, int matIndex);
  void matCall_AccVal1(const MdsMatInfoHeader *header, int valueIndex, int matIndex);
  void matCall_AccVal2(const MdsMatInfoHeader *header, int valueIndex, int matIndex);
  void matCall_AccVal3(const MdsMatInfoHeader *header, int valueIndex, int matIndex);
  void matCall_AccVal4(const MdsMatInfoHeader *header, int valueIndex, int matIndex);
  void matCall_AccMulVal1(const MdsMatInfoHeader *header, int valueIndex, int matIndex);
  void matCall_AccMulVal2(const MdsMatInfoHeader *header, int valueIndex, int matIndex);
  void matCall_AccMulVal3(const MdsMatInfoHeader *header, int valueIndex, int matIndex);
  void matCall_AccMulVal4(const MdsMatInfoHeader *header, int valueIndex, int matIndex);
  void matCall_CalcBloom(const MdsMatInfoHeader *header, int valueIndex, int matIndex);
  void matCall_ToonId(const MdsMatInfoHeader *header, int valueIndex, int matIndex);
  void matCall_VolumeTest(const MdsMatInfoHeader *header, int valueIndex, int matIndex);
  void updateMatFstCallBack();
  void updateMatCallBack(const u32 *flags);

  static void (DrMdoMat::*s_pCallFstMatVal[])(const MdsMatInfoHeader *header, int valueIndex, int matIndex);
  static void (DrMdoMat::*s_pCallMatVal[])(const MdsMatInfoHeader *header, int valueIndex, int matIndex);

  u8 _24[0x28 - 0x24];
  DrMan *m_drMan;
  MdoWrkMatRes *unknown30;             // Inferred role: mat res; original member name unknown.
  DrShdMdoDecExtTbl *unknown38;        // Inferred role: dec ext tbl; original member name unknown.
  mm::Col3 *unknown40;                 // Inferred role: avatar map colors; original member name unknown.
  const MdsMatCharMkTexTbl *unknown48; // Inferred role: char mk textures; original member name unknown.
  void *unknown50;                     // Inferred role: dammy data; original member name unknown.
  DrMdoSmlShadowObj *unknown58;        // Inferred role: sml shadow; original member name unknown.
  mm::Mat44 *unknown60[2];             // Inferred role: node matrices per double-buffer slot; original member name unknown.
  mm::Mat44 m_matrix;
  mm::Vec3 m_offsetPos;
  u8 _c0[0xd0 - 0xc0];
  f32 unknownd0[2]; // Inferred role: alphas; original member name unknown.
  f32 unknownd8;    // Inferred role: bloom; original member name unknown.
  f32 unknowndc;    // Inferred role: frame spd; original member name unknown.
  f32 unknowne0;    // Inferred role: alpha; original member name unknown.
  u8 _e4[0xe8 - 0xe4];
  f32 unknowne8; // Inferred role: scale; original member name unknown.
  u8 _ec[0xf0 - 0xec];
  bool m_gpuRender;
  u8 _f1;
  u16 unknownf2; // Inferred role: render pass id; original member name unknown.
  s16 unknownf4; // Inferred role: render pass index; original member name unknown.
  u8 _f6[0xf8 - 0xf6];
  u32 unknownf8;  // Inferred role: mdl flags; original member name unknown.
  u32 unknownfc;  // Inferred role: draw flags; original member name unknown.
  u32 unknown100; // Inferred role: alpha flags; original member name unknown.
  u8 _104[0x110 - 0x104];
  mm::Vec3 m_AABBMax;
  mm::Vec3 m_AABBMin;
  mm::Vec3 unknown130; // Inferred role: center; original member name unknown.
  u8 _140[0x160 - 0x140];
  f32 unknown160; // Inferred role: length; original member name unknown.
  u8 _164[0x168 - 0x164];
  u16 unknown168; // Inferred role: update flags; original member name unknown.
  u8 unknown16a;  // Inferred role: char mk texture count; original member name unknown.
  u8 _16b;
};

static_assert(offsetof(DrMdoMat, m_drMan) == 0x28, "");
static_assert(offsetof(DrMdoMat, unknown30) == 0x30, "");
static_assert(offsetof(DrMdoMat, unknown38) == 0x38, "");
static_assert(offsetof(DrMdoMat, unknown40) == 0x40, "");
static_assert(offsetof(DrMdoMat, unknown48) == 0x48, "");
static_assert(offsetof(DrMdoMat, unknown50) == 0x50, "");
static_assert(offsetof(DrMdoMat, unknown58) == 0x58, "");
static_assert(offsetof(DrMdoMat, unknown60) == 0x60, "");
static_assert(offsetof(DrMdoMat, m_matrix) == 0x70, "");
static_assert(offsetof(DrMdoMat, m_offsetPos) == 0xb0, "");
static_assert(offsetof(DrMdoMat, unknownd0) == 0xd0, "");
static_assert(offsetof(DrMdoMat, unknownd8) == 0xd8, "");
static_assert(offsetof(DrMdoMat, unknowndc) == 0xdc, "");
static_assert(offsetof(DrMdoMat, unknowne0) == 0xe0, "");
static_assert(offsetof(DrMdoMat, unknowne8) == 0xe8, "");
static_assert(offsetof(DrMdoMat, m_gpuRender) == 0xf0, "");
static_assert(offsetof(DrMdoMat, unknownf2) == 0xf2, "");
static_assert(offsetof(DrMdoMat, unknownf4) == 0xf4, "");
static_assert(offsetof(DrMdoMat, unknownf8) == 0xf8, "");
static_assert(offsetof(DrMdoMat, unknownfc) == 0xfc, "");
static_assert(offsetof(DrMdoMat, unknown100) == 0x100, "");
static_assert(offsetof(DrMdoMat, m_AABBMax) == 0x110, "");
static_assert(offsetof(DrMdoMat, unknown168) == 0x168, "");
static_assert(offsetof(DrMdoMat, m_AABBMin) == 0x120, "");
static_assert(offsetof(DrMdoMat, unknown130) == 0x130, "");
static_assert(offsetof(DrMdoMat, unknown160) == 0x160, "");
static_assert(offsetof(DrMdoMat, unknown16a) == 0x16a, "");

} // namespace ml
