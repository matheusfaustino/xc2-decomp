#pragma once
// Original file name from the game string C:/Monolithsoft/BF2/prog/application/BF2\gf/object/component/pc/field/ai/gf_pc_ai_jump_task_field.h.
#include "ai/ai_behavior_node.h"
#include "gf/object/component/pc/gf_pc_ai_agent_field.h"

namespace mm {
namespace mtl {
class RTTI;
} // namespace mtl
} // namespace mm

namespace gf {
namespace pc {

// Layout evidence: BehaviorTree::createTask<TaskFieldJump> (0x710053fc0c) allocates 0x38 bytes and passes flags 0 to the
// TaskNode constructor; the game emits the vtable and the inline virtuals in gf_pc_ai_agent_field.cpp.
class TaskFieldJump : public ai::TaskNode {
public:
  static mm::mtl::RTTI m_rtti;

  TaskFieldJump(const char *name) : ai::TaskNode(name, 0) {}
  const mm::mtl::RTTI *getRTTI() const override { return &m_rtti; }
  bool testCondition(const BlackBoardField &blackBoard);
  void update(BlackBoardField &blackBoard);

  // __LINE__ must be 23 as in the original header: it is the line argument of the asserts.
#line 23
  UNKNOWN001
};

static_assert(sizeof(TaskFieldJump) == 0x38, "");

} // namespace pc
} // namespace gf
