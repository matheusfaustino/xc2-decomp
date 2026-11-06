#pragma once
#include <stddef.h>

#include "mm_math/mm_quat.h"
#include "mm_math/mm_vec3.h"
#include "mm_mtl/reference_object.h"
#include "types.h"

namespace wnd {

// Reconstructed from the constructors, the destructors and calcAABB of the game's shapes.
class AABB {
public:
  AABB() : unknown00(mm::Vec3::zero), unknown10(mm::Vec3::zero) {}

  mm::Vec3 unknown00; // Inferred role: minimum; original member name unknown.
  mm::Vec3 unknown10; // Inferred role: maximum; original member name unknown.
};

static_assert(offsetof(AABB, unknown10) == 0x10, "");
static_assert(sizeof(AABB) == 0x20, "");

struct Transform {
  mm::Vec3 unknown00; // Inferred role: position; original member name unknown.
  mm::Quat unknown10; // Inferred role: rotation; original member name unknown.
};

static_assert(offsetof(Transform, unknown10) == 0x10, "");
static_assert(sizeof(Transform) == 0x20, "");

// SphereShape passes 0 and BoxShape passes 1 to Shape::Shape; the original enumerator names are unknown.
enum ShapeType {
  unknown001_0 = 0,
  unknown001_1 = 1,
};

class Shape : public mm::mtl::ReferenceObject {
public:
  explicit Shape(ShapeType type);
  virtual ~Shape();
  virtual void calcAABB(const Transform &transform, AABB *aabb) = 0;

  ShapeType unknown0c; // Inferred role: shape type, stored by the constructor; original member name unknown.
};

static_assert(offsetof(Shape, unknown0c) == 0x0c, "");
static_assert(sizeof(Shape) == 0x10, "");

class SphereShape : public Shape {
public:
  explicit SphereShape(f32 radius);
  void calcAABB(const Transform &transform, AABB *aabb) override;

  f32 unknown10; // Inferred role: radius; original member name unknown.
};

static_assert(offsetof(SphereShape, unknown10) == 0x10, "");
static_assert(sizeof(SphereShape) == 0x18, "");

class BoxShape : public Shape {
public:
  explicit BoxShape(const mm::Vec3 &extent);
  void calcAABB(const Transform &transform, AABB *aabb) override;

  mm::Vec3 unknown10; // Inferred role: extent; original member name unknown.
};

static_assert(offsetof(BoxShape, unknown10) == 0x10, "");
static_assert(sizeof(BoxShape) == 0x20, "");

} // namespace wnd
