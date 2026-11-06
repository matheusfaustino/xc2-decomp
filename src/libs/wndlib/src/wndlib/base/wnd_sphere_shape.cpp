// Original file name unknown: the shapes have no assertion string naming their source file.
#include "mm_math/mm_vec3.h"
#include "types.h"
#include "wnd/wnd_shape.h" // IWYU pragma: keep

namespace wnd {

SphereShape::SphereShape(f32 radius) : Shape(unknown001_0), unknown10(radius) {}

void SphereShape::calcAABB(const Transform &transform, AABB *aabb) {
  mm::Vec3 radius(unknown10, unknown10, unknown10);
  aabb->unknown00 = transform.unknown00 - radius;
  aabb->unknown10 = transform.unknown00 + radius;
}

} // namespace wnd
