#pragma once
#include "mm_math/mm_vec3.h"
#include "types.h"
#include <stddef.h>

namespace ml {

// Inferred role: g buff debug range; original type name unknown.
struct unknown038 {
  f32 unknown00; // Inferred role: min; original member name unknown.
  f32 unknown04; // Inferred role: max; original member name unknown.
};

static_assert(sizeof(unknown038) == 0x8, "");

// Inferred role: pixl post dbl parm; original type name unknown.
// Evidence: the DrPixlPostParm constructor clears the two entries with one memset of 0xa40 bytes from 0x514.
struct unknown039 {
  u8 _000[0x004 - 0x000];
  bool unknown04; // Inferred role: override tone map; original member name unknown.
  u8 _005[0x008 - 0x005];
  f32 unknown08; // Inferred role: tone map; original member name unknown.
  u8 _00c[0x204 - 0x00c];
  bool unknown204; // Inferred role: override mot blur; original member name unknown.
  bool unknown205; // Inferred role: mot blur2; original member name unknown.
  u8 _206[0x220 - 0x206];
  bool unknown220; // Inferred role: override dof; original member name unknown.
  bool unknown221; // Inferred role: dof; original member name unknown.
  u8 _222[0x223 - 0x222];
  bool unknown223; // Inferred role: dof hlv; original member name unknown.
  bool unknown224; // Inferred role: dof down sec; original member name unknown.
  bool unknown225; // Inferred role: dof auto focus; original member name unknown.
  u8 _226[0x228 - 0x226];
  f32 unknown228; // Inferred role: dof pixl ratio; original member name unknown.
  f32 unknown22c; // Inferred role: dof distance; original member name unknown.
  f32 unknown230; // Inferred role: dof range; original member name unknown.
  f32 unknown234; // Inferred role: dof strength; original member name unknown.
  f32 unknown238; // Inferred role: dof lens; original member name unknown.
  f32 unknown23c; // Inferred role: dof blend; original member name unknown.
  s32 unknown240; // Inferred role: dof pixl lv; original member name unknown.
  s32 unknown244; // Inferred role: dof focus num; original member name unknown.
  f32 unknown248; // Inferred role: dof focus ratio; original member name unknown.
  f32 unknown24c; // Inferred role: dof focus min; original member name unknown.
  u8 _250[0x272 - 0x250];
  bool unknown272; // Inferred role: override color filter; original member name unknown.
  u8 _273[0x274 - 0x273];
  s32 unknown274; // Inferred role: color filter num; original member name unknown.
  f32 unknown278; // Inferred role: color filter frm; original member name unknown.
  u8 _27c[0x280 - 0x27c];
  s32 unknown280; // Inferred role: color filter far num; original member name unknown.
  u8 _284[0x520 - 0x284];
};

static_assert(offsetof(unknown039, unknown08) == 0x008, "");
static_assert(offsetof(unknown039, unknown204) == 0x204, "");
static_assert(offsetof(unknown039, unknown220) == 0x220, "");
static_assert(offsetof(unknown039, unknown223) == 0x223, "");
static_assert(offsetof(unknown039, unknown228) == 0x228, "");
static_assert(offsetof(unknown039, unknown24c) == 0x24c, "");
static_assert(offsetof(unknown039, unknown272) == 0x272, "");
static_assert(offsetof(unknown039, unknown274) == 0x274, "");
static_assert(offsetof(unknown039, unknown280) == 0x280, "");
static_assert(sizeof(unknown039) == 0x520, "");

class DrPixlPostParm {
public:
  struct stRainDropLayerInfo {
    f32 unknown00; // Role and original member name unknown.
    f32 unknown04; // Role and original member name unknown.
    f32 unknown08; // Role and original member name unknown.
    f32 unknown0c; // Role and original member name unknown.
    f32 unknown10; // Role and original member name unknown.
  };

  bool isShwRaytrace() const;
  void calcReductionTransBuffer(int &width, int &height) const;
  void setDOF(bool dof);
  bool getTMAAOn() const;
  f32 getTMAASharpness(u32 index) const;
  bool isMotBlur2() const;
  bool getBloomOn() const;
  bool getToneMapOn() const;
  bool isMotBlur() const;
  bool isBlm2PassExtract() const;
  bool isShwRaytraceTemporalWithOthers() const;
  void update();
  bool getAAOn() const;
  bool getColTblOn() const;
  f32 getBorderBlmMul() const;
  static DrPixlPostParm *get();
  void setInst();
  DrPixlPostParm();
  void reset();
  ~DrPixlPostParm();
  DrPixlPostParm &operator=(const DrPixlPostParm &parm);
  void calcOfsRenderCheck(f32 x, f32 y);
  void calcReductionBuffer(int &width, int &height);
  void setOverrideToneMap(f32 overrideToneMap);
  f32 getToonDirLightScale() const;
  void OnCloseAttriMenu();
  void resetScene();
  void setWeatherFrame(f32 weatherFrame);
  f32 getWeatherFrame() const;
  void resetWeatherFrame();
  void writeParameter(const char *path, const char *name);
  void readParameter(const char *path, bool isCreate, const char *name);
  void updateParameter(const char *name);
  void setOverrideMotBlur(bool overrideMotBlur);
  void setMotBlur2(bool motBlur2);
  void setOverrideDOF(bool overrideDOF);
  void setDOFHlv(bool dofHlv);
  void setDOFDownSec(bool dofDownSec);
  void setDOFAutoFocus(bool dofAutoFocus);
  void setDOFAutoFocusDebug(bool dofAutoFocusDebug);
  void setDoFPixlRatio(f32 dofPixlRatio);
  void setDoFDistance(f32 dofDistance);
  void setDoFRange(f32 dofRange);
  void setDoFStrength(f32 dofStrength);
  void setDoFLens(f32 dofLens);
  void setDoFBlend(f32 dofBlend);
  void setDoFPixlLv(s32 dofPixlLv);
  void setDoFocusNum(s32 dofFocusNum);
  void setDoFDofFocusRat(f32 dofFocusRatio);
  void setDofFocusMin(f32 dofFocusMin);
  bool isOverrideDOF() const;
  bool isOverrideColorFilter() const;
  void setOverrideColorFilter(bool overrideColorFilter);
  void setColorFilterNum(s32 colorFilterNum);
  void setColorFilterFrm(f32 colorFilterFrm);
  void setColorFilterTransZ(bool colorFilterTransZ);
  void setColorFilterFarNum(s32 colorFilterFarNum);
  bool getRainDropOn() const;
  bool getSSRWithTrans() const;
  bool getSSROn() const;
  void setGlassBrk(bool glassBrk, u32 type);
  void getRanDropLayerInfo(u32 layer, stRainDropLayerInfo &info) const;

  static DrPixlPostParm *s_pIns;

  bool m_toneMapOn;
  u8 _0001[0x0008 - 0x0001];
  f32 unknown08; // Inferred role: tone max; original member name unknown.
  f32 unknown0c; // Inferred role: tone min; original member name unknown.
  f32 unknown10; // Inferred role: tone ratio; original member name unknown.
  f32 unknown14; // Inferred role: tone ratio last; original member name unknown.
  f32 unknown18; // Inferred role: tone blm max; original member name unknown.
  f32 unknown1c; // Inferred role: tone blm min; original member name unknown.
  f32 unknown20; // Inferred role: tone blm ratio; original member name unknown.
  f32 unknown24; // Inferred role: tone blm ratio last; original member name unknown.
  f32 unknown28; // Inferred role: tone ave min rate; original member name unknown.
  f32 unknown2c; // Inferred role: tone ave max rate; original member name unknown.
  u8 _0030[0x003c - 0x0030];
  bool m_bloomOn;
  u8 _003d[0x003e - 0x003d];
  bool unknown3e; // Inferred role: lens flare; original member name unknown.
  u8 _003f[0x0040 - 0x003f];
  f32 unknown40; // Inferred role: border blm sub; original member name unknown.
  f32 m_borderBlmMul;
  f32 unknown48;    // Inferred role: border blm sec sub; original member name unknown.
  f32 unknown4c;    // Inferred role: border blm sec mul; original member name unknown.
  f32 unknown50;    // Inferred role: border blm max; original member name unknown.
  f32 unknown54[3]; // Inferred role: blm weight; original member name unknown.
  u8 _0060[0x007c - 0x0060];
  f32 unknown7c[3]; // Inferred role: blm str; original member name unknown.
  u8 _0088[0x0090 - 0x0088];
  f32 unknown90[3]; // Inferred role: blm gauss; original member name unknown.
  u8 _009c[0x00c8 - 0x009c];
  f32 unknownc8;  // Inferred role: blm lens sub; original member name unknown.
  f32 unknowncc;  // Inferred role: blm lens mul; original member name unknown.
  s32 unknownd0;  // Inferred role: blm lens max; original member name unknown.
  f32 unknownd4;  // Inferred role: blm lens rot; original member name unknown.
  f32 unknownd8;  // Inferred role: blm lens weight; original member name unknown.
  f32 unknowndc;  // Inferred role: blm lens decay; original member name unknown.
  f32 unknowne0;  // Inferred role: blm lens strength; original member name unknown.
  f32 unknowne4;  // Inferred role: blm lens parm max; original member name unknown.
  f32 unknowne8;  // Inferred role: blm lens uv decay; original member name unknown.
  bool unknownec; // Inferred role: god ray; original member name unknown.
  u8 _00ed[0x00f0 - 0x00ed];
  f32 unknownf0; // Inferred role: god ray weight; original member name unknown.
  f32 unknownf4; // Inferred role: god ray decay; original member name unknown.
  f32 unknownf8; // Inferred role: god ray strength; original member name unknown.
  bool m_AAOn;
  u8 _00fd[0x0100 - 0x00fd];
  f32 unknown100; // Inferred role: aa edge sharpness; original member name unknown.
  f32 unknown104; // Inferred role: aa edge threshold; original member name unknown.
  f32 unknown108; // Inferred role: aa edge threshold min; original member name unknown.
  f32 unknown10c; // Inferred role: aa edge ranges; original member name unknown.
  u8 _0110[0x0111 - 0x0110];
  bool m_TMAAOn;
  u8 _0112[0x0114 - 0x0112];
  f32 unknown114[6]; // Inferred role: tmaa sharpness; original member name unknown.
  f32 unknown12c[6]; // Inferred role: tmaa sharpness; original member name unknown.
  u8 _0144[0x0158 - 0x0144];
  bool unknown158; // Inferred role: tmaa reset request; original member name unknown.
  u8 _0159[0x0174 - 0x0159];
  bool unknown174; // Inferred role: tssao; original member name unknown.
  u8 _0175[0x0188 - 0x0175];
  f32 unknown188; // Inferred role: tssao post blur strength; original member name unknown.
  u8 _018c[0x0194 - 0x018c];
  bool unknown194; // Inferred role: ssao; original member name unknown.
  u8 _0195[0x019c - 0x0195];
  f32 unknown19c; // Inferred role: ssao blur str; original member name unknown.
  u8 _01a0[0x01ca - 0x01a0];
  bool unknown1ca; // Inferred role: mot blur; original member name unknown.
  bool unknown1cb; // Role and original member name unknown.
  u8 _01cc[0x01cd - 0x01cc];
  bool unknown1cd; // Inferred role: obj mot blur; original member name unknown.
  bool unknown1ce; // Inferred role: app blur; original member name unknown.
  u8 _01cf[0x01d0 - 0x01cf];
  bool unknown1d0; // Inferred role: sp blur; original member name unknown.
  u8 _01d1[0x01d4 - 0x01d1];
  f32 unknown1d4; // Inferred role: mot blur strength; original member name unknown.
  f32 unknown1d8; // Inferred role: mot blur sub; original member name unknown.
  u8 _01dc[0x01ec - 0x01dc];
  f32 unknown1ec; // Inferred role: obj mot blur strength; original member name unknown.
  f32 unknown1f0; // Inferred role: blur sp parm; original member name unknown.
  u8 _01f4[0x01fc - 0x01f4];
  bool m_motBlur2;
  u8 _01fd[0x0218 - 0x01fd];
  bool unknown218; // Inferred role: dof; original member name unknown.
  bool unknown219; // Inferred role: dof override; original member name unknown.
  bool unknown21a; // Inferred role: dof h lv mode; original member name unknown.
  bool unknown21b; // Inferred role: dof down sec; original member name unknown.
  bool unknown21c; // Inferred role: dof auto focus; original member name unknown.
  u8 _021d[0x021e - 0x021d];
  bool unknown21e; // Inferred role: dof focus debug; original member name unknown.
  u8 _021f[0x0220 - 0x021f];
  f32 unknown220;  // Inferred role: dof pixl ratio; original member name unknown.
  f32 unknown224;  // Inferred role: dof distance; original member name unknown.
  f32 unknown228;  // Inferred role: dof range; original member name unknown.
  f32 unknown22c;  // Inferred role: dof strength; original member name unknown.
  f32 unknown230;  // Inferred role: dof lens; original member name unknown.
  f32 unknown234;  // Inferred role: dof blend; original member name unknown.
  s32 unknown238;  // Inferred role: dof pixl lv; original member name unknown.
  s32 unknown23c;  // Inferred role: dof focus num; original member name unknown.
  f32 unknown240;  // Inferred role: dof focus ratio; original member name unknown.
  f32 unknown244;  // Inferred role: dof focus min; original member name unknown.
  bool unknown248; // Inferred role: disp global ibl tex; original member name unknown.
  u8 _0249[0x0269 - 0x0249];
  bool unknown269; // Inferred role: global IBL relight request enable; original member name unknown.
  bool m_colTblOn;
  u8 _026b[0x026c - 0x026b];
  u32 unknown26c;  // Inferred role: color filter near first texture id; original member name unknown.
  f32 unknown270;  // Inferred role: color filter frm; original member name unknown.
  bool unknown274; // Inferred role: color filter trans z; original member name unknown.
  u8 _0275[0x0278 - 0x0275];
  u32 unknown278; // Inferred role: color filter far first texture id; original member name unknown.
  u8 _027c[0x028c - 0x027c];
  u32 unknown28c; // Inferred role: color filter near second texture id; original member name unknown.
  u32 unknown290; // Inferred role: color filter far second texture id; original member name unknown.
  f32 unknown294; // Inferred role: color filter near ratio; original member name unknown.
  f32 unknown298; // Inferred role: color filter far ratio; original member name unknown.
  u8 _029c[0x02a0 - 0x029c];
  f32 unknown2a0; // Inferred role: toon dir light scale; original member name unknown.
  u8 _02a4[0x0304 - 0x02a4];
  bool unknown304; // Inferred role: deffered color; original member name unknown.
  u8 _0305[0x0320 - 0x0305];
  unknown038 unknown320[5]; // Inferred role: g buff debug ranges; original member name unknown.
  u8 _0348[0x0360 - 0x0348];
  f32 unknown360; // Inferred role: shadow str; original member name unknown.
  u8 _0364[0x037c - 0x0364];
  f32 unknown37c; // Inferred role: shadow chache angle; original member name unknown.
  u8 _0380[0x03b8 - 0x0380];
  bool m_rainDropOn;
  u8 _03b9[0x040c - 0x03b9];
  f32 unknown40c[3]; // Inferred role: rain drop layer value; original member name unknown.
  f32 unknown418[3]; // Inferred role: rain drop layer value; original member name unknown.
  f32 unknown424[3]; // Inferred role: rain drop layer value; original member name unknown.
  f32 unknown430[3]; // Inferred role: rain drop layer value; original member name unknown.
  f32 unknown43c[3]; // Inferred role: rain drop layer value; original member name unknown.
  u8 _0448[0x0460 - 0x0448];
  bool m_shwRaytrace;
  u8 _0461[0x0468 - 0x0461];
  f32 unknown468; // Inferred role: sky luminance weight; original member name unknown.
  f32 unknown46c; // Inferred role: sky luminance weight; original member name unknown.
  f32 unknown470; // Inferred role: specular str; original member name unknown.
  u8 _0474[0x0484 - 0x0474];
  f32 unknown484;  // Inferred role: illuminance debug scale; original member name unknown.
  bool unknown488; // Inferred role: g buff debug; original member name unknown.
  bool unknown489; // Inferred role: SSR debug; original member name unknown.
  bool unknown48a; // Inferred role: SSR; original member name unknown.
  u8 _048b[0x04d0 - 0x048b];
  bool unknown4d0; // Inferred role: glass br; original member name unknown.
  u8 _04d1[0x0514 - 0x04d1];
  unknown039 unknown514[2]; // Inferred role: dbl parms; original member name unknown.
  bool unknownf54;          // Inferred role: histgram reset; original member name unknown.
  u8 _0f55[0x0f58 - 0x0f55];
  s32 unknownf58; // Inferred role: bloom scale type; original member name unknown.
  u8 _0f5c[0x0f72 - 0x0f5c];
  bool unknownf72; // Inferred role: blm 2 pass; original member name unknown.
  u8 _0f73[0x0f78 - 0x0f73];
  u32 unknownf78; // Inferred role: SSR debug scale shift; original member name unknown.
  u8 _0f7c[0x0f94 - 0x0f7c];
  f32 unknownf94; // Inferred role: tssao prev blur strength; original member name unknown.
  u8 _0f98[0x0fb1 - 0x0f98];
  bool unknownfb1; // Inferred role: tssao quarter normal; original member name unknown.
  bool unknownfb2; // Inferred role: reduction; original member name unknown.
  bool unknownfb3; // Inferred role: reduction buffer; original member name unknown.
  bool unknownfb4; // Inferred role: separate trans; original member name unknown.
  u8 _0fb5[0x0fb8 - 0x0fb5];
  f32 unknownfb8; // Inferred role: reduction sc x; original member name unknown.
  f32 unknownfbc; // Inferred role: reduction sc y; original member name unknown.
  f32 unknownfc0; // Inferred role: reduction buffer sc x; original member name unknown.
  f32 unknownfc4; // Inferred role: reduction buffer sc y; original member name unknown.
  u32 unknownfc8; // Inferred role: ofs render check; original member name unknown.
  u8 _0fcc[0x0fdc - 0x0fcc];
  f32 unknownfdc; // Inferred role: reduction trans sc x; original member name unknown.
  f32 unknownfe0; // Inferred role: reduction trans sc y; original member name unknown.
  s32 unknownfe4; // Inferred role: render mode; original member name unknown.
  u8 _0fe8[0x0ff0 - 0x0fe8];
  mm::Vec3 unknownff0; // Inferred role: sp blur target; original member name unknown.
  u8 _1000[0x1024 - 0x1000];
  bool unknown1024; // Inferred role: shw raytrace temporal; original member name unknown.
  u8 _1025[0x103c - 0x1025];
  bool unknown103c; // Inferred role: temporal shadow enable; original member name unknown.
  bool unknown103d; // Inferred role: temporal shadow history valid; original member name unknown.
  bool unknown103e; // Inferred role: char dir shadow; original member name unknown.
  u8 _103f[0x1061 - 0x103f];
  bool unknown1061; // Inferred role: selects the IBL pixl objs; original member name unknown.
  u8 _1062[0x1064 - 0x1062];
  f32 unknown1064; // Inferred role: toon gradation value set from the ambient light data; original member name unknown.
  u8 _1068[0x107c - 0x1068];
  s32 unknown107c;  // Inferred role: light num; original member name unknown.
  bool unknown1080; // Inferred role: in out; original member name unknown.
  u8 _1081[0x1084 - 0x1081];
  s32 unknown1084;    // Inferred role: weather; original member name unknown.
  f32 unknown1088[2]; // Inferred role: weather frames; original member name unknown.
  u8 _1090[0x109c - 0x1090];
  f32 unknown109c; // Inferred role: wetness value, its ini default is the fallback of DrIniSys::getWetnessSetting; original member name unknown.
  u8 _10a0[0x10c0 - 0x10a0];
  u32 unknown10c0; // Inferred role: rain drop fade frames; original member name unknown.
  u8 _10c4[0x10fc - 0x10c4];
  bool unknown10fc; // Inferred role: attri menu closed; original member name unknown.
  u8 _10fd[0x10fe - 0x10fd];
  bool unknown10fe; // Inferred role: disp reduction; original member name unknown.
  u8 _10ff[0x1104 - 0x10ff];
  bool unknown1104;    // Inferred role: SSR disable by scene; original member name unknown.
  bool unknown1105[2]; // Inferred role: reset scenes; original member name unknown.
  u8 _1107[0x1110 - 0x1107];
};

static_assert(offsetof(DrPixlPostParm, m_toneMapOn) == 0x0000, "");
static_assert(offsetof(DrPixlPostParm, unknown08) == 0x0008, "");
static_assert(offsetof(DrPixlPostParm, unknown0c) == 0x000c, "");
static_assert(offsetof(DrPixlPostParm, unknown10) == 0x0010, "");
static_assert(offsetof(DrPixlPostParm, unknown14) == 0x0014, "");
static_assert(offsetof(DrPixlPostParm, unknown18) == 0x0018, "");
static_assert(offsetof(DrPixlPostParm, unknown1c) == 0x001c, "");
static_assert(offsetof(DrPixlPostParm, unknown20) == 0x0020, "");
static_assert(offsetof(DrPixlPostParm, unknown48a) == 0x048a, "");
static_assert(offsetof(DrPixlPostParm, unknown1104) == 0x1104, "");
static_assert(offsetof(DrPixlPostParm, unknown24) == 0x0024, "");
static_assert(offsetof(DrPixlPostParm, unknown28) == 0x0028, "");
static_assert(offsetof(DrPixlPostParm, unknown2c) == 0x002c, "");
static_assert(offsetof(DrPixlPostParm, m_bloomOn) == 0x003c, "");
static_assert(offsetof(DrPixlPostParm, unknown3e) == 0x003e, "");
static_assert(offsetof(DrPixlPostParm, unknown40) == 0x0040, "");
static_assert(offsetof(DrPixlPostParm, m_borderBlmMul) == 0x0044, "");
static_assert(offsetof(DrPixlPostParm, unknown48) == 0x0048, "");
static_assert(offsetof(DrPixlPostParm, unknown4c) == 0x004c, "");
static_assert(offsetof(DrPixlPostParm, unknown50) == 0x0050, "");
static_assert(offsetof(DrPixlPostParm, unknown54) == 0x0054, "");
static_assert(offsetof(DrPixlPostParm, unknown7c) == 0x007c, "");
static_assert(offsetof(DrPixlPostParm, unknown90) == 0x0090, "");
static_assert(offsetof(DrPixlPostParm, unknownc8) == 0x00c8, "");
static_assert(offsetof(DrPixlPostParm, unknowncc) == 0x00cc, "");
static_assert(offsetof(DrPixlPostParm, unknownd0) == 0x00d0, "");
static_assert(offsetof(DrPixlPostParm, unknownd4) == 0x00d4, "");
static_assert(offsetof(DrPixlPostParm, unknownd8) == 0x00d8, "");
static_assert(offsetof(DrPixlPostParm, unknowndc) == 0x00dc, "");
static_assert(offsetof(DrPixlPostParm, unknowne0) == 0x00e0, "");
static_assert(offsetof(DrPixlPostParm, unknowne4) == 0x00e4, "");
static_assert(offsetof(DrPixlPostParm, unknowne8) == 0x00e8, "");
static_assert(offsetof(DrPixlPostParm, unknownec) == 0x00ec, "");
static_assert(offsetof(DrPixlPostParm, unknownf0) == 0x00f0, "");
static_assert(offsetof(DrPixlPostParm, unknownf4) == 0x00f4, "");
static_assert(offsetof(DrPixlPostParm, unknownf8) == 0x00f8, "");
static_assert(offsetof(DrPixlPostParm, m_AAOn) == 0x00fc, "");
static_assert(offsetof(DrPixlPostParm, unknown100) == 0x0100, "");
static_assert(offsetof(DrPixlPostParm, unknown114) == 0x0114, "");
static_assert(offsetof(DrPixlPostParm, unknown12c) == 0x012c, "");
static_assert(offsetof(DrPixlPostParm, unknown158) == 0x0158, "");
static_assert(offsetof(DrPixlPostParm, unknown174) == 0x0174, "");
static_assert(offsetof(DrPixlPostParm, unknown188) == 0x0188, "");
static_assert(offsetof(DrPixlPostParm, unknown104) == 0x0104, "");
static_assert(offsetof(DrPixlPostParm, unknown108) == 0x0108, "");
static_assert(offsetof(DrPixlPostParm, unknown10c) == 0x010c, "");
static_assert(offsetof(DrPixlPostParm, m_TMAAOn) == 0x0111, "");
static_assert(offsetof(DrPixlPostParm, unknown194) == 0x0194, "");
static_assert(offsetof(DrPixlPostParm, unknown19c) == 0x019c, "");
static_assert(offsetof(DrPixlPostParm, unknown1ca) == 0x01ca, "");
static_assert(offsetof(DrPixlPostParm, unknown1cb) == 0x01cb, "");
static_assert(offsetof(DrPixlPostParm, unknown1cd) == 0x01cd, "");
static_assert(offsetof(DrPixlPostParm, unknown1ce) == 0x01ce, "");
static_assert(offsetof(DrPixlPostParm, unknown1d0) == 0x01d0, "");
static_assert(offsetof(DrPixlPostParm, unknown1d4) == 0x01d4, "");
static_assert(offsetof(DrPixlPostParm, unknown1d8) == 0x01d8, "");
static_assert(offsetof(DrPixlPostParm, unknown1ec) == 0x01ec, "");
static_assert(offsetof(DrPixlPostParm, unknown1f0) == 0x01f0, "");
static_assert(offsetof(DrPixlPostParm, m_motBlur2) == 0x01fc, "");
static_assert(offsetof(DrPixlPostParm, unknown218) == 0x0218, "");
static_assert(offsetof(DrPixlPostParm, unknown219) == 0x0219, "");
static_assert(offsetof(DrPixlPostParm, unknown21a) == 0x021a, "");
static_assert(offsetof(DrPixlPostParm, unknown21b) == 0x021b, "");
static_assert(offsetof(DrPixlPostParm, unknown21c) == 0x021c, "");
static_assert(offsetof(DrPixlPostParm, unknown21e) == 0x021e, "");
static_assert(offsetof(DrPixlPostParm, unknown220) == 0x0220, "");
static_assert(offsetof(DrPixlPostParm, unknown224) == 0x0224, "");
static_assert(offsetof(DrPixlPostParm, unknown228) == 0x0228, "");
static_assert(offsetof(DrPixlPostParm, unknown22c) == 0x022c, "");
static_assert(offsetof(DrPixlPostParm, unknown230) == 0x0230, "");
static_assert(offsetof(DrPixlPostParm, unknown234) == 0x0234, "");
static_assert(offsetof(DrPixlPostParm, unknown238) == 0x0238, "");
static_assert(offsetof(DrPixlPostParm, unknown23c) == 0x023c, "");
static_assert(offsetof(DrPixlPostParm, unknown240) == 0x0240, "");
static_assert(offsetof(DrPixlPostParm, unknown244) == 0x0244, "");
static_assert(offsetof(DrPixlPostParm, unknown248) == 0x0248, "");
static_assert(offsetof(DrPixlPostParm, unknown269) == 0x0269, "");
static_assert(offsetof(DrPixlPostParm, m_colTblOn) == 0x026a, "");
static_assert(offsetof(DrPixlPostParm, unknown26c) == 0x026c, "");
static_assert(offsetof(DrPixlPostParm, unknown278) == 0x0278, "");
static_assert(offsetof(DrPixlPostParm, unknown28c) == 0x028c, "");
static_assert(offsetof(DrPixlPostParm, unknown294) == 0x0294, "");
static_assert(offsetof(DrPixlPostParm, unknown298) == 0x0298, "");
static_assert(offsetof(DrPixlPostParm, unknown270) == 0x0270, "");
static_assert(offsetof(DrPixlPostParm, unknown274) == 0x0274, "");
static_assert(offsetof(DrPixlPostParm, unknown2a0) == 0x02a0, "");
static_assert(offsetof(DrPixlPostParm, unknown304) == 0x0304, "");
static_assert(offsetof(DrPixlPostParm, unknown320) == 0x0320, "");
static_assert(offsetof(DrPixlPostParm, unknown360) == 0x0360, "");
static_assert(offsetof(DrPixlPostParm, unknown37c) == 0x037c, "");
static_assert(offsetof(DrPixlPostParm, m_rainDropOn) == 0x03b8, "");
static_assert(offsetof(DrPixlPostParm, unknown40c) == 0x040c, "");
static_assert(offsetof(DrPixlPostParm, unknown418) == 0x0418, "");
static_assert(offsetof(DrPixlPostParm, unknown424) == 0x0424, "");
static_assert(offsetof(DrPixlPostParm, unknown430) == 0x0430, "");
static_assert(offsetof(DrPixlPostParm, unknown43c) == 0x043c, "");
static_assert(offsetof(DrPixlPostParm, m_shwRaytrace) == 0x0460, "");
static_assert(offsetof(DrPixlPostParm, unknown484) == 0x0484, "");
static_assert(offsetof(DrPixlPostParm, unknown468) == 0x0468, "");
static_assert(offsetof(DrPixlPostParm, unknown46c) == 0x046c, "");
static_assert(offsetof(DrPixlPostParm, unknown470) == 0x0470, "");
static_assert(offsetof(DrPixlPostParm, unknown488) == 0x0488, "");
static_assert(offsetof(DrPixlPostParm, unknown489) == 0x0489, "");
static_assert(offsetof(DrPixlPostParm, unknown4d0) == 0x04d0, "");
static_assert(offsetof(DrPixlPostParm, unknown514) == 0x0514, "");
static_assert(offsetof(DrPixlPostParm, unknownf54) == 0x0f54, "");
static_assert(offsetof(DrPixlPostParm, unknownf58) == 0x0f58, "");
static_assert(offsetof(DrPixlPostParm, unknownf72) == 0x0f72, "");
static_assert(offsetof(DrPixlPostParm, unknownf94) == 0x0f94, "");
static_assert(offsetof(DrPixlPostParm, unknownf78) == 0x0f78, "");
static_assert(offsetof(DrPixlPostParm, unknownfb1) == 0x0fb1, "");
static_assert(offsetof(DrPixlPostParm, unknownfb2) == 0x0fb2, "");
static_assert(offsetof(DrPixlPostParm, unknownfb3) == 0x0fb3, "");
static_assert(offsetof(DrPixlPostParm, unknownfb4) == 0x0fb4, "");
static_assert(offsetof(DrPixlPostParm, unknownfdc) == 0x0fdc, "");
static_assert(offsetof(DrPixlPostParm, unknownfb8) == 0x0fb8, "");
static_assert(offsetof(DrPixlPostParm, unknownfbc) == 0x0fbc, "");
static_assert(offsetof(DrPixlPostParm, unknownfc0) == 0x0fc0, "");
static_assert(offsetof(DrPixlPostParm, unknownfc8) == 0x0fc8, "");
static_assert(offsetof(DrPixlPostParm, unknownff0) == 0x0ff0, "");
static_assert(offsetof(DrPixlPostParm, unknown1024) == 0x1024, "");
static_assert(offsetof(DrPixlPostParm, unknown103c) == 0x103c, "");
static_assert(offsetof(DrPixlPostParm, unknown103d) == 0x103d, "");
static_assert(offsetof(DrPixlPostParm, unknown103e) == 0x103e, "");
static_assert(offsetof(DrPixlPostParm, unknown1061) == 0x1061, "");
static_assert(offsetof(DrPixlPostParm, unknown1064) == 0x1064, "");
static_assert(offsetof(DrPixlPostParm, unknown107c) == 0x107c, "");
static_assert(offsetof(DrPixlPostParm, unknown1080) == 0x1080, "");
static_assert(offsetof(DrPixlPostParm, unknown1084) == 0x1084, "");
static_assert(offsetof(DrPixlPostParm, unknown1088) == 0x1088, "");
static_assert(offsetof(DrPixlPostParm, unknown109c) == 0x109c, "");
static_assert(offsetof(DrPixlPostParm, unknown10c0) == 0x10c0, "");
static_assert(offsetof(DrPixlPostParm, unknown10fc) == 0x10fc, "");
static_assert(offsetof(DrPixlPostParm, unknown10fe) == 0x10fe, "");
static_assert(offsetof(DrPixlPostParm, unknown1105) == 0x1105, "");
static_assert(sizeof(DrPixlPostParm) == 0x1110, "");
static_assert(sizeof(DrPixlPostParm::stRainDropLayerInfo) == 0x14, "");

} // namespace ml
