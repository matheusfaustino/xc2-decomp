#pragma once
#include <stddef.h>

#include "ai/ai_behavior_node.h"
#include "ai/ai_behavior_tree.h"
#include "fw/fw_transform.h"
#include "mm/mm_std_base.h"
#include "mm_math/mm_plane.h"
#include "mm_math/mm_vec3.h"
#include "mm_mtl/bitflag.h"
#include "mm_mtl/fixed_queue.h"
#include "mm_mtl/intrusive_ptr.h"
#include "mm_mtl/rtti.h" // IWYU pragma: keep
#include "types.h"

namespace fw {
class MessageObject;
struct Message;
struct UpdateInfo;
class ParamList;
} // namespace fw

namespace navi {
class NvmshAcObj;
} // namespace navi

namespace gf {

class GfComPropertyPc;
struct MsgPcFieldNotifyHighJump;
struct MsgPcFieldNotifyWall;
struct MsgPcFieldNotifyWallJumpStart;
struct MsgPcFieldNotifyWaterWarp;
struct MsgPcFieldNotifyWaterWarpBuddy;

namespace pc {

// Layout evidence: BlackBoardField::BlackBoardField (0x7100542c94) and the AIAgentField message handlers, which fill the
// wall members from MsgPcFieldNotifyWall and the flags at 0x200.
class BlackBoardField : public ai::BlackBoard {
public:
  static mm::mtl::RTTI m_rtti;

  // Inline: the game only has the base object constructor symbol (C2).
  BlackBoardField()
      : unknown08(nullptr), unknown10(nullptr), unknown18(nullptr), unknown20(mm::Vec3::zero), unknown30(0.0f), unknown40(mm::Vec3::zero), unknown50(0.0f), unknown60(mm::Vec3::zero),
        unknown70(mm::Vec3::unitY, 0.0f), unknown90(mm::Vec3::zero), unknown1f0(mm::Vec3::zero), unknown204(0.0f) {}
  const mm::mtl::RTTI *getRTTI() const override;

  GfComPropertyPc *unknown08;                       // Inferred role: property during the update; original member name unknown.
  navi::NvmshAcObj *unknown10;                      // Inferred role: navi mesh object during the update; original member name unknown.
  const fw::UpdateInfo *unknown18;                  // Inferred role: update info during the update; original member name unknown.
  mm::Vec3 unknown20;                               // Role and original member name unknown.
  f32 unknown30;                                    // Role and original member name unknown.
  mm::Vec3 unknown40;                               // Role and original member name unknown.
  f32 unknown50;                                    // Role and original member name unknown.
  mm::Vec3 unknown60;                               // Inferred role: wall position; original member name unknown.
  mm::Plane unknown70;                              // Inferred role: wall plane; original member name unknown.
  mm::Vec3 unknown90;                               // Inferred role: wall leave position; original member name unknown.
  mm::mtl::FixedQueue<fw::Transform, 10> unknowna0; // Inferred role: wall move history; original member name unknown.
  mm::Vec3 unknown1f0;                              // Inferred role: last position on the navi mesh; original member name unknown.
  mm::mtl::unknown027<32> unknown200; // Inferred role: flags (bit 3 idle turn request, 4 on wall, 5 wall jump, 6 high jump, 7 water warp, 8 wall jump start, 9 water warp buddy, 10 leader moving);
                                      // original member name unknown.
  f32 unknown204;                     // Inferred role: wait time of the forced warp; original member name unknown.
};

static_assert(offsetof(BlackBoardField, unknown08) == 0x08, "");
static_assert(offsetof(BlackBoardField, unknown20) == 0x20, "");
static_assert(offsetof(BlackBoardField, unknown30) == 0x30, "");
static_assert(offsetof(BlackBoardField, unknown40) == 0x40, "");
static_assert(offsetof(BlackBoardField, unknown60) == 0x60, "");
static_assert(offsetof(BlackBoardField, unknown70) == 0x70, "");
static_assert(offsetof(BlackBoardField, unknown90) == 0x90, "");
static_assert(offsetof(BlackBoardField, unknowna0) == 0xa0, "");
static_assert(offsetof(BlackBoardField, unknown1f0) == 0x1f0, "");
static_assert(offsetof(BlackBoardField, unknown200) == 0x200, "");
static_assert(offsetof(BlackBoardField, unknown204) == 0x204, "");
static_assert(sizeof(BlackBoardField) == 0x210, "");

// Inferred role: defines the checkCondition and execute overrides of a gf::pc::TaskField class from its testCondition and
// update; original name unknown. Both overrides of a class assert "pBB" on the same header line, the line of the class
// declaration that uses it (gf_pc_ai_idle_task_field.h line 23 for TaskFieldIdle).
#define UNKNOWN001                                                                                                                                                                                     \
  bool checkCondition(const ai::BlackBoard &blackBoard) override {                                                                                                                                     \
    const BlackBoardField *pBB = mm::mtl::dynamicCast<const BlackBoardField>(&blackBoard);                                                                                                             \
    MM_ASSERT(pBB);                                                                                                                                                                                    \
    return testCondition(*pBB);                                                                                                                                                                        \
  }                                                                                                                                                                                                    \
  bool execute(ai::BlackBoard &blackBoard) override {                                                                                                                                                  \
    BlackBoardField *pBB = mm::mtl::dynamicCast<BlackBoardField>(&blackBoard);                                                                                                                         \
    MM_ASSERT(pBB);                                                                                                                                                                                    \
    update(*pBB);                                                                                                                                                                                      \
    return true;                                                                                                                                                                                       \
  }

class AIAgentField {
public:
  AIAgentField();
  ~AIAgentField();
  void setup(GfComPropertyPc &property);
  void cleanup(GfComPropertyPc &property);
  void onActivate(GfComPropertyPc &property);
  void onDeactivate(GfComPropertyPc &property);
  void update(const fw::UpdateInfo &info, GfComPropertyPc &property);
  bool recieveMessage(GfComPropertyPc &property, fw::MessageObject *sender, fw::Message &message);
  void procMsgPcFieldNotifyWall(fw::MessageObject *sender, MsgPcFieldNotifyWall &message);
  void procMsgPcFieldHighJump(fw::MessageObject *sender, MsgPcFieldNotifyHighJump &message);
  void procMsgPcFieldNotifyWaterWarp(fw::MessageObject *sender, MsgPcFieldNotifyWaterWarp &message);
  void procMsgPcFieldNotifyWaterWarpBuddy(fw::MessageObject *sender, MsgPcFieldNotifyWaterWarpBuddy &message);
  void procMsgPcFieldNotifyWallJumpStart(fw::MessageObject *sender, MsgPcFieldNotifyWallJumpStart &message);

  navi::NvmshAcObj *unknown00;                    // Inferred role: navi mesh object; original member name unknown.
  ai::BehaviorTree unknown08;                     // Inferred role: behavior tree; original member name unknown.
  mm::mtl::IntrusivePtr<fw::ParamList> unknown88; // Inferred role: field AI param list; original member name unknown.
};

static_assert(offsetof(AIAgentField, unknown00) == 0x00, "");
static_assert(offsetof(AIAgentField, unknown08) == 0x08, "");
static_assert(offsetof(AIAgentField, unknown88) == 0x88, "");
static_assert(sizeof(AIAgentField) == 0x90, "");

} // namespace pc
} // namespace gf
