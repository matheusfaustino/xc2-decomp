#pragma once
#include "ai/ai_behavior_node.h"
#include "gf/object/component/pc/gf_pc_ai_agent_field.h"

namespace mm {
namespace mtl {
class RTTI;
} // namespace mtl
} // namespace mm

namespace gf {
namespace pc {

// Layout evidence: BehaviorTree::createTask<TaskFieldIdle> (0x7100541de8) allocates 0x38 bytes and passes flags 0 to the
// TaskNode constructor; the game emits the vtable and the inline virtuals in gf_pc_ai_agent_field.cpp.
class TaskFieldIdle : public ai::TaskNode {
public:
  static mm::mtl::RTTI m_rtti;

  TaskFieldIdle(const char *name) : ai::TaskNode(name, 0) {}
  const mm::mtl::RTTI *getRTTI() const override { return &m_rtti; }
  bool testCondition(const BlackBoardField &blackBoard);
  void update(BlackBoardField &blackBoard);

  // __LINE__ must be 23 as in the original header: it is the line argument of the asserts.
#line 23
  UNKNOWN001
};

static_assert(sizeof(TaskFieldIdle) == 0x38, "");

// Layout evidence: BehaviorTree::createTask<TaskFieldIdleTurn> (0x7100541b00) allocates 0x38 bytes and passes flags 0.
class TaskFieldIdleTurn : public ai::TaskNode {
public:
  static mm::mtl::RTTI m_rtti;

  TaskFieldIdleTurn(const char *name) : ai::TaskNode(name, 0) {}
  const mm::mtl::RTTI *getRTTI() const override { return &m_rtti; }
  bool testCondition(const BlackBoardField &blackBoard);
  void update(BlackBoardField &blackBoard);

  // __LINE__ must be 36 as in the original header: it is the line argument of the asserts.
#line 36
  UNKNOWN001
};

static_assert(sizeof(TaskFieldIdleTurn) == 0x38, "");

} // namespace pc
} // namespace gf
