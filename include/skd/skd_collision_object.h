#pragma once
#include <stddef.h>

#include "anm/anm_reference_object.h"
#include "mm_math/mm_quat.h"
#include "mm_math/mm_vec3.h"
#include "mm_math/mm_vec4.h"
#include "mm_mtl/vector.h"
#include "types.h"

namespace mm {
namespace mtl {
class IAllocator;
} // namespace mtl
} // namespace mm

namespace anm {
class Pose;
} // namespace anm

namespace skd {

struct ResCollisionCapsule;
struct ResCollisionPlane;
struct ResCollisionShpere;
struct ResCollisionArray;

// Layout evidence for the collision shapes below: ShpereCollObj::calc (0x710097ae94), CapsuleCollObj::calc
// (0x710097b0ec) and PlaneCollObj::calc (0x710097b6dc) read the resource and the bone index and write the world
// space members; the copies of their Vector expand show the Vec3 and Vec4 members copied one by one, and their
// Vector resize zero-fills new elements (no user-provided default constructor).
class ShpereCollObj {
public:
  void calc(anm::Pose &pose);
  bool detect(f32 radius, const mm::Vec3 &position, mm::Vec3 *normal) const;

  const ResCollisionShpere *unknown00; // Inferred role: resource; original member name unknown.
  mm::Vec3 unknown10;                  // Inferred role: world center; original member name unknown.
  u16 unknown20;                       // Inferred role: bone index; original member name unknown.
};

static_assert(offsetof(ShpereCollObj, unknown10) == 0x10, "");
static_assert(offsetof(ShpereCollObj, unknown20) == 0x20, "");
static_assert(sizeof(ShpereCollObj) == 0x30, "");

class CapsuleCollObj {
public:
  void calc(anm::Pose &pose);
  bool detect(f32 radius, const mm::Vec3 &position, mm::Vec3 *normal) const;

  const ResCollisionCapsule *unknown00; // Inferred role: resource; original member name unknown.
  mm::Vec3 unknown10;                   // Inferred role: first world end point; original member name unknown.
  mm::Vec3 unknown20;                   // Inferred role: second world end point; original member name unknown.
  u16 unknown30;                        // Inferred role: bone index; original member name unknown.
};

static_assert(offsetof(CapsuleCollObj, unknown10) == 0x10, "");
static_assert(offsetof(CapsuleCollObj, unknown20) == 0x20, "");
static_assert(offsetof(CapsuleCollObj, unknown30) == 0x30, "");
static_assert(sizeof(CapsuleCollObj) == 0x40, "");

// Inferred world transform of a plane collision: its copy (Vec3 member by member, then the Quat as raw bytes) runs
// before the bone index in Vector<PlaneCollObj>::expand (0x710097d0d4), so the two members form one struct; original
// name unknown.
struct unknown001 {
  mm::Vec3 unknown00; // Inferred role: world position; original member name unknown.
  mm::Quat unknown10; // Inferred role: world rotation; original member name unknown.
};

static_assert(offsetof(unknown001, unknown10) == 0x10, "");
static_assert(sizeof(unknown001) == 0x20, "");

class PlaneCollObj {
public:
  void calc(anm::Pose &pose);
  bool detect(f32 radius, const mm::Vec3 &position, mm::Vec3 *normal) const;

  const ResCollisionPlane *unknown00; // Inferred role: resource; original member name unknown.
  mm::Vec4 unknown10;                 // Inferred role: world plane (normal and distance); original member name unknown.
  unknown001 unknown20;               // Inferred role: world transform; original member name unknown.
  u16 unknown40;                      // Inferred role: bone index; original member name unknown.
};

static_assert(offsetof(PlaneCollObj, unknown10) == 0x10, "");
static_assert(offsetof(PlaneCollObj, unknown20) == 0x20, "");
static_assert(offsetof(PlaneCollObj, unknown40) == 0x40, "");
static_assert(sizeof(PlaneCollObj) == 0x50, "");

// Layout evidence: CollionArray::CollionArray (0x710097bb08) stores the resource array at 0x0 and builds the sphere,
// capsule and plane vectors at 0x8, 0x30 and 0x58 with the four flag at 0x80; its destructor (0x7100979a78) destroys
// the three vectors in reverse order.
class CollionArray {
public:
  CollionArray(mm::mtl::IAllocator *allocator, const ResCollisionArray &collisions, const anm::Pose &pose);
  void update(anm::Pose &pose);

  const ResCollisionArray *unknown00;        // Inferred role: resource collisions; original member name unknown.
  mm::mtl::Vector<ShpereCollObj> unknown08;  // Inferred role: spheres; original member name unknown.
  mm::mtl::Vector<CapsuleCollObj> unknown30; // Inferred role: capsules; original member name unknown.
  mm::mtl::Vector<PlaneCollObj> unknown58;   // Inferred role: planes; original member name unknown.
  bool unknown80;                            // Inferred role: updated once; original member name unknown.
};

static_assert(offsetof(CollionArray, unknown08) == 0x08, "");
static_assert(offsetof(CollionArray, unknown30) == 0x30, "");
static_assert(offsetof(CollionArray, unknown58) == 0x58, "");
static_assert(offsetof(CollionArray, unknown80) == 0x80, "");
static_assert(sizeof(CollionArray) == 0x88, "");

// Layout evidence: CollisionObject::CollisionObject (0x7100979930) constructs the CollionArray at 0x18 and stores the
// pose at 0xa0; CollisionObject::~CollisionObject (0x71009799d8) destroys the array and releases the pose, and
// CollisionObject::update (0x7100979c20) calls CollionArray::update with the pose at 0xa0.
class CollisionObject : public anm::ReferenceObject {
public:
  CollisionObject(const ResCollisionArray &collisions, anm::Pose &pose, mm::mtl::IAllocator *allocator);
  ~CollisionObject() override;
  void update();

  CollionArray unknown18; // Inferred role: collision shapes; original member name unknown.
  anm::Pose *unknowna0;   // Inferred role: bind pose; original member name unknown.
};

static_assert(offsetof(CollisionObject, unknown18) == 0x18, "");
static_assert(offsetof(CollisionObject, unknowna0) == 0xa0, "");
static_assert(sizeof(CollisionObject) == 0xa8, "");

} // namespace skd
