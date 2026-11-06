#include "gf/object/component/gf_com_dynamics.h"

#include "anm/anm_array-inl.h"
#include "framework/dynamics/dynamics_manager.h"
#include "framework/dynamics/force_calculator.h"
#include "framework/message/message_object.h"
#include "framework/object/updatable_object.h"
#include "framework/reference_frame.h"
#include "gf/gf_allocator.h"
#include "gf/object/component/animation/gf_com_asm.h"
#include "gf/object/component/gf_com_wind.h"
#include "gf/object/component/model/gf_com_model.h"
#include "mm_mtl/delegate.h"
#include "mm_mtl/rtti.h"
#include "mm_mtl/vector.h"
#include "skd/skd_collision_object.h"
#include "skd/skd_dynamics.h"
#include "skd/skd_factory.h"
#include "skd/skd_packet.h"

namespace anm {
class Pose;
} // namespace anm
namespace fw {
class WindProbe;
} // namespace fw

namespace gf {

bool GfComDynamics::recieveMessage(fw::MessageObject *sender, fw::Message &message) {
  if (message.unknown00 == MSG_DEBUG_ACTIVATE_COMPONENT_WINDOW) {
    procMsgActivateComponentDebugWindow(sender, static_cast<MsgDebugActivateComponentWindow &>(message));
    return true;
  }

  return false;
}

void GfComDynamics::procMsgActivateComponentDebugWindow(fw::MessageObject *sender, MsgDebugActivateComponentWindow &message) {}

GfComDynamics::GfComDynamics() : unknowne8(ITERATION_COUNT_DEFAULT), unknownec(false), unknown100(nullptr) { unknownf0.unknown08 = this; }

GfComDynamics::~GfComDynamics() {}

void GfComDynamics::setup(const SetupInfo &info) {
  if (info.resDynamics == nullptr) {
    return;
  }

  GfComModel *model = unknown08->getComponent<GfComModel>();
  if (model == nullptr) {
    return;
  }

  anm::Pose *pose = model->unknown30;
  if (pose == nullptr) {
    return;
  }

  skd::CollisionObject *collisionObject = info.collisionObject;
  bool isCreateCollision = false;
  if (collisionObject == nullptr) {
    collisionObject = skd::Factory::createColObject(info.resDynamics->unknown18, *pose, getAllocator(ALLOCATOR_DEFAULT));
    isCreateCollision = true;
  }

  if (info.forceCalculator != nullptr) {
    unknowne0 = info.forceCalculator;
  } else {
    GfComWind *wind = unknown08->getComponent<GfComWind>();
    if (wind != nullptr) {
      unknowne0 = fw::WindForceCalculator::create().get();
      fw::WindProbe *probe = wind->unknown10;
      fw::ForceCalculator *calculator = unknowne0.operator->();
      fw::WindForceCalculator *windForceCalculator = calculator != nullptr ? mm::mtl::dynamicCast<fw::WindForceCalculator>(calculator) : nullptr;
      windForceCalculator->bindProbe(*probe);
    }
  }

  skd::Dynamics *dynamics = skd::Factory::createDynamics(collisionObject, *info.resDynamics->unknown20, *pose, unknowne0.get(), getAllocator(ALLOCATOR_DEFAULT));
  dynamics->unknown48.unknown140 = info.iterationCount;
  fw::DyanmicsManager *manager = fw::DyanmicsManager::get();
  if (manager != nullptr) {
    u32 layer = getLayer();
    unknown10.bind(dynamics, model->unknown20);
    unknown10.unknown78.bind(this, &GfComDynamics::applyAttribute);
    manager->addUpdater(layer, unknown10);
    if (dynamics != nullptr) {
      dynamics->release();
    }

    if (isCreateCollision) {
      unknownc0.bind(collisionObject);
      manager->addColUpdater(layer, unknownc0);
      if (collisionObject != nullptr) {
        collisionObject->release();
      }
    }
  }

  unknowne8 = info.iterationCount;
}

void GfComDynamics::applyAttribute() {
  GfComAsm *comAsm = unknown08->getComponent<GfComAsm>();
  if (comAsm == nullptr) {
    return;
  }

  skd::Dynamics *dynamics = unknown10.unknown18;
  u64 attribute = comAsm->getAttribute(static_cast<AsmCategory>(0), ANIM_LAYER_BASE);
  bool isFreeMovement = (attribute & ASM_ATTRIBUTE_LIMIT_MOVEMENT) == 0;
  f32 maxMoveDistance = isFreeMovement ? 0.1f : 0.03f;
  f32 maxRotation = isFreeMovement ? 0.0f : 0.1f;
  dynamics->unknown198 = maxMoveDistance;
  dynamics->unknown19c = maxRotation;
  if ((attribute & ASM_ATTRIBUTE_MAX_ITERATION) != 0 || unknownec) {
    dynamics->unknown48.unknown140 = ITERATION_COUNT_MAX;
  } else {
    dynamics->unknown48.unknown140 = unknowne8;
  }
}

void GfComDynamics::cleanup() {
  if (unknowne0.get() != nullptr) {
    fw::WindForceCalculator *windForceCalculator = mm::mtl::dynamicCast<fw::WindForceCalculator>(unknowne0.get());
    if (windForceCalculator != nullptr) {
      windForceCalculator->unbindProbe();
    }
  }

  fw::DyanmicsManager *manager = fw::DyanmicsManager::get();
  if (manager == nullptr) {
    return;
  }

  u32 layer = getLayer();
  if (unknown10.unknown18 != nullptr && unknown10.unknown20.get() != nullptr) {
    manager->removeUpdater(layer, unknown10);
    unknown10.unknown78.clear();
    unknown10.unbind();
  }
  if (unknownc0.unknown18 != nullptr) {
    manager->removeColUpdater(layer, unknownc0);
    unknownc0.unbind();
  }
}

void GfComDynamics::changeLayer(u32 oldLayer, u32 newLayer) {
  fw::DyanmicsManager *manager = fw::DyanmicsManager::get();
  if (manager == nullptr) {
    return;
  }

  if (unknown10.unknown18 != nullptr && unknown10.unknown20.get() != nullptr) {
    manager->removeUpdater(oldLayer, unknown10);
    manager->addUpdater(newLayer, unknown10);
  }
  if (unknownc0.unknown18 != nullptr) {
    manager->removeColUpdater(oldLayer, unknownc0);
    manager->addColUpdater(newLayer, unknownc0);
  }
}

void GfComDynamics::resetMovement() {
  unknown10.setEnableMovement(false);
  unknown10.setEnableMovement(true);
}

u32 GfComDynamics::getSolverCount() const {
  if (unknown10.unknown18 != nullptr) {
    return unknown10.unknown18->m_solvers.size();
  }

  return 0;
}

static const char *findSolverName(const skd::Dynamics *dynamics, u32 index) {
  if (index == static_cast<u32>(-1) || dynamics == nullptr) {
    return nullptr;
  }

  const anm::Array<skd::ResSolver> &solvers = *dynamics->m_pResSolver;
  if (index < solvers.size()) {
    return solvers[index].unknown00;
  }

  return nullptr;
}

const char *GfComDynamics::getSolverName(u32 index) const { return findSolverName(unknown10.unknown18, index); }

u32 GfComDynamics::getSolverID(const char *name) const {
  if (unknown10.unknown18 != nullptr) {
    return unknown10.unknown18->m_solvers.size();
  }

  return static_cast<u32>(-1);
}

void GfComDynamics::setSolverBlendWeight(u32 index, f32 weight) {
  if (unknown10.unknown18 != nullptr) {
    unknown10.unknown18->setSolverBlendWeight(index, weight);
  }
}

f32 GfComDynamics::getSolverBlendWeight(u32 index) const {
  if (unknown10.unknown18 != nullptr) {
    return unknown10.unknown18->getSolverBlendWeight(index);
  }

  return 1.0f;
}

const mm::mtl::RTTI *GfComDynamics::getRTTI() const { return &m_rtti; }

} // namespace gf
