// Original file name inferred from the ai manager sources: the game keeps every AiData call out of line.
#include "battle/ai/ai_manager.h"
#include "battle/utility_battle.h"
#include "fw/fw_transform.h"
#include "gf/game/gf_game_party.h"
#include "gf/object/gf_obj_acc.h"
#include "gf/object/gf_obj_handle.h"
#include "mm_math/mm_vec3.h"
#include "types.h"

namespace fw {
struct UpdateInfo;
} // namespace fw
namespace gf {
struct GF_OBJ_HANDLE;
} // namespace gf

namespace ai {

AiData::AiData() : m_handle(gf::INVALID_HANDLE), m_id(-1), m_sleep(false), m_frameCount(0.0f) {
  for (int i = 0; i < 16; i++) {
    m_param[i] = 0;
  }
}

AiData::~AiData() {}

void AiData::init() {}

void AiData::destroy() {}

void AiData::update(const fw::UpdateInfo &) {
  if (m_frameCount > 0.0f && m_handle != gf::INVALID_HANDLE) {
    if (!btl::Utility::AI_CheckState(m_handle, static_cast<btl::Utility::CHECK_STATE>(1), 0)) {
      m_frameCount -= 1.0f;
    }
  }
}

gf::GF_OBJ_HANDLE *AiData::getHandle() const { return m_handle; }

void AiData::setHandle(gf::GF_OBJ_HANDLE *handle) { m_handle = handle; }

gf::GF_OBJ_HANDLE *AiData::getHandleTarget() const {
  if (m_handle == gf::INVALID_HANDLE) {
    return gf::INVALID_HANDLE;
  }
  return btl::Utility::AI_GetCharacterHandle(btl::Utility::AI_GetTargetCharacterID(m_handle));
}

int AiData::getId() const { return m_id; }

void AiData::setId(int id) { m_id = id; }

bool AiData::isSleep() const { return m_sleep; }

void AiData::setSleep(bool isSleep) { m_sleep = isSleep; }

bool AiData::isValid() const {
  if (m_sleep) {
    return false;
  }
  if (m_handle == gf::INVALID_HANDLE) {
    return false;
  }
  if (gf::GfGameParty::isLeaderDriver(m_handle)) {
    return false;
  }
  gf::GfObjAcc objAcc(m_handle);
  if (!objAcc.isBattle()) {
    return false;
  }
  if (btl::Utility::AI_CheckState(m_handle, static_cast<btl::Utility::CHECK_STATE>(1), 0)) {
    return false;
  }
  if (btl::Utility::AI_CheckState(m_handle, static_cast<btl::Utility::CHECK_STATE>(3), 0)) {
    return false;
  }
  if (btl::Utility::AI_CheckState(m_handle, static_cast<btl::Utility::CHECK_STATE>(4), 0)) {
    return false;
  }
  if (btl::Utility::AI_CheckState(m_handle, static_cast<btl::Utility::CHECK_STATE>(5), 0)) {
    return false;
  }
  if (btl::Utility::AI_CheckState(m_handle, static_cast<btl::Utility::CHECK_STATE>(7), 0)) {
    return false;
  }
  if (btl::Utility::AI_CheckState(m_handle, static_cast<btl::Utility::CHECK_STATE>(8), 0)) {
    return false;
  }
  return !btl::Utility::AI_CheckState(m_handle, static_cast<btl::Utility::CHECK_STATE>(9), 0);
}

int AiData::getParam(int index) const { return m_param[index]; }

void AiData::setParam(int index, int value) { m_param[index] = value; }

void AiData::setFrameCount(f32 frameCount) { m_frameCount = frameCount; }

bool AiData::isFrameCountEnd() const { return m_frameCount <= 0.0f; }

mm::Vec3 AiData::getPosition() const { return getPosition(m_handle); }

mm::Vec3 AiData::getPosition(gf::GF_OBJ_HANDLE *handle) const {
  if (m_handle != gf::INVALID_HANDLE) {
    gf::GfObjAcc objAcc(handle);
    if (objAcc.isValid()) {
      return objAcc.getWorldTransform().unknown00;
    }
  }
  return mm::Vec3::zero;
}

mm::Vec3 AiData::getPositionTarget() const {
  if (m_handle == gf::INVALID_HANDLE) {
    return mm::Vec3::zero;
  }
  return getPosition(btl::Utility::AI_GetCharacterHandle(btl::Utility::AI_GetTargetCharacterID(m_handle)));
}

f32 AiData::getRadius() const { return getRadius(m_handle); }

f32 AiData::getRadiusTarget() const {
  gf::GF_OBJ_HANDLE *target = gf::INVALID_HANDLE;
  if (m_handle != gf::INVALID_HANDLE) {
    target = btl::Utility::AI_GetCharacterHandle(btl::Utility::AI_GetTargetCharacterID(m_handle));
  }
  return getRadius(target);
}

} // namespace ai
