// Original file name inferred from the ai manager sources: the game keeps every AiData call out of line.
#include "battle/ai/ai_manager.h"
#include "battle/utility_battle.h"
#include "fw/fw_transform.h"
#include "gf/data/gf_bdat_chrenparam.h"
#include "gf/data/gf_bdat_rsc.h"
#include "gf/data/gf_data_enemy.h"
#include "gf/data/gf_data_param_acc.h"
#include "gf/data/gf_object_type.h"
#include "gf/game/gf_game_party.h"
#include "gf/object/gf_obj.h"
#include "gf/object/gf_obj_acc.h"
#include "gf/object/gf_obj_handle.h"
#include "gf/object/gf_obj_manager.h"
#include "gf/object/gf_obj_setup.h"
#include "mm_math/mm_vec3.h"
#include "mm_mtl/rtti.h"
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

static bool checkState(gf::GF_OBJ_HANDLE *handle, int state) { return btl::Utility::AI_CheckState(handle, static_cast<btl::Utility::CHECK_STATE>(state), 0); }

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
  if (checkState(m_handle, 1) || checkState(m_handle, 3) || checkState(m_handle, 4) || checkState(m_handle, 5) || checkState(m_handle, 7) || checkState(m_handle, 8)) {
    return false;
  }
  return !checkState(m_handle, 9);
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

f32 AiData::getRadius(gf::GF_OBJ_HANDLE *handle) const {
  f32 radius = 0.0f;
  if (m_handle != gf::INVALID_HANDLE) {
    gf::GfObjAcc objAcc(handle);
    if (objAcc.isValid()) {
      u32 type = objAcc.getType();
      if (type != gf::OBJECT_TYPE_ENEMY) {
        if (type == gf::OBJECT_TYPE_DRIVER) {
          radius = 0.5f;
        }
      } else {
        gf::DataParamAcc dataParam(handle);
        const gf::SDataEnemy *dataEnemy = dataParam.getSDataEnemy();
        gf::GfBdatRsc *bdatRsc = gf::GfBdatRsc::get();
        if (dataEnemy != nullptr && bdatRsc != nullptr) {
          radius = static_cast<f32>(bdatRsc->getRscEnEnRadius(dataEnemy->unknown44));
          gf::GFOBJ_INFO *info = gf::GfObjManager::getObjInfo(handle);
          if (info != nullptr && info->unknown30 != nullptr) {
            gf::GfObjSetup *setup = info->unknown30;
            if (setup->unknown08 == info->unknown18->unknown28 && info->unknown18->unknown28 == gf::OBJECT_TYPE_ENEMY) {
              if (setup->getRTTI()->isKindOf(&gf::GfObjSetupEnemy::m_rtti)) {
                radius *= setup->unknown40;
              }
            }
          }
        }
      }
    }
  }
  return radius;
}

int AiData::getScoutStartHp() const {
  int scoutStartHp = 0;
  if (m_handle != gf::INVALID_HANDLE) {
    gf::GfObjAcc objAcc(m_handle);
    if (objAcc.isValid() && objAcc.getType() == gf::OBJECT_TYPE_ENEMY) {
      gf::DataParamAcc dataParam(m_handle);
      const gf::SDataEnemy *dataEnemy = dataParam.getSDataEnemy();
      gf::GfBdatChrEnParam *chrEnParam = gf::GfBdatChrEnParam::get();
      if (dataEnemy != nullptr && chrEnParam != nullptr) {
        scoutStartHp = chrEnParam->getChrEnParamScoutStartHp(dataEnemy->unknown08);
      }
    }
  }
  return scoutStartHp;
}

f32 AiData::getRadiusTarget() const {
  gf::GF_OBJ_HANDLE *target = gf::INVALID_HANDLE;
  if (m_handle != gf::INVALID_HANDLE) {
    target = btl::Utility::AI_GetCharacterHandle(btl::Utility::AI_GetTargetCharacterID(m_handle));
  }
  return getRadius(target);
}

} // namespace ai
