#include "skd/skd_factory.h"

#include <new>

#include "anm/anm_array-inl.h"
#include "mm_mtl/allocator.h"
#include "skd/skd_collision_object.h"
#include "skd/skd_dynamics.h"

namespace skd {

CollisionObject *Factory::createColObject(const ResCollisionArray *collisions, anm::Pose &pose, mm::mtl::IAllocator *allocator) {
  if (collisions == nullptr) {
    return nullptr;
  }
  CollisionObject *object = new (allocator->allocate(sizeof(CollisionObject), 8)) CollisionObject(*collisions, pose, allocator);
  object->addRef();
  return object;
}

Dynamics *Factory::createDynamics(CollisionObject *collisionObject, const anm::Array<ResSolver> &solvers, anm::Pose &pose, ForceInterface *force, mm::mtl::IAllocator *allocator) {
  if (collisionObject == nullptr) {
    return nullptr;
  }
  Dynamics *dynamics = new (allocator->allocate(sizeof(Dynamics), 8)) Dynamics(*collisionObject, solvers, pose, force, allocator);
  dynamics->addRef();
  return dynamics;
}

} // namespace skd
