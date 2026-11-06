#pragma once
#include <stddef.h>

#include "mm_mtl/intrusive_list.h"
#include "types.h"
#include "wnd/wnd_object_pool_memory.h"
#include "wnd/wnd_shape.h"

namespace mm {
struct Vec3;
} // namespace mm

namespace wnd {

class WindObject;

// Axis aligned octree: the box and the number of subdivision levels.
class Octree {
public:
  Octree();
  void setAABB(const AABB &aabb, u32 level);
  u32 getBitFlag(const mm::Vec3 &position) const;

  AABB unknown00; // Inferred role: bounds; original member name unknown.
  u32 unknown20;  // Inferred role: number of subdivision levels; original member name unknown.
};

static_assert(offsetof(Octree, unknown20) == 0x20, "");
static_assert(sizeof(Octree) == 0x30, "");

// One cell of the broad phase octree. The member names come from the assertions of wnd_area.cpp and
// wnd_broad_phaze_interface.h; the others are addressed by their offset.
class AreaBlock : public mm::mtl::IntrusiveListNodeBase {
public:
  AreaBlock();
  ~AreaBlock();

  AreaBlock *createChild(u32 index, PoolObjectMemory<AreaBlock> &pool);
  void destroyChild(u32 index, PoolObjectMemory<AreaBlock> &pool);

  AreaBlock *unknown10;                       // Inferred role: parent; original member name unknown.
  AreaBlock *unknown18[8];                    // Inferred role: children; original member name unknown.
  u32 unknown58;                              // Inferred role: mask of the allocated children; original member name unknown.
  u32 unknown5c;                              // Inferred role: depth; original member name unknown.
  AABB unknown60;                             // Inferred role: bounds; original member name unknown.
  mm::mtl::IntrusiveList<WindObject> m_winds; // Named by the assertion "m_winds.empty()" of the destructor.
};

static_assert(offsetof(AreaBlock, unknown18) == 0x18, "");
static_assert(offsetof(AreaBlock, unknown58) == 0x58, "");
static_assert(offsetof(AreaBlock, unknown5c) == 0x5c, "");
static_assert(offsetof(AreaBlock, unknown60) == 0x60, "");
static_assert(offsetof(AreaBlock, m_winds) == 0x80, "");
static_assert(sizeof(AreaBlock) == 0xa0, "");

} // namespace wnd
