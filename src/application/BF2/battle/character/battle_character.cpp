#include "battle/character/battle_character.h"
#include "fw/fw_pad.h"

#include <float.h>
#include <math.h>

#include "battle/battle_global.h"
#include "battle/blade_arts_manager.h"
#include "battle/blade_change_manager.h"
#include "battle/btl_system_balance_access.h"
#include "battle/button_challenge.h"
#include "battle/chain_attack_manager.h"
#include "battle/character/character_manager_battle.h"
#include "battle/character/character_parameter_battle.h"
#include "battle/data_manager_battle.h"
#include "battle/group_data.h"
#include "battle/hate_ring_manager.h"
#include "battle/misc_battle.h"
#include "battle/sp_attack_manager.h"
#include "battle/talk_manager_battle.h"
#include "battle/ui_manager_battle.h"
#include "battle/utility_battle.h"
#include "bdat.h"
#include "framework/coli/coli_object.h"
#include "framework/effect/effect_manager.h"
#include "framework/navi/navi_manager.h"
#include "gf/data/gf_bdat_id.h"
#include "gf/data/gf_data_bdat.h"
#include "gf/data/gf_data_blade.h"
#include "gf/data/gf_data_param_acc.h"
#include "gf/data/gf_data_weapon.h"
#include "gf/game/gf_game_manager.h"
#include "gf/game/gf_game_mount.h"
#include "gf/game/gf_game_party.h"
#include "gf/menu/object/gf_menu_obj_damage_buff.h"
#include "gf/object/component/animation/gf_com_asm.h"
#include "gf/object/component/effect/gf_com_effect_caster.h"
#include "gf/object/component/gf_com_collider.h"
#include "gf/object/component/gf_com_sound.h"
#include "gf/object/component/pc/gf_com_property_pc.h"
#include "gf/object/component/property/gf_com_property.h"
#include "gf/object/gf_obj.h"
#include "gf/object/gf_obj_acc.h"
#include "gf/object/gf_obj_handle.h"
#include "gf/object/gf_obj_manager.h"
#include "gf/object/gf_obj_util.h"
#include "gf/request/gf_req_scom.h"
#include "ml/rand.h"
#include "mm_math/mm_math.h"
#include "mm_mtl/bitflag.h"
#include "mm_mtl/fixed_array.h"
#include "mm_mtl/fixed_vector.h"
#include "mm_mtl/fixstr.h"
#include "mm_mtl/intrusive_ptr.h"
#include "mm_mtl/vector.h"

namespace btl {

constexpr int ALPHA_RATE_NUM = 7;
constexpr int BLADE_BDAT_ID_UNKNOWN_3EB = 0x3eb;
constexpr s16 COLLISION_PRIORITY_UNKNOWN_8AD0 = -30000;
constexpr f32 PAUSE_DT = 0.000001f;
constexpr f32 NORMALIZE_LENGTH_MIN = 0.000001f;
constexpr u16 ANIMATION_INVALID = 39;
constexpr int ALPHA_WEAPON_FIRST = 12;
constexpr int ALPHA_WEAPON_LAST = 16;
constexpr int ALPHA_PROC_WEAPON = 6;
constexpr int ALPHA_PROC_LAST = 11;
constexpr int SE_HIT_ATTRIBUTE = 9;
constexpr int SE_ATTRIBUTE_NONE = 9;
constexpr int SE_HIT_BULLET = 17;
constexpr int SLOT_ALL = -17;
constexpr f32 DELAY_VOICE_TIME_SCALE = 4096.0f;
constexpr int BUFF_EFFECT_UNKNOWN_7 = 7;
constexpr int ENHANCE_GUARD_REFLECT_UNKNOWN_65 = 0x65;
constexpr int KAKUSEI_BDAT_ID_UNKNOWN_3EA = 0x3ea;
constexpr int KAKUSEI_BDAT_ID_UNKNOWN_3EB = 0x3eb;
constexpr int ACTION_FLAGS_SLIDE = 0x3c000;
constexpr int ENHANCE_CRITICAL_DAMAGE_UNKNOWN_2E = 0x2e;
constexpr int ANIM_SPECIAL_PROC_TYPE_UNKNOWN_2 = 2;
constexpr f32 LAND_HEIGHT_NONE = -2999.0f;
constexpr u8 ATTACK_POSITION_UNKNOWN_3 = 3;
constexpr int BUFF_EFFECT_TARGET_LOCK = 8;
constexpr int COMMAND_NUM = 10;
constexpr u16 ACTION_PHASE_BLADE_ARTS_MOVE = 8;
constexpr u16 ACTION_PHASE_UNKNOWN_3 = 3;
constexpr u16 ACTION_PHASE_BLADE_CHANGE_RETURN = 6;
constexpr int AI_STATE_ACTIVE = 1;
constexpr f32 AI_SCOUT_TIME = 10.0f;
constexpr u32 BLADE_CHANGE_ACTIONS = 0xd2;
constexpr int BLADE_SUPPORT_CONDITION_HATE_ELSE = 8;
constexpr int SP_ATTACK_SLOT = 15;
constexpr int CONDITION_UNKNOWN_F = 15;
constexpr int KIZUNA_STAGE_MAX = 6;

BattleCharacter::~BattleCharacter() {
  for (int i = 0; i < EFFECT_ID_NUM; i++) {
    DeleteEffect(i, -1);
  }
  if (unknown170 != nullptr) {
    if (fw::NaviManager::get() != nullptr) {
      fw::NaviManager::get()->releaseObject(unknown170);
    }
    unknown170 = nullptr;
    unknownf8c_5 = false;
  }
}

void BattleCharacter::ResetHateRingData() {
  if (!m_player && unknown118 != gf::INVALID_HANDLE && unknownf7d >= 0) {
    HateRingManager::get()->SetData(unknownf7d, unknown118, -1);
  }
}

void BattleCharacter::ResetDelayVoice() { unknowndd0 = -1; }

void BattleCharacter::ResetCustomVoiceData() {
  unknownf5c = 0;
  unknownf60 = 0;
  for (int i = 0; i < 3; i++) {
    unknownf64[i].unknown00 = 0;
    unknownf64[i].unknown01 = 0;
  }
}

void BattleCharacter::SetupCustomVoiceData() {
  ResetCustomVoiceData();
  const CharacterParameter *parameter = GetCharacterParameterConst(true);
  if (parameter == nullptr || !parameter->m_invalidTarget) {
    return;
  }
  gf::DataParamAcc acc(unknown118);
  int personality = acc.getPersonality();
  if (personality > 0) {
    u8 *bdat = gf::GfDataBdat::getFP(gf::BDAT_BTL_BL_PERSONALITY);
    mm::mtl::FixStr<32> name;
    for (int i = 0; i < 3; i++) {
      u32 voice = Bdat::getValCheck(bdat, name.format("Custom%02d", i + 1).c_str(), personality, 1) & 0xff;
      unknownf64[i].unknown00 = voice;
      unknownf64[i].unknown01 = Bdat::getValCheck(bdat, name.format("CustomParam%02d", i + 1).c_str(), personality, 1);
      unknownf5c |= 1 << voice;
    }
  }
}

const CharacterParameter *BattleCharacter::GetCharacterParameterConst(bool) const {
  if (unknown168 == nullptr) {
    if (unknownf6a < 0) {
      return nullptr;
    }
    unknown168 = DataManager::get()->GetCharacterParameterConst(unknownf6a);
  }
  return unknown168;
}

bool BattleCharacter::IsControlCharacter() const {
  if (!unknownf8c_1) {
    bool isControl = false;
    if (m_player) {
      gf::GF_OBJ_HANDLE *control = Utility::GetControlCharacterHandle();
      isControl = unknown118 == control;
    }
    unknownf8c_0 = isControl;
    unknownf8c_1 = true;
    return isControl;
  }
  return unknownf8c_0;
}

bool BattleCharacter::IsControlCharacterGroup() const {
  if (unknownf8c_3) {
    return unknownf8c_2;
  }

  bool isGroup = false;
  if (IsControlCharacter()) {
    isGroup = true;
  } else {
    const CharacterParameter *parameter = GetCharacterParameterConst(true);
    int party = parameter != nullptr ? parameter->unknown1c : -1;
    if (party >= 0) {
      gf::GF_OBJ_HANDLE *controlHandle = Utility::GetControlCharacterHandle();
      const BattleCharacter *control = CharacterManager::get()->GetCharacterConst(controlHandle);
      if (control != nullptr) {
        const CharacterParameter *controlParameter = control->GetCharacterParameterConst(true);
        isGroup = controlParameter != nullptr && party == controlParameter->unknown1c;
      }
    }
  }
  unknownf8c_2 = isGroup;
  unknownf8c_3 = true;
  return isGroup;
}

void BattleCharacter::SetCollisionPriority(int priority) {
  if (priority < 0) {
    return;
  }

  gf::GfObj *obj = gf::GfObjUtil::getObj(unknown118);
  gf::GfComCollider *collider = obj->getComponent<gf::GfComCollider>();
  if (collider != nullptr) {
    fw::ColiObject *coli = collider->unknown18[0];
    if (coli != nullptr) {
      s16 oldPriority = coli->getPriority();
      coli->setPriority(priority);
      if (unknownf6c == COLLISION_PRIORITY_UNKNOWN_8AD0) {
        unknownf6c = oldPriority;
      }
    }
  }
  int oldObjectPriority = obj->unknown100->unknownb0;
  obj->unknown100->unknownb0 = priority;
  if (unknownf6e == COLLISION_PRIORITY_UNKNOWN_8AD0) {
    unknownf6e = oldObjectPriority;
  }
}

void BattleCharacter::SetChrCollisionFlag(bool isEnable) {
  if (unknownf90_4 != isEnable) {
    unknownf90_4 = isEnable;
    unknownf90_5 = true;
    WriteBackChrCollisionFlag();
  }
}

void BattleCharacter::WriteBackChrCollisionFlag() {
  if (!unknownf90_5) {
    return;
  }

  gf::GfComCollider *collider = gf::GfObjUtil::getObj(unknown118)->getComponent<gf::GfComCollider>();
  if (collider == nullptr) {
    return;
  }

  fw::ColiObject *coli = collider->unknown18[0];
  if (coli == nullptr) {
    return;
  }

  if (!unknownf90_4) {
    coli->clearState(fw::COLI_STATE_ENABLE);
  } else {
    if (IsDead(0)) {
      return;
    }
    coli->setState(fw::COLI_STATE_ENABLE);
  }
  unknownf90_5 = false;
}

void BattleCharacter::SetWeaponType(int weaponType) {
  unknownf7f = weaponType;
  gf::GfComAsm *comAsm = GetObject()->getComponent<gf::GfComAsm>();
  if (comAsm != nullptr) {
    comAsm->setVariable("V_weaponselect", static_cast<int>(gf::GfDataWeapon::getAsmWeaponType(unknownf7f)));
  }
  u8 *bdat = gf::GfDataBdat::getFP(gf::BDAT_ITM_PC_WPN_TYPE);
  unknownf8b_0 = Bdat::getValCheck(bdat, "VoiceType", weaponType, 1);
}

bool BattleCharacter::IsOnSlide() const {
  if (!m_player) {
    return false;
  }

  return GetObject()->getComponent<gf::GfComPropertyPc>()->unknown110_7;
}

void BattleCharacter::TurnObject(gf::GF_OBJ_HANDLE *handle, bool a, bool b) {
  if (handle == gf::INVALID_HANDLE) {
    handle = unknown120;
  }

  mm::Vec3 position;
  if (Misc::GetObjectTransform(handle, position, Misc::TRANS_TYPE_POSITION)) {
    unknownf8c_5 = false;
    mm::Vec3 direction;
    if (Misc::CalcDirectionXZ(unknown10, position, direction)) {
      TurnDirection(direction, a, b);
    }
  }
}

void BattleCharacter::TurnTarget(bool isSecond, bool a, bool b) {
  gf::GF_OBJ_HANDLE *target = isSecond ? unknown128 : unknown120;
  if (target == gf::INVALID_HANDLE) {
    target = unknown120;
  }

  mm::Vec3 position;
  if (Misc::GetObjectTransform(target, position, Misc::TRANS_TYPE_POSITION)) {
    unknownf8c_5 = false;
    mm::Vec3 direction;
    if (Misc::CalcDirectionXZ(unknown10, position, direction)) {
      TurnDirection(direction, a, b);
    }
  }
}

bool BattleCharacter::AI_Warp(ACTION_ID action) {
  if (IsControlCharacter()) {
    return false;
  }

  mm::Vec3 position;
  bool isDirect;
  if (!CheckWarp(action, position, isDirect)) {
    unknown11de = 0;
    return false;
  }

  const CharacterParameter *parameter = GetCharacterParameterConst(true);
  if (parameter != nullptr && parameter->m_invalidTarget) {
    unknown11de++;
    if (unknown11de < 5) {
      return false;
    }
    unknown11de = 0;
  }
  SetWarp(position, isDirect, 2.0f);
  return true;
}

void BattleCharacter::ActionRun() {
  if (unknown107e == 0) {
    if (!unknownf8f_7 && (!m_player || !IsControlCharacter())) {
      unknownf8e_2 = true;
    }
    unknown107e = 1;
  }
  unknown1080 = 0;
}

void BattleCharacter::LockTarget() {
  if (IsControlCharacter()) {
    BattleGlobal::get()->unknown65de_6 = true;
  }
}

void BattleCharacter::ActionCommandEndBladeArts() {
  if (unknown107e == ACTION_PHASE_BLADE_ARTS_MOVE && fabsf(unknownb0.x) <= NORMALIZE_LENGTH_MIN && fabsf(unknownb0.y) <= NORMALIZE_LENGTH_MIN && fabsf(unknownb0.z) <= NORMALIZE_LENGTH_MIN &&
      unknownf7c != 1 && unknownf7c != 0x15) {
    SetAnimation(static_cast<ANIM_ID>(1), true, 0, 0, true, true);
    unknownf91_3 = false;
    unknownf91_4 = false;
  }
}

void BattleCharacter::SetExitBattle(bool a, bool b) {
  unknownf8e_5 = a;
  unknownf8e_6 = b;
  if (b && IsControlCharacter()) {
    BattleGlobal::get()->unknown65e0_7 = true;
  }
}

bool BattleCharacter::IsInputButtonDirect(gf::pc::InputID input) const {
  if (!m_player) {
    return false;
  }

  gf::GF_OBJ_HANDLE *handle = ChainAttackManager::get()->IsProc(false) ? Utility::GetLeader() : unknown118;
  return gf::GfObjUtil::getObj(handle)->getComponent<gf::GfComPropertyPc>()->unknown60.unknown08.test(input);
}

inline bool BattleCharacter::IsBanOperate() const {
  if (unknownf8e_4) {
    return true;
  }
  if (unknownf82 != 0 && !ChainAttackManager::get()->IsSelectAction()) {
    return true;
  }
  if (BattleGlobal::get()->unknown5b38.get() != nullptr || BattleGlobal::get()->unknown65df_0 || BattleGlobal::get()->unknown5b00.get() != nullptr) {
    return true;
  }
  if (SpAttackManager::get()->unknown08.get() != nullptr) {
    return true;
  }
  return UIManager::get()->unknown45_1;
}

bool BattleCharacter::IsInputButton(gf::pc::InputID input) const {
  if (!IsControlCharacter() || !BattleGlobal::get()->IsEnableBattle()) {
    return false;
  }

  bool isEnable = !IsBanOperate();
  if (((input & ~1) == gf::pc::INPUT_ID_REQUEST_PERMIT_01 || isEnable) && (!ChainAttackManager::get()->IsProc(false) || ChainAttackManager::get()->IsSelectAction())) {
    return IsInputButtonDirect(input);
  }
  return false;
}

bool BattleCharacter::IsInputStick(mm::Vec3 *stick, bool isCamera) const {
  if (IsBanOperate() || !IsControlCharacter() || !m_player) {
    return false;
  }

  gf::GfComPropertyPc *property = GetObject()->getComponent<gf::GfComPropertyPc>();
  if (!property->existInputXY()) {
    return false;
  }
  if (stick == nullptr) {
    return true;
  }

  f32 x = property->unknown60.unknown00;
  f32 y = property->unknown60.unknown04;
  mm::Vec3 input;
  if (isCamera) {
    input = property->unknown2b0 * y + property->unknown2d0 * x;
    if (fabsf(input.x) <= NORMALIZE_LENGTH_MIN && fabsf(input.y) <= NORMALIZE_LENGTH_MIN && fabsf(input.z) <= NORMALIZE_LENGTH_MIN) {
      input = property->unknown1e0;
    }
  } else {
    input = mm::Vec3(x, y, 0.0f);
  }
  *stick = input;
  return true;
}

void BattleCharacter::SetWarpPostureByDir(const mm::Vec3 &direction) {
  const mm::Vec3 &axis = mm::Vec3::unitZ;
  f32 dot = axis.x * direction.x + axis.y * direction.y + axis.z * direction.z;
  if (!(dot < -0.9999f)) {
    f32 x = axis.y * direction.z - direction.y * axis.z;
    f32 y = direction.x * axis.z - axis.x * direction.z;
    f32 z = axis.x * direction.y - direction.x * axis.y;
    f32 value = dot + 1.0f;
    value = value + value;
    f32 length = sqrtf(value);
    f32 scale = 1.0f / length;
    unknowna0.x = x * scale;
    unknowna0.y = y * scale;
    unknowna0.z = z * scale;
    unknowna0.w = length * 0.5f;
  } else {
    unknowna0.x = 0.0f;
    unknowna0.y = 1.0f;
    unknowna0.z = 0.0f;
    unknowna0.w = 0.0f;
  }
  unknownf8d_7 = true;
}

void BattleCharacter::NotifyBuddyWarp() {
  const CharacterParameter *parameter = GetCharacterParameterConst(true);
  if (parameter != nullptr && parameter->m_invalidTarget) {
    unknownf8e_0 = true;
  }
}

void BattleCharacter::SetPosition(const mm::Vec3 &position, bool isDirect) {
  unknown20 = position;
  unknownf8d_5 = true;
  unknownf8d_6 = isDirect;
  unknownd0 = mm::Vec3::zero;
  unknowne0 = 1.0f;
  unknowne4 = 1.0f;
  const CharacterParameter *parameter = GetCharacterParameterConst(true);
  if (parameter != nullptr) {
    gf::GF_OBJ_HANDLE *buddyHandle = parameter->unknown10;
    BattleCharacter *buddy = CharacterManager::get()->GetCharacterConst(buddyHandle);
    if (buddy != nullptr) {
      buddy->NotifyBuddyWarp();
    }
  }
}

void BattleCharacter::StopEnd() {
  unknownccc = 0.0f;
  unknownf93_6 = false;
}

bool BattleCharacter::StartBackHome() {
  unknownf8f_1 = true;
  return true;
}

void BattleCharacter::SetUpperCollisionPriority(int priority) {
  int value = priority + 0x50;
  const CharacterParameter *parameter = GetCharacterParameterConst(true);
  if (parameter != nullptr && parameter->unknown1c >= 0) {
    value = value - parameter->unknown1c * 2 + parameter->m_invalidTarget;
  }
  SetCollisionPriority(value);
}

bool BattleCharacter::IsPlayer() const { return m_player; }

bool BattleCharacter::IsBlade() const {
  const CharacterParameter *parameter = GetCharacterParameterConst(true);
  return parameter != nullptr && parameter->m_invalidTarget;
}

gf::GfObj *BattleCharacter::GetObject() const { return gf::GfObjUtil::getObj(unknown118); }

bool BattleCharacter::IsEnemy() const { return !m_player; }

bool BattleCharacter::IsDriver() const {
  const CharacterParameter *parameter = GetCharacterParameterConst(true);
  return parameter != nullptr && !parameter->m_invalidTarget && parameter->unknown10 != gf::INVALID_HANDLE;
}

CharacterParameter *BattleCharacter::GetCharacterParameter(bool isBattle) { return const_cast<CharacterParameter *>(GetCharacterParameterConst(isBattle)); }

gf::GF_OBJ_HANDLE *BattleCharacter::GetControlCharacterHandle() const { return Utility::GetControlCharacterHandle(); }

gf::GfObj *BattleCharacter::GetObject(gf::GF_OBJ_HANDLE *handle) const { return gf::GfObjUtil::getObj(handle); }

bool BattleCharacter::IsBattle() const { return m_battle; }

void BattleCharacter::ResetAnimationMoveDistance() {
  for (int i = 0; i < 20; i++) {
    unknownce4[i].unknown00 = ANIMATION_INVALID;
  }
}

void BattleCharacter::ResetAlphaAnimation() {
  for (int i = 0; i < 17; i++) {
    unknowndd8[i].unknown10 = 0.0f;
    unknowndd8[i].unknown0c = 0.0f;
    unknowndd8[i].unknown04 = -1.0f;
    unknowndd8[i].unknown08 = -1.0f;
    unknowndd8[i].unknown00 = -1.0f;
  }
}

void BattleCharacter::InitializeEnhanceProcData(bool isFinalizeArtsEffect) {
  for (f32 &timer : unknown1108) {
    timer = 0.0f;
  }
  if (isFinalizeArtsEffect) {
    FinalizeArtsEffect(true);
  }
}

void BattleCharacter::DeleteKizunaAuraEffect() {
  if (unknown130[EFFECT_ID_KIZUNA_AURA] != 0) {
    if (gf::GfGameManager::isPlayEvent()) {
      fw::EffectManager::get()->remove(unknown130[EFFECT_ID_KIZUNA_AURA]);
    } else {
      fw::EffectManager::get()->dead(unknown130[EFFECT_ID_KIZUNA_AURA]);
    }
    unknown130[EFFECT_ID_KIZUNA_AURA] = 0;
  }
}

void BattleCharacter::DeleteKizunaLinkEffect() {
  if (unknown130[EFFECT_ID_KIZUNA_LINK] != 0) {
    fw::EffectManager::get()->destroyKizunaLine(unknown130[EFFECT_ID_KIZUNA_LINK]);
    unknown130[EFFECT_ID_KIZUNA_LINK] = 0;
    PlaySE(BTL_SE_HIT, 0x25, nullptr, true);
  }
  unknownf7a &= 0xfffe;
  const CharacterParameter *parameter = GetCharacterParameterConst(true);
  if (parameter == nullptr) {
    return;
  }
  gf::GF_OBJ_HANDLE *handle = parameter->unknown10;
  BattleCharacter *character = CharacterManager::get()->GetCharacterConst(handle);
  if (character != nullptr) {
    character->unknownf7a &= 0xfffe;
  }
}

void BattleCharacter::DeleteEffect(int effect, int fadeFrame) {
  if (effect == EFFECT_ID_KIZUNA_LINK) {
    DeleteKizunaLinkEffect();
    return;
  }
  if (unknown130[effect] != 0) {
    if (gf::GfGameManager::isPlayEvent()) {
      fadeFrame = -1;
    }
    fw::EffectManager *manager = fw::EffectManager::get();
    switch (fadeFrame) {
    case -1:
      manager->remove(unknown130[effect]);
      break;
    case 0:
      manager->dead(unknown130[effect]);
      break;
    default:
      manager->fadeOutDead(unknown130[effect], fadeFrame);
      break;
    }
    unknown130[effect] = 0;
  }
}

int BattleCharacter::GetBuffEffectIndex(int buff) const {
  if (Utility::IsBladeArtsBuff(buff)) {
    return IsBlade() ? -1 : 7;
  }
  switch (buff) {
  case 0xb:
    return IsBlade() ? -1 : 8;
  case 0xc:
    return IsBlade() ? -1 : 9;
  case 0xd:
    return IsBlade() ? -1 : 10;
  case 0xe:
    return IsBlade() ? 10 : -1;
  case 0xf:
    return IsBlade() ? -1 : 11;
  case 0x10:
    return IsBlade() ? -1 : 12;
  case 0x11:
    return IsBlade() ? -1 : 7;
  default:
    return -1;
  }
}

void BattleCharacter::StopStart(f32 time, bool isSyncBuddy) {
  unknown001(time);
  if (!isSyncBuddy) {
    return;
  }
  if (!SpAttackManager::get()->IsRegister(unknown118)) {
    return;
  }
  if (unknown107c == 10) {
    const CharacterParameter *parameter = GetCharacterParameterConst(true);
    if (parameter == nullptr) {
      return;
    }
    gf::GF_OBJ_HANDLE *handle = parameter->unknown10;
    BattleCharacter *buddy = CharacterManager::get()->GetCharacterConst(handle);
    if (buddy != nullptr) {
      buddy->unknown001(time);
    }
  }
}

bool BattleCharacter::CheckPowerCharge(u32 attack) {
  const CharacterParameter *parameter = GetCharacterParameterConst(true);
  if (parameter == nullptr) {
    return false;
  }
  if (!parameter->m_invalidTarget && parameter->unknown10 != gf::INVALID_HANDLE && parameter->IsBuff(3)) {
    f32 rate = parameter->GetBuffEffect(3) * 0.01f + 1.0f;
    BattleGlobal::get()->MulDamageRate(attack, rate, false);
    if (parameter->IsBuffInfinite(3)) {
      const_cast<CharacterParameter *>(parameter)->ClearBuff(3, false);
    }
    return true;
  }
  return false;
}

void BattleCharacter::AddTrust(u32 point, int level) {
  const CharacterParameter *parameter = GetCharacterParameterConst(true);
  if (parameter == nullptr) {
    return;
  }
  int partyId = parameter->unknown1c;
  if (DataManager::get()->unknown001(partyId)) {
    const GROUP_BLADE_DATA *blade = DataManager::get()->unknown10758[partyId].GetBladeDataConst(-1);
    u16 bladeId = blade->unknown24;
    if (static_cast<u32>(bladeId - 1001) < 1000) {
      if (level >= 0) {
        const CharacterParameter *levelParameter = GetCharacterParameterConst(true);
        if (levelParameter == nullptr) {
          return;
        }
        if (level < static_cast<int>(levelParameter->GetLevel()) - BtlSystemBalanceAccess::get()->unknownea) {
          return;
        }
      }
      gf::SCOM_ADD_TRUST::create(bladeId, point);
    }
  }
}

bool BattleCharacter::GetTargetDirection(mm::Vec3 &direction, bool flag1, bool flag2, gf::GF_OBJ_HANDLE *target) const {
  if (target == gf::INVALID_HANDLE) {
    target = unknown120;
  }
  mm::Vec3 position;
  bool result = false;
  if (Misc::GetObjectTransform(target, position, Misc::TRANS_TYPE_POSITION)) {
    result = GetTargetPosDirection(position, direction, flag1, flag2);
  }
  return result;
}

static const CharacterParameter *unknown002(const BattleCharacter *character) { // Inferred role: gets the cached parameter without checking the id; original function name unknown.
  if (character->unknown168 == nullptr) {
    character->unknown168 = DataManager::get()->GetCharacterParameterConst(character->unknownf6a);
  }
  return character->unknown168;
}

static const char *const unknown007[] = {"S", "M", "L", "LL"}; // Inferred role: size class names of the reaction bdat columns; original name unknown.

bool BattleCharacter::GetReactionBlowParam(int reaction, int level, f32 &horizontal, f32 &vertical) const {
  int sizeClass = unknown002(this)->unknown5a8_0;
  if (sizeClass > 2) {
    sizeClass = 3;
  }
  mm::mtl::FixStr<32> name;
  name.format("PowLv%d_%s", level, unknown007[sizeClass]);
  u8 *bdat = gf::GfDataBdat::getFP(gf::BDAT_BTL_REACTION);
  u32 percent = static_cast<u16>(Bdat::getValCheck(bdat, name.c_str(), reaction, 2));
  f32 rate = 1.0f;
  if (percent != 0) {
    rate = percent * 0.01f;
  }
  bool result = true;
  switch (reaction) {
  case 1:
    horizontal = 0.3f;
    vertical = 0.59999996f;
    break;
  case 2:
    horizontal = 0.1f;
    vertical = 4.5f;
    break;
  case 4:
    horizontal = 0.0f;
    vertical = 1.05f;
    break;
  case 5:
    horizontal = 0.0f;
    vertical = 6.0f;
    break;
  case 6:
    horizontal = 0.0f;
    vertical = -9.0f;
    break;
  default:
    result = false;
    horizontal = 0.0f;
    vertical = 0.0f;
    break;
  }
  horizontal = rate * horizontal;
  vertical = rate * vertical;
  return result;
}

bool BattleCharacter::SetBlow(const mm::Vec3 &direction, f32 horizontalRate, f32 verticalRate, f32 time) {
  mm::Vec3 blow = direction;
  if (blow.y != 0.0f) {
    Misc::NormalizeXZ(blow);
  }
  blow.x = blow.x * horizontalRate;
  blow.z = blow.z * horizontalRate;
  bool isLand = false;
  if (verticalRate != 0.0f) {
    f32 landHeight = GetLandHeight();
    f32 rate;
    if (unknown10.y < landHeight + 2.7f) {
      rate = 1.0f;
    } else if (unknown10.y >= landHeight + 10.0f) {
      rate = 0.1f;
    } else {
      rate = ((10.0f - (unknown10.y - landHeight)) * 0.11986301f + 0.125f) * 0.9f;
    }
    blow.y = rate * verticalRate;
    if (blow.y < 0.0f) {
      isLand = unknownf90_1;
    }
  }
  if (blow.y > 0.0f) {
    if (unknownd0.y > blow.y) {
      blow.y = unknownd0.y;
    }
  } else if (unknownd0.y < blow.y) {
    blow.y = unknownd0.y;
  }
  unknownd0 = blow;
  f32 blowTime = 0.38f;
  if (0.0f <= time) {
    blowTime = time;
  }
  unknownd98 = blowTime;
  unknownf91_0 = false;
  if (isLand) {
    OnLandCallback(true, true);
  }
  if (blow.y > 0.0f) {
    f32 height = GetLandHeight() + BtlSystemBalanceAccess::get()->unknowna6 * 0.01f;
    if (height >= unknown110) {
      unknown110 = height;
      unknown114 = 3.0f;
    }
  }
  return true;
}

void BattleCharacter::WarpMirrorCopy() {
  gf::GF_OBJ_HANDLE *buddyHandle = unknown002(this)->unknown10;
  BattleCharacter *buddy = CharacterManager::get()->GetCharacterConst(buddyHandle);
  mm::Vec3 position;
  Misc::GetObjectTransform(buddyHandle, position, Misc::TRANS_TYPE_POSITION);
  SetPosition(position, true);
  SetWarpPostureByDir(buddy->unknown60);
}

bool BattleCharacter::IsEnableRecastSpAttack() const {
  const CharacterParameter *parameter = GetCharacterParameterConst(true);
  if (parameter == nullptr || !parameter->m_invalidTarget) {
    return false;
  }

  const ARTS_SLOT_DATA *slot = parameter->GetArtsPaletteDataConst(-1)->GetSlotDataConst(SP_ATTACK_SLOT, false);
  if (slot->unknown00_0 == 0) {
    return false;
  }

  const CharacterParameter *buddy = GetCharacterParameterConst(true);
  if (buddy == nullptr || slot->unknown03_7) {
    return false;
  }
  if (m_player && !IsControlCharacterGroup()) {
    return false;
  }
  if (BladeChangeManager::get()->IsRegister(unknown118)) {
    return false;
  }
  if (ChainAttackManager::get()->IsProc(true)) {
    return false;
  }

  if (unknowncc8 == KAKUSEI_BDAT_ID_UNKNOWN_3EA) {
    if (!BattleGlobal::get()->unknown65df_1 && !IsControlCharacterGroup()) {
      return false;
    }
  }
  if (unknowncc8 == KAKUSEI_BDAT_ID_UNKNOWN_3EB && !Utility::IsSatisfyCondition(CONDITION_UNKNOWN_F)) {
    return false;
  }

  if (!Utility::IsThirdCharacter(unknown118)) {
    const CharacterParameter *third = GetCharacterParameterConst(true);
    if (third == nullptr) {
      return false;
    }
    int partyId = third->unknown1c;
    if (!DataManager::get()->unknown001(partyId)) {
      return false;
    }
    if (DataManager::get()->unknown10758[partyId].GetBladeDataConst(-1)->GetKizunaStage(nullptr) != KIZUNA_STAGE_MAX) {
      return false;
    }
  }

  int partyId = buddy->unknown1c;
  if (partyId >= 0 && DataManager::get()->unknown10758[partyId].unknown105_0) {
    return true;
  }
  return false;
}

void BattleCharacter::ArtsGravityProc(const ARTS_DATA &arts, int level) {
  if (arts.unknown20 == 0) {
    return;
  }

  unknownf92_1 = false;
  f32 frame = 0.0f;
  gf::GfComAsm *comAsm = GetObject()->getComponent<gf::GfComAsm>();
  if (comAsm != nullptr) {
    frame = comAsm->getLocalTime(gf::ANIM_LAYER_BASE) * 30.0f;
  }
  u32 gravity = arts.unknown20;
  int endLevel = gravity >> 16 & 7;
  if (endLevel < level || (endLevel == level && frame >= static_cast<f32>(gravity >> 19))) {
    unknownf90_7 = true;
  } else {
    int startLevel = gravity & 7;
    if (level <= startLevel) {
      if (startLevel != level) {
        return;
      }
      if (!(frame >= static_cast<f32>(gravity >> 3 & 0x1fff))) {
        return;
      }
    }
    unknownf90_7 = false;
  }
}

// Inferred role: turns to the direction of a move command, then starts moving; original function name unknown.
static void unknown003(BattleCharacter *character, const ActionCommand *command) {
  mm::Vec3 direction;
  static_cast<const act_cmd::MOVE_DIR_RATE *>(command)->GetDirection(direction);
  character->TurnDirection(direction, false, true);
  character->SetAction(BattleCharacter::ACTION_ID_MOVE, false, false, 0, 0.0f);
}

// Inferred role: turns to the target position when allowed, then starts moving; original function name unknown.
static void unknown004(BattleCharacter *character, bool isTurnTarget) {
  mm::Vec3 position;
  mm::Vec3 direction;
  if (isTurnTarget && Misc::GetObjectTransform(character->unknown120, position, Misc::TRANS_TYPE_POSITION)) {
    character->unknownf8c_5 = false;
    if (Misc::CalcDirectionXZ(character->unknown10, position, direction)) {
      character->TurnDirection(direction, false, true);
    }
  }
  character->SetAction(BattleCharacter::ACTION_ID_MOVE, false, false, 0, 0.0f);
}

// Inferred role: gets the sound component of a character; original function name unknown.
static gf::GfComSound *unknown005(const BattleCharacter *character) { return gf::GfObjUtil::getObj(character->unknown118)->getComponent<gf::GfComSound>(); }

// Inferred role: makes the name of a battle sound effect; original function name unknown.
static mm::mtl::FixStr<32> unknown006(const char *tag, int number) {
  mm::mtl::FixStr<32> name;
  name.format("btl_%s_%03d", tag, number);
  return name;
}

void BattleCharacter::UpdateEffectPositionScale(int effect, const char *node, f32 scale) {
  if (unknown130[effect] == 0) {
    return;
  }

  fw::EffectManager *manager = fw::EffectManager::get();
  if (!manager->isExist(unknown130[effect])) {
    unknown130[effect] = 0;
    return;
  }

  mm::Vec3 position;
  if (node == reinterpret_cast<const char *>(-1)) {
    position = unknown10;
  } else {
    if (node == nullptr) {
      node = unknownc30;
    }
    if (!Misc::GetNodePosition(unknown118, node, position)) {
      position = unknown10;
      position.y += unknown002(this)->unknown38;
    }
  }
  if (scale <= 0.0f) {
    scale = unknown002(this)->unknown28;
  }
  mm::Vec3 scaleVector(scale, scale, scale);
  manager->setParam(unknown130[effect], position, scaleVector);
}

void BattleCharacter::CalcLocomotionMoveRate(f32 maxRate) {
  f32 length = GetAnimationMoveLength(0.0f, 1.0f, true);
  if (length <= 0.0f) {
    return;
  }
  mm::Vec3 position;
  gf::GF_OBJ_HANDLE *target = unknown128;
  if (GetObjectNearPosition(unknown118, target, position, nullptr, nullptr)) {
    f32 distance = Misc::CalcLengthXZ(unknown10, position);
    f32 rate = (distance - unknown002(this)->unknown2c) / length;
    unknowncdc = mm::clamp(rate, 0.15f, maxRate < 0.0f ? 1.0f : maxRate);
    unknownce0 = 1.0f;
  }
}

void BattleCharacter::DispEnhanceName(const CharacterParameter::ENHANCE_DATA &data) const {
  u16 name = data.unknown06;
  if ((name & 0xff) != 0) {
    u32 flag = (name >> 8) & 1;
    f32 x = unknown10.x;
    f32 y = unknown10.y;
    f32 z = unknown10.z;
    y += unknown002(this)->unknown38;
    gf::GfMenuObjDamageBuff::OpenParam param;
    param.unknown00 = flag ^ 1;
    param.unknown10 = gf::GfDataBdat::getFP(0x158);
    param.unknown18 = name & 0xff;
    param.unknown20.x = x;
    param.unknown20.y = y;
    param.unknown20.z = z;
    gf::GfMenuObjDamageBuff::open(param);
  }
}

f32 BattleCharacter::GetGuardRate() const {
  const CharacterParameter *parameter = GetCharacterParameterConst(true);
  u32 guardPercent = parameter->GetGuardPercent();
  f32 guardRate = static_cast<f32>(guardPercent) * 0.01f;
  f32 hpRate = parameter->GetHPRate();
  int last;
  if (hpRate >= 1.0f) {
    last = 0x79;
  } else {
    last = 0x82 - static_cast<int>(hpRate * 10.0f);
  }
  f32 bonus = 0.0f;
  if (last >= 0x79) {
    for (int enhance = 0x79; enhance <= last; enhance++) {
      CharacterParameter::ENHANCE_DATA data;
      parameter->GetEnhanceEffect(data, enhance);
      bonus += data.unknown00;
    }
  }
  f32 rate = guardRate + bonus;

  f32 result = rate;
  if (IsDriver()) {
    gf::GF_OBJ_HANDLE *buddyHandle = parameter->unknown10;
    const BattleCharacter *buddy = CharacterManager::get()->GetCharacterConst(buddyHandle);
    result = 0.0f;
    if (!parameter->unknown5df_6 && !parameter->IsBuff(0xe) && buddy != nullptr && buddy->m_battle) {
      DataManager *dataManager = DataManager::get();
      f32 half = rate * 0.5f;
      rate = dataManager->unknown10758[parameter->unknown1c].unknown105_0 ? rate : half;
      result = rate;
      if (BladeArtsManager::get()->IsBladeAttack(unknown118)) {
        result = rate + 0.5f;
      }
    }
  }
  return result;
}

f32 BattleCharacter::GetHealerEnhanceRate(HEALER_ENHANCE_PARAM &param) const {
  const CharacterParameter *parameter = GetCharacterParameterConst(true);
  f32 total = 0.0f;
  for (int i = 0; i < 2; i++) {
    f32 rate = 0.0f;
    switch (i) {
    case 0:
      if (param.unknown08) {
        CharacterParameter::ENHANCE_DATA data;
        if (parameter->GetEnhanceEffect(data, 0xac)) {
          rate = data.unknown00;
          DispEnhanceName(data);
        }
      }
      break;
    }
    total += rate;
  }
  return total;
}

f32 BattleCharacter::GetHealTargetEnhanceRate(HEAL_TARGET_ENHANCE_PARAM &param) const {
  const CharacterParameter *parameter = GetCharacterParameterConst(true);
  f32 total = 0.0f;
  for (int i = 0; i < 2; i++) {
    f32 rate = 0.0f;
    switch (i) {
    case 0: {
      int enhance = !param.unknown08 ? 0xad : 0x108;
      CharacterParameter::ENHANCE_DATA data;
      if (parameter->GetEnhanceEffect(data, enhance)) {
        rate = data.unknown00;
        DispEnhanceName(data);
      }
      break;
    }
    }
    total += rate;
  }
  total += parameter->GetSpecialEnhanceRate(CharacterParameter::SPECIAL_ENHANCE_TYPE_UNKNOWN_5, nullptr);
  if (IsPlayer()) {
    int partyId = parameter->unknown1c;
    f32 pouch = DataManager::get()->GetPouchBuffParam(DataManager::POUCH_BUFF_TYPE_UNKNOWN_1, partyId);
    total += pouch * 0.01f;
  }
  return total;
}

void BattleCharacter::DispEnhanceNameDirect(int text, bool flag) const {
  if (text != 0) {
    f32 x = unknown10.x;
    f32 y = unknown10.y;
    f32 z = unknown10.z;
    y += unknown002(this)->unknown38;
    gf::GfMenuObjDamageBuff::OpenParam param;
    param.unknown00 = !flag;
    param.unknown10 = gf::GfDataBdat::getFP(0x158);
    param.unknown18 = text;
    param.unknown20.x = x;
    param.unknown20.y = y;
    param.unknown20.z = z;
    gf::GfMenuObjDamageBuff::open(param);
  }
}

int BattleCharacter::GetArtsAttr(const ARTS_DATA &data, u32 attack) const {
  if (attack != 0) {
    int attribute = BattleGlobal::get()->GetAttackAttr(attack);
    if (attribute >= 0) {
      return attribute;
    }
  }
  int attribute = data.unknown0a_3;
  if (attribute == 0) {
    const CharacterParameter *parameter = GetCharacterParameterConst(true);
    if (parameter == nullptr) {
      return 0;
    }
    attribute = parameter->GetAttribute();
  }
  return attribute;
}

bool BattleCharacter::IsNoDamage(bool) const {
  if (unknown10e0 > 0.0f) {
    return true;
  }
  const CharacterParameter *parameter = unknown002(this);
  if (parameter->m_perfectAvoid == 10 || parameter->m_perfectAvoid == 15) {
    return true;
  }
  if (parameter->IsInvalidTarget()) {
    return true;
  }
  BattleGlobal *global = BattleGlobal::get();
  if (m_player) {
    if (global->unknown5b38.get() != nullptr) {
      return true;
    }
    if (global->unknown65df_0 || global->unknown5b00.get() != nullptr) {
      return true;
    }
    if (SpAttackManager::get()->unknown08.get() != nullptr) {
      return true;
    }
    if (ChainAttackManager::get()->IsProc(false)) {
      return true;
    }
  } else if (global->IsGameOver()) {
    return true;
  }
  return false;
}

void BattleCharacter::CancelButtonChallenge() {
  if (unknown10c0 != 0) {
    BattleGlobal::get()->unknown5e8.Cancel(unknown10c0);
    unknown10c0 = 0;
  }
}

bool BattleCharacter::IsBound() const {
  if (!unknownf90_3 && !Misc::IsInWater(unknown10, 5.0f)) {
    return ((1ull << unknownf7c) & 0xe00000000ull) != 0;
  }
  return false;
}

void BattleCharacter::SetRimBloom(RIMBLOOM_ID id, bool isEnable) {
  if (id == 0) {
    if (isEnable) {
      unknownf7a |= 1;
    } else {
      unknownf7a &= 0xfffe;
    }
  } else if (static_cast<u32>(id - 1) < 8) {
    unknownf7a &= 0xfe01;
    if (isEnable) {
      unknownf7a |= 1u << id;
    }
  }
}

bool BattleCharacter::IsDispWeaponAlpha(int alpha) const {
  if (alpha >= ALPHA_WEAPON_FIRST && alpha <= ALPHA_WEAPON_LAST) {
    f32 rate = unknowndd8[alpha].unknown08;
    return rate < 0.0f || rate > 0.0f;
  }
  return false;
}

bool BattleCharacter::IsBoss() const {
  const CharacterParameter *parameter = GetCharacterParameterConst(true);
  return parameter != nullptr && parameter->IsBoss();
}

bool BattleCharacter::IsNamed() const {
  const CharacterParameter *parameter = GetCharacterParameterConst(true);
  return parameter != nullptr && parameter->IsNamed();
}

bool BattleCharacter::IsBossOrNamed() const {
  const CharacterParameter *parameter = GetCharacterParameterConst(true);
  return parameter != nullptr && parameter->IsBossOrNamed();
}

bool BattleCharacter::IsMove() const { return unknown107c == ACTION_ID_MOVE || unknown1178 == AI_STATE_ACTIVE; }

bool BattleCharacter::IsMoveHealPot() const {
  if (unknown107c == ACTION_ID_MOVE && unknown1178 == AI_STATE_ACTIVE && unknown1160.unknown00 == 20) {
    return true;
  }
  return false;
}

int BattleCharacter::GetSeType(int type, int attr) const {
  int seType = 1;
  switch (type) {
  case 60000:
    break;
  case 60001:
    seType = 2;
    break;
  case 60002:
    seType = Utility::GetWeaponSeType(unknownf7f);
    break;
  case 60003:
    seType = Utility::GetAttrSeType(attr);
    break;
  case 60004:
    seType = unknownf78_0;
    break;
  case 0:
    return 0;
  default:
    seType = type;
    break;
  }
  return seType;
}

int BattleCharacter::GetKizunaStage(f32 *rate) const {
  if (Utility::IsThirdCharacter(unknown118)) {
    return 6;
  }
  if (rate != nullptr) {
    *rate = 0.0f;
  }

  const CharacterParameter *parameter = GetCharacterParameterConst(false);
  if (parameter == nullptr) {
    return 0;
  }
  int partyId = parameter->unknown1c;
  if (!DataManager::get()->unknown001(partyId)) {
    return 3;
  }
  return DataManager::get()->unknown10758[partyId].GetBladeDataConst(-1)->GetKizunaStage(rate);
}

int BattleCharacter::GetHateEnemyNum() const {
  const CharacterParameter *parameter = GetCharacterParameterConst(false);
  if (parameter == nullptr) {
    return 0;
  }
  return HateRingManager::get()->GetHateEnemyNum(parameter->unknown1c);
}

void BattleCharacter::NotifyAngryEnd() {
  if (unknown130[6] == 0) {
    return;
  }
  if (!gf::GfGameManager::isPlayEvent()) {
    fw::EffectManager::get()->fadeOutDead(unknown130[6], 5);
  } else {
    fw::EffectManager::get()->remove(unknown130[6]);
  }
  unknown130[6] = 0;
}

void BattleCharacter::FinishLaunchRotate() {
  if (!unknown10c9_3) {
    return;
  }

  gf::GfObjUtil::setEnableBlowRotateIK(unknown118, false, 0.3f);
  unknown10c9_3 = false;
  unknown10b8 = 0;
  if (unknown130[5] != 0) {
    if (!gf::GfGameManager::isPlayEvent()) {
      fw::EffectManager::get()->dead(unknown130[5]);
    } else {
      fw::EffectManager::get()->remove(unknown130[5]);
    }
    unknown130[5] = 0;
  }
}

void BattleCharacter::CreateBuffEffect(int buff, bool isReset) {
  int index = GetBuffEffectIndex(buff);
  if (index < 0) {
    return;
  }

  fw::EffectManager *manager = fw::EffectManager::get();
  if (unknown130[index] != 0 && manager->isExist(unknown130[index])) {
    if (index != BUFF_EFFECT_UNKNOWN_7 && index != BUFF_EFFECT_TARGET_LOCK) {
      return;
    }
    DeleteEffect(index, 0);
  }

  u8 *bdat = gf::GfDataBdat::getFP(gf::BDAT_BTL_BUFF);
  unknown130[index] = manager->create(reinterpret_cast<const char *>(Bdat::getValCheck(bdat, "Effect", buff, 7)), nullptr, true);
  if (index == BUFF_EFFECT_TARGET_LOCK && isReset) {
    unknown70 = mm::Vec3::zero;
    unknowncd4 = 0.0f;
  }
  if (index != BUFF_EFFECT_TARGET_LOCK) {
    UpdateEffectPositionScale(index, nullptr, -1.0f);
    return;
  }
  UpdateTargetLockArrow();
}

f32 BattleCharacter::GetArtsAttackDistance(const ARTS_DATA &data) const {
  f32 distance = static_cast<u32>(data.unknown1c_0);
  const CharacterParameter *parameter = GetCharacterParameterConst(true);
  if (parameter == nullptr) {
    return distance;
  }

  f32 result = distance;
  if (data.unknown08_5 == 12) {
    CharacterParameter::ENHANCE_DATA enhance;
    if (parameter->GetEnhanceEffect(enhance, 0xc3)) {
      result = distance + enhance.unknown00;
    }
  }
  return result * parameter->unknown28;
}

u8 BattleCharacter::CheckAttackPosition(gf::GF_OBJ_HANDLE *handle, bool isCheck, bool *isInvalid) const {
  const BattleCharacter *target = CharacterManager::get()->GetCharacterConst(handle);
  if (target == nullptr) {
    if (isInvalid != nullptr) {
      *isInvalid = true;
      return 0;
    }
    return 0;
  }

  if (isInvalid != nullptr) {
    *isInvalid = false;
  }
  const CharacterParameter *parameter = target->GetCharacterParameterConst(true);
  if (!isCheck || parameter->m_perfectAvoid != 9) {
    mm::Vec3 direction = target->unknown50;
    mm::Vec3 vector;
    vector.x = unknown10.x - target->unknown10.x;
    vector.y = unknown10.y - target->unknown10.y;
    vector.z = unknown10.z - target->unknown10.z;
    Misc::NormalizeXZ(vector);
    f32 angle = Misc::CalcAngle(direction, vector, true);
    f32 absAngle = fabsf(angle);
    f32 half = parameter->unknown4c * 0.5f;
    if (absAngle <= half) {
      return 0;
    }
    if (absAngle >= 3.1415927f - half) {
      return 3;
    }
    return !(angle >= 0.0f) ? 2 : 1;
  }
  return 3;
}

void BattleCharacter::AchieveCondition(int condition, u32 value) const {
  if (!m_player) {
    return;
  }

  gf::GF_OBJ_HANDLE *const *handle;
  if ((condition == Utility::BTL_ACHIEVE_COND_BLADE_CHANGE) != IsBlade()) {
    const CharacterParameter *parameter = GetCharacterParameterConst(true);
    if (parameter == nullptr || parameter->unknown5df_6) {
      return;
    }
    handle = &parameter->unknown10;
  } else {
    handle = &unknown118;
  }
  Utility::AchieveCondition(static_cast<Utility::BTL_ACHIEVE_COND>(condition), *handle, value);
}

void BattleCharacter::SetAttackTarget(gf::GF_OBJ_HANDLE *handle, bool isNotify) {
  if (unknown10c8 >= 0) {
    BattleCharacter *character = CharacterManager::get()->GetCharacterConst(static_cast<int>(unknown10c8));
    if (character != nullptr) {
      handle = character->unknown118;
    }
    unknown10c8 = -1;
  }

  unknown128 = handle;
  if (handle == gf::INVALID_HANDLE) {
    unknown1101 = 0;
    return;
  }

  u8 position = CheckAttackPosition(handle, true, nullptr);
  unknown1101 = position;
  if (isNotify && position == ATTACK_POSITION_UNKNOWN_3) {
    AchieveCondition(Utility::BTL_ACHIEVE_COND_BACK_ATTACK, 0);
  }
  CheckWeaponDistance(handle);
}

void BattleCharacter::NotifyBuffStart(int buff) {
  CreateBuffEffect(buff, true);
  if (GetBuffEffectIndex(buff) >= 0) {
    u8 *bdat = gf::GfDataBdat::getFP(gf::BDAT_BTL_BUFF);
    if (Bdat::getFlagVal(bdat, "Flag", buff, "SE") != 0) {
      int se = Bdat::getFlagVal(bdat, "Flag", buff, "Buff") != 0 ? 0x35 : 0x36;
      PlaySE(BTL_SE_HIT, se, nullptr, true);
    }
  }
}

void BattleCharacter::ResetHate() {
  if (m_player) {
    return;
  }
  CharacterParameter *characterParameter = GetCharacterParameter(false);
  if (characterParameter == nullptr) {
    return;
  }
  static_cast<EnemyParameter *>(characterParameter)->ResetHate(-1, false);
}

void BattleCharacter::AddUseWeapon() {
  CharacterParameter *parameter = GetCharacterParameter(true);
  if (parameter != nullptr) {
    int weapon = unknownf7f;
    parameter->AddUseWeapon(weapon);
  }
}

void BattleCharacter::StartBladeTimer(BLADE_TIMER_MODE mode) {
  if (unknownf77_1 > mode) {
    return;
  }

  f32 time;
  if (mode == 1) {
    time = 5.0f;
  } else if (mode == 2) {
    time = static_cast<f32>(BtlSystemBalanceAccess::get()->unknownaf);
  } else if (mode == 3) {
    time = static_cast<f32>(BtlSystemBalanceAccess::get()->unknownb0);
  } else {
    time = 0.0f;
  }
  unknowncd0 = time;
  unknownf77_1 = mode;
}

void BattleCharacter::EndBladeTimer(BLADE_TIMER_MODE mode) {
  if (unknownf77_1 == mode) {
    unknowncd0 = 0.0f;
    unknownf77_1 = 0;
  }
}

void BattleCharacter::PlayHitAttrSE(int attribute, const mm::Vec3 *position) {
  if (attribute >= 1 && attribute <= ATTRIBUTE_COUNT) {
    PlaySE(BTL_SE_HIT, attribute + SE_HIT_ATTRIBUTE, position, true);
  }
}

void BattleCharacter::PlayHitBulletSE(int attribute, const mm::Vec3 *position) {
  if (attribute >= 1 && attribute <= ATTRIBUTE_COUNT) {
    PlaySE(BTL_SE_HIT, attribute + SE_HIT_BULLET, position, true);
  }
}

int BattleCharacter::GetIKData(f32 &weight, f32 &height) const {
  int id = unknowndc0_0;
  if (id == 0) {
    weight = 0.0f;
    height = 0.0f;
    return id;
  }
  weight = unknowndb8;
  height = unknowndbc;
  return id;
}

void BattleCharacter::SetupPosDir() {
  unknownf4 = FLT_MAX;
  Misc::GetObjectTransform(unknown118, unknown10, Misc::TRANS_TYPE_POSITION);
  Misc::GetObjectDirection(unknown118, unknown50, true);
}

gf::GF_OBJ_HANDLE *BattleCharacter::GetBuddyTargetObjectHandle() const {
  const CharacterParameter *parameter = GetCharacterParameterConst(false);
  if (parameter == nullptr) {
    return gf::INVALID_HANDLE;
  }

  gf::GF_OBJ_HANDLE *handle = parameter->unknown10;
  BattleCharacter *buddy = CharacterManager::get()->GetCharacterConst(handle);
  return buddy != nullptr ? buddy->unknown120 : gf::INVALID_HANDLE;
}

int BattleCharacter::GetTrustRank(int index) const {
  if (!m_player) {
    return 5;
  }

  const CharacterParameter *parameter = GetCharacterParameterConst(false);
  if (parameter == nullptr) {
    return 0;
  }
  int partyId = parameter->unknown1c;
  if (partyId < 0) {
    return 0;
  }

  const GROUP_DATA *group = &DataManager::get()->unknown10758[partyId];
  if (!group->unknown105_5) {
    return 0;
  }
  return group->GetBladeDataConst(index)->GetTrustRank();
}

bool BattleCharacter::IsPriorityReaction(int left, int right) const {
  if (right < 0) {
    right = unknown10c6_0;
  }
  if (right == left) {
    return static_cast<u32>(left - 1) < 2;
  }

  u8 *table = gf::GfDataBdat::getFP(0x34);
  u8 leftPriority = 0;
  if (left != 0) {
    leftPriority = Bdat::getValCheck(table, "Priority", left, 1);
  }
  u8 rightPriority = 0;
  if (right != 0) {
    rightPriority = Bdat::getValCheck(table, "Priority", right, 1);
  }
  return leftPriority >= rightPriority;
}

int BattleCharacter::GetArtsSlotIndex(int arts) const {
  const CharacterParameter *parameter = GetCharacterParameterConst(false);
  if (parameter == nullptr) {
    return -1;
  }

  const ARTS_PALETTE_DATA *palette = parameter->GetArtsPaletteDataConst(-1);
  int index = -1;
  for (int i = 0; i < 3; i++) {
    bool isNoMatch = palette->GetSlotDataConst(i, false)->unknown00_0 != arts;
    if (isNoMatch) {
      continue;
    }
    index = i;
    break;
  }
  return index;
}

bool BattleCharacter::IsBladeArtsEnd() const {
  const CharacterParameter *parameter = GetCharacterParameterConst(false);
  if (parameter == nullptr) {
    return false;
  }
  if (!parameter->m_invalidTarget && parameter->unknown10 != gf::INVALID_HANDLE) {
    return unknown10c6_5 != 1;
  }
  return false;
}

gf::GF_OBJ_HANDLE *BattleCharacter::GetWeaponObjectHandle(bool isAlternate) const {
  gf::GfWeapon *weapon = GetWeaponInfo();
  if (weapon == nullptr) {
    return gf::INVALID_HANDLE;
  }

  const mm::mtl::FixedArray<gf::GF_OBJ_HANDLE *, 10> &handles = weapon->getCurrentHandles();
  if (isAlternate) {
    gf::GF_OBJ_HANDLE *handle = handles[1];
    if (handle == gf::INVALID_HANDLE) {
      handle = handles[0];
    }
    return handle;
  }
  gf::GF_OBJ_HANDLE *handle = handles[0];
  if (handle == gf::INVALID_HANDLE) {
    handle = handles[1];
  }
  return handle;
}

bool BattleCharacter::IsPerfectAvoid() const {
  if (unknown10e8 > 0.0f) {
    return true;
  }

  const CharacterParameter *parameter = GetCharacterParameterConst(false);
  if (parameter->m_perfectAvoid == 0) {
    return false;
  }
  return (1U << parameter->m_perfectAvoid & 0xc4U) != 0;
}

void BattleCharacter::ResetRequestPermit() {
  CharacterParameter *parameter = GetCharacterParameter(false);
  if (parameter == nullptr) {
    return;
  }
  int partyId = parameter->unknown1c;
  if (partyId < 0) {
    return;
  }

  DataManager *manager = DataManager::get();
  manager->unknown10758[partyId].unknown104_2 = 0;
  if (manager->unknown132a0 == partyId) {
    manager->unknown132a8 = true;
  }
}

gf::GfComProperty *BattleCharacter::GetComPropertyConst() const {
  gf::GfObj *obj = GetObject();
  if (obj == nullptr) {
    return nullptr;
  }
  return obj->getComponent<gf::GfComProperty>();
}

gf::GfComEffectCaster *BattleCharacter::GetEffectCaster() {
  gf::GfObj *obj = GetObject();
  if (obj == nullptr) {
    return nullptr;
  }
  return obj->getComponent<gf::GfComEffectCaster>();
}

gf::GfWeapon *BattleCharacter::GetWeaponInfo() const {
  if (!m_player) {
    return gf::GfMountManager::getGfWeapon(unknown118, gf::GFATTACH_WEAPON);
  }

  const CharacterParameter *parameter = GetCharacterParameterConst(false);
  if (parameter == nullptr) {
    return nullptr;
  }
  return gf::GfGameParty::getHandleWeapon(parameter->unknown1c, -1);
}

gf::GfComEffectCaster *BattleCharacter::GetWeaponEffectCaster() {
  gf::GfWeapon *weapon = GetWeaponInfo();
  if (weapon == nullptr) {
    return nullptr;
  }

  gf::GF_OBJ_HANDLE *handle = weapon->getCurrentHandles()[0];
  if (handle == gf::INVALID_HANDLE) {
    handle = weapon->getCurrentHandles()[1];
  }
  if (handle == gf::INVALID_HANDLE) {
    return nullptr;
  }

  gf::GfObj *obj = gf::GfObjUtil::getObj(handle);
  if (obj == nullptr) {
    return nullptr;
  }
  return obj->getComponent<gf::GfComEffectCaster>();
}

int BattleCharacter::GetDriverType() const {
  if (m_player) {
    return 4;
  }

  const CharacterParameter *parameter = GetCharacterParameterConst(false);
  if (parameter != nullptr && parameter->m_invalidTarget && !parameter->unknown5df_6) {
    BattleCharacter *driver = CharacterManager::get()->GetCharacterConst(parameter->unknown10);
    if (driver != nullptr) {
      return driver->m_driverType;
    }
  }
  return m_driverType;
}

const GROUP_DATA *BattleCharacter::GetGroupDataConst() const {
  const CharacterParameter *parameter = GetCharacterParameterConst(false);
  if (parameter == nullptr) {
    return nullptr;
  }
  int partyId = parameter->unknown1c;
  if (partyId < 0) {
    return nullptr;
  }

  const GROUP_DATA *group = &DataManager::get()->unknown10758[partyId];
  return group->unknown105_5 ? group : nullptr;
}

bool BattleCharacter::IsAnimationEnd() const {
  gf::GfComAsm *comAsm = GetObject()->getComponent<gf::GfComAsm>();
  if (comAsm == nullptr) {
    return true;
  }
  return comAsm->isEnd(gf::ANIM_LAYER_BASE);
}

const void *BattleCharacter::GetAnimationAppendix(const char *name) const {
  gf::GfComAsm *comAsm = GetObject()->getComponent<gf::GfComAsm>();
  if (comAsm == nullptr) {
    return nullptr;
  }
  return comAsm->getAppendix(name);
}

const char *BattleCharacter::GetNowAnimationStateName() const {
  gf::GfObj *obj = GetObject();
  if (obj == nullptr) {
    return "";
  }
  gf::GfComAsm *comAsm = obj->getComponent<gf::GfComAsm>();
  if (comAsm == nullptr) {
    return "";
  }
  return comAsm->getStateName(gf::ANIM_LAYER_BASE);
}

bool BattleCharacter::IsAnimationState(const char *name) const {
  gf::GfObj *obj = GetObject();
  const char *state = "";
  if (obj != nullptr) {
    gf::GfComAsm *comAsm = obj->getComponent<gf::GfComAsm>();
    if (comAsm != nullptr) {
      state = comAsm->getStateName(gf::ANIM_LAYER_BASE);
    }
  }
  return __builtin_strcmp(state + 2, name + 2) == 0;
}

bool BattleCharacter::IsPlayAnimation(ANIM_ID animId, int a, int b) const {
  gf::GfComAsm *comAsm = GetObject()->getComponent<gf::GfComAsm>();
  if (comAsm != nullptr) {
    const char *state = comAsm->getStateName(gf::ANIM_LAYER_BASE);
    const char *name = GetAnimationStateName(animId, a, b);
    return __builtin_strcmp(state + 2, name + 2) == 0;
  }
  return false;
}

bool BattleCharacter::DeleteRegisterCommand(ActionCommand::COMMAND_TYPE type) {
  bool isDeleted = false;
  int count = unknown1078;
  for (int i = 0; i < count; i++) {
    if (unknownfb0[i].unknown10_0 == type) {
      count--;
      ActionCommand *last = &unknownfb0[count];
      unknownfb0[i] = *last;
      last->Reset();
      i--;
      isDeleted = true;
      unknown1078 = count;
    }
  }
  return isDeleted;
}

f32 BattleCharacter::GetAnimationTime() const {
  gf::GfComAsm *comAsm = GetObject()->getComponent<gf::GfComAsm>();
  if (comAsm == nullptr) {
    return 0.0f;
  }
  return comAsm->getLocalTime(gf::ANIM_LAYER_BASE);
}

f32 BattleCharacter::GetAnimationFrame() const {
  gf::GfComAsm *comAsm = GetObject()->getComponent<gf::GfComAsm>();
  if (comAsm == nullptr) {
    return 0.0f;
  }
  return comAsm->getLocalTime(gf::ANIM_LAYER_BASE) * FRAME_RATE;
}

f32 BattleCharacter::GetAnimationLengthFrame() const {
  gf::GfComAsm *comAsm = GetObject()->getComponent<gf::GfComAsm>();
  if (comAsm == nullptr) {
    return 0.0f;
  }
  const char *state = comAsm->getStateName(gf::ANIM_LAYER_BASE);
  if (state == nullptr) {
    return 0.0f;
  }
  f32 time;
  if (!comAsm->getPlaybackTime(state, &time)) {
    return 0.0f;
  }
  f32 frame = time * FRAME_RATE;
  int base = static_cast<int>(frame);
  return static_cast<f32>(base + (frame - static_cast<f32>(base) >= 0.9f));
}

f32 BattleCharacter::GetAnimationLengthSec() const { return GetAnimationLengthFrame() * (1.0f / FRAME_RATE); }

int BattleCharacter::GetGroupIndex() const {
  const CharacterParameter *parameter = GetCharacterParameterConst(true);
  return parameter != nullptr ? parameter->unknown1c : -1;
}

bool BattleCharacter::IsEnableBackHome() const { return unknown107c == ACTION_ID_IDLE; }

bool BattleCharacter::IsDisp() const { return unknownf2c[0] > 0.0f; }

int BattleCharacter::GetWeaponType(bool isAsm) const {
  if (isAsm) {
    return gf::GfDataWeapon::getAsmWeaponType(unknownf7f);
  }

  return unknownf7f;
}

const BattleCharacter::CUSTOM_VOICE_DATA *BattleCharacter::GetCustomVoiceData(int voice, int *index) const {
  for (int i = 0; i < 3; i++) {
    if (unknownf64[i].unknown00 == voice) {
      if (index != nullptr) {
        *index = i;
      }
      return &unknownf64[i];
    }
  }
  return nullptr;
}

bool BattleCharacter::IsCustomVoice(int voice) const {
  u32 flag = 1 << voice;
  if ((unknownf5c & flag) == 0) {
    return false;
  }

  return (unknownf60 & flag) == 0;
}

void BattleCharacter::SetCustomVoiceEndFlag(int voice, bool isEnd) {
  if (isEnd) {
    unknownf60 |= 1 << voice;
  } else {
    unknownf60 &= ~(1 << voice);
  }
}

bool BattleCharacter::PlayCustomVoice(int voice) {
  if (!IsCustomVoice(voice)) {
    return false;
  }

  int index;
  if (GetCustomVoiceData(voice, &index) == nullptr) {
    return false;
  }
  if (!TalkManager::get()->NotifyTrigger(unknownf7d, static_cast<TalkManager::TRIGGER>(0x2b + index), 0, 0)) {
    return false;
  }
  SetCustomVoiceEndFlag(voice, true);
  return true;
}

void BattleCharacter::ClearCustomVoiceEndFlag(u32 flags) { unknownf60 &= ~flags; }

int BattleCharacter::GetWeaponSeType() const { return Utility::GetWeaponSeType(unknownf7f); }

bool BattleCharacter::IsEnableTurnAI() const { return ((1 << unknown107c) & ((1 << ACTION_ID_IDLE) | (1 << ACTION_ID_MOVE))) != 0; }

void BattleCharacter::GetMoveDataPerSec(mm::Vec3 &moveData) const {
  moveData.x = this->unknownb0.x * FRAME_RATE;
  moveData.y = this->unknownb0.y * FRAME_RATE;
  moveData.z = this->unknownb0.z * FRAME_RATE;
}

void BattleCharacter::SetAnimationSpeed(f32 speed) { m_animationSpeed = speed; }

f32 BattleCharacter::GetAnimationMoveDistance() {
  int param0 = unknowncb0;
  int param1 = unknowncb4;
  int animation = unknownf7c;
  for (int i = 0; i < 20; i++) {
    if (unknownce4[i].unknown00 == animation && unknownce4[i].unknown04 == param0 && unknownce4[i].unknown06 == param1) {
      f32 distance = unknownce4[i].unknown02 * 0.01f;
      if (i != 0) {
        ANIMATION_MOVE_DISTANCE_DATA data = unknownce4[i];
        for (int j = i; j > 0; j--) {
          unknownce4[j] = unknownce4[j - 1];
        }
        unknownce4[0] = data;
      }
      return distance;
    }
  }
  return -1.0f;
}

void BattleCharacter::SetIKData(const ARTS_DATA &data) {
  unknowndc0_7 = data.unknown15_0;
  unknowndc1_3 = data.unknown15_4;
  unknowndc2_3 = data.unknown16_4;
  unknowndc2_7 = data.unknown17_0;
  unknowndc3_7 = data.unknown2f_0;
}

void BattleCharacter::NotifyStrikeDamage(int) { TalkManager::get()->NotifyTrigger(unknownf7d, TalkManager::TRIGGER_STRIKE_DAMAGE, 0, 0); }

void BattleCharacter::HealDirect(u32 value, int type, BattleCharacter *healer) {
  DIRECT_DAMAGE_DATA data;
  data.unknown08 = type;
  data.unknown0c = value;
  data.unknown11 = 0;
  data.unknown12 = 0;
  data.unknown00 = healer;
  data.unknown10 = 1;
  NotifyDirectDamage(data);
}

void BattleCharacter::HealByRate(u32 value, f32 rate, int type, BattleCharacter *healer) {
  f32 total = static_cast<f32>(value) * rate;
  u32 heal = static_cast<u32>(total);
  if (total - static_cast<f32>(heal) > 0.998f) {
    heal++;
  }
  DIRECT_DAMAGE_DATA data;
  data.unknown11 = 0;
  data.unknown12 = 0;
  data.unknown00 = healer;
  data.unknown08 = type;
  data.unknown0c = mm::max(1u, heal);
  data.unknown10 = 1;
  NotifyDirectDamage(data);
}

void BattleCharacter::LandingCallback() {}

void BattleCharacter::BoundCallback() {}

void BattleCharacter::ResetAlphaRate(int alpha) {
  int first = 0;
  int last = ALPHA_RATE_NUM - 1;
  if (alpha >= 0) {
    first = alpha;
    last = alpha;
  }

  for (int i = first; i <= last; i++) {
    unknownf2c[i] = -1.0f;
  }
}

f32 BattleCharacter::GetSyncCount(bool isSlow) const { return GetDt(isSlow, true) * FRAME_RATE; }

f32 BattleCharacter::GetDt(bool isSlow, bool isStopCheck) const {
  if (isStopCheck && unknownf93_6) {
    return 0.0f;
  }
  return isSlow ? unknowncc0 * unknownf58 : unknowncc0;
}

void BattleCharacter::RegisterSlow(SLOW_PROC_ID slow, f32 rate, f32 time) {
  unknownf4c[slow].unknown00 = rate;
  unknownf4c[slow].unknown04 = time;
  unknownf4c[slow].unknown08 = 0.0f;
}

void BattleCharacter::DeleteSlow(SLOW_PROC_ID slow) {
  unknownf4c[slow].unknown00 = 1.0f;
  unknownf4c[slow].unknown04 = 0.0f;
  unknownf4c[slow].unknown08 = 0.0f;
}

int BattleCharacter::GetAlphaSetID(ALPHA_PROC_ID alpha) {
  int id = alpha;
  if (id > ALPHA_PROC_LAST) {
    return ALPHA_PROC_WEAPON;
  }
  if (id >= 0 && id < ALPHA_PROC_WEAPON) {
    return id;
  }
  if (id >= ALPHA_PROC_WEAPON && id <= ALPHA_PROC_LAST) {
    return id - ALPHA_PROC_WEAPON;
  }
  return -1;
}

bool BattleCharacter::CheckForceExit(ACTION_ID action) {
  if (unknownf8e_5 && unknownf8e_6) {
    const CharacterParameter *parameter = GetCharacterParameterConst(true);
    if (parameter != nullptr && parameter->m_invalidTarget && unknowncc8 == BLADE_BDAT_ID_UNKNOWN_3EB) {
      if (BladeChangeManager::get()->IsRegister(unknown118)) {
        return true;
      }
      gf::GF_OBJ_HANDLE *driver = unknown002(this)->unknown10;
      int type = 3;
      if (!BattleGlobal::get()->unknown65e1_2) {
        type = BattleGlobal::get()->unknown65dc;
      }
      BladeChangeManager::get()->StartBladeChange(driver, unknown118, type, 5, false);
      return true;
    }

    if (((1 << action) & 0x7fbde) != 0 || (IsDriver() && action == ACTION_ID_BLADE_ARTS)) {
      unknownf8e_4 = true;
      return true;
    }
  }
  return false;
}

void BattleCharacter::UpdateInvincible() {
  if (IsBlade()) {
    return;
  }

  if (!m_player && !m_battle) {
    if (!unknownf95_4) {
      unknownf95_3 = gf::GfObjUtil::testInvincible(unknown118);
      unknownf95_4 = true;
    }
    if (unknownf95_3) {
      gf::GfObjUtil::setInvincible(unknown118, true, false);
      return;
    }
  }

  if (!unknownf8e_4) {
    bool isNoDamage = IsNoDamage(false);
    gf::GfObjUtil::setInvincible(unknown118, isNoDamage, isNoDamage);
    if (isNoDamage) {
      return;
    }
  } else {
    gf::GfObjUtil::setInvincible(unknown118, false, false);
  }

  const CharacterParameter *parameter = GetCharacterParameterConst(true);
  if (parameter == nullptr) {
    return;
  }
  u32 limit = 0;
  if (parameter->unknown5a4 != 0) {
    f32 maxHp = parameter->GetMaxHP();
    f32 rate = static_cast<f32>(parameter->unknown5a4) * 0.01f;
    limit = static_cast<u32>(maxHp * rate);
  }
  u32 limitHp = IsImmortal(false, nullptr) + limit;
  gf::GfObjUtil::setLimitHP(unknown118, limitHp);
}

void BattleCharacter::LeverReduceProc() {
  if (unknown107c == ACTION_ID_UNKNOWN_0E) {
    const FIGHT_COMBO_DATA *combo = GetCharacterParameterConst(true)->GetFightComboDataConst();
    if ((combo->unknown00 | 1) == 3) {
      const fw::PadData *pad = fw::PadManager::getControlPadData(0);
      f32 x = pad->unknown1c;
      f32 z = pad->unknown20;
      u32 trigger = pad->unknown04;
      mm::Vec3 stick;
      stick.x = x;
      stick.y = 0.0f;
      stick.z = z;
      if (Misc::CalcLengthXZ(stick) >= 0.7f && Misc::NormalizeXZ(stick)) {
        f32 angle = Misc::CalcAngle(mm::Vec3::unitX, stick, true) + 3.1415927f;
        BattleGlobal::get()->InputLeverGachaStick(angle);
      }
      if (trigger == 0) {
        return;
      }
      if ((trigger & 8) != 0) {
        BattleGlobal::get()->InputLeverGachaButton(static_cast<BattleGlobal::LEVER_GACHA_DATA::BUTTON>(0));
      }
      if ((trigger & 2) != 0) {
        BattleGlobal::get()->InputLeverGachaButton(static_cast<BattleGlobal::LEVER_GACHA_DATA::BUTTON>(2));
      }
      if ((trigger & 1) != 0) {
        BattleGlobal::get()->InputLeverGachaButton(static_cast<BattleGlobal::LEVER_GACHA_DATA::BUTTON>(3));
      }
      if ((trigger & 4) != 0) {
        BattleGlobal::get()->InputLeverGachaButton(static_cast<BattleGlobal::LEVER_GACHA_DATA::BUTTON>(1));
      }
      return;
    }
  }

  BattleGlobal::LEVER_GACHA_DATA &lever = BattleGlobal::get()->unknown5a88;
  lever.unknown00[0] = 0.0f;
  lever.unknown00[1] = 0.0f;
  lever.unknown00[2] = 0.0f;
  lever.unknown00[3] = 0.0f;
  lever.unknown10 = 0xffff;
  lever.unknown12 = 0xffff;
  lever.unknown14 = 0;
  lever.unknown15 = 0;
  lever.unknown16 = false;
}

void BattleCharacter::AddBladeArtsRecast(int chain, f32 rate) {
  BattleCharacter *character = this;
  while (true) {
    if (ChainAttackManager::get()->IsProc(true)) {
      return;
    }
    if (!character->IsDriver()) {
      break;
    }
    character = CharacterManager::get()->GetCharacterConst(character->GetCharacterParameterConst(true)->unknown10);
    if (character == nullptr) {
      return;
    }
  }
  if (!character->IsBlade()) {
    return;
  }

  const char *column = "Param";
  if (character->IsPlayer()) {
    if (!character->IsControlCharacterGroup()) {
      f32 enhance = character->SatisfyEnhanceCondition(0x146, 0, 0.0f, nullptr, true);
      column = enhance <= 0.0f ? "Param" : "Param2";
    }
  }
  u8 *bdat = gf::GfDataBdat::getFP(gf::BDAT_BTL_SP_ARTS_RECAST);
  f32 recast = static_cast<u8>(Bdat::getValCheck(bdat, column, chain, 1));
  character->AddSlotRecast(SLOT_ALL, (rate + 1.0f) * recast, false);
}

void BattleCharacter::AddRecast(f32 recast, bool isRate, int slot) { AddSlotRecast(slot < 0 ? SLOT_ALL : -1 - slot, recast, isRate); }

void BattleCharacter::RegisterDelayVoice(DELAY_VOICE_TYPE type, f32 time, int voice, int group, int priority) {
  if (type == DELAY_VOICE_TYPE_NORMAL) {
    unknowndd0 = time * (1.0f / FRAME_RATE) * DELAY_VOICE_TIME_SCALE;
    unknowndd2 = priority;
    unknowndd4 = group;
    unknowndd6 = voice;
  }
}

bool BattleCharacter::StartBladeSupport(int support) {
  const CharacterParameter *parameter = GetCharacterParameterConst(true);
  if (parameter == nullptr) {
    return false;
  }

  if (parameter->unknown5df_1 && parameter->m_invalidTarget) {
    gf::GfObjAcc acc(unknown118);
    if (!gf::GfDataBlade::isDummyBlade(acc.getBdatId()) && IsEnableTurnAI()) {
      SetAction(static_cast<ACTION_ID>(0xb), false, false, support, 0.0f);
      return true;
    }
  }
  return false;
}

void BattleCharacter::IsKakuseiEffect(bool &isEnable, bool &isKakusei) const {
  bool kakusei = false;
  bool enable = false;
  if (m_player && BattleGlobal::get()->unknown65df_4) {
    const CharacterParameter *parameter = GetCharacterParameterConst(true);
    if (parameter != nullptr && !parameter->m_invalidTarget) {
      if (parameter->unknown10 == gf::INVALID_HANDLE) {
        kakusei = unknowncc8 == KAKUSEI_BDAT_ID_UNKNOWN_3EB;
      } else {
        kakusei = unknowncc8 == 1;
      }
    } else {
      kakusei = unknowncc8 == KAKUSEI_BDAT_ID_UNKNOWN_3EB;
    }
    enable = true;
  }
  isEnable = enable;
  isKakusei = kakusei;
}

bool BattleCharacter::IsGuardReflect(int guard) {
  if (SatisfyEnhanceCondition(guard + ENHANCE_GUARD_REFLECT_UNKNOWN_65, 0, 0.0f, nullptr, true) > 0.0f) {
    return true;
  }

  bool isEnable;
  bool isKakusei;
  IsKakuseiEffect(isEnable, isKakusei);
  return isKakusei;
}

void BattleCharacter::FluctuateTrust(int type, int level) {
  if (!m_player) {
    return;
  }

  u8 *bdat = gf::GfDataBdat::getFP(gf::BDAT_BTL_BL_TRUST_UP);
  u32 point = static_cast<u16>(Bdat::getValCheck(bdat, "Point", type, 2));
  if (point == 0) {
    return;
  }

  if (type != 1) {
    AddTrust(point, level);
    return;
  }

  mm::mtl::FixedVector<u8, 90> list;
  Utility::GetCharacterList(list, unknown118, Utility::CHECK_CHR_NUM_MODE_FRIEND, Utility::CHARACTER_LIST_BATTLE | Utility::CHARACTER_LIST_ALIVE | Utility::CHARACTER_LIST_EXCLUDE_BLADE);
  CharacterManager *characterManager = CharacterManager::get();
  for (u8 id : list) {
    characterManager->GetCharacterConst(id)->AddTrust(point, level);
  }
}

bool BattleCharacter::CheckPermitArts(u32 attack) {
  if (!m_player) {
    return false;
  }

  if (unknown107c != ACTION_ID_SP_ATTACK) {
    const CharacterParameter *parameter = GetCharacterParameterConst(true);
    if (parameter == nullptr || !parameter->m_invalidTarget) {
      return false;
    }
  }

  if (!BattleGlobal::get()->IsPermitArts(attack)) {
    const CharacterParameter *parameter = GetCharacterParameterConst(true);
    if (parameter == nullptr) {
      return false;
    }
    DataManager *dataManager = DataManager::get();
    u16 flags = dataManager->unknown10758[parameter->unknown1c].unknown104;
    if (!(flags & 2)) {
      return false;
    }
    unknown10c9_1 = (flags & 4) >> 2;
    if (!(flags & 4)) {
      return false;
    }
    if (attack != 0) {
      BattleGlobal::get()->SetPermitArts(attack);
    }
  }
  return true;
}

bool BattleCharacter::IsAngry() const {
  const CharacterParameter *parameter = GetCharacterParameterConst(true);
  return parameter != nullptr && parameter->IsAngry();
}

void BattleCharacter::UpdateBuffEffect(int buff) {
  if (buff == BUFF_EFFECT_TARGET_LOCK) {
    UpdateTargetLockArrow();
  } else {
    UpdateEffectPositionScale(buff, nullptr, -1.0f);
  }
}

void BattleCharacter::ResetChangeActionStopSeData() { unknown10c4_0 = 0; }

void BattleCharacter::ReturnBladeChange() {
  SetAction(ACTION_ID_BLADE_CHANGE, false, false, 0, 0.0f);
  unknown107e = ACTION_PHASE_BLADE_CHANGE_RETURN;
}

void BattleCharacter::ReturnBladeArts() {
  SetAction(ACTION_ID_BLADE_ARTS, false, false, 0, 0.0f);
  BladeArtsManager *manager = BladeArtsManager::get();
  if (manager->IsRegister(unknown118)) {
    if (manager->IsBladeStart(unknown118)) {
      unknown107e = 6;
    } else {
      unknown107e = 11;
    }
  } else {
    unknown107e = 7;
  }
}

void BattleCharacter::NotifyChangeAttr(gf::GF_OBJ_HANDLE *handle, u32 attribute, u32 oldAttribute) {
  if (unknown120 == handle || unknown128 == handle) {
    BattleCharacter *character = CharacterManager::get()->GetCharacterConst(handle);
    if (character != nullptr && !character->IsDead(0)) {
      TalkManager::get()->NotifyTrigger(unknownf7d, static_cast<TalkManager::TRIGGER>(0x18), attribute, 0);
    }
  }
}

bool BattleCharacter::SetAngry(bool isAngry, f32 time) {
  CharacterParameter *parameter = GetCharacterParameter(false);
  if (parameter == nullptr) {
    return false;
  }

  return parameter->SetAngry(isAngry, time);
}

ActionCommand *BattleCharacter::GetActionCommand(ACTION_ID action, bool isSet) {
  int num = unknown1078;
  if (num <= 0) {
    return nullptr;
  }

  if (isSet) {
    for (long i = 0; i < num; i++) {
      ActionCommand *command = &unknownfb0[i];
      if (static_cast<s32>(command->unknown10) >= 0 && command->IsProcAction(static_cast<u8>(action))) {
        command->unknown10 |= 0x80000000;
        return command;
      }
    }
  } else {
    for (long i = 0; i < num; i++) {
      ActionCommand *command = &unknownfb0[i];
      if (static_cast<s32>(command->unknown10) >= 0 && command->IsProcAction(static_cast<u8>(action))) {
        return command;
      }
    }
  }

  return nullptr;
}

ActionCommand *BattleCharacter::GetFreeCommand() {
  if (unknown1078 >= COMMAND_NUM) {
    return nullptr;
  }
  ActionCommand *command = &unknownfb0[unknown1078];
  unknown1078++;
  command->Reset();
  return command;
}

bool BattleCharacter::IsRegisterCommand(ActionCommand::COMMAND_TYPE type) const {
  for (int i = 0; i < unknown1078; i++) {
    if (unknownfb0[i].unknown10_0 == type) {
      return true;
    }
  }
  return false;
}

void BattleCharacter::RegisterBladeGuardCommand() {
  if (IsRegisterCommand(ActionCommand::COMMAND_TYPE_BLADE_GUARD)) {
    return;
  }

  ActionCommand *command = GetFreeCommand();
  if (command != nullptr) {
    static_cast<act_cmd::BLADE_GUARD *>(command)->Initialize();
  }
  TalkManager::TRIGGER trigger = static_cast<TalkManager::TRIGGER>(TalkManager::TRIGGER_UNKNOWN_1C - m_player);
  TalkManager::get()->NotifyTrigger(unknownf7d, trigger, 0, 0);
}

void BattleCharacter::ActionCommandBladeArts(const ActionCommand *command, ActionCommand::COMMAND_TYPE type) {
  if ((type == ActionCommand::COMMAND_TYPE_MOVE_DIR_RATE || type == ActionCommand::COMMAND_TYPE_MOVE_LOCK) && unknown107e == ACTION_PHASE_BLADE_ARTS_MOVE) {
    ActionCommandRun(command, type);
  }
}

bool BattleCharacter::IsRescue() const {
  if (unknown107c == ACTION_ID_RESCUE) {
    return true;
  }
  if (unknown107c == ACTION_ID_MOVE && unknown1178 == AI_STATE_ACTIVE && unknown1160.unknown00 == ACTION_ID_RESCUE) {
    return true;
  }
  return false;
}

bool BattleCharacter::MoveStop(bool isForce) {
  if (unknown1178 != AI_STATE_ACTIVE || unknown1160.unknown00 != ACTION_ID_IDLE || (unknown1180 & AI_FLAG_SCOUT) != 0) {
    return false;
  }
  unknown1180 |= isForce ? AI_FLAG_STOP_FORCE : AI_FLAG_STOP;
  return true;
}

void BattleCharacter::SetScout(bool isScout) {
  if (unknown1178 == AI_STATE_ACTIVE) {
    if (isScout) {
      unknown1180 |= AI_FLAG_SCOUT;
      unknown1174 += AI_SCOUT_TIME;
    } else {
      unknown1180 &= ~AI_FLAG_SCOUT;
    }
  }
}

bool BattleCharacter::IsScout() const { return unknown1178 == AI_STATE_ACTIVE && (unknown1180 & AI_FLAG_SCOUT) != 0; }

gf::GF_OBJ_HANDLE *BattleCharacter::GetRescueTarget() const {
  int target;
  if (unknown107c == ACTION_ID_RESCUE) {
    target = unknown108c;
  } else if (unknown107c == ACTION_ID_MOVE && unknown1178 == AI_STATE_ACTIVE) {
    AI_ACTION_DATA action = GetAIActionData();
    if (action.unknown00 != ACTION_ID_RESCUE) {
      return gf::INVALID_HANDLE;
    }
    target = action.unknown04;
  } else {
    return gf::INVALID_HANDLE;
  }

  if (target < 0) {
    return gf::INVALID_HANDLE;
  }
  return gf::GfGameParty::getHandleDriver(target);
}

bool BattleCharacter::IsPause() const { return GetDt(true, true) <= PAUSE_DT; }

f32 BattleCharacter::GetSystemDt() const { return m_systemDt; }

f32 BattleCharacter::GetSystemSyncCount() const { return m_systemDt * FRAME_RATE; }

bool BattleCharacter::IsEnableBladeChangeAction(ACTION_ID actionId) const { return ((1 << static_cast<u8>(actionId)) & BLADE_CHANGE_ACTIONS) != 0; }

void BattleCharacter::EnhanceProc() { EnhanceTimerProc(); }

bool BattleCharacter::IsEnableBattleFace() const { return m_player; }

void BattleCharacter::NotifyHateElse() { SetBladeSupportCondition(BLADE_SUPPORT_CONDITION_HATE_ELSE); }

void BattleCharacter::ActionCommandEndIdle() {}

void BattleCharacter::ActionCommandPutOutWeapon(const ActionCommand *, ActionCommand::COMMAND_TYPE) {}

void BattleCharacter::ActionCommandEndPutOutWeapon() {}

void BattleCharacter::ActionCommandPutAwayWeapon(const ActionCommand *, ActionCommand::COMMAND_TYPE) {}

void BattleCharacter::ActionCommandEndPutAwayWeapon() {}

void BattleCharacter::ActionCommandKizunaMax(const ActionCommand *, ActionCommand::COMMAND_TYPE) {}

void BattleCharacter::ActionCommandEndKizunaMax() {}

void BattleCharacter::ActionCommandEndAttack() {}

void BattleCharacter::ActionCommandEndArts() {}

void BattleCharacter::ActionCommandBladeChange(const ActionCommand *, ActionCommand::COMMAND_TYPE) {}

void BattleCharacter::ActionCommandEndBladeChange() {}

void BattleCharacter::ActionCommandSpAttack(const ActionCommand *, ActionCommand::COMMAND_TYPE) {}

void BattleCharacter::ActionCommandEndSpAttack() {}

void BattleCharacter::ActionCommandSupport(const ActionCommand *, ActionCommand::COMMAND_TYPE) {}

void BattleCharacter::ActionCommandEndSupport() {}

void BattleCharacter::ActionCommandBladeGuard(const ActionCommand *, ActionCommand::COMMAND_TYPE) {}

void BattleCharacter::ActionCommandEndBladeGuard() {}

void BattleCharacter::ActionCommandRepelled(const ActionCommand *, ActionCommand::COMMAND_TYPE) {}

void BattleCharacter::ActionCommandEndRepelled() {}

void BattleCharacter::ActionCommandDown(const ActionCommand *, ActionCommand::COMMAND_TYPE) {}

void BattleCharacter::ActionCommandEndDown() {}

void BattleCharacter::ActionCommandKnockBack(const ActionCommand *, ActionCommand::COMMAND_TYPE) {}

void BattleCharacter::ActionCommandEndKnockBack() {}

void BattleCharacter::ActionCommandBreak(const ActionCommand *, ActionCommand::COMMAND_TYPE) {}

void BattleCharacter::ActionCommandEndBreak() {}

void BattleCharacter::ActionCommandDead(const ActionCommand *, ActionCommand::COMMAND_TYPE) {}

void BattleCharacter::ActionCommandEndDead() {}

void BattleCharacter::ActionCommandRescue(const ActionCommand *, ActionCommand::COMMAND_TYPE) {}

void BattleCharacter::ActionCommandEndRescue() {}

void BattleCharacter::AddIdea(u32 idea, u32 point) { gf::SCOM_ADD_IDEA::create(unknown118, idea, point); }

void BattleCharacter::SetNoDamageTime(f32 time) { unknown10e0 = unknown10e0 > time ? unknown10e0 : time; }

void BattleCharacter::SetSuperArmorTime(f32 time) { unknown10e4 = unknown10e4 > time ? unknown10e4 : time; }

void BattleCharacter::SetPerfectAvoidTime(f32 time) { unknown10e8 = unknown10e8 > time ? unknown10e8 : time; }

f32 BattleCharacter::GetArtsDamageRate(const ARTS_DATA::LEVEL_DATA &level) const { return level.unknown00 * 0.01f; }

f32 BattleCharacter::ApplyResistantAttr(f32 damage, int attribute) const {
  if (GetCharacterParameterConst(false)->unknown59e & (1 << attribute)) {
    damage = BtlSystemBalanceAccess::get()->unknown10 * damage;
  }
  return damage;
}

f32 BattleCharacter::GetArtsDamageRate(const ARTS_DATA &data, int level) const { return GetArtsDamageRate(data.unknown90[level - 1]); }

u32 BattleCharacter::CreateElementalComboEffect(int combo) {
  u32 effect = 0;
  if (combo != 0) {
    gf::GfComEffectCaster *caster = GetEffectCaster();
    if (caster != nullptr) {
      gf::GfComProperty *property = GetComPropertyConst();
      if (property != nullptr) {
        u8 *bdat = gf::GfDataBdat::getFP(gf::BDAT_BTL_ELEMENTAL_COMBO);
        gf::MsgFxCombo message;
        message.unknown10 = reinterpret_cast<const char *>(Bdat::getValCheck(bdat, "Effect", combo, 7));
        if (*message.unknown10 != '\0') {
          gf::GfObj *obj = static_cast<gf::GfObj *>(property->unknown08);
          caster->recieveMessage(obj, message);
          BattleGlobal *global = BattleGlobal::get();
          gf::GF_OBJ_HANDLE *handle = unknown118;
          bool isTarget = global->unknown5af0 == handle;
          if (message.unknown18 != 0) {
            if (isTarget) {
              global->RegisterElementalComboInvalidSlowEffect(1, message.unknown18, caster);
            }
            effect = message.unknown18;
            caster->startTimer(effect);
          }
          PlaySE(BTL_SE_HIT, combo + 79, nullptr, isTarget);
        }
      }
    }
  }
  return effect;
}

void BattleCharacter::CreateElementalEffect(int effect) {
  gf::GfComEffectCaster *caster = GetEffectCaster();
  if (caster != nullptr) {
    u8 *bdat = gf::GfDataBdat::getFP(gf::BDAT_BTL_ELEMENTAL_EFFECT);
    gf::MsgFxComboBuff message;
    message.unknown10 = reinterpret_cast<const char *>(Bdat::getValCheck(bdat, "Effect", effect, 7));
    if (*message.unknown10 != '\0') {
      gf::GfComProperty *property = GetComPropertyConst();
      if (property != nullptr) {
        gf::GfObj *obj = static_cast<gf::GfObj *>(property->unknown08);
        caster->recieveMessage(obj, message);
      }
    }
  }
}

void BattleCharacter::StopAI(f32 time) { unknown11cc = unknown11cc > time ? unknown11cc : time; }

bool BattleCharacter::CheckRandFriendEnhanceRate(int enhance) const {
  f32 rate = GetFriendEnhanceRate(enhance, false);
  if (rate <= 0.0f) {
    return false;
  }

  return ml::mtRandf1() <= rate;
}

void BattleCharacter::ResetBladeSupportConditionFlag() { unknown11e0 = 0; }

void BattleCharacter::FluctuatePartyGauge(int value) {
  if (!m_player) {
    return;
  }

  int partyId = GetGroupIndex();
  DataManager::get()->FluctuatePartyGauge(value, partyId);
}

void BattleCharacter::AddPartyGauge(f32 value) {
  if (!m_player) {
    return;
  }

  int partyId = GetGroupIndex();
  DataManager::get()->AddPartyGauge(value, partyId, true);
}

void BattleCharacter::NotifyFriendDead(int) { PlayCustomVoice(1); }

void BattleCharacter::NotifyDropHealpot() { PlayCustomVoice(3); }

void BattleCharacter::NotifyEnemyAngry(gf::GF_OBJ_HANDLE *) { PlayCustomVoice(7); }

void BattleCharacter::NotifyBackAttack(int) { PlayCustomVoice(10); }

void BattleCharacter::NotifyElementalCombo(gf::GF_OBJ_HANDLE *, int, bool) {
  if (IsBlade()) {
    PlayCustomVoice(13);
  }
}

void BattleCharacter::NotifySufferElementalCombo(int, int) {
  if (IsBlade()) {
    PlayCustomVoice(14);
  }
}

void BattleCharacter::NotifyBuddyFightCombo(gf::GF_OBJ_HANDLE *, int fightCombo) {
  if (!IsCustomVoice(11)) {
    return;
  }
  const CUSTOM_VOICE_DATA *data = GetCustomVoiceData(11, nullptr);
  if (data != nullptr && (data->unknown01 == fightCombo || data->unknown01 == 0)) {
    PlayCustomVoice(11);
  }
}

void BattleCharacter::NotifyBuddySufferFightCombo(int, int fightCombo) {
  if (!IsCustomVoice(12)) {
    return;
  }
  const CUSTOM_VOICE_DATA *data = GetCustomVoiceData(12, nullptr);
  if (data != nullptr && (data->unknown01 == fightCombo || data->unknown01 == 0)) {
    PlayCustomVoice(12);
  }
}

void BattleCharacter::NotifyKillSpAttack() { PlayCustomVoice(9); }

void BattleCharacter::NotifyLostHateTarget(int) {
  if (!IsCustomVoice(5)) {
    return;
  }

  const CharacterParameter *parameter = GetCharacterParameterConst(false);
  if (parameter == nullptr) {
    return;
  }
  if (parameter->m_invalidTarget && gf::GfDataBlade::getRole(unknowncc8) == 1) {
    PlayCustomVoice(5);
  }
}

void BattleCharacter::AddKizuna(f32 kizuna) {
  if (kizuna == 0.0f) {
    return;
  }

  int partyId = GetGroupIndex();
  if (DataManager::get()->unknown001(partyId)) {
    DataManager::get()->unknown10758[partyId].unknown001(-1)->AddGauge(kizuna, true);
  }
}

void BattleCharacter::SetKizunaStage(int stage) {
  int partyId = GetGroupIndex();
  if (DataManager::get()->unknown001(partyId)) {
    DataManager::get()->unknown10758[partyId].unknown001(-1)->SetKizunaStage(stage, 0.5f, false);
  }
}

void BattleCharacter::AddKizunaStage(int value) { SetKizunaStage(mm::clamp(GetKizunaStage(nullptr) + value, 0, 6)); }

void BattleCharacter::ScoutProc() {
  if (m_player) {
    return;
  }

  gf::DataParamAcc acc(unknown118);
  int id = acc.getBdatIdEnParam();
  u8 radius = Bdat::getValCheck(gf::GfDataBdat::getFP(gf::BDAT_CHR_EN_PARAM), "ScoutJoinRad", id, 1);
  JoinProc(static_cast<f32>(radius), true);
}

f32 BattleCharacter::GetLaunchEffectScale() const {
  f32 scale = static_cast<f32>(unknownf72) * 0.01f;
  const CharacterParameter *parameter = GetCharacterParameterConst(false);
  if (parameter != nullptr) {
    scale *= parameter->unknown28;
  }
  return scale;
}

void BattleCharacter::NotifyAngryStart() {
  PlaySE(BTL_SE_HIT, 0x3c, nullptr, true);
  TalkManager::get()->NotifyTrigger(unknownf7d, static_cast<TalkManager::TRIGGER>(0x33), 0, 0);
  unknown130[6] = fw::EffectManager::get()->create("cmn_000_025", nullptr, true);
  UpdateEffectPositionScale(6, nullptr, GetLaunchEffectScale());
}

void BattleCharacter::SetActionCallback(ACTION_ID) {}

void BattleCharacter::StartFullBurst(int count) {
  const CharacterParameter *parameter = GetCharacterParameterConst(false);

  if (!parameter->unknown5df_6) {
    BattleCharacter *buddy = CharacterManager::get()->GetCharacterConst(parameter->unknown10);
    if (buddy == nullptr || !buddy->m_battle) {
      return;
    }
  }

  unknown10ac = 0;
  unknown10ae = count;
  unknown10b0 = static_cast<f32>(count << 3);
}

void BattleCharacter::ResetActionCommand() {
  int count = unknown1078;
  for (int i = 0; i < count; i++) {
    // The game resets a stack copy of each command (0x2ec4c..0x2ec68), not the stored one.
    ActionCommand command = unknownfb0[i];
    command.Reset();
  }
  unknown1078 = 0;
}

void BattleCharacter::ActionCommandEndRun() {
  if (!unknownf8e_4 && unknown1080 == 0) {
    SetAction(ACTION_ID_IDLE, false, false, 0, 0.0f);
  }
}

void BattleCharacter::UpdateCulling() {
  bool isDisable = unknownf83 != 0;
  gf::GfObjAcc acc(unknown118);
  acc.setCameraFadeDisableForBattle(isDisable);
}

void BattleCharacter::UpdateLaunchEffect() { UpdateEffectPositionScale(5, nullptr, GetLaunchEffectScale()); }

void BattleCharacter::UpdateBuffEffect() {
  UpdateEffectPositionScale(7, nullptr, -1.0f);
  UpdateTargetLockArrow();
  UpdateEffectPositionScale(9, nullptr, -1.0f);
  UpdateEffectPositionScale(10, nullptr, -1.0f);
  UpdateEffectPositionScale(11, nullptr, -1.0f);
  UpdateEffectPositionScale(12, nullptr, -1.0f);
}

void BattleCharacter::SlowProc() {
  f32 time = unknownf93_6 ? 0.0f : unknowncc0;
  SLOW_PROC_DATA &slow = unknownf4c[0];
  f32 rate = 1.0f;
  if (slow.unknown04 != 0.0f) {
    if (slow.unknown04 > 0.0f) {
      if (time + slow.unknown08 >= slow.unknown04) {
        slow.unknown00 = 1.0f;
        slow.unknown04 = 0.0f;
        slow.unknown08 = 0.0f;
        rate = 1.0f;
      } else {
        slow.unknown08 = time + slow.unknown08;
        rate = mm::min(1.0f, slow.unknown00);
      }
    } else {
      rate = mm::min(1.0f, slow.unknown00);
    }
  }

  unknownf58 = rate;
}

void BattleCharacter::DispElementalEffectUI(int effect) {
  u32 flags = unknown10f4;
  u32 bit = 1u << effect;
  if ((flags & bit) != 0) {
    return;
  }
  unknown10f4 = flags | bit;
  UIManager::get()->DispElementalEffectUI(unknown118, effect);
}

f32 BattleCharacter::GetChainSec() {
  f32 sec = static_cast<f32>(BtlSystemBalanceAccess::get()->unknown68) * (1.0f / 30.0f);
  f32 rate = SatisfyEnhanceCondition(0x147, 0, 0.0f, nullptr, true);
  return rate > 0.0f ? sec + rate * sec : sec;
}

bool BattleCharacter::IsInvalidDead(bool isIgnoreBattle) const {
  gf::GF_OBJ_HANDLE *handle = unknown118;
  if ((!isIgnoreBattle && !BattleGlobal::get()->IsEnableBattle()) || BattleGlobal::get()->m_elementalComboImmortal == handle) {
    return true;
  }
  if (!m_player) {
    return false;
  }
  if (ChainAttackManager::get()->IsProc(false)) {
    return true;
  }
  if (!BladeArtsManager::get()->IsRegister(handle)) {
    return false;
  }
  s8 party = DataManager::get()->unknown132a0;
  const CharacterParameter *parameter = GetCharacterParameterConst(false);
  int partyId = parameter != nullptr ? parameter->unknown1c : -1;
  if (partyId == party && DataManager::get()->unknown132a2 == 3) {
    return true;
  }
  return false;
}

void BattleCharacter::RecoverNextBladeArtsRecast() {
  const CharacterParameter *parameter = GetCharacterParameterConst(false);
  if (parameter == nullptr || !parameter->m_invalidTarget) {
    return;
  }
  const ARTS_PALETTE_DATA *palette = parameter->GetArtsPaletteDataConst(-1);
  for (int i = 0; i < 3; i++) {
    ARTS_SLOT_DATA *slot = palette->unknown001(i, false);
    if (slot->unknown00_0 == 0) {
      return;
    }
    if (slot->unknown04 < 1.0f) {
      slot->AddRecast(1.0f, true);
      return;
    }
  }
}

bool BattleCharacter::CheckRandSatisfyEnhanceCondition(int enhance, int value, f32 rate, void *param) {
  f32 satisfyRate = SatisfyEnhanceCondition(enhance, value, rate, param, false);
  if (satisfyRate <= 0.0f) {
    return false;
  }
  if (ml::mtRandf1() > satisfyRate) {
    return false;
  }
  DispEnhanceName(unknown1124);
  return true;
}

void BattleCharacter::NotifyGetHateTarget(int id, bool isFirst) {
  if (IsControlCharacter()) {
    PlaySystemSE(BTL_SE_HIT, 0x32, true, false);
  }
  if (!isFirst && IsCustomVoice(4)) {
    int index;
    if (GetCustomVoiceData(4, &index) == nullptr) {
      return;
    }
    if (TalkManager::get()->NotifyTrigger(unknownf7d, static_cast<TalkManager::TRIGGER>(TalkManager::TRIGGER_UNKNOWN_2B + index), 0, 0)) {
      SetCustomVoiceEndFlag(4, true);
    }
  }
}

void BattleCharacter::StopSE(BTL_SE se, int number) {
  const char *tag = BattleGlobal::get()->GetSeTag(se);
  if (tag == nullptr) {
    return;
  }
  gf::GfComSound *sound = unknown005(this);
  if (sound != nullptr) {
    mm::mtl::FixStr<32> name = unknown006(tag, number);
    sound->stopSe(name.c_str());
  }
}

void BattleCharacter::PlaySystemSE(BTL_SE se, int number, bool isLoop, bool isStream) {
  if (number == 0) {
    return;
  }
  const char *tag = BattleGlobal::get()->GetSeTag(se);
  if (tag == nullptr) {
    return;
  }
  gf::GfComSound *sound = unknown005(this);
  if (sound != nullptr) {
    mm::mtl::FixStr<32> name = unknown006(tag, number);
    sound->playSe2D(name.c_str(), 1.0f, isLoop, isStream);
  }
}

bool BattleCharacter::PlaySE(BTL_SE se, int number, const mm::Vec3 *position, bool isFollow) {
  int attribute = unknownf77_4;
  if (attribute < SE_ATTRIBUTE_NONE) {
    unknownf77_4 = SE_ATTRIBUTE_NONE;
  } else {
    const CharacterParameter *parameter = GetCharacterParameterConst(false);
    if (parameter != nullptr) {
      attribute = parameter->GetAttribute();
    }
  }

  bool isPlayed = false;
  if (se != 0 && number != 0) {
    const char *tag = BattleGlobal::get()->GetSeTag(se);
    if (tag != nullptr) {
      gf::GfComSound *sound = unknown005(this);
      if (sound != nullptr) {
        mm::mtl::FixStr<32> name = unknown006(tag, number);
        mm::Vec3 offset;
        if (position == nullptr) {
          offset = mm::Vec3::zero;
        } else {
          offset.x = position->x - unknown10.x;
          offset.y = position->y - unknown10.y;
          offset.z = position->z - unknown10.z;
        }
        int oldAttribute = sound->getAttributeVariable();
        sound->setAttributeVariable(attribute);
        isPlayed = sound->playSe(name.c_str(), 1.0f, offset, isFollow);
        sound->setAttributeVariable(oldAttribute);
      }
    }
  }
  return isPlayed;
}

bool BattleCharacter::SetAnimationNoComplement(ANIM_ID animId, int a, int b, bool) {
  gf::GfComAsm *comAsm = gf::GfObjUtil::getObj(unknown118)->getComponent<gf::GfComAsm>();
  if (comAsm != nullptr) {
    const char *state = GetAnimationStateName(animId, a, b);
    if (comAsm->setState(state, gf::ANIM_LAYER_BASE)) {
      SetAnimationCallback(animId, a, b, state);
      return false;
    }
  }
  return true;
}

bool BattleCharacter::IsSlide(bool isIgnoreBladeChange) const {
  if (((1 << unknown107c) & ACTION_FLAGS_SLIDE) != 0) {
    return true;
  }

  gf::GF_OBJ_HANDLE *handle = unknown118;
  if (BladeChangeManager::get()->IsRegister(handle)) {
    if (isIgnoreBladeChange) {
      return false;
    }
    const CharacterParameter *parameter = GetCharacterParameterConst(false);
    if (parameter != nullptr && parameter->m_invalidTarget) {
      return false;
    }
    if (unknown107c == ACTION_ID_BLADE_CHANGE) {
      return false;
    }
  }
  if (BladeArtsManager::get()->IsRegister(handle)) {
    const CharacterParameter *parameter = GetCharacterParameterConst(false);
    if (parameter != nullptr && parameter->m_invalidTarget) {
      return false;
    }
    if (unknownf7c != ACTION_ID_ATTACK) {
      return false;
    }
  }
  if (!SpAttackManager::get()->IsRegister(handle)) {
    if (ChainAttackManager::get()->IsProc(false)) {
      return false;
    }
    f32 landHeight = GetLandHeight();
    if (landHeight > unknown10.y + LAND_HEIGHT_NONE) {
      return true;
    }
  }
  return false;
}

bool BattleCharacter::IsSuperArmor(int level) const {
  if (IsNoDamage(false)) {
    return true;
  }
  if (unknown10e4 > 0.0f) {
    return true;
  }
  if (SpAttackManager::get()->IsRegister(unknown118)) {
    return true;
  }
  if (m_player) {
    if (ChainAttackManager::get()->IsProc(false)) {
      return true;
    }
    bool isEnable;
    bool isKakusei;
    IsKakuseiEffect(isEnable, isKakusei);
    if (isKakusei) {
      return true;
    }
  }

  const CharacterParameter *parameter = GetCharacterParameterConst(false);
  if (parameter == nullptr) {
    return false;
  }
  if (((parameter->m_perfectAvoid & 0x1d) | 2) != 3) {
    int armor = parameter->unknown5a9_5;
    if (armor == 0) {
      return false;
    }
    if (armor < level) {
      return false;
    }
  }
  return true;
}

void BattleCharacter::ActionCommandIdle(const ActionCommand *command, ActionCommand::COMMAND_TYPE type) {
  switch (type) {
  case ActionCommand::COMMAND_TYPE_MOVE_DIR_RATE:
    unknown003(this, command);
    break;
  case ActionCommand::COMMAND_TYPE_MOVE_LOCK:
    unknown004(this, static_cast<const act_cmd::MOVE_LOCK *>(command)->IsTurnTarget());
    break;
  case ActionCommand::COMMAND_TYPE_SUFFER_DAMAGE:
    unknown107e = ACTION_PHASE_UNKNOWN_3;
    unknown10c9_2 = false;
    break;
  case ActionCommand::COMMAND_TYPE_BLADE_GUARD: {
    const CharacterParameter *parameter = GetCharacterParameterConst(false);
    if (parameter == nullptr) {
      return;
    }
    if (parameter->m_invalidTarget) {
      SetAction(ACTION_ID_UNKNOWN_12, false, false, 0, 0.0f);
    }
    break;
  }
  default:
    break;
  }
}

void BattleCharacter::ActionCommandAttack(const ActionCommand *command, ActionCommand::COMMAND_TYPE type) {
  if (type == ActionCommand::COMMAND_TYPE_MOVE_DIR_RATE) {
    unknown003(this, command);
  } else if (type == ActionCommand::COMMAND_TYPE_MOVE_LOCK) {
    unknown004(this, true);
  } else if (type == ActionCommand::COMMAND_TYPE_ATTACK_REPELLED) {
    SetAction(ACTION_ID_UNKNOWN_13, false, false, 0, 0.0f);
  }
}

void BattleCharacter::ActionCommandArts(const ActionCommand *command, ActionCommand::COMMAND_TYPE type) {
  if (type == ActionCommand::COMMAND_TYPE_MOVE_DIR_RATE) {
    if (!unknown1105_17) {
      return;
    }
    unknown003(this, command);
  } else if (type == ActionCommand::COMMAND_TYPE_MOVE_LOCK) {
    if (!unknown1105_17) {
      return;
    }
    unknown004(this, static_cast<const act_cmd::MOVE_LOCK *>(command)->IsTurnTarget());
  } else if (type == ActionCommand::COMMAND_TYPE_ATTACK_REPELLED) {
    SetAction(ACTION_ID_UNKNOWN_13, false, false, 0, 0.0f);
  }
}

bool BattleCharacter::CheckAttackAction(ACTION_ID action, bool isIncludeBladeArts) const {
  switch (static_cast<u8>(action)) {
  case ACTION_ID_ATTACK:
  case ACTION_ID_ARTS:
  case ACTION_ID_SP_ATTACK:
    return true;
  case ACTION_ID_BLADE_ARTS: {
    const CharacterParameter *parameter = GetCharacterParameterConst(false);
    bool isInvalidTarget = false;
    if (parameter != nullptr) {
      isInvalidTarget = parameter->m_invalidTarget;
    }
    return isInvalidTarget | isIncludeBladeArts;
  }
  default:
    return false;
  }
}

f32 BattleCharacter::GetCriticalDamageRate(bool isUnknown) {
  f32 rate = isUnknown ? BtlSystemBalanceAccess::get()->unknown24 : BtlSystemBalanceAccess::get()->unknown20;
  f32 enhance = SatisfyEnhanceCondition(ENHANCE_CRITICAL_DAMAGE_UNKNOWN_2E, 0, 0.0f, nullptr, true);
  return NextFloat(rate + enhance);
}

void BattleCharacter::ResetBattleEnd_Attack() {
  unknown1103 = 0;
  unknown10d0 = -1.0f;
  unknown10e8 = 0.0f;
  unknown10ec = 0.0f;
  unknown10e0 = 0.0f;
  unknown10e4 = 0.0f;
  unknown10f0 = 0.7f;
  unknown10f6 = 0;
  unknown1102 = 0;
  unknown1105_0 = 0;
  unknown1106_5 = false;
}

u64 BattleCharacter::GetAnimSpecialProcData(ANIM_SPDATA_TYPE type) const {
  if (type != 0) {
    return 0;
  }
  if (unknowndc7_0 == ANIM_SPECIAL_PROC_TYPE_UNKNOWN_2) {
    return unknowndcc - unknowndc8;
  }
  return 0;
}

void BattleCharacter::ModifyMoveData_FloatWater() {
  if (unknownf90_3) {
    mm::Vec3 position;
    position.x = unknown10.x + unknownb0.x;
    position.y = unknown10.y + unknownb0.y;
    position.z = unknown10.z + unknownb0.z;
    f32 landHeight = GetPosLandHeight(position, unknownf0 + 5.0f, -1.0f);
    if (!(landHeight > position.y + LAND_HEIGHT_NONE) && unknown10c6_0 == 0) {
      unknownb0.x = 0.0f;
      unknownb0.z = 0.0f;
      GetLandHeight();
    }
  }
}

void BattleCharacter::ResetAllEffect() {
  CharacterParameter *parameter = GetCharacterParameter(false);
  if (parameter != nullptr) {
    parameter->unknown59e = 0;
    parameter->ClearAllBuff();
    parameter->unknownc0.unknown001();
    parameter->SetAngry(false, -1.0f);
  }
  unknown180.Reset(false);
  CancelButtonChallenge();
}

void BattleCharacter::InitializeAttackData(bool isReset) {
  if (isReset) {
    unknown10d4 = -1.0f;
    unknown10db = 0;
    unknown1103 = 0;
    unknown10cc = 0.0f;
    unknown10d0 = -1.0f;
    unknown10e4 = 0.0f;
    unknown10e8 = 0.0f;
    unknown10dc = 0;
    unknown10e0 = 0.0f;
    unknown10f0 = 0.7f;
    unknown10ec = 0.0f;
    unknown10f4 = 0;
    unknown10f6 = 0;
    unknown10fc = -1;
    unknown10fe = -1;
    unknown1100 = 0;
    unknown1101 = 0;
    unknown1102 = 0;
    unknown1104 = 0xff;
    unknown1105_0 = 0;
    unknown1106_3 = 0;
    unknown1106_5 = 0;
    unknown1106_6 = 0;
    unknown1106_7 = 0;
    unknown1107_2 = 0;
    unknown1107_3 = 1;
    unknown1107_4 = 0;
  }
  unknown10fa = -1;
  unknown10fb = 0;
  unknown10f8 = 0;
  unknown1106_0 = 0;
  unknown1106_1 = 0;
  unknown1107_0 = 0;
  unknown1105_17 = 0;
}

void BattleCharacter::CreateDeadEffect() {
  if (!m_player) {
    return;
  }
  const CharacterParameter *parameter = GetCharacterParameterConst(false);
  if (parameter != nullptr && parameter->m_invalidTarget) {
    return;
  }
  if (!gf::GfGameManager::isPlayEvent() && unknown130[0] == 0) {
    fw::EffectManager *manager = fw::EffectManager::get();
    u32 effect = manager->create("cmn_000_022", nullptr, true);
    if (effect != 0) {
      unknown130[0] = effect;
      f32 scale = GetCharacterParameterConst(false)->unknown28;
      manager->setParam(effect, unknown10, mm::Vec3(scale, scale, scale));
      unknownd0.z = 0.0f;
      unknownd0.x = 0.0f;
    }
  }
}

void BattleCharacter::NotifyDead(const gf::MsgObjectNotifyDead &message) {
  if (IsControlCharacter()) {
    BattleGlobal *global = BattleGlobal::get();
    if (!global->unknown65de_0) {
      DataManager::get()->SetPartyGauge(0, true);
    } else if (message.unknown08 == 4) {
      global->unknown65df_7 = true;
    } else if (message.unknown08 == 3) {
      if (message.unknown10 != nullptr && message.unknown10->unknown00 == 0x1000032) {
        const CharacterParameter *parameter = GetCharacterParameterConst(false);
        if (parameter != nullptr) {
          u32 maxHp = parameter->GetMaxHP();
          if (message.unknown10->unknown08 >= maxHp) {
            BattleGlobal::get()->unknown65df_7 = true;
          }
        }
      }
    }
  }
  unknown1106_6 = true;
}

void BattleCharacter::FinalizeArtsEffect(bool isFinalize) {
  if (unknown112c != 0) {
    CharacterParameter *parameter = GetCharacterParameter(false);
    parameter->SetArtsEffect(0);
    parameter->unknown5a9_5 = 0;
    if (isFinalize) {
      parameter->SetAdditionalEnhance(CharacterParameter::ADD_ENHANCE_TYPE_HP_RATE, 0);
      parameter->SetAdditionalEnhance(CharacterParameter::ADD_ENHANCE_TYPE_ETHER_POWER_RATE, 0);
    }
    parameter->ClearBuff(0x16, false);
    if (!Utility::IsThirdCharacter(unknown118)) {
      SetChrCollisionFlag(true);
    }
    unknown112c = 0;
  }
}

void BattleCharacter::SuccessChain(int chain, bool isUnknown) {
  if (ChainAttackManager::get()->IsProc(true)) {
    return;
  }
  if (isUnknown) {
    SatisfyEnhanceCondition(0xaf, GetCharacterParameter(false)->GetMaxHP(), 0.0f, nullptr, true);
  }
  f32 recast = SatisfyEnhanceCondition(0xb0, 0, 0.0f, nullptr, true);
  AddBladeArtsRecast(chain, recast);
  if (m_player) {
    bool isControl = IsControlCharacter();
    BattleGlobal::get()->AddChainCount(isControl);
    if (isControl) {
      UIManager::get()->DispChainUI(unknown118);
      PlaySystemSE(BTL_SE_HIT, 0x46, true, false);
    }
  }
  unknown10d0 = -1.0f;
}

void BattleCharacter::ManagerUpdate_Attack() {
  f32 dt = unknownf93_6 ? 0.0f : unknowncc0 * unknownf58;
  if (unknown10e0 > 0.0f) {
    unknown10e0 = __builtin_fmaxf(unknown10e0 - dt, 0.0f);
  }
  if (unknown10e4 > 0.0f) {
    unknown10e4 = __builtin_fmaxf(unknown10e4 - dt, 0.0f);
  }
  if (unknown10e8 > 0.0f) {
    unknown10e8 = __builtin_fmaxf(unknown10e8 - dt, 0.0f);
  }
  if (unknown10ec > 0.0f) {
    unknown10ec = unknown10ec - dt;
    if (unknown10ec <= 0.0f) {
      unknown10ec = 0.0f;
      unknown1105_0 = 0;
    }
  }
  unknown10f0 = dt + unknown10f0;
  if (unknown10f6 != 0) {
    int elapsed = static_cast<int>(dt * 4096.0f);
    unknown10f6 = unknown10f6 > elapsed ? unknown10f6 - elapsed : 0;
  }
  if (unknown10d0 > 0.0f) {
    unknown10d0 = unknown10d0 - m_systemDt;
  }
}

void BattleCharacter::PreManagerUpdate(const fw::UpdateInfo &) {
  unknown30 = unknown10;
  unknown10f4 = 0;
  unknown168 = nullptr;
  unknownf8c_1 = false;
  unknownf8c_3 = false;
  unknownf91_6 = false;
  if (!m_battle) {
    unknownf82 = 0;
    return;
  }
  bool isEnableBattle = BattleGlobal::get()->IsEnableBattle();
  gf::GF_OBJ_HANDLE *handle = unknown118;
  if (!isEnableBattle) {
    unknownf82 |= 1;
  } else {
    unknownf82 &= 0xfe;
  }
  if (unknownf82 & 2) {
    if (!BladeChangeManager::get()->IsRegister(handle)) {
      unknownf82 &= 0xfd;
    }
  }
  if (unknownf82 & 4) {
    if (!BladeArtsManager::get()->IsRegister(handle)) {
      unknownf82 &= 0xfb;
    }
  }
  if (unknownf82 & 8) {
    if (!SpAttackManager::get()->IsRegister(handle)) {
      unknownf82 &= 0xf7;
    }
  }
}

} // namespace btl
