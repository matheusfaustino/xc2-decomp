#pragma once
// Original file name from the game string C:/Monolithsoft/BF2/prog/application/BF2\gf/object/component/pc/field/ai/gf_pc_ai_warp_task_field.h.
#include <stddef.h>

#include "ai/ai_behavior_node.h"
#include "fw/fw_param_manager.h"
#include "gf/object/component/pc/gf_pc_ai_agent_field.h"
#include "types.h"

namespace mm {
namespace mtl {
class RTTI;
} // namespace mtl
} // namespace mm

namespace gf {
namespace pc {

// Layout evidence: BehaviorTree::createTask<TaskFieldCheckWarp> (0x710053f350) allocates 0x40 bytes and passes flags 1
// (activate event); onEvent and update reset and accumulate the frame time at 0x38. onEvent is out of line, so the vtable
// is in gf_pc_ai_warp_task_field.cpp.
class TaskFieldCheckWarp : public ai::TaskNode {
public:
  static mm::mtl::RTTI m_rtti;

  TaskFieldCheckWarp(const char *name) : ai::TaskNode(name, 1) {}
  const mm::mtl::RTTI *getRTTI() const override { return &m_rtti; }
  void onEvent(ai::BlackBoard &blackBoard, const ai::Event &event) override;
  void onChangeActivate(BlackBoardField &blackBoard, const ai::ActivateEvent &event);
  bool testCondition(const BlackBoardField &blackBoard);
  void update(BlackBoardField &blackBoard);

  // __LINE__ must be 26 as in the original header: it is the line argument of the asserts.
#line 26
  UNKNOWN001

  f32 unknown38; // Inferred role: timer; original member name unknown.
};

static_assert(offsetof(TaskFieldCheckWarp, unknown38) == 0x38, "");
static_assert(sizeof(TaskFieldCheckWarp) == 0x40, "");

// Layout evidence: BehaviorTree::createTask<TaskFieldCheckWarpBattleWaterDead> (0x710053f638) allocates 0x48 bytes,
// passes flags 3 (activate and create events) and clears 0x40; onEvent (0x71005460cc) resets the timer at 0x38 and, on
// the create event, takes the first entry of a ParamManager param list into 0x40. onEvent is out of line, so the vtable
// is in gf_pc_ai_warp_task_field.cpp.
class TaskFieldCheckWarpBattleWaterDead : public ai::TaskNode {
public:
  static mm::mtl::RTTI m_rtti;

  TaskFieldCheckWarpBattleWaterDead(const char *name) : ai::TaskNode(name, 3), unknown40() {}
  const mm::mtl::RTTI *getRTTI() const override { return &m_rtti; }
  void onEvent(ai::BlackBoard &blackBoard, const ai::Event &event) override;
  void onChangeActivate(BlackBoardField &blackBoard, const ai::ActivateEvent &event);
  void onCreate(BlackBoardField &blackBoard, const ai::CreateEvent &event);
  bool testCondition(const BlackBoardField &blackBoard);
  void update(BlackBoardField &blackBoard);

  // __LINE__ must be 44 as in the original header: it is the line argument of the asserts.
#line 44
  UNKNOWN001

  f32 unknown38;              // Inferred role: timer; original member name unknown.
  fw::ParamAdapter unknown40; // Inferred role: warp param; original member name unknown.
};

static_assert(offsetof(TaskFieldCheckWarpBattleWaterDead, unknown38) == 0x38, "");
static_assert(offsetof(TaskFieldCheckWarpBattleWaterDead, unknown40) == 0x40, "");
static_assert(sizeof(TaskFieldCheckWarpBattleWaterDead) == 0x48, "");

// Layout evidence: BehaviorTree::createTask<TaskFieldCheckWarpFallDead> (0x710053f924) allocates 0x38 bytes and passes
// flags 0 to the TaskNode constructor; the game emits the vtable and the inline virtuals in gf_pc_ai_agent_field.cpp.
class TaskFieldCheckWarpFallDead : public ai::TaskNode {
public:
  static mm::mtl::RTTI m_rtti;

  TaskFieldCheckWarpFallDead(const char *name) : ai::TaskNode(name, 0) {}
  const mm::mtl::RTTI *getRTTI() const override { return &m_rtti; }
  bool testCondition(const BlackBoardField &blackBoard);
  void update(BlackBoardField &blackBoard);

  // __LINE__ must be 66 as in the original header: it is the line argument of the asserts.
#line 66
  UNKNOWN001
};

static_assert(sizeof(TaskFieldCheckWarpFallDead) == 0x38, "");

} // namespace pc
} // namespace gf
