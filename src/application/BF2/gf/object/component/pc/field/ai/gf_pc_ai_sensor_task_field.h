#pragma once
// Original file name from the game string C:/Monolithsoft/BF2/prog/application/BF2\gf/object/component/pc/field/ai/gf_pc_ai_sensor_task_field.h.
#include <stddef.h>

#include "ai/ai_behavior_node.h"
#include "gf/object/component/pc/gf_pc_ai_agent_field.h"
#include "mm_math/mm_vec3.h"

namespace mm {
namespace mtl {
class RTTI;
} // namespace mtl
} // namespace mm

namespace gf {
namespace pc {

// Layout evidence: BehaviorTree::createTask<TaskFieldSensor> (0x710053ee88) allocates 0x50 bytes aligned on 16 and passes
// flags 1 (activate event) to the TaskNode constructor; onEvent is out of line, so the vtable is in
// gf_pc_ai_sensor_task_field.cpp.
class TaskFieldSensor : public ai::TaskNode {
public:
  static mm::mtl::RTTI m_rtti;

  TaskFieldSensor(const char *name) : ai::TaskNode(name, 1) {}
  const mm::mtl::RTTI *getRTTI() const override { return &m_rtti; }
  void onEvent(ai::BlackBoard &blackBoard, const ai::Event &event) override;
  void onChangeActivate(BlackBoardField &blackBoard, const ai::ActivateEvent &event);
  bool testCondition(const BlackBoardField &blackBoard);
  void update(BlackBoardField &blackBoard);

  // __LINE__ must be 23 as in the original header: it is the line argument of the asserts.
#line 23
  UNKNOWN001

  mm::Vec3 unknown40; // Role and original member name unknown: only the 16-byte alignment of the allocation shows it.
};

static_assert(offsetof(TaskFieldSensor, unknown40) == 0x40, "");
static_assert(sizeof(TaskFieldSensor) == 0x50, "");

} // namespace pc
} // namespace gf
