// Original file name unknown: the octree has no assertion string naming its source file.
#include "types.h"
#include "wnd/wnd_area.h" // IWYU pragma: keep
#include "wnd/wnd_shape.h"

namespace wnd {

Octree::Octree() : unknown20(0) {}

void Octree::setAABB(const AABB &aabb, u32 level) {
  unknown00 = aabb;
  unknown20 = level;
}

} // namespace wnd
