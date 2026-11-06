#pragma once
// Original file name from the game string C:/Monolithsoft/BF2/prog/application/BF2\gf/object/component/pc/field/ai/gf_pc_ai_follow_task_field.h.
#include <stddef.h>

#include "ai/ai_behavior_node.h"
#include "gf/object/component/pc/gf_pc_ai_agent_field.h"
#include "types.h"

namespace mm {
namespace mtl {
class RTTI;
} // namespace mtl
} // namespace mm

namespace gf {
namespace pc {

// Layout evidence: BehaviorTree::createTask<TaskFieldFollowTarget> (0x71005400d4) allocates 0x38 bytes and passes flags 0
// to the TaskNode constructor; the game emits the vtable and the inline virtuals in gf_pc_ai_agent_field.cpp.
class TaskFieldFollowTarget : public ai::TaskNode {
public:
  static mm::mtl::RTTI m_rtti;

  TaskFieldFollowTarget(const char *name) : ai::TaskNode(name, 0) {}
  const mm::mtl::RTTI *getRTTI() const override { return &m_rtti; }
  bool testCondition(const BlackBoardField &blackBoard);
  void update(BlackBoardField &blackBoard);

  // __LINE__ must be 23 as in the original header: it is the line argument of the asserts.
#line 23
  UNKNOWN001
};

static_assert(sizeof(TaskFieldFollowTarget) == 0x38, "");

// Layout evidence: BehaviorTree::createTask<TaskFieldFollowMainPc> (0x71005403bc) allocates 0x40 bytes and passes flags 3
// (activate and create events); onEvent (0x7100543a64) resets the timer at 0x38 and the flag at 0x3c and is out of line,
// so the vtable is in gf_pc_ai_follow_task_field.cpp.
class TaskFieldFollowMainPc : public ai::TaskNode {
public:
  static mm::mtl::RTTI m_rtti;

  TaskFieldFollowMainPc(const char *name) : ai::TaskNode(name, 3) {}
  const mm::mtl::RTTI *getRTTI() const override { return &m_rtti; }
  void onEvent(ai::BlackBoard &blackBoard, const ai::Event &event) override;
  void onChangeActivate(BlackBoardField &blackBoard, const ai::ActivateEvent &event);
  void onCreate(BlackBoardField &blackBoard, const ai::CreateEvent &event);
  bool testCondition(const BlackBoardField &blackBoard);
  void update(BlackBoardField &blackBoard);

  // __LINE__ must be 36 as in the original header: it is the line argument of the asserts.
#line 36
  UNKNOWN001

  f32 unknown38;  // Inferred role: timer; original member name unknown.
  bool unknown3c; // Role and original member name unknown.
};

static_assert(offsetof(TaskFieldFollowMainPc, unknown38) == 0x38, "");
static_assert(offsetof(TaskFieldFollowMainPc, unknown3c) == 0x3c, "");
static_assert(sizeof(TaskFieldFollowMainPc) == 0x40, "");

// Layout evidence: BehaviorTree::createTask<TaskFieldFollowBuddyPc> (0x7100541530) allocates 0x38 bytes and passes flags 0
// to the TaskNode constructor; the game emits the vtable and the inline virtuals in gf_pc_ai_agent_field.cpp.
class TaskFieldFollowBuddyPc : public ai::TaskNode {
public:
  static mm::mtl::RTTI m_rtti;

  TaskFieldFollowBuddyPc(const char *name) : ai::TaskNode(name, 0) {}
  const mm::mtl::RTTI *getRTTI() const override { return &m_rtti; }
  bool testCondition(const BlackBoardField &blackBoard);
  void update(BlackBoardField &blackBoard);

  // __LINE__ must be 57 as in the original header: it is the line argument of the asserts.
#line 57
  UNKNOWN001
};

static_assert(sizeof(TaskFieldFollowBuddyPc) == 0x38, "");

} // namespace pc
} // namespace gf
