#pragma once
#include <stddef.h>

#include "grlib/cglib_dbl_uniform_block.h"
#include "grlib/cglib_tri_uniform_block.h"
#include "types.h"

namespace ml {
class DrMan;
struct _MPFWorkAutoInf;

class DrMdoMpfMan {
public:
  DrMdoMpfMan(DrMan *man);
  ~DrMdoMpfMan();
  void freeMemWork(_MPFWorkAutoInf *work);
  void begin();
  void beginBitTbl(int num);
  void update();
  static int getMpfWindMax();
  void updateInstanceBuff(u32 offset, u32 size);
  u32 advanceInstanceBuffOsf(u32 size);
  void bindInstanceBuffAsVertex(u32 stream, u32 offset, u32 stride, u32 divisor);
  void bindInstanceBuffAsSSBO(u32 index, u32 offset, u32 size);
  void addMpfObj();
  bool delMpfObj();
  u8 *getTempMemTbl(int size);
  void releaseList(_MPFWorkAutoInf *work);
  void debugListCheck(_MPFWorkAutoInf *work, int type);

  u8 _00[0x08 - 0x00];
  s32 unknown08; // Inferred role: mpf object count; original member name unknown.
  u8 _0c[0x18 - 0x0c];
  grlib::CGLibTriUniformBlock unknown18; // Inferred role: uniform block; original member name unknown.
  grlib::CGLibDblUniformBlock unknown60; // Inferred role: uniform block; original member name unknown.
  grlib::CGLibDblUniformBlock unknown98; // Inferred role: instance uniform block; original member name unknown.
  u8 _c8[0xd0 - 0x98 - sizeof(grlib::CGLibDblUniformBlock)];
  u32 unknownd0; // Inferred role: instance buffer offset; original member name unknown.
  u8 _d4[0xd8 - 0xd4];
  _MPFWorkAutoInf *unknownd8; // Inferred role: work list end; original member name unknown.
  _MPFWorkAutoInf *unknowne0; // Inferred role: work list end; original member name unknown.
  _MPFWorkAutoInf *unknowne8; // Inferred role: first memory block; original member name unknown.
  _MPFWorkAutoInf *unknownf0; // Inferred role: smallest free work found by the last search; original member name unknown.
  u8 _f8[0x3d00 - 0xf8];
  u8 *unknown3d00; // Inferred role: bit table cleared by beginBitTbl; original member name unknown.
  u8 _3d08[0x3d10 - 0x3d08];
  u8 *unknown3d10; // Inferred role: temporary memory; original member name unknown.
  u8 _3d18[0x3d20 - 0x3d18];
  s32 unknown3d20;   // Inferred role: temporary memory used size; original member name unknown.
  f32 unknown3d24;   // Inferred role: unknown value set to one by begin; original member name unknown.
  void *unknown3d28; // Inferred role: work memory; original member name unknown.
  u8 _3d30[0x3d40 - 0x3d30];
};

static_assert(offsetof(DrMdoMpfMan, unknown08) == 0x08, "");
static_assert(offsetof(DrMdoMpfMan, unknown18) == 0x18, "");
static_assert(offsetof(DrMdoMpfMan, unknown60) == 0x60, "");
static_assert(offsetof(DrMdoMpfMan, unknown98) == 0x98, "");
static_assert(offsetof(DrMdoMpfMan, unknown3d28) == 0x3d28, "");
static_assert(sizeof(DrMdoMpfMan) == 0x3d40, "");
static_assert(offsetof(DrMdoMpfMan, unknownd0) == 0xd0, "");
static_assert(offsetof(DrMdoMpfMan, unknownd8) == 0xd8, "");
static_assert(offsetof(DrMdoMpfMan, unknowne0) == 0xe0, "");
static_assert(offsetof(DrMdoMpfMan, unknown3d10) == 0x3d10, "");
static_assert(offsetof(DrMdoMpfMan, unknown3d20) == 0x3d20, "");
static_assert(offsetof(DrMdoMpfMan, unknown3d24) == 0x3d24, "");
static_assert(offsetof(DrMdoMpfMan, unknowne8) == 0xe8, "");
static_assert(offsetof(DrMdoMpfMan, unknown3d00) == 0x3d00, "");
static_assert(offsetof(DrMdoMpfMan, unknownf0) == 0xf0, "");

} // namespace ml
