#pragma once
// Original file name from the game string C:/Monolithsoft/BF2/prog/application/BF2\gf/object/component/pc/field/ai/gf_pc_ai_wall_task_field.h.
#include <stddef.h>

#include "ai/ai_behavior_node.h"
#include "ai/ai_state_fsm.h"
#include "gf/object/component/pc/field/ai/gf_pc_state_field_wall_ai.h"
#include "gf/object/component/pc/gf_pc_ai_agent_field.h"
#include "mm_math/mm_vec3.h"
#include "mm_mtl/unique_ptr.h"
#include "types.h"

namespace mm {
namespace mtl {
class RTTI;
} // namespace mtl
} // namespace mm

namespace gf {
namespace pc {

// Layout evidence: BehaviorTree::createTask<TaskFieldWallPrepare> (0x7100540c74) allocates 0x48 bytes, passes flags 7
// (activate, create and destroy events) and clears 0x38 and 0x40; onChangeActivate (0x7100545084) stores the black
// board in the context at 0x40 around Fsm::setState on the state machine at 0x38. onEvent is out of line, so the vtable
// is in gf_pc_ai_wall_task_field.cpp.
class TaskFieldWallPrepare : public ai::TaskNode {
public:
  static mm::mtl::RTTI m_rtti;

  TaskFieldWallPrepare(const char *name) : ai::TaskNode(name, 7), unknown40() {}
  const mm::mtl::RTTI *getRTTI() const override { return &m_rtti; }
  void onEvent(ai::BlackBoard &blackBoard, const ai::Event &event) override;
  void onChangeActivate(BlackBoardField &blackBoard, const ai::ActivateEvent &event);
  void onCreate(BlackBoardField &blackBoard, const ai::CreateEvent &event);
  void onDestroy(BlackBoardField &blackBoard, const ai::DestroyEvent &event);
  bool testCondition(const BlackBoardField &blackBoard);
  void update(BlackBoardField &blackBoard);

  // __LINE__ must be 25 as in the original header: it is the line argument of the asserts.
#line 25
  UNKNOWN001

  mm::mtl::UniquePtr<ai::Fsm> unknown38; // Inferred role: wall state machine; original member name unknown.
  WallAiContextField unknown40;          // Inferred role: owner of the state machine; original member name unknown.
};

static_assert(offsetof(TaskFieldWallPrepare, unknown38) == 0x38, "");
static_assert(offsetof(TaskFieldWallPrepare, unknown40) == 0x40, "");
static_assert(sizeof(TaskFieldWallPrepare) == 0x48, "");

// Layout evidence: BehaviorTree::createTask<TaskFieldWallFollow> (0x7100541248) allocates 0x38 bytes and passes flags 0
// to the TaskNode constructor; the game emits the vtable and the inline virtuals in gf_pc_ai_agent_field.cpp.
class TaskFieldWallFollow : public ai::TaskNode {
public:
  static mm::mtl::RTTI m_rtti;

  TaskFieldWallFollow(const char *name) : ai::TaskNode(name, 0) {}
  const mm::mtl::RTTI *getRTTI() const override { return &m_rtti; }
  bool testCondition(const BlackBoardField &blackBoard);
  void update(BlackBoardField &blackBoard);

  // __LINE__ must be 48 as in the original header: it is the line argument of the asserts.
#line 48
  UNKNOWN001
};

static_assert(sizeof(TaskFieldWallFollow) == 0x38, "");

// Layout evidence: BehaviorTree::createTask<TaskFieldWallFall> (0x71005406a4) allocates 0x40 bytes and passes flags 3
// (activate and create events); update (0x71005456b8) accumulates the frame time at 0x38. onEvent is out of line, so the
// vtable is in gf_pc_ai_wall_task_field.cpp.
class TaskFieldWallFall : public ai::TaskNode {
public:
  static mm::mtl::RTTI m_rtti;

  TaskFieldWallFall(const char *name) : ai::TaskNode(name, 3) {}
  const mm::mtl::RTTI *getRTTI() const override { return &m_rtti; }
  void onEvent(ai::BlackBoard &blackBoard, const ai::Event &event) override;
  void onChangeActivate(BlackBoardField &blackBoard, const ai::ActivateEvent &event);
  void onCreate(BlackBoardField &blackBoard, const ai::CreateEvent &event);
  bool testCondition(const BlackBoardField &blackBoard);
  void update(BlackBoardField &blackBoard);

  // __LINE__ must be 61 as in the original header: it is the line argument of the asserts.
#line 61
  UNKNOWN001

  f32 unknown38; // Inferred role: timer; original member name unknown.
};

static_assert(offsetof(TaskFieldWallFall, unknown38) == 0x38, "");
static_assert(sizeof(TaskFieldWallFall) == 0x40, "");

// Layout evidence: BehaviorTree::createTask<TaskFieldWallEscape> (0x7100540f60) allocates 0x50 bytes aligned on 16 and
// passes flags 0; testCondition (0x7100545718) fills the vector at 0x40. The game emits the vtable and the inline
// virtuals in gf_pc_ai_agent_field.cpp.
class TaskFieldWallEscape : public ai::TaskNode {
public:
  static mm::mtl::RTTI m_rtti;

  TaskFieldWallEscape(const char *name) : ai::TaskNode(name, 0) {}
  const mm::mtl::RTTI *getRTTI() const override { return &m_rtti; }
  bool testCondition(const BlackBoardField &blackBoard);
  void update(BlackBoardField &blackBoard);

  // __LINE__ must be 81 as in the original header: it is the line argument of the asserts.
#line 81
  UNKNOWN001

  mm::Vec3 unknown40; // Inferred role: escape position; original member name unknown.
};

static_assert(offsetof(TaskFieldWallEscape, unknown40) == 0x40, "");
static_assert(sizeof(TaskFieldWallEscape) == 0x50, "");

// Layout evidence: BehaviorTree::createTask<TaskFieldWallJumpWait> (0x710054098c) allocates 0x38 bytes and passes flags 0
// to the TaskNode constructor; the game emits the vtable and the inline virtuals in gf_pc_ai_agent_field.cpp.
class TaskFieldWallJumpWait : public ai::TaskNode {
public:
  static mm::mtl::RTTI m_rtti;

  TaskFieldWallJumpWait(const char *name) : ai::TaskNode(name, 0) {}
  const mm::mtl::RTTI *getRTTI() const override { return &m_rtti; }
  bool testCondition(const BlackBoardField &blackBoard);
  void update(BlackBoardField &blackBoard);

  // __LINE__ must be 97 as in the original header: it is the line argument of the asserts.
#line 97
  UNKNOWN001
};

static_assert(sizeof(TaskFieldWallJumpWait) == 0x38, "");

} // namespace pc
} // namespace gf
