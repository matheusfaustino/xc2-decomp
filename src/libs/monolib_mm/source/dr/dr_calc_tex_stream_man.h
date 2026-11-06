#pragma once
#include <stddef.h>

#include "dr/dr_res_chank_list.h"
#include "mm_mtl/singleton.h"
#include "types.h"

namespace ml {

class CriticalSection;
class DrCalcStmListObj;

// Inferred layout from DrCalcTexStreamMan list walks; the original member names are unknown.
class DrCalcTexStmObj {
public:
  u8 _00[0x08];
  DrCalcTexStmObj *unknown08; // Inferred role: previous object; original member name unknown.
  DrCalcTexStmObj *unknown10; // Inferred role: next object; original member name unknown.
  u8 *unknown18;              // Inferred role: address; original member name unknown.
  s32 unknown20;              // Inferred role: size; original member name unknown.
  u32 unknown24;              // Inferred role: flags, bit 0 is in use; original member name unknown.
};

static_assert(offsetof(DrCalcTexStmObj, unknown08) == 0x08, "");
static_assert(offsetof(DrCalcTexStmObj, unknown10) == 0x10, "");
static_assert(offsetof(DrCalcTexStmObj, unknown18) == 0x18, "");
static_assert(offsetof(DrCalcTexStmObj, unknown20) == 0x20, "");
static_assert(offsetof(DrCalcTexStmObj, unknown24) == 0x24, "");

class DrCalcTexStreamMan : public mm::mtl::PtrSingleton<DrCalcTexStreamMan> {
public:
  static CriticalSection &getCriticalSection();
  int getUseMax() const;
  int getFreeMax() const;
  int getAllFreeSize() const;
  int getAllUseSize() const;
  DrCalcTexStmObj *getTopAheadFreeObj();

  DrCalcStmListObj *createTexStmListObj(const char *path);
  DrCalcStmListObj *createBufferListObj(u32 size);
  void entryJob();
  void update();
  static u32 jobUpdate(void *argument, void *userData);
  void clearMemObj(DrCalcTexStmObj *object);
  void releaseFreeObj(DrCalcTexStmObj *&object);
  void jobWait();
  void callHlvTexOptimization();
  void callOptimization(u32 value);
  void callReadStopMode();
  void calcPieceMem(int &pieceCount, u32 &freeSize, u32 &objectCount);
  void cntMemUpdate();
  bool memSeachOptimization(u32 size);

  // Original enumerator names unknown.
  enum DrTxtmType {};

  bool callReadType(DrTxtmType type);
  DrCalcTexStmObj *createAheadFreeObj(u32 size);
  bool createSizeFreeObj(DrCalcTexStmObj *object, u32 size);
  DrCalcTexStmObj *createChildObj(DrCalcTexStmObj *parent, u32 size);

  enum Flag {
    FLAG_CACHER_CLEAR_MODE = 1 << 4,
  };

  DrResChankList<DrCalcTexStmObj> unknown08; // Inferred role: object list; original member name unknown.
  DrResChankList<DrCalcTexStmObj> unknown18; // Inferred role: free object list; original member name unknown.
  u8 _0028[0x2068 - 0x0028];
  u32 unknown2068; // Inferred role: flags; original member name unknown.
  u8 _206c[0x2080 - 0x206c];
  DrCalcTexStmObj *unknown2080[64]; // Inferred role: objects to clear; original member name unknown.
  DrCalcTexStmObj *unknown2280[64]; // Inferred role: objects to optimize; original member name unknown.
  s32 unknown2480;                  // Inferred role: clear wait count; original member name unknown.
  s32 unknown2484;                  // Inferred role: listed object count; original member name unknown.
  u32 unknown2488;                  // Inferred role: optimization count; original member name unknown.
  u16 unknown248c;                  // Inferred role: read mode; original member name unknown.
  u16 unknown248e;                  // Inferred role: read mode flags; original member name unknown.
  s32 unknown2490;                  // Inferred role: read type; original member name unknown.
  s32 unknown2494;                  // Inferred role: previous read type; original member name unknown.
  u32 unknown2498;                  // Inferred role: job cpu; original member name unknown.
};

static_assert(offsetof(DrCalcTexStreamMan, unknown08) == 0x08, "");
static_assert(offsetof(DrCalcTexStreamMan, unknown18) == 0x18, "");
static_assert(offsetof(DrCalcTexStreamMan, unknown2068) == 0x2068, "");
static_assert(offsetof(DrCalcTexStreamMan, unknown2080) == 0x2080, "");
static_assert(offsetof(DrCalcTexStreamMan, unknown2280) == 0x2280, "");
static_assert(offsetof(DrCalcTexStreamMan, unknown2480) == 0x2480, "");
static_assert(offsetof(DrCalcTexStreamMan, unknown2484) == 0x2484, "");
static_assert(offsetof(DrCalcTexStreamMan, unknown248c) == 0x248c, "");
static_assert(offsetof(DrCalcTexStreamMan, unknown248e) == 0x248e, "");
static_assert(offsetof(DrCalcTexStreamMan, unknown2490) == 0x2490, "");
static_assert(offsetof(DrCalcTexStreamMan, unknown2498) == 0x2498, "");

} // namespace ml
