#pragma once
#include "battle/arts_data.h"
#include "mm_mtl/fixed_vector.h"
#include "types.h"

namespace mm {
struct Vec3;
}

namespace gf {
struct GF_OBJ_HANDLE;
}

namespace btl {

constexpr int ATTRIBUTE_COUNT = 8;

class BattleCharacter;

class Utility {
public:
  enum BTL_ACHIEVE_COND {
    BTL_ACHIEVE_COND_BLADE_ARTS,
    BTL_ACHIEVE_COND_BLADE_ARTS_SP,
    BTL_ACHIEVE_COND_BLADE_N_ARTS,
    BTL_ACHIEVE_COND_KIZUNA,
    BTL_ACHIEVE_COND_FIGHT_COMBO,
    BTL_ACHIEVE_COND_FIGHT_COMBO_FINISH,
    BTL_ACHIEVE_COND_ELEMENTAL_COMBO,
    BTL_ACHIEVE_COND_ELEMENTAL_COMBO_FINISH,
    BTL_ACHIEVE_COND_CHAIN_ATTACK,
    BTL_ACHIEVE_COND_BUTTON_CHALLENGE,
    BTL_ACHIEVE_COND_HELP,
    BTL_ACHIEVE_COND_POT_GET,
    BTL_ACHIEVE_COND_AVOID,
    BTL_ACHIEVE_COND_GUARD,
    BTL_ACHIEVE_COND_BLADE_CHANGE,
    BTL_ACHIEVE_COND_RECOVER,
    BTL_ACHIEVE_COND_BACK_ATTACK,
  };

  // Inferred values passed by AI_CheckState callers; original enumerator names unknown.
  enum CHECK_STATE {
    CHECK_STATE_UNKNOWN_1 = 1,
    CHECK_STATE_UNKNOWN_3 = 3,
    CHECK_STATE_UNKNOWN_4 = 4,
    CHECK_STATE_UNKNOWN_5 = 5,
    CHECK_STATE_UNKNOWN_7 = 7,
    CHECK_STATE_UNKNOWN_8 = 8,
    CHECK_STATE_UNKNOWN_9 = 9,
  };

  enum CHECK_CHR_NUM_MODE {
    CHECK_CHR_NUM_MODE_ENEMY,
    CHECK_CHR_NUM_MODE_FRIEND,
    CHECK_CHR_NUM_MODE_ALL,
  };

  enum {
    CHARACTER_LIST_BATTLE = 0x01,
    CHARACTER_LIST_ALIVE = 0x02,
    CHARACTER_LIST_DEAD = 0x04,
    CHARACTER_LIST_EXCLUDE_BLADE = 0x08,
    CHARACTER_LIST_EXCLUDE_SELF = 0x10,
  };

  enum ATTACKER_TYPE {
    ATTACKER_TYPE_ENEMY,
    ATTACKER_TYPE_PARTY,
    ATTACKER_TYPE_PLAYER,
  };

  enum ENBOOK_TYPE {
    ENBOOK_TYPE_IN_BATTLE,
    ENBOOK_TYPE_BOSS_TIME,
    ENBOOK_TYPE_DEFEAT_ENEMY,
    ENBOOK_TYPE_DEAD_PLAYER,
  };

  static gf::GF_OBJ_HANDLE *GetLeader();
  static gf::GF_OBJ_HANDLE *GetControlCharacterHandle();
  static bool IsSatisfyCondition(int condition);
  static bool IsSatisfyConditionFld(int condition);
  static void AchieveCondition(BTL_ACHIEVE_COND condition, gf::GF_OBJ_HANDLE *handle, u32 value);
  static void GetArtsData(ARTS_DATA::DATA_TYPE type, int id, ARTS_DATA &data);
  static int GetArtsEnhance(const ARTS_SLOT_DATA &slot);
  static int GetArtsEnhance(const ARTS_DATA &data, int level);
  static int GetSeType(const char *packName);
  static int GetWeaponSeType(int weaponType);
  static int GetAttrSeType(int attribute);
  static int GetAntiAttribute(u32 attribute);
  static int GetElementalEffectID();
  static int GetUniteComboID(int attribute, int combo);
  static int GetCharacterList(mm::mtl::FixedVector<u8, 90> &list, gf::GF_OBJ_HANDLE *handle, CHECK_CHR_NUM_MODE mode, u32 flags);
  static int GetDamageAttackerType(gf::GF_OBJ_HANDLE *handle, gf::GF_OBJ_HANDLE *attacker, bool isFriendly);
  static int GetPartyID(gf::GF_OBJ_HANDLE *handle);
  static f32 GetDyingHpRate();
  static int GetResistantAttrMaxHP();
  static int GetTrustRank(int blade);
  static f32 GetBladeCoolTime(int blade);
  static bool IsEnableFieldBladeChange();
  static bool IsEnableDeadSequence();
  static bool IsEnableEventStart();
  static bool IsThirdCharacter(gf::GF_OBJ_HANDLE *handle);
  static bool IsBladeArtsBuff(int buff);
  static void SomebodyPraise(int id, bool isPartyPraise, bool isSpecialPraise);
  static void RegisterEnemyBook(ENBOOK_TYPE type, gf::GF_OBJ_HANDLE *handle, int value);
  static int AI_GetCharacterID(gf::GF_OBJ_HANDLE *handle);
  static int AI_GetCharacterAIID(gf::GF_OBJ_HANDLE *handle);
  static int AI_GetCharacterAIID(const BattleCharacter *character);
  static gf::GF_OBJ_HANDLE *AI_GetCharacterHandle(int aiId);
  static int AI_ConversionID_AItoBtl(int aiId);
  static int AI_ConversionID_BtltoAI(int id);
  static bool AI_IsValidID(int aiId);
  static void AI_GetObjectPosition(gf::GF_OBJ_HANDLE *handle, mm::Vec3 &position);
  static int AI_GetTargetCharacterID(gf::GF_OBJ_HANDLE *handle);
  static int AI_GetLinkCharacterID(gf::GF_OBJ_HANDLE *handle);
  static bool AI_IsFly(gf::GF_OBJ_HANDLE *handle);
  static f32 AI_GetVertDiff(gf::GF_OBJ_HANDLE *handle, gf::GF_OBJ_HANDLE *target);
  static int AI_GetEatTargetCharacterID(gf::GF_OBJ_HANDLE *handle, f32 range);
  static int AI_GetEnemyNum();
  static int AI_GetArtsEnemyNum();
  static int AI_GetArtsPlayerNum();
  static int AI_GetAutoAttackState(gf::GF_OBJ_HANDLE *handle);
  static int AI_GetPlayerNum();
  static int AI_GetAimCharacterNum(gf::GF_OBJ_HANDLE *handle);
  static int AI_GetFightComboID(gf::GF_OBJ_HANDLE *handle);
  static int AI_GetElementalComboID(gf::GF_OBJ_HANDLE *handle);
  static int AI_GetReactionID(gf::GF_OBJ_HANDLE *handle);
  static int AI_GetStamp(gf::GF_OBJ_HANDLE *handle);
  static int AI_GetStampCharacterID(int stamp);
  static f32 AI_GetHpPercent(gf::GF_OBJ_HANDLE *handle);
  static u32 AI_GetLevel(gf::GF_OBJ_HANDLE *handle);
  static int AI_GetAttribute(gf::GF_OBJ_HANDLE *handle);
  static int AI_GetRescueTargetCharacterID(gf::GF_OBJ_HANDLE *handle);
  static int AI_GetBackAttackerCharacterID(gf::GF_OBJ_HANDLE *handle);
  static bool AI_StartActionRescue(gf::GF_OBJ_HANDLE *handle, int aiId);
  static void AI_StartActionEscape(gf::GF_OBJ_HANDLE *handle);
  static bool AI_StartActionArts(gf::GF_OBJ_HANDLE *handle, int slot, int aiId);
  static bool AI_StartActionArtsByID(gf::GF_OBJ_HANDLE *handle, int arts, int aiId);
  static bool AI_StartActionAttack(gf::GF_OBJ_HANDLE *handle);
  static bool AI_StartActionBladeChange(gf::GF_OBJ_HANDLE *handle, int blade);
  static bool AI_IsAngry(gf::GF_OBJ_HANDLE *handle);
  static void AI_SetAngry(gf::GF_OBJ_HANDLE *handle, f32 time);
  static void AI_EndAngry(gf::GF_OBJ_HANDLE *handle);
  static bool AI_TurnTarget(gf::GF_OBJ_HANDLE *handle);
  static bool AI_TurnDirection(gf::GF_OBJ_HANDLE *handle, const mm::Vec3 &direction);
  static void AI_SetUseNavi(gf::GF_OBJ_HANDLE *handle, bool isUseNavi);
  static bool AI_StartMovePosition(gf::GF_OBJ_HANDLE *handle, const mm::Vec3 &position, f32 time);
  static bool AI_StartMoveTarget(gf::GF_OBJ_HANDLE *handle, int aiId, f32 distance, f32 time);
  static bool AI_StartMoveFront(gf::GF_OBJ_HANDLE *handle, int aiId, f32 distance, f32 time);
  static bool AI_StartMoveSide(gf::GF_OBJ_HANDLE *handle, int aiId, f32 distance, f32 time);
  static bool AI_StartMoveLeft(gf::GF_OBJ_HANDLE *handle, int aiId, f32 distance, f32 time);
  static bool AI_StartMoveRight(gf::GF_OBJ_HANDLE *handle, int aiId, f32 distance, f32 time);
  static bool AI_StartMoveBack(gf::GF_OBJ_HANDLE *handle, int aiId, f32 distance, f32 time);
  static bool AI_StartMoveFriendBack(gf::GF_OBJ_HANDLE *handle, f32 distance, f32 time);
  static bool AI_StartMoveSatellite(gf::GF_OBJ_HANDLE *handle, bool isClockwise, f32 frame);
  static bool AI_StartMoveBackward(gf::GF_OBJ_HANDLE *handle, f32 distance, f32 time);
  static bool AI_StartMovePosBackward(gf::GF_OBJ_HANDLE *handle, mm::Vec3 &position, f32 time);
  static void AI_MoveStop(gf::GF_OBJ_HANDLE *handle, bool isImmediate);
  static bool AI_IsDestruct(gf::GF_OBJ_HANDLE *handle);
  static int AI_GetTargetDirection(gf::GF_OBJ_HANDLE *handle, gf::GF_OBJ_HANDLE *target);
  static int AI_CheckCharacterNum(gf::GF_OBJ_HANDLE *handle, int direction, CHECK_CHR_NUM_MODE mode);
  static int AI_CheckHpRateCharacterNum(gf::GF_OBJ_HANDLE *handle, int percent, CHECK_CHR_NUM_MODE mode);
  static int AI_GetDeadCharacterNum(gf::GF_OBJ_HANDLE *handle, CHECK_CHR_NUM_MODE mode);
  static void AI_AddKizuna(gf::GF_OBJ_HANDLE *handle, int kizuna);
  static int AI_GetKizunaStage(gf::GF_OBJ_HANDLE *handle);
  static int AI_GetSpAttackLevel(gf::GF_OBJ_HANDLE *handle);
  static bool AI_StartSpAttack(gf::GF_OBJ_HANDLE *handle);
  static bool AI_IsEnableSpAttack(gf::GF_OBJ_HANDLE *handle, int level);
  static bool AI_IsBattleOpening(gf::GF_OBJ_HANDLE *handle);
  static bool AI_IsInAttackRange(gf::GF_OBJ_HANDLE *handle);
  static bool AI_CheckState(gf::GF_OBJ_HANDLE *handle, CHECK_STATE state, int value);
  static bool AI_IsEnableArts(gf::GF_OBJ_HANDLE *handle, int slot, int aiId);
  static bool AI_IsEnableArtsByID(gf::GF_OBJ_HANDLE *handle, int arts, int aiId);
  static int AI_SearchVoiceCodeCharacter(int voiceCode);
  static int AI_GetVoiceCode(gf::GF_OBJ_HANDLE *handle);
  static void AI_PlayVoice(gf::GF_OBJ_HANDLE *handle, int voice);
  static int AI_GetPlayVoiceID(gf::GF_OBJ_HANDLE *handle);
  static void AI_AddBladeArtsRecast(gf::GF_OBJ_HANDLE *handle, f32 recast, bool isRate);
  static void AI_SetBladeArtsRecastByLevel(gf::GF_OBJ_HANDLE *handle, int level, bool isDecrease);
  static bool AI_IsBuff(gf::GF_OBJ_HANDLE *handle, int buff);
  static bool AI_IsProcSpAttack();
  static int AI_GetBdatID(gf::GF_OBJ_HANDLE *handle);
  static bool AI_GetCharacterGimmickPoint(gf::GF_OBJ_HANDLE *handle, mm::Vec3 &position);
  static bool AI_GetGimmickPoint(mm::Vec3 &position, u32 gimmick);
  static void AI_SetScout(gf::GF_OBJ_HANDLE *handle, bool isScout);
  static void AI_SetSuperArmor(gf::GF_OBJ_HANDLE *handle, f32 time);
  static bool AI_IsEnableButtonChallenge();
  static bool AI_RegisterButtonChallenge(int challenge, bool isSuccess);
  static int AI_GetButtonChallengeResult();
  static int AI_EnemyKillNum();
  static bool AI_IsKillChild();
  static bool AI_IsBenkeiWeather();
};

} // namespace btl
