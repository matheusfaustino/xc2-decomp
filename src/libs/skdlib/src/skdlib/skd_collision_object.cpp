// No game string names this file: named after its header skd/skd_collision_object.h. The game places these
// instantiations right after skd::CollionArray's constructor, which builds the three shape vectors.
#include "skd/skd_collision_object.h"

#include "anm/anm_pose.h"
#include "anm/anm_qs_transform.h"
#include "mm_mtl/vector.h" // IWYU pragma: keep
#include "skd/skd_dynamics.h"

namespace mm {
namespace mtl {
class IAllocator;
} // namespace mtl
} // namespace mm

namespace skd {

void ShpereCollObj::calc(anm::Pose &pose) {
  const anm::QsTransform &transform = pose.getBoneModelSpace(unknown20);
  mm::Vec3 local(unknown00->unknown20.x, unknown00->unknown20.y, unknown00->unknown20.z);
  mm::Vec3 world = transform.unknown10.rotate(local);
  unknown10 = world + mm::Vec3(transform.unknown00.x, transform.unknown00.y, transform.unknown00.z);
}

CollisionObject::CollisionObject(const ResCollisionArray &collisions, anm::Pose &pose, mm::mtl::IAllocator *allocator)
    : anm::ReferenceObject(allocator), unknown18(allocator, collisions, pose), unknowna0(&pose) {
  pose.addRef();
}

CollisionObject::~CollisionObject() { anm::unknown001(unknowna0); }

void CollisionObject::update() { unknown18.update(*unknowna0); }

} // namespace skd

template void mm::mtl::Vector<skd::ShpereCollObj>::resize(size_t);
template void mm::mtl::Vector<skd::CapsuleCollObj>::resize(size_t);
template void mm::mtl::Vector<skd::PlaneCollObj>::resize(size_t);
template void mm::mtl::Vector<skd::ShpereCollObj>::expand(size_t, size_t);
template void mm::mtl::Vector<skd::ShpereCollObj>::extend(size_t, size_t);
template void mm::mtl::Vector<skd::CapsuleCollObj>::expand(size_t, size_t);
template void mm::mtl::Vector<skd::CapsuleCollObj>::extend(size_t, size_t);
template void mm::mtl::Vector<skd::PlaneCollObj>::expand(size_t, size_t);
template void mm::mtl::Vector<skd::PlaneCollObj>::extend(size_t, size_t);
