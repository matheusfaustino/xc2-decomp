#pragma once
#include <stddef.h>

#include "mm_math/mm_vec3.h"
#include "navi/nvmsh_ac_obj.h"
#include "types.h"

namespace navi {

class NvmshResMan;
struct _NaviCalcResTbl;
struct _NaviTempColiResl;

// Navigation mesh agent object implementation. The game's vtable is at 0x71011b74d0 and its constructor at 0x71008e906c
// initialises the fields below; the destructor slot of the vtable points at NvmshAcObj's.
class NvmshAcObjBase : public NvmshAcObj {
public:
  const mm::Vec3 &getMoveVec() const override;
  void setBeginPos(const mm::Vec3 &position) override;
  void setEndPos(const mm::Vec3 &position) override;
  void setBeginRadius(f32 radius) override;
  void setEndRadius(f32 radius) override;
  void setBeginSeachRadius(f32 radius) override;
  void setEndSeachRadius(f32 radius) override;
  void setBeginHeight(f32 param1, f32 param2) override;
  void setEndHeight(f32 param1, f32 param2) override;
  void setMeshNum(int num) override;
  int getMeshNum() const override;
  void setMoveSpd(f32 speed) override;

  void rootReset();
  void conectReset();
  void resetCache();
  void setRootListBuff(u8 *buffer, int size);
  void calcNextVec(const _NaviCalcResTbl *resTable, const _NaviTempColiResl &res);
  void calcRootMeshVec(const _NaviCalcResTbl *resTable, const _NaviTempColiResl &res);
  void calcTargetVecLine(const mm::Vec3 &start, const mm::Vec3 &end);

  u32 unknown14;             // Inferred role: never written by the constructor; original member name unknown.
  NvmshAcObjBase *unknown18; // Inferred role: previous object in the update list; original member name unknown.
  NvmshAcObjBase *unknown20; // Inferred role: next object in the update list; original member name unknown.
  u32 unknown28;             // Inferred role: state flags cleared by resetCache; original member name unknown.
  u32 unknown2c;             // Inferred role: unknown; original member name unknown.
  u8 *unknown30;             // Inferred role: root list buffer; original member name unknown.
  u32 unknown38;             // Inferred role: root list buffer size; original member name unknown.
  u32 unknown3c;             // Inferred role: unknown; original member name unknown.
  u8 *unknown40;             // Inferred role: root list buffer; original member name unknown.
  u8 *unknown48;             // Inferred role: root list buffer; original member name unknown.
  u16 unknown50;             // Inferred role: unknown; original member name unknown.
  u16 unknown52;             // Inferred role: compared with unknown50 by calcNextVec; original member name unknown.
  u16 unknown54;             // Inferred role: unknown; original member name unknown.
  u16 unknown56;             // Inferred role: unknown; original member name unknown.
  u16 unknown58;             // Inferred role: unknown; original member name unknown.
  u16 unknown5a;             // Inferred role: unknown; original member name unknown.
  u16 unknown5c;             // Inferred role: unknown; original member name unknown.
  u8 unknown5e;              // Inferred role: unknown; original member name unknown.
  u8 unknown5f;              // Inferred role: unknown; original member name unknown.
  u8 unknown60;              // Inferred role: unknown; original member name unknown.
  u8 unknown61;              // Inferred role: unknown; original member name unknown.
  u8 unknown62;              // Inferred role: unknown; original member name unknown.
  s8 m_meshNum;
  u8 unknown64; // Inferred role: set from setMeshNum's argument being positive; original member name unknown.
  u8 unknown65; // Inferred role: bit 0 requests a release; original member name unknown.
  u8 _66[0xb0 - 0x66];
  mm::Vec3 m_beginPos;
  // Two floats because setBeginHeight takes two values and the game stores them adjacently.
  f32 m_beginHeight[2];
  f32 m_beginRadius;
  f32 m_beginSeachRadius;
  u8 _d0[0xe0 - 0xd0];
  mm::Vec3 m_endPos;
  // Two floats because setEndHeight takes two values and the game stores them adjacently.
  f32 m_endHeight[2];
  f32 m_endRadius;
  f32 m_endSeachRadius;
  u8 _100[0x110 - 0x100];
  mm::Vec3 unknown110;
  mm::Vec3 unknown120;
  mm::Vec3 unknown130;
  u8 _140[0x150 - 0x140];
  mm::Vec3 m_moveVec;
  f32 m_moveSpd;
};

static_assert(offsetof(NvmshAcObjBase, unknown14) == 0x14, "");
static_assert(offsetof(NvmshAcObjBase, unknown18) == 0x18, "");
static_assert(offsetof(NvmshAcObjBase, unknown20) == 0x20, "");
static_assert(offsetof(NvmshAcObjBase, unknown28) == 0x28, "");
static_assert(offsetof(NvmshAcObjBase, unknown2c) == 0x2c, "");
static_assert(offsetof(NvmshAcObjBase, unknown30) == 0x30, "");
static_assert(offsetof(NvmshAcObjBase, unknown38) == 0x38, "");
static_assert(offsetof(NvmshAcObjBase, unknown3c) == 0x3c, "");
static_assert(offsetof(NvmshAcObjBase, unknown40) == 0x40, "");
static_assert(offsetof(NvmshAcObjBase, unknown48) == 0x48, "");
static_assert(offsetof(NvmshAcObjBase, unknown50) == 0x50, "");
static_assert(offsetof(NvmshAcObjBase, unknown52) == 0x52, "");
static_assert(offsetof(NvmshAcObjBase, unknown54) == 0x54, "");
static_assert(offsetof(NvmshAcObjBase, unknown56) == 0x56, "");
static_assert(offsetof(NvmshAcObjBase, unknown58) == 0x58, "");
static_assert(offsetof(NvmshAcObjBase, unknown5a) == 0x5a, "");
static_assert(offsetof(NvmshAcObjBase, unknown5c) == 0x5c, "");
static_assert(offsetof(NvmshAcObjBase, unknown5e) == 0x5e, "");
static_assert(offsetof(NvmshAcObjBase, unknown5f) == 0x5f, "");
static_assert(offsetof(NvmshAcObjBase, unknown60) == 0x60, "");
static_assert(offsetof(NvmshAcObjBase, unknown61) == 0x61, "");
static_assert(offsetof(NvmshAcObjBase, unknown62) == 0x62, "");
static_assert(offsetof(NvmshAcObjBase, m_meshNum) == 0x63, "");
static_assert(offsetof(NvmshAcObjBase, unknown64) == 0x64, "");
static_assert(offsetof(NvmshAcObjBase, unknown65) == 0x65, "");
static_assert(offsetof(NvmshAcObjBase, m_beginPos) == 0xb0, "");
static_assert(offsetof(NvmshAcObjBase, m_beginHeight) == 0xc0, "");
static_assert(offsetof(NvmshAcObjBase, m_beginRadius) == 0xc8, "");
static_assert(offsetof(NvmshAcObjBase, m_beginSeachRadius) == 0xcc, "");
static_assert(offsetof(NvmshAcObjBase, m_endPos) == 0xe0, "");
static_assert(offsetof(NvmshAcObjBase, m_endHeight) == 0xf0, "");
static_assert(offsetof(NvmshAcObjBase, m_endRadius) == 0xf8, "");
static_assert(offsetof(NvmshAcObjBase, m_endSeachRadius) == 0xfc, "");
static_assert(offsetof(NvmshAcObjBase, unknown110) == 0x110, "");
static_assert(offsetof(NvmshAcObjBase, unknown120) == 0x120, "");
static_assert(offsetof(NvmshAcObjBase, unknown130) == 0x130, "");
static_assert(offsetof(NvmshAcObjBase, m_moveVec) == 0x150, "");
static_assert(offsetof(NvmshAcObjBase, m_moveSpd) == 0x160, "");
static_assert(sizeof(NvmshAcObjBase) == 0x170, "");

// Both classes only override the destructor: their vtables (0x71011b7560 and 0x71011b75f0) hold the
// NvmshAcObjBase slots after it. The class names come from the game symbols, their role is unknown.
class NvmshAcObjBaseDef : public NvmshAcObjBase {
public:
  ~NvmshAcObjBaseDef();
  void release();
  bool update(bool &unknown, NvmshResMan *resMan, bool param3);
};

class NvmshAcObjBaseGbl : public NvmshAcObjBase {
public:
  ~NvmshAcObjBaseGbl();
  // The game has this function (0x71008ee2b4); it is not reconstructed yet.
  void release();
  bool update(bool &unknown, NvmshResMan *resMan, bool param3);
};

static_assert(sizeof(NvmshAcObjBaseDef) == 0x170, "");
static_assert(sizeof(NvmshAcObjBaseGbl) == 0x170, "");

} // namespace navi
