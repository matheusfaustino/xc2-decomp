// Original file name from the game string C:/Monolithsoft/BF2/prog/libs/skdlib/src/skdlib/skd_dynamics.cpp.
#include "skd/skd_dynamics.h"

#include <string.h>

#include "anm/anm_pose.h"
#include "mm/mm_std_base.h"
#include "mm_mtl/vector.h" // IWYU pragma: keep
#include "skd/skd_collision_object.h"

namespace skd {

NodeArray::NodeArray(mm::mtl::IAllocator *allocator, const anm::Array<ResNode> &resNodes, const anm::Array<ResNodeCollision> &resNodeCollisions)
    : unknown00(&resNodes), unknown08(&resNodeCollisions), unknown10(allocator) {
  unknown10.resize(resNodes.size());
}

#line 1 "../../src/skdlib/composite/skd_packet.h"
Packet::Packet(mm::mtl::IAllocator *allocator, anm::Pose &pose) : m_pBindPose(nullptr), unknown08(nullptr), unknown10(allocator), unknown140(6) {
  registerApplyDetectJobExecuter(&unknown10);
  registerDetectCapsuleJobExecuter(&unknown10);
  registerDetectPlaneJobExecuter(&unknown10);
  registerDetectSphereJobExecuter(&unknown10);
  MM_ASSERT(m_pBindPose == nullptr);
  m_pBindPose = &pose;
  pose.addRef();
}

Packet::~Packet() { anm::unknown001(m_pBindPose); }

#line 1 "../../src/skdlib/src/skdlib/skd_dynamics.cpp"
Dynamics::Dynamics(CollisionObject &collisionObject, const anm::Array<ResSolver> &solvers, anm::Pose &pose, ForceInterface *force, mm::mtl::IAllocator *allocator)
    : anm::ReferenceObject(allocator), m_pResSolver(&solvers), m_solvers(allocator), unknown48(allocator, pose), unknown198(0.1f), unknown19c(0.0f) {
  unknown190 = &collisionObject;
  collisionObject.addRef();
  unknown48.unknown08 = force;
  createSolver();
}

Dynamics::~Dynamics() {
  anm::unknown001(unknown190);
  u32 count = m_pResSolver->size();
  for (u32 i = 0; i < count; i++) {
    delete m_solvers[i];
  }
  m_solvers.clear();
}

void Dynamics::destroySolver() {
  u32 count = m_pResSolver->size();
  for (u32 i = 0; i < count; i++) {
    delete m_solvers[i];
  }
  m_solvers.clear();
}

void Dynamics::update(f32 deltaTime) {
  if (deltaTime < 0.00000011920928955078125f) {
    return;
  }
  u32 count = m_solvers.size();
  if (count == 0) {
    return;
  }
  for (u32 i = 0; i < count; i++) {
    MM_ASSERT(m_solvers[i]);
    m_solvers[i]->solve(deltaTime, unknown190->unknown18, unknown48);
  }
}

void Dynamics::updateMT(f32 deltaTime, anm::ThreadPool &threadPool) {
  if (deltaTime < 0.00000011920928955078125f) {
    return;
  }
  u32 count = m_solvers.size();
  if (count == 0) {
    return;
  }
  for (u32 i = 0; i < count; i++) {
    MM_ASSERT(m_solvers[i]);
    m_solvers[i]->solveMT(deltaTime, unknown190->unknown18, unknown48, threadPool);
  }
}

void Dynamics::resetDynamics() {
  u32 count = m_pResSolver->size();
  for (u32 i = 0; i < count; i++) {
    m_solvers[i]->resetNodes(unknown48);
  }
}

f32 Dynamics::getSolverBlendWeight(u32 index) const {
  if (index == static_cast<u32>(-1)) {
    return 0.0f;
  }
  if (m_solvers[index] == nullptr) {
    return 0.0f;
  }
  return m_solvers[index]->m_solverBlendWeight;
}

void Dynamics::setSolverBlendWeight(u32 index, f32 weight) {
  if (index == static_cast<u32>(-1)) {
    return;
  }
  if (m_solvers[index] != nullptr) {
    m_solvers[index]->m_solverBlendWeight = weight;
  }
}

s32 Dynamics::getSolverIndex(const char *name) const {
  s32 index = -1;
  for (u32 i = 0; i < m_solvers.size(); i++) {
    if (m_solvers[i] == nullptr) {
      continue;
    }
    const ResSolver *resSolver = m_solvers[i]->m_pResSolver;
    if (resSolver == nullptr) {
      continue;
    }
    if (strcmp(name, resSolver->unknown00) == 0) {
      index = i;
      break;
    }
  }
  return index;
}

} // namespace skd

template void mm::mtl::Vector<skd::DynamicsSolver *>::extend(size_t, size_t);
template void mm::mtl::Vector<skd::DynamicsSolver *>::resize(size_t);
template void mm::mtl::Vector<skd::Node>::resize(size_t);
template void mm::mtl::Vector<skd::Node>::extend(size_t, size_t);
