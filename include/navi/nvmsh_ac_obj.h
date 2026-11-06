#pragma once
#include <stddef.h>

#include "types.h"

namespace mm {
struct Vec3;
} // namespace mm

namespace navi {

// Navi mesh agent object. The game has a NvmshAcObj destructor (D2 at 0x71008ee5c4) but no NvmshAcObj vtable and no NvmshAcObj
// definition of the other virtual functions: they are only defined by NvmshAcObjBase, whose vtable (0x71011b74d0) gives their order.
class NvmshAcObj {
public:
  virtual ~NvmshAcObj() {}
  virtual const mm::Vec3 &getMoveVec() const = 0;
  virtual void setBeginPos(const mm::Vec3 &position) = 0;
  virtual void setEndPos(const mm::Vec3 &position) = 0;
  virtual void setBeginRadius(f32 radius) = 0;
  virtual void setEndRadius(f32 radius) = 0;
  virtual void setBeginSeachRadius(f32 radius) = 0;
  virtual void setEndSeachRadius(f32 radius) = 0;
  virtual void setBeginHeight(f32 param1, f32 param2) = 0;
  virtual void setEndHeight(f32 param1, f32 param2) = 0;
  virtual void setMeshNum(int num) = 0;
  virtual int getMeshNum() const = 0;
  virtual void setMoveSpd(f32 speed) = 0;

  int unknown08; // Inferred role: search state, 1 when the move vector is valid (gf::getMoveVec); original member name unknown.
  u32 unknown0c; // Inferred role: reset with unknown10 by the agent constructors; original member name unknown.
  u32 unknown10; // Inferred role: flags, bit 1 enables the move vector (gf::getMoveVec); original member name unknown.
};

static_assert(offsetof(NvmshAcObj, unknown08) == 0x08, "");
static_assert(offsetof(NvmshAcObj, unknown0c) == 0x0c, "");
static_assert(offsetof(NvmshAcObj, unknown10) == 0x10, "");

} // namespace navi
