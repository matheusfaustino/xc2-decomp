#include "framework/dynamics/dynamics_updater.h"

#include "anm/anm_pose.h"
#include "fw/fw_update_info.h"
#include "mm_math/mm_quat.h"
#include "mm_math/mm_vec3.h"
#include "skd/skd_collision_object.h"
#include "skd/skd_dynamics.h"
#include "skd/skd_packet.h"

namespace fw {

DynamicsUpdater::DynamicsUpdater() : unknown18(nullptr), unknown20(), unknown70(true), unknown71(true), unknown72(false), unknown78(), unknown98(1.0f), unknowna0(nullptr) {
  unknown30.unknown00 = mm::Vec3::zero;
  unknown30.unknown10 = mm::Quat::identity;
  unknown50.unknown00 = mm::Vec3::zero;
  unknown50.unknown10 = mm::Quat::identity;
}

DynamicsUpdater::~DynamicsUpdater() { unbind(); }

void DynamicsUpdater::unbind() {
  if (unknown18 != nullptr) {
    unknown18->release();
    unknown18 = nullptr;
  }

  unknown20 = nullptr;
}

void DynamicsUpdater::bind(skd::Dynamics *dynamics, mm::mtl::IntrusivePtr<ReferenceFrame> frame) {
  unbind();
  this->unknown18 = dynamics;
  dynamics->addRef();
  unknown20 = frame;
  cacheTransform();
}

void DynamicsUpdater::cacheTransform() {
  if (unknown20.get() != nullptr) {
    unknown30 = unknown20->getSyncedWorldTransform();
  }
}

void DynamicsUpdater::setEnable(bool enable) {
  unknown70 = enable;
  if (unknown18 != nullptr) {
    unknown18->resetDynamics();
  }

  if (unknown71) {
    cacheTransform();
  }
}

void DynamicsUpdater::setEnableMovement(bool enable) {
  if (enable != unknown71) {
    unknown71 = enable;
    if (enable) {
      cacheTransform();
    }
  }
}

void DynamicsUpdater::update(const UpdateInfo &info) {
  if (!unknown70) {
    return;
  }

  if (unknown78.isValid()) {
    unknown78();
  }

  if (unknown72) {
    unknown18->resetDynamics();
    unknown72 = false;
  } else if (unknown71) {
    const Transform &world = unknown20->getSyncedWorldTransform();
    unknown18->notifyMovement(world.unknown00, world.unknown10, unknown30.unknown00 + unknown50.unknown00, mm::Quat::mul(unknown30.unknown10, unknown50.unknown10));
    unknown30 = world;
  }

  unknown18->update(info.unknown04 * unknown98);
  anm::Pose *pose = unknown18->unknown48.m_pBindPose;
  pose->syncLocalSpace();
  pose->syncModelSpace();
}

DynamicsColObjUpdater::DynamicsColObjUpdater() : unknown18(nullptr) {}

DynamicsColObjUpdater::~DynamicsColObjUpdater() { unbind(); }

void DynamicsColObjUpdater::unbind() {
  if (unknown18 != nullptr) {
    unknown18->release();
    unknown18 = nullptr;
  }
}

void DynamicsColObjUpdater::bind(skd::CollisionObject *collisionObject) {
  unbind();
  this->unknown18 = collisionObject;
  collisionObject->addRef();
}

void DynamicsColObjUpdater::update() { unknown18->update(); }

} // namespace fw
