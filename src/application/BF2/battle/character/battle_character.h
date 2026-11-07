#pragma once
#include <stddef.h>

#include "battle/arts_data.h"
#include "battle/character/action_command.h"
#include "battle/character/character_parameter_battle.h"
#include "battle/character/elemental_combo_manager.h"
#include "gf/object/component/pc/gf_com_property_pc.h"
#include "mm_math/mm_quat.h"
#include "mm_math/mm_vec3.h"
#include "types.h"

namespace fw {
struct UpdateInfo;
}

namespace navi {
class NvmshAcObj;
} // namespace navi

namespace gf {
struct GF_OBJ_HANDLE;
class GfComEffectCaster;
class GfComProperty;
class GfObj;
class GfWeapon;
struct MsgObjectNotifyAsmCallback;
struct MsgObjectNotifyDamage;
struct MsgObjectNotifyDead;
} // namespace gf

enum BTL_SE {
  BTL_SE_HIT = 1,
};

namespace btl {

class GROUP_DATA;

class BattleCharacter {
public:
  enum ANIM_ID {};
  enum SLOW_PROC_ID {};
  enum DELAY_VOICE_TYPE {
    DELAY_VOICE_TYPE_NORMAL,
  };
  enum ALPHA_PROC_ID {};
  enum ANIM_SPDATA_TYPE {};
  enum BLADE_TIMER_MODE {};
  enum RIMBLOOM_ID {};
  enum ACTION_ID : u8 {
    ACTION_ID_UNKNOWN_0 = 0, // Inferred role: battle start, no action; original name unknown.
    ACTION_ID_IDLE = 1,
    ACTION_ID_UNKNOWN_2 = 2, // Inferred role: put out weapon; original name unknown.
    ACTION_ID_PUT_AWAY_WEAPON = 3,
    ACTION_ID_MOVE = 4,
    ACTION_ID_UNKNOWN_5 = 5, // Inferred role: kizuna max, paired with STATE_ID_BATTLE_KIZUNA_MAX; original name unknown.
    ACTION_ID_ATTACK = 6,
    ACTION_ID_ARTS = 7,
    ACTION_ID_BLADE_ARTS = 8,
    ACTION_ID_BLADE_CHANGE = 9,
    ACTION_ID_SP_ATTACK = 10,
    ACTION_ID_UNKNOWN_11 = 11, // Inferred role: support, paired with STATE_ID_BATTLE_SUPPORT; original name unknown.
    ACTION_ID_UNKNOWN_0E = 14, // Inferred role: lever gacha action; original name unknown.
    ACTION_ID_UNKNOWN_12 = 12, // Inferred role: set on a BLADE_GUARD action command; original name unknown.
    ACTION_ID_UNKNOWN_13 = 13, // Inferred role: set on an ATTACK_REPELLED action command; original name unknown.
    ACTION_ID_UNKNOWN_15 = 15, // Inferred role: knock back, paired with STATE_ID_BATTLE_KNOCK_BACK; original name unknown.
    ACTION_ID_UNKNOWN_16 = 16, // Inferred role: break, paired with STATE_ID_BATTLE_BREAK; original name unknown.
    ACTION_ID_UNKNOWN_17 = 17, // Inferred role: dead, paired with STATE_ID_BATTLE_DEAD; original name unknown.
    ACTION_ID_RESCUE = 18,
  };

  enum {
    DEAD_CHECK_IGNORE_CHAIN_ATTACK = 0x02,
  };

  enum {
    AI_FLAG_STOP = 0x02,
    AI_FLAG_STOP_FORCE = 0x04,
    AI_FLAG_SCOUT = 0x10,
  };

  enum MOVE_TYPE {
    MOVE_TYPE_TARGET = 0,
    MOVE_TYPE_FRONT = 2,
    MOVE_TYPE_SIDE = 3,
    MOVE_TYPE_LEFT = 4,
    MOVE_TYPE_RIGHT = 5,
    MOVE_TYPE_BACK = 6,
    MOVE_TYPE_BACKWARD = 7,
    MOVE_TYPE_SATELLITE_CLOCKWISE = 8,
    MOVE_TYPE_SATELLITE_COUNTERCLOCKWISE = 9,
    MOVE_TYPE_POSITION = 10,
  };

  struct CUSTOM_VOICE_DATA {
    u8 unknown00; // Inferred role: voice id; original member name unknown.
    u8 unknown01; // Inferred role: voice type; original member name unknown.
  };

  enum EFFECT_ID {
    EFFECT_ID_KIZUNA_LINK = 1,
    EFFECT_ID_KIZUNA_AURA = 2,
    EFFECT_ID_NUM = 13,
  };

  // Layout evidence: SetAnimationMoveDistance (0x7100028cc4) fills the four halves of an 8-byte entry at 0xce4, and
  // GetAnimationMoveDistance (0x7100028e14) matches the first, third and fourth ones and reads the second unsigned.
  struct ANIMATION_MOVE_DISTANCE_DATA {
    u16 unknown00; // Inferred role: animation id; original member name unknown.
    u16 unknown02; // Inferred role: distance in hundredths; original member name unknown.
    s16 unknown04; // Inferred role: first animation parameter; original member name unknown.
    s16 unknown06; // Inferred role: second animation parameter; original member name unknown.
  };

  struct ALPHA_PROC_DATA {
    f32 unknown00; // Inferred role: target alpha rate; original member name unknown.
    f32 unknown04; // Inferred role: start alpha rate; original member name unknown.
    f32 unknown08; // Inferred role: alpha rate; original member name unknown.
    f32 unknown0c; // Inferred role: time; original member name unknown.
    f32 unknown10; // Inferred role: duration; original member name unknown.
  };

  struct SLOW_PROC_DATA {
    f32 unknown00; // Inferred role: slow rate; original member name unknown.
    f32 unknown04; // Inferred role: time; original member name unknown.
    f32 unknown08; // Inferred role: counter; original member name unknown.
  };

  struct AI_ACTION_DATA {
    u8 unknown00;  // Inferred role: action; original member name unknown.
    int unknown04; // Inferred role: target; original member name unknown.
  };

  struct HEALER_ENHANCE_PARAM {
    BattleCharacter *unknown00; // Inferred role: the other character; original member name unknown.
    bool unknown08;             // Inferred role: restricts the enhance to the healer; original member name unknown.
  };

  struct HEAL_TARGET_ENHANCE_PARAM {
    BattleCharacter *unknown00; // Inferred role: the other character; original member name unknown.
    bool unknown08;             // Inferred role: selects the second enhance; original member name unknown.
  };

  struct DIRECT_DAMAGE_DATA {
    BattleCharacter *unknown00; // Inferred role: healer; original member name unknown.
    int unknown08;              // Inferred role: damage type; original member name unknown.
    u32 unknown0c;              // Inferred role: value; original member name unknown.
    u8 unknown10;               // Inferred role: heal flag; original member name unknown.
    u8 unknown11;               // Role and original member name unknown.
    u8 unknown12;               // Role and original member name unknown.
  };

  struct AI_MOVE_PARAM {
    AI_MOVE_PARAM() : unknown00(0.0f), unknown04(0.0f), unknown08(MOVE_TYPE_TARGET), unknown10(nullptr), unknown18(reinterpret_cast<gf::GF_OBJ_HANDLE *>(-1)) {}

    f32 unknown00;                // Inferred role: distance; original member name unknown.
    f32 unknown04;                // Inferred role: time; original member name unknown.
    MOVE_TYPE unknown08;          // Inferred role: move type; original member name unknown.
    const mm::Vec3 *unknown10;    // Inferred role: position; original member name unknown.
    gf::GF_OBJ_HANDLE *unknown18; // Inferred role: target handle; original member name unknown.
  };

  BattleCharacter();
  virtual ~BattleCharacter();
  virtual void BeforeUpdate(const fw::UpdateInfo &updateInfo);
  virtual void Update();
  virtual void AfterUpdate();
  virtual void SetAnimationCallback(ANIM_ID animId, int, int, const char *);
  virtual void OnLandCallback(bool, bool);
  virtual void LandingCallback();
  virtual void BoundCallback();
  virtual void SetActionCallback(ACTION_ID actionId);

  void ResetCustomVoiceData();
  void SetupCustomVoiceData();
  void ResetAnimationMoveDistance();
  void ResetAlphaAnimation();
  void InitializeEnhanceProcData(bool isFinalizeArtsEffect);
  const CharacterParameter *GetCharacterParameterConst(bool) const;
  bool IsPlayer() const;
  bool IsBlade() const;
  bool IsEnemy() const;
  bool IsDriver() const;
  void NotifySufferDamage(const gf::MsgObjectNotifyDamage &message);
  void NotifyBattleStart(bool isStart);
  void AsmCallback(const gf::MsgObjectNotifyAsmCallback &message);
  void NotifyBattleEnd();
  void JoinProc(f32 rate, bool isScout);
  void SetWeaponType(int weaponType);
  void SetCollisionPriority(int priority);
  void SetChrCollisionFlag(bool isEnable);
  void WriteBackChrCollisionFlag();
  bool IsOnSlide() const;
  void TurnObject(gf::GF_OBJ_HANDLE *handle, bool, bool);
  void ActionRun();
  void LockTarget();
  bool CheckPermitArts(u32 attack);
  void FluctuateTrust(int type, int level);
  void IsKakuseiEffect(bool &isEnable, bool &isKakusei) const;
  bool IsGuardReflect(int guard);
  bool StartBladeSupport(int support);
  void PostManagerUpdate(const fw::UpdateInfo &updateInfo);
  void UpdateHateRingData();
  void AchieveCondition(int condition, u32 value) const;
  void SetAttackTarget(gf::GF_OBJ_HANDLE *handle, bool isNotify);
  void CheckWeaponDistance(gf::GF_OBJ_HANDLE *handle);
  f32 GetArtsAttackDistance(const ARTS_DATA &data) const;
  void NotifyBuffStart(int buff);
  void CreateBuffEffect(int buff, bool);
  void FinishLaunchRotate();
  bool IsPlayAnimation(ANIM_ID animId, int, int) const;
  const char *GetAnimationStateName(ANIM_ID animId, int, int) const;
  bool DeleteRegisterCommand(ActionCommand::COMMAND_TYPE type);
  void SetUpperCollisionPriority(int priority);
  void StopEnd();
  bool StartBackHome();
  void SetPosition(const mm::Vec3 &position, bool);
  void NotifyBuddyWarp();
  void SetWarpPostureByDir(const mm::Vec3 &direction);
  bool IsInputStick(mm::Vec3 *stick, bool isCamera) const;
  inline bool IsBanOperate() const;
  bool IsInputButton(gf::pc::InputID input) const;
  bool IsInputButtonDirect(gf::pc::InputID input) const;
  void RegisterBladeGuardCommand();
  void SetExitBattle(bool, bool);
  void ActionCommandEndBladeArts();
  void SetAnimation(ANIM_ID animId, bool, int, int, bool, bool);
  void StartFullBurst(int value);
  CharacterParameter *GetCharacterParameter(bool);
  bool IsControlCharacter() const;
  bool AI_Warp(ACTION_ID action);
  bool CheckWarp(ACTION_ID action, mm::Vec3 &position, bool &isDirect);
  void SetWarp(const mm::Vec3 &position, bool isDirect, f32 time);
  void SetLimitHP(int limit);
  bool IsControlCharacterGroup() const;
  bool IsDead(u32 flags) const;
  bool IsMoveHealPot() const;
  void NotifyAngryEnd();
  void DispEnhanceName(const CharacterParameter::ENHANCE_DATA &data) const;
  void DispEnhanceNameDirect(int text, bool flag) const;
  int GetArtsAttr(const ARTS_DATA &data, u32 attack) const;
  f32 GetAnimationMoveLength(f32, f32, bool);
  bool GetObjectNearPosition(gf::GF_OBJ_HANDLE *handle, gf::GF_OBJ_HANDLE *target, mm::Vec3 &position, mm::Vec3 *, mm::Vec3 *) const;
  void CalcLocomotionMoveRate(f32 maxRate);
  bool IsNoDamage(bool) const;
  bool GetTargetPosDirection(const mm::Vec3 &position, mm::Vec3 &direction, bool, bool) const;
  bool GetTargetDirection(mm::Vec3 &direction, bool, bool, gf::GF_OBJ_HANDLE *target) const;
  void AddTrust(u32 point, int level);
  void StopStart(f32 time, bool isSyncBuddy);
  bool CheckPowerCharge(u32 attack);
  void NotifyBuddyFightCombo(gf::GF_OBJ_HANDLE *handle, int fightCombo);
  void NotifyBuddySufferFightCombo(int id, int fightCombo);
  void NotifyElementalCombo(gf::GF_OBJ_HANDLE *handle, int combo, bool isUnite);
  void NotifySufferElementalCombo(int id, int combo);
  void NotifyBuffEnd(int buff, f32 rest, bool isEnd);
  void NotifyAngryStart();
  void NotifyBackAttack(int id);
  void NotifyChangeAttr(gf::GF_OBJ_HANDLE *handle, u32 attribute, u32 oldAttribute);
  void NotifyDropHealpot();
  void NotifyEnemyAngry(gf::GF_OBJ_HANDLE *handle);
  void NotifyFriendDead(int id);
  void NotifyKillSpAttack();
  void NotifyLostHateTarget(int);
  void NotifyGetHateTarget(int id, bool isFirst);
  void ResetHate();
  void SetAction(ACTION_ID actionId, bool, bool, int, f32);
  u8 CheckAttackPosition(gf::GF_OBJ_HANDLE *handle, bool, bool *) const;
  bool IsBattle() const;
  void DeleteKizunaAuraEffect();
  void DeleteKizunaLinkEffect();
  int GetBuffEffectIndex(int buff) const;
  void DeleteEffect(int effect, int fadeFrame);
  void CancelButtonChallenge();
  bool IsDispWeaponAlpha(int alpha) const;
  bool IsBoss() const;
  bool IsNamed() const;
  bool IsBossOrNamed() const;
  bool IsMove() const;
  bool IsBound() const;
  void SetRimBloom(RIMBLOOM_ID id, bool isEnable);
  int GetGroupIndex() const;
  void AddUseWeapon();
  void StartBladeTimer(BLADE_TIMER_MODE mode);
  void EndBladeTimer(BLADE_TIMER_MODE mode);
  bool PlaySE(BTL_SE se, int number, const mm::Vec3 *position, bool isFollow);
  void PlayHitAttrSE(int attribute, const mm::Vec3 *position);
  void PlayHitBulletSE(int attribute, const mm::Vec3 *position);
  int GetIKData(f32 &weight, f32 &height) const;
  void SetupPosDir();
  const GROUP_DATA *GetGroupDataConst() const;
  bool IsEnableSpeakVoice() const;
  gf::GF_OBJ_HANDLE *GetBuddyTargetObjectHandle() const;
  int GetTrustRank(int index) const;
  bool IsPriorityReaction(int left, int right) const;
  int GetArtsSlotIndex(int arts) const;
  bool IsBladeArtsEnd() const;
  bool IsPerfectAvoid() const;
  gf::GF_OBJ_HANDLE *GetWeaponObjectHandle(bool isAlternate) const;
  void ResetRequestPermit();
  gf::GfComProperty *GetComPropertyConst() const;
  gf::GfComEffectCaster *GetEffectCaster();
  gf::GfWeapon *GetWeaponInfo() const;
  gf::GfComEffectCaster *GetWeaponEffectCaster();
  int GetDriverType() const;
  bool IsAnimationEnd() const;
  const void *GetAnimationAppendix(const char *) const;
  const char *GetNowAnimationStateName() const;
  bool IsAnimationState(const char *) const;
  f32 GetAnimationTime() const;
  f32 GetAnimationFrame() const;
  f32 GetAnimationLengthFrame() const;
  f32 GetAnimationLengthSec() const;
  bool IsEnableTurnAI() const;
  void TurnDirection(const mm::Vec3 &direction, bool, bool);
  void TurnTarget(bool, bool, bool);
  int GetKizunaStage(f32 *rate) const;
  int GetHateEnemyNum() const;
  int GetSeType(int type, int attr) const;
  void AddRecast(f32 recast, bool isRate, int slot);
  bool SetAngry(bool isAngry, f32 time);
  void RegisterDelayVoice(DELAY_VOICE_TYPE type, f32 time, int voice, int group, int priority);
  bool IsAngry() const;
  void UpdateBuffEffect();
  void UpdateBuffEffect(int buff);
  void UpdateLaunchEffect();
  f32 GetLaunchEffectScale() const;
  void UpdateTargetLockArrow();
  void DispElementalEffectUI(int effect);
  void SlowProc();
  void ScoutProc();
  f32 GetChainSec();
  f32 SatisfyEnhanceCondition(int enhance, int, f32, void *, bool);
  void ResetChangeActionStopSeData();
  void ReturnBladeChange();
  void ReturnBladeArts();
  void FinalizeArtsEffect(bool);
  ActionCommand *GetFreeCommand();
  ActionCommand *GetActionCommand(ACTION_ID action, bool isSet);
  bool IsRegisterCommand(ActionCommand::COMMAND_TYPE type) const;
  void ActionCommandRun(const ActionCommand *command, ActionCommand::COMMAND_TYPE type);
  void ActionCommandBladeArts(const ActionCommand *command, ActionCommand::COMMAND_TYPE type);
  void HealDirect(u32 value, int, BattleCharacter *healer);
  void HealByRate(u32 value, f32 rate, int, BattleCharacter *healer);
  void HealHpRate(f32 rate, int, BattleCharacter *healer);
  void AddPartyGauge(f32 value);
  void FluctuatePartyGauge(int);
  bool IsInAttackRange(int slot, f32 *distance) const;
  int IsEnableArts(int, int slot, int, gf::GF_OBJ_HANDLE *target) const;
  bool IsRescue() const;
  void AddKizuna(f32 kizuna);
  void AddKizunaStage(int stage);
  void SetKizunaStage(int stage);
  void SetSpAttackRecastByLevel(int level, bool);
  void SetSuperArmorTime(f32 time);
  void AddSlotRecast(int slot, f32 recast, bool isRate);
  bool AI_Rescue(ACTION_ID actionId, int target);
  bool AI_BladeChange(ACTION_ID actionId, int blade);
  bool AI_SpAttack(ACTION_ID actionId, bool, bool);
  bool AI_Arts(ACTION_ID actionId, int slot);
  bool AI_Attack(ACTION_ID actionId, bool);
  bool IsProcAI(bool) const;
  bool AI_Move(ACTION_ID actionId, bool, const AI_MOVE_PARAM *param);
  bool MoveStop(bool isForce);
  AI_ACTION_DATA GetAIActionData() const { return unknown1160; }
  void SetScout(bool isScout);
  bool IsScout() const;
  gf::GF_OBJ_HANDLE *GetRescueTarget() const;
  void BattleEndInitialize();
  void PreManagerUpdate(const fw::UpdateInfo &updateInfo);
  void ManagerUpdate(const fw::UpdateInfo &updateInfo);
  void ResetHateRingData();
  void ResetDelayVoice();
  gf::GfObj *GetObject() const;
  gf::GF_OBJ_HANDLE *GetControlCharacterHandle() const;
  gf::GfObj *GetObject(gf::GF_OBJ_HANDLE *handle) const;
  bool IsEnableBackHome() const;
  bool IsDisp() const;
  int GetWeaponType(bool isAsm) const;
  const CUSTOM_VOICE_DATA *GetCustomVoiceData(int voice, int *index) const;
  bool IsCustomVoice(int voice) const;
  void SetCustomVoiceEndFlag(int voice, bool isEnd);
  bool PlayCustomVoice(int voice);
  void ClearCustomVoiceEndFlag(u32 flags);
  int GetWeaponSeType() const;
  void GetMoveDataPerSec(mm::Vec3 &moveData) const;
  void SetAnimationSpeed(f32 speed);
  f32 GetAnimationMoveDistance();
  void SetIKData(const ARTS_DATA &data);
  void NotifyStrikeDamage(int);
  void NotifyDirectDamage(const DIRECT_DAMAGE_DATA &data);
  void ResetAlphaRate(int alpha);
  void UpdateEffectPositionScale(int effect, const char *node, f32 scale);
  f32 GetSyncCount(bool isSlow) const;
  f32 GetDt(bool isSlow, bool isStopCheck) const;
  void RegisterSlow(SLOW_PROC_ID slow, f32 rate, f32 time);
  void DeleteSlow(SLOW_PROC_ID slow);
  int GetAlphaSetID(ALPHA_PROC_ID alpha);
  bool IsPause() const;
  f32 GetSystemDt() const;
  f32 GetSystemSyncCount() const;
  bool IsEnableBladeChangeAction(ACTION_ID actionId) const;
  void EnhanceProc();
  bool IsEnableBattleFace() const;
  void NotifyHateElse();
  void SetBladeSupportCondition(int condition);
  void AddIdea(u32 idea, u32 point);
  void SetNoDamageTime(f32 time);
  void SetPerfectAvoidTime(f32 time);
  f32 GetFriendEnhanceRate(int enhance, bool) const;
  f32 GetHealerEnhanceRate(HEALER_ENHANCE_PARAM &param) const;
  f32 GetHealTargetEnhanceRate(HEAL_TARGET_ENHANCE_PARAM &param) const;
  f32 GetGuardRate() const;
  bool GetReactionBlowParam(int reaction, int level, f32 &horizontal, f32 &vertical) const;
  f32 GetArtsDamageRate(const ARTS_DATA::LEVEL_DATA &level) const;
  f32 ApplyResistantAttr(f32 damage, int attribute) const;
  f32 GetArtsDamageRate(const ARTS_DATA &data, int level) const;
  u32 CreateElementalComboEffect(int combo);
  void CreateElementalEffect(int effect);
  void EnhanceTimerProc();
  void StopAI(f32 time);
  bool CheckRandFriendEnhanceRate(int enhance) const;
  void ResetBladeSupportConditionFlag();
  void LeverReduceProc();
  void UpdateInvincible();
  bool IsImmortal(bool, bool *);
  bool CheckForceExit(ACTION_ID action);
  void ResetActionCommand();
  void ActionCommandEndRun();
  void UpdateCulling();
  void ActionCommandIdle(const ActionCommand *command, ActionCommand::COMMAND_TYPE type);
  void ActionCommandAttack(const ActionCommand *command, ActionCommand::COMMAND_TYPE type);
  void ActionCommandArts(const ActionCommand *command, ActionCommand::COMMAND_TYPE type);
  void ActionCommandEndIdle();
  void ActionCommandPutOutWeapon(const ActionCommand *command, ActionCommand::COMMAND_TYPE type);
  void ActionCommandEndPutOutWeapon();
  void ActionCommandPutAwayWeapon(const ActionCommand *command, ActionCommand::COMMAND_TYPE type);
  void ActionCommandEndPutAwayWeapon();
  void ActionCommandKizunaMax(const ActionCommand *command, ActionCommand::COMMAND_TYPE type);
  void ActionCommandEndKizunaMax();
  void ActionCommandEndAttack();
  void ActionCommandEndArts();
  void ActionCommandBladeChange(const ActionCommand *command, ActionCommand::COMMAND_TYPE type);
  void ActionCommandEndBladeChange();
  void ActionCommandSpAttack(const ActionCommand *command, ActionCommand::COMMAND_TYPE type);
  void ActionCommandEndSpAttack();
  void ActionCommandSupport(const ActionCommand *command, ActionCommand::COMMAND_TYPE type);
  void ActionCommandEndSupport();
  void ActionCommandBladeGuard(const ActionCommand *command, ActionCommand::COMMAND_TYPE type);
  void ActionCommandEndBladeGuard();
  void ActionCommandRepelled(const ActionCommand *command, ActionCommand::COMMAND_TYPE type);
  void ActionCommandEndRepelled();
  void ActionCommandDown(const ActionCommand *command, ActionCommand::COMMAND_TYPE type);
  void ActionCommandEndDown();
  void ActionCommandKnockBack(const ActionCommand *command, ActionCommand::COMMAND_TYPE type);
  void ActionCommandEndKnockBack();
  void ActionCommandBreak(const ActionCommand *command, ActionCommand::COMMAND_TYPE type);
  void ActionCommandEndBreak();
  void ActionCommandDead(const ActionCommand *command, ActionCommand::COMMAND_TYPE type);
  void ActionCommandEndDead();
  void ActionCommandRescue(const ActionCommand *command, ActionCommand::COMMAND_TYPE type);
  void ActionCommandEndRescue();
  bool CheckAttackAction(ACTION_ID action, bool isIncludeBladeArts) const;
  f32 GetCriticalDamageRate(bool isUnknown);
  void ResetBattleEnd_Attack();
  u64 GetAnimSpecialProcData(ANIM_SPDATA_TYPE type) const;
  bool IsSuperArmor(int level) const;
  bool IsSlide(bool isIgnoreBladeChange) const;
  f32 GetLandHeight() const;
  bool IsEnableRecastSpAttack() const;
  void WarpMirrorCopy();
  bool SetBlow(const mm::Vec3 &direction, f32 horizontalRate, f32 verticalRate, f32 time);
  f32 GetPosLandHeight(const mm::Vec3 &position, f32, f32) const;
  void ModifyMoveData_FloatWater();
  int IsActBladeArts(int arts, bool) const;
  bool CheckInvalidSpAttack();
  void ManagerUpdate_Attack();
  void ArtsGravityProc(const ARTS_DATA &arts, int level);
  void SuccessChain(int chain, bool isUnknown);
  void AddBladeArtsRecast(int, f32);
  void NotifyDead(const gf::MsgObjectNotifyDead &message);
  void CreateDeadEffect();
  void InitializeAttackData(bool isReset);
  void ResetAllEffect();
  bool SetAnimationNoComplement(ANIM_ID animId, int, int, bool);
  void StopSE(BTL_SE se, int number);
  void PlaySystemSE(BTL_SE se, int number, bool, bool);
  bool CheckRandSatisfyEnhanceCondition(int enhance, int, f32, void *);
  void RecoverNextBladeArtsRecast();
  bool IsInvalidDead(bool isIgnoreBattle) const;

  // Inferred role: sets the stop time to the longest of the current and the new one; original name unknown.
  void unknown001(f32 time) {
    f32 stop;
    if (time <= 0.0f) {
      stop = 0.0f;
    } else {
      stop = unknownccc > time ? unknownccc : time;
    }
    unknownccc = stop;
  }

  u8 _08[0x10 - 0x08];
  mm::Vec3 unknown10; // Inferred role: position; original member name unknown.
  mm::Vec3 unknown20; // Inferred role: warp position; original member name unknown.
  mm::Vec3 unknown30; // Inferred role: previous position; original member name unknown.
  u8 _40[0x50 - 0x40];
  mm::Vec3 unknown50; // Inferred role: direction; original member name unknown.
  mm::Vec3 unknown60; // Role and original member name unknown.
  mm::Vec3 unknown70; // Role and original member name unknown.
  u8 _80[0xa0 - 0x80];
  mm::Quat unknowna0; // Inferred role: warp posture; original member name unknown.
  mm::Vec3 unknownb0; // Inferred role: move data; original member name unknown.
  u8 _c0[0xd0 - 0xc0];
  mm::Vec3 unknownd0; // Role and original member name unknown.
  f32 unknowne0;      // Role and original member name unknown.
  f32 unknowne4;      // Role and original member name unknown.
  u8 _e8[0xf0 - 0xe8];
  f32 unknownf0; // Role and original member name unknown.
  f32 unknownf4; // Inferred role: land height; original member name unknown.
  u8 _f8[0x110 - 0xf8];
  f32 unknown110;                // Role and original member name unknown.
  f32 unknown114;                // Role and original member name unknown.
  gf::GF_OBJ_HANDLE *unknown118; // Inferred role: object handle; original member name unknown.
  gf::GF_OBJ_HANDLE *unknown120; // Inferred role: target handle; original member name unknown.
  gf::GF_OBJ_HANDLE *unknown128; // Inferred role: second target handle; original member name unknown.
  u32 unknown130[EFFECT_ID_NUM]; // Inferred role: effects; original member name unknown.
  u8 _164[0x168 - 0x164];
  mutable const CharacterParameter *unknown168; // Inferred role: parameter; original member name unknown.
  navi::NvmshAcObj *unknown170;                 // Inferred role: navi object; original member name unknown.
  u8 _178[0x180 - 0x178];
  ElementalComboManager unknown180; // Inferred role: elemental combo manager; original member name unknown.
  char unknownc30[0x28];            // Inferred role: default effect node name; original member name unknown.
  char unknownc58[0x10];            // Inferred role: voice code; original member name unknown.
  u8 _c68[0xcb0 - 0xc68];
  int unknowncb0; // Inferred role: first animation parameter (SetAnimation); original member name unknown.
  int unknowncb4; // Inferred role: second animation parameter (SetAnimation); original member name unknown.
  f32 m_animationSpeed;
  u8 _cbc[0xcc0 - 0xcbc];
  f32 unknowncc0; // Inferred role: delta time; original member name unknown.
  f32 m_systemDt;
  int unknowncc8; // Inferred role: bdat id; original member name unknown.
  f32 unknownccc; // Inferred role: stop time; original member name unknown.
  f32 unknowncd0; // Inferred role: blade timer; original member name unknown.
  f32 unknowncd4; // Inferred role: target lock arrow timer; original member name unknown.
  u8 _cd8[0xcdc - 0xcd8];
  f32 unknowncdc;                              // Role and original member name unknown.
  f32 unknownce0;                              // Role and original member name unknown.
  ANIMATION_MOVE_DISTANCE_DATA unknownce4[20]; // Inferred role: animation move distances; original member name unknown.
  u8 _d84[0xd98 - 0xd84];
  f32 unknownd98; // Inferred role: blow time; original member name unknown.
  u8 _d9c[0xdb4 - 0xd9c];
  u8 : 7;
  u8 unknowndb4_7 : 1; // Role and original member name unknown.
  u8 _db5[0xdb8 - 0xdb5];
  f32 unknowndb8;       // Inferred role: IK weight; original member name unknown.
  f32 unknowndbc;       // Inferred role: IK height; original member name unknown.
  u32 unknowndc0_0 : 7; // Inferred role: IK id; original member name unknown.
  u32 unknowndc0_7 : 4; // Inferred role: IK start number; original member name unknown.
  u32 unknowndc1_3 : 8; // Inferred role: IK start frame; original member name unknown.
  u32 unknowndc2_3 : 4; // Inferred role: IK end number; original member name unknown.
  u32 unknowndc2_7 : 8; // Inferred role: IK end frame; original member name unknown.
  u32 unknowndc3_7 : 1; // Inferred role: IK flag; original member name unknown.
  u8 _dc4[0xdc7 - 0xdc4];
  u8 unknowndc7_0 : 3; // Inferred role: animation special proc type; original member name unknown.
  u8 : 5;
  u32 unknowndc8; // Inferred role: animation special proc start; original member name unknown.
  u32 unknowndcc; // Inferred role: animation special proc end; original member name unknown.
  s16 unknowndd0; // Inferred role: delay voice time; original member name unknown.
  s16 unknowndd2; // Inferred role: delay voice priority; original member name unknown.
  s16 unknowndd4; // Inferred role: delay voice group; original member name unknown.
  u8 unknowndd6;  // Inferred role: delay voice id; original member name unknown.
  u8 _dd7[0xdd8 - 0xdd7];
  ALPHA_PROC_DATA unknowndd8[17]; // Inferred role: alpha procs; original member name unknown.
  f32 unknownf2c[7];              // Inferred role: alpha rates; original member name unknown.
  u8 _f48[0xf4c - 0xf48];
  SLOW_PROC_DATA unknownf4c[1];    // Inferred role: slows; original member name unknown.
  f32 unknownf58;                  // Inferred role: slow rate; original member name unknown.
  u32 unknownf5c;                  // Inferred role: custom voice flags; original member name unknown.
  u32 unknownf60;                  // Inferred role: custom voice end flags; original member name unknown.
  CUSTOM_VOICE_DATA unknownf64[3]; // Inferred role: custom voices; original member name unknown.
  s16 unknownf6a;                  // Inferred role: parameter id; original member name unknown.
  s16 unknownf6c;                  // Inferred role: saved collision priority; original member name unknown.
  s16 unknownf6e;                  // Inferred role: saved object collision priority; original member name unknown.
  u8 _f70[0xf72 - 0xf70];
  u16 unknownf72; // Inferred role: effect scale rate; original member name unknown.
  u8 _f74[0xf76 - 0xf74];
  u16 m_driverType : 3;
  u16 : 6;
  u16 unknownf77_1 : 3;  // Inferred role: blade timer mode; original member name unknown.
  u16 unknownf77_4 : 4;  // Inferred role: SE attribute; original member name unknown.
  u16 unknownf78_0 : 10; // Inferred role: SE type; original member name unknown.
  u16 : 6;
  u16 unknownf7a; // Inferred role: rim bloom flags; original member name unknown.
  u8 unknownf7c;  // Inferred role: bound action id; original member name unknown.
  s8 unknownf7d;  // Inferred role: id; original member name unknown.
  s8 unknownf7e;  // Inferred role: AI id; original member name unknown.
  s8 unknownf7f;  // Inferred role: weapon type; original member name unknown.
  u8 _f80[0xf82 - 0xf80];
  u8 unknownf82; // Role and original member name unknown.
  u8 unknownf83; // Inferred role: camera fade disable for battle; original member name unknown.
  u8 _f84[0xf86 - 0xf84];
  u8 unknownf86; // Role and original member name unknown.
  u8 _f87[0xf8b - 0xf87];
  u64 unknownf8b_0 : 3; // Inferred role: voice type; original member name unknown.
  u64 : 5;
  mutable u64 unknownf8c_0 : 1; // Inferred role: cached control character result; original member name unknown.
  mutable u64 unknownf8c_1 : 1; // Inferred role: control character result is cached; original member name unknown.
  mutable u64 unknownf8c_2 : 1; // Inferred role: cached control character group result; original member name unknown.
  mutable u64 unknownf8c_3 : 1; // Inferred role: control character group result is cached; original member name unknown.
  u64 : 1;
  u64 unknownf8c_5 : 1; // Role and original member name unknown.
  u64 : 1;
  u64 m_player : 1;
  u64 m_battle : 1;
  u64 : 2;
  u64 unknownf8d_3 : 1; // Role and original member name unknown.
  u64 : 1;
  u64 unknownf8d_5 : 1; // Inferred role: warp pending; original member name unknown.
  u64 unknownf8d_6 : 1; // Role and original member name unknown.
  u64 unknownf8d_7 : 1; // Role and original member name unknown.
  u64 unknownf8e_0 : 1; // Role and original member name unknown.
  u64 : 1;
  u64 unknownf8e_2 : 1; // Role and original member name unknown.
  u64 : 1;
  u64 unknownf8e_4 : 1; // Inferred role: event flag; original member name unknown.
  u64 unknownf8e_5 : 1; // Role and original member name unknown.
  u64 unknownf8e_6 : 1; // Role and original member name unknown.
  u64 : 2;
  u64 unknownf8f_1 : 1; // Role and original member name unknown.
  u64 : 5;
  u64 unknownf8f_7 : 1; // Inferred role: fly flag; original member name unknown.
  u64 : 1;
  u64 unknownf90_1 : 1; // Role and original member name unknown.
  u64 : 1;
  u64 unknownf90_3 : 1; // Inferred role: bound flag; original member name unknown.
  u64 unknownf90_4 : 1; // Inferred role: enable character collision; original member name unknown.
  u64 unknownf90_5 : 1; // Inferred role: character collision needs write back; original member name unknown.
  u64 : 1;
  u64 unknownf90_7 : 1; // Role and original member name unknown.
  u64 unknownf91_0 : 1; // Role and original member name unknown.
  u64 : 2;
  u64 unknownf91_3 : 1; // Role and original member name unknown.
  u64 unknownf91_4 : 1; // Role and original member name unknown.
  u64 : 1;
  u64 unknownf91_6 : 1; // Role and original member name unknown.
  u64 : 2;
  u64 unknownf92_1 : 1; // Role and original member name unknown.
  u64 : 12;
  u64 unknownf93_6 : 1; // Inferred role: stop flag; original member name unknown.
  u64 : 12;
  u64 unknownf95_3 : 1; // Inferred role: cached invincible state; original member name unknown.
  u64 unknownf95_4 : 1; // Inferred role: invincible state is cached; original member name unknown.
  u64 : 18;
  u64 : 1;
  u64 : 24;
  u8 _f9b[0xfb0 - 0xf9b];
  ActionCommand unknownfb0[10]; // Inferred role: action commands; original member name unknown.
  int unknown1078;              // Inferred role: command count; original member name unknown.
  u8 unknown107c;               // Inferred role: action id; original member name unknown.
  u8 _107d[0x107e - 0x107d];
  u16 unknown107e; // Inferred role: action phase; original member name unknown.
  int unknown1080; // Inferred role: action phase time; original member name unknown.
  u8 _1084[0x108c - 0x1084];
  int unknown108c; // Inferred role: rescue target; original member name unknown.
  u8 _1090[0x10a0 - 0x1090];
  int unknown10a0; // Inferred role: send power state; original member name unknown.
  f32 unknown10a4; // Inferred role: send power time; original member name unknown.
  u32 unknown10a8; // Inferred role: send power value; original member name unknown.
  u16 unknown10ac; // Inferred role: full burst counter; original member name unknown.
  u8 unknown10ae;  // Inferred role: full burst count; original member name unknown.
  u8 _10af[0x10b0 - 0x10af];
  f32 unknown10b0; // Inferred role: full burst time; original member name unknown.
  u8 _10b4[0x10b8 - 0x10b4];
  u64 unknown10b8;        // Role and original member name unknown.
  u32 unknown10c0;        // Inferred role: button challenge id; original member name unknown.
  u32 unknown10c4_0 : 16; // Inferred role: change action stop SE; original member name unknown.
  u32 unknown10c6_0 : 5;  // Inferred role: reaction id; original member name unknown.
  u32 unknown10c6_5 : 3;  // Inferred role: reaction state; original member name unknown.
  u8 _10c7[0x10c8 - 0x10c7];
  s8 unknown10c8; // Inferred role: arts target; original member name unknown.
  u8 : 1;
  u8 unknown10c9_1 : 1; // Role and original member name unknown.
  u8 unknown10c9_2 : 1; // Role and original member name unknown.
  u8 unknown10c9_3 : 1; // Inferred role: launch rotate; original member name unknown.
  u8 : 4;
  u8 _10ca[0x10cc - 0x10ca];
  f32 unknown10cc; // Role and original member name unknown.
  f32 unknown10d0; // Role and original member name unknown.
  f32 unknown10d4; // Role and original member name unknown.
  u8 _10d8[0x10db - 0x10d8];
  u8 unknown10db;         // Role and original member name unknown.
  u32 unknown10dc;        // Role and original member name unknown.
  f32 unknown10e0;        // Inferred role: no damage time; original member name unknown.
  f32 unknown10e4;        // Inferred role: super armor time; original member name unknown.
  f32 unknown10e8;        // Inferred role: perfect avoid time; original member name unknown.
  f32 unknown10ec;        // Role and original member name unknown.
  f32 unknown10f0;        // Role and original member name unknown.
  u16 unknown10f4;        // Inferred role: displayed elemental effect flags; original member name unknown.
  u16 unknown10f6;        // Role and original member name unknown.
  u16 unknown10f8;        // Role and original member name unknown.
  s8 unknown10fa;         // Role and original member name unknown.
  u8 unknown10fb;         // Role and original member name unknown.
  s16 unknown10fc;        // Role and original member name unknown.
  s16 unknown10fe;        // Inferred role: back attacker; original member name unknown.
  u8 unknown1100;         // Role and original member name unknown.
  u8 unknown1101;         // Inferred role: attack position; original member name unknown.
  u8 unknown1102;         // Role and original member name unknown.
  u8 unknown1103;         // Role and original member name unknown.
  u8 unknown1104;         // Role and original member name unknown.
  u32 unknown1105_0 : 8;  // Role and original member name unknown.
  u32 unknown1106_0 : 1;  // Role and original member name unknown.
  u32 unknown1106_1 : 2;  // Role and original member name unknown.
  u32 unknown1106_3 : 2;  // Role and original member name unknown.
  u32 unknown1106_5 : 1;  // Role and original member name unknown.
  u32 unknown1106_6 : 1;  // Role and original member name unknown.
  u32 unknown1106_7 : 1;  // Role and original member name unknown.
  u32 unknown1107_0 : 1;  // Role and original member name unknown.
  u32 unknown1105_17 : 1; // Role and original member name unknown.
  u32 unknown1107_2 : 1;  // Role and original member name unknown.
  u32 unknown1107_3 : 1;  // Role and original member name unknown.
  u32 unknown1107_4 : 1;  // Role and original member name unknown.
  u32 : 3;
  f32 unknown1108[7];                           // Inferred role: enhance timers; original member name unknown.
  CharacterParameter::ENHANCE_DATA unknown1124; // Inferred role: last satisfied enhance; original member name unknown.
  u8 unknown112c;                               // Role and original member name unknown.
  u8 _112d[0x1160 - 0x112d];
  AI_ACTION_DATA unknown1160; // Inferred role: AI action; original member name unknown.
  u8 _1168[0x1174 - 0x1168];
  f32 unknown1174; // Inferred role: AI scout time; original member name unknown.
  int unknown1178; // Inferred role: AI state; original member name unknown.
  u8 _117c[0x1180 - 0x117c];
  u32 unknown1180; // Inferred role: AI flags; original member name unknown.
  u8 _1184[0x11cc - 0x1184];
  f32 unknown11cc; // Inferred role: stop AI time; original member name unknown.
  u8 _11d0[0x11d8 - 0x11d0];
  int unknown11d8; // Inferred role: stamp; original member name unknown.
  u8 _11dc[0x11de - 0x11dc];
  u8 unknown11de; // Inferred role: AI warp retry count; original member name unknown.
  u8 _11df[0x11e0 - 0x11df];
  u16 unknown11e0; // Inferred role: blade support condition flags; original member name unknown.
  u16 : 9;
  u16 unknown11e3_1 : 1; // Role and original member name unknown.
  u16 unknown11e3_2 : 1; // Inferred role: use navi flag; original member name unknown.
  u16 unknown11e3_3 : 1; // Inferred role: battle opening flag; original member name unknown.
  u8 _11e4[0x11f0 - 0x11e4];
};

static_assert(sizeof(BattleCharacter::DIRECT_DAMAGE_DATA) == 0x18, "");
static_assert(offsetof(BattleCharacter, unknown10) == 0x10, "");
static_assert(offsetof(BattleCharacter, unknown20) == 0x20, "");
static_assert(offsetof(BattleCharacter, unknown30) == 0x30, "");
static_assert(offsetof(BattleCharacter, unknown50) == 0x50, "");
static_assert(offsetof(BattleCharacter, unknown60) == 0x60, "");
static_assert(offsetof(BattleCharacter, unknown70) == 0x70, "");
static_assert(offsetof(BattleCharacter, unknowna0) == 0xa0, "");
static_assert(offsetof(BattleCharacter, unknownb0) == 0xb0, "");
static_assert(offsetof(BattleCharacter, unknownd0) == 0xd0, "");
static_assert(offsetof(BattleCharacter, unknowne0) == 0xe0, "");
static_assert(offsetof(BattleCharacter, unknowne4) == 0xe4, "");
static_assert(offsetof(BattleCharacter, unknownf0) == 0xf0, "");
static_assert(offsetof(BattleCharacter, unknownf4) == 0xf4, "");
static_assert(offsetof(BattleCharacter, unknown110) == 0x110, "");
static_assert(offsetof(BattleCharacter, unknown114) == 0x114, "");
static_assert(offsetof(BattleCharacter, unknown118) == 0x118, "");
static_assert(offsetof(BattleCharacter, unknown120) == 0x120, "");
static_assert(offsetof(BattleCharacter, unknown128) == 0x128, "");
static_assert(offsetof(BattleCharacter, unknown130) == 0x130, "");
static_assert(offsetof(BattleCharacter, unknown168) == 0x168, "");
static_assert(offsetof(BattleCharacter, unknown170) == 0x170, "");
static_assert(offsetof(BattleCharacter, unknown180) == 0x180, "");
static_assert(offsetof(BattleCharacter, unknownc30) == 0xc30, "");
static_assert(offsetof(BattleCharacter, unknownc58) == 0xc58, "");
static_assert(offsetof(BattleCharacter, m_animationSpeed) == 0xcb8, "");
static_assert(offsetof(BattleCharacter, m_systemDt) == 0xcc4, "");
static_assert(offsetof(BattleCharacter, unknowncc8) == 0xcc8, "");
static_assert(offsetof(BattleCharacter, unknownccc) == 0xccc, "");
static_assert(offsetof(BattleCharacter, unknowncd4) == 0xcd4, "");
static_assert(offsetof(BattleCharacter, unknowncdc) == 0xcdc, "");
static_assert(offsetof(BattleCharacter, unknowncb0) == 0xcb0, "");
static_assert(offsetof(BattleCharacter, unknowncb4) == 0xcb4, "");
static_assert(offsetof(BattleCharacter, unknownce0) == 0xce0, "");
static_assert(offsetof(BattleCharacter, unknownce4) == 0xce4, "");
static_assert(sizeof(BattleCharacter::ANIMATION_MOVE_DISTANCE_DATA) == 0x8, "");
static_assert(offsetof(BattleCharacter, unknownd98) == 0xd98, "");
static_assert(offsetof(BattleCharacter, unknowndb8) == 0xdb8, "");
static_assert(offsetof(BattleCharacter, unknowndc8) == 0xdc8, "");
static_assert(offsetof(BattleCharacter, unknowndd0) == 0xdd0, "");
static_assert(offsetof(BattleCharacter, unknowndd6) == 0xdd6, "");
static_assert(offsetof(BattleCharacter, unknowndd8) == 0xdd8, "");
static_assert(offsetof(BattleCharacter, unknownf2c) == 0xf2c, "");
static_assert(offsetof(BattleCharacter, unknowncc0) == 0xcc0, "");
static_assert(offsetof(BattleCharacter, unknownf4c) == 0xf4c, "");
static_assert(offsetof(BattleCharacter, unknownf58) == 0xf58, "");
static_assert(offsetof(BattleCharacter, unknownf5c) == 0xf5c, "");
static_assert(offsetof(BattleCharacter, unknownf64) == 0xf64, "");
static_assert(offsetof(BattleCharacter, unknownf6a) == 0xf6a, "");
static_assert(offsetof(BattleCharacter, unknownf6c) == 0xf6c, "");
static_assert(offsetof(BattleCharacter, unknownf6e) == 0xf6e, "");
static_assert(offsetof(BattleCharacter, unknownf72) == 0xf72, "");
static_assert(offsetof(BattleCharacter, unknownf7a) == 0xf7a, "");
static_assert(offsetof(BattleCharacter, unknownf7c) == 0xf7c, "");
static_assert(offsetof(BattleCharacter, unknownf7d) == 0xf7d, "");
static_assert(offsetof(BattleCharacter, unknownf7e) == 0xf7e, "");
static_assert(offsetof(BattleCharacter, unknownf7f) == 0xf7f, "");
static_assert(offsetof(BattleCharacter, unknownf82) == 0xf82, "");
static_assert(offsetof(BattleCharacter, unknownf83) == 0xf83, "");
static_assert(offsetof(BattleCharacter, unknownf86) == 0xf86, "");
static_assert(offsetof(BattleCharacter, unknownfb0) == 0xfb0, "");
static_assert(offsetof(BattleCharacter, unknown1078) == 0x1078, "");
static_assert(offsetof(BattleCharacter, unknown107c) == 0x107c, "");
static_assert(offsetof(BattleCharacter, unknown107e) == 0x107e, "");
static_assert(offsetof(BattleCharacter, unknown1080) == 0x1080, "");
static_assert(offsetof(BattleCharacter, unknown108c) == 0x108c, "");
static_assert(offsetof(BattleCharacter, unknown10a0) == 0x10a0, "");
static_assert(offsetof(BattleCharacter, unknown10a4) == 0x10a4, "");
static_assert(offsetof(BattleCharacter, unknown10b8) == 0x10b8, "");
static_assert(offsetof(BattleCharacter, unknown10c0) == 0x10c0, "");
static_assert(offsetof(BattleCharacter, unknown10c8) == 0x10c8, "");
static_assert(offsetof(BattleCharacter, unknown10cc) == 0x10cc, "");
static_assert(offsetof(BattleCharacter, unknown10d0) == 0x10d0, "");
static_assert(offsetof(BattleCharacter, unknown10d4) == 0x10d4, "");
static_assert(offsetof(BattleCharacter, unknown10db) == 0x10db, "");
static_assert(offsetof(BattleCharacter, unknown10dc) == 0x10dc, "");
static_assert(offsetof(BattleCharacter, unknown10f4) == 0x10f4, "");
static_assert(offsetof(BattleCharacter, unknown10e0) == 0x10e0, "");
static_assert(offsetof(BattleCharacter, unknown10ec) == 0x10ec, "");
static_assert(offsetof(BattleCharacter, unknown10f0) == 0x10f0, "");
static_assert(offsetof(BattleCharacter, unknown10f6) == 0x10f6, "");
static_assert(offsetof(BattleCharacter, unknown10f8) == 0x10f8, "");
static_assert(offsetof(BattleCharacter, unknown10fa) == 0x10fa, "");
static_assert(offsetof(BattleCharacter, unknown10fb) == 0x10fb, "");
static_assert(offsetof(BattleCharacter, unknown10fc) == 0x10fc, "");
static_assert(offsetof(BattleCharacter, unknown10fe) == 0x10fe, "");
static_assert(offsetof(BattleCharacter, unknown1100) == 0x1100, "");
static_assert(offsetof(BattleCharacter, unknown1104) == 0x1104, "");
static_assert(offsetof(BattleCharacter, unknown1101) == 0x1101, "");
static_assert(offsetof(BattleCharacter, unknown1102) == 0x1102, "");
static_assert(offsetof(BattleCharacter, unknown1103) == 0x1103, "");
static_assert(offsetof(BattleCharacter, unknown1108) == 0x1108, "");
static_assert(offsetof(BattleCharacter, unknown1124) == 0x1124, "");
static_assert(offsetof(BattleCharacter, unknown112c) == 0x112c, "");
static_assert(offsetof(BattleCharacter, unknown1160) == 0x1160, "");
static_assert(offsetof(BattleCharacter, unknown1174) == 0x1174, "");
static_assert(offsetof(BattleCharacter, unknown1178) == 0x1178, "");
static_assert(offsetof(BattleCharacter, unknown1180) == 0x1180, "");
static_assert(offsetof(BattleCharacter, unknown11cc) == 0x11cc, "");
static_assert(offsetof(BattleCharacter, unknown11d8) == 0x11d8, "");
static_assert(offsetof(BattleCharacter, unknown11de) == 0x11de, "");
static_assert(offsetof(BattleCharacter, unknown11e0) == 0x11e0, "");
static_assert(sizeof(BattleCharacter) == 0x11f0, "");

} // namespace btl
