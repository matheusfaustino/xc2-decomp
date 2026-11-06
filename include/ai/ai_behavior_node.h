#pragma once
#include <stddef.h>

#include "mm_mtl/fixstr.h"
#include "mm_mtl/intrusive_ptr.h"
#include "mm_mtl/reference_object.h"
#include "mm_mtl/vector.h"
#include "types.h"

namespace mm {
namespace mtl {
class IAllocator;
class RTTI;
} // namespace mtl
} // namespace mm

namespace ai {

// Inferred role: type of an Event, switched on by the onEvent overrides of the gf::pc::TaskField classes; original name
// unknown.
enum unknown002 {
  UNKNOWN002_UNKNOWN_1 = 1, // Inferred role: ActivateEvent.
  UNKNOWN002_UNKNOWN_2 = 2, // Inferred role: CreateEvent.
  UNKNOWN002_UNKNOWN_4 = 4, // Inferred role: DestroyEvent.
};

// Layout evidence: BehaviorNode::setActiveNotiryChildren (0x7100542eec) passes the type at 0x00 and the active flag at
// 0x04 to onEvent; BehaviorTree::createTask passes type 2 and the gf::pc::TaskField onEvent overrides dispatch types 1, 2
// and 4 to onChangeActivate, onCreate and onDestroy.
struct Event {
  explicit Event(unknown002 type) : unknown00(type) {}

  u32 unknown00; // Inferred role: type; original member name unknown.
};

struct ActivateEvent : Event {
  explicit ActivateEvent(bool isActive) : Event(UNKNOWN002_UNKNOWN_1), unknown04(isActive) {}

  bool unknown04; // Inferred role: new active state; original member name unknown.
};

struct CreateEvent : Event {
  CreateEvent() : Event(UNKNOWN002_UNKNOWN_2) {}
};

struct DestroyEvent : Event {
  DestroyEvent() : Event(UNKNOWN002_UNKNOWN_4) {}
};

static_assert(offsetof(ActivateEvent, unknown04) == 0x04, "");
static_assert(sizeof(ActivateEvent) == 0x08, "");

// Layout evidence: BlackBoard::BlackBoard (0x7100941ae8) only stores the vtable, and the UniquePtr release in
// BehaviorTree::~BehaviorTree calls the deleting destructor at vtable slot 2.
class BlackBoard {
public:
  static mm::mtl::RTTI m_rtti;

  BlackBoard();
  virtual const mm::mtl::RTTI *getRTTI() const;
  virtual ~BlackBoard();
};

static_assert(sizeof(BlackBoard) == 0x08, "");

// Layout evidence: BehaviorNode::BehaviorNode (0x7100943408) stores the name FixStr at 0x0c (length at 0x2c), the
// active flag at 0x30 and the flags argument at 0x34; BehaviorTree::execute calls onEvent at vtable offset 0x30.
class BehaviorNode : public mm::mtl::ReferenceObject {
public:
  static mm::mtl::RTTI m_rtti;

  BehaviorNode(const char *name, u32 flags);
  ~BehaviorNode() override;
  virtual const mm::mtl::RTTI *getRTTI() const;
  virtual bool checkCondition(const BlackBoard &blackBoard);
  virtual bool execute(BlackBoard &blackBoard);
  // Inferred role: updates this node's active flag and notifies it, without touching its children; original name unknown.
  // Inline: the game emits it with the vtables of the gf::pc::TaskField classes (0x7100542eec).
  void setActive(BlackBoard &blackBoard, bool isActive) {
    if (unknown30 != isActive) {
      unknown30 = isActive;
      if ((unknown34 & 1) != 0) {
        ActivateEvent event(isActive);
        onEvent(blackBoard, event);
      }
    }
  }
  // Inline: the game emits it with the vtables of the gf::pc::TaskField classes (0x7100542eec).
  virtual void setActiveNotiryChildren(BlackBoard &blackBoard, bool isActive) { setActive(blackBoard, isActive); }
  // Inline: the game emits it with the vtables of the gf::pc::TaskField classes (0x7100542f3c).
  virtual void onEvent(BlackBoard &blackBoard, const Event &event) {}

  mm::mtl::FixStr<32> unknown0c; // Inferred role: name; original member name unknown.
  bool unknown30;                // Inferred role: active; original member name unknown.
  u32 unknown34;                 // Inferred role: flags (bit 0 activate event, 1 create event, 2 destroy event); original member name unknown.
};

static_assert(offsetof(BehaviorNode, unknown0c) == 0x0c, "");
static_assert(offsetof(BehaviorNode, unknown30) == 0x30, "");
static_assert(offsetof(BehaviorNode, unknown34) == 0x34, "");
static_assert(sizeof(BehaviorNode) == 0x38, "");

// Layout evidence: BehaviorTree::queryNode (0x71009423d4) stores the node name at 0x10 and its active flag at 0x18, then
// calls setAllocator on the vector at 0x20; BehaviorNodeInfo::~BehaviorNodeInfo (0x71009429e4) destroys that vector.
class BehaviorNodeInfo : public mm::mtl::ReferenceObject {
public:
  const char *unknown10;                                              // Inferred role: node name; original member name unknown.
  bool unknown18;                                                     // Inferred role: node active flag; original member name unknown.
  mm::mtl::Vector<mm::mtl::IntrusivePtr<BehaviorNodeInfo>> unknown20; // Inferred role: child infos; original member name unknown.
};

static_assert(offsetof(BehaviorNodeInfo, unknown10) == 0x10, "");
static_assert(offsetof(BehaviorNodeInfo, unknown18) == 0x18, "");
static_assert(offsetof(BehaviorNodeInfo, unknown20) == 0x20, "");
static_assert(sizeof(BehaviorNodeInfo) == 0x48, "");

// Layout evidence: TaskNode::TaskNode (0x7100944540) only calls the BehaviorNode constructor and stores the vtable.
class TaskNode : public BehaviorNode {
public:
  static mm::mtl::RTTI m_rtti;

  TaskNode(const char *name, u32 flags);
  ~TaskNode() override;
  const mm::mtl::RTTI *getRTTI() const override;
};

static_assert(sizeof(TaskNode) == 0x38, "");

// Layout evidence: CompositeNode::CompositeNode (0x7100943720) builds the child vector at 0x38 with the allocator argument
// and resizes it to the child count; the member name comes from the assert of CompositeNode::attachChild.
class CompositeNode : public BehaviorNode {
public:
  static mm::mtl::RTTI m_rtti;

  CompositeNode(const char *name, u32 childNum, mm::mtl::IAllocator *allocator);
  ~CompositeNode() override;
  const mm::mtl::RTTI *getRTTI() const override;
  void setActiveNotiryChildren(BlackBoard &blackBoard, bool isActive) override;
  void attachChild(u32 index, BehaviorNode *node);
  void detachChild(u32 index);
  void detachChildAll();

  mm::mtl::Vector<BehaviorNode *> m_childNodes;

protected:
  // Folds visit over the attached children in order; visit returns the updated result for each child.
  template <typename Visitor> bool foldChildren(bool result, Visitor visit) {
    BehaviorNode **it = m_childNodes.begin();
    BehaviorNode **end = m_childNodes.end();
    for (; it != end; it++) {
      BehaviorNode *pNode = *it;
      if (pNode != nullptr) {
        result = visit(pNode, result);
      }
    }
    return result;
  }
};

static_assert(offsetof(CompositeNode, m_childNodes) == 0x38, "");
static_assert(sizeof(CompositeNode) == 0x60, "");

// The game has no constructor symbol for the three composite nodes: BehaviorTree::createComposite inlines them.
class ParallelNode : public CompositeNode {
public:
  static mm::mtl::RTTI m_rtti;

  ParallelNode(const char *name, u32 childNum, mm::mtl::IAllocator *allocator) : CompositeNode(name, childNum, allocator) {}
  ~ParallelNode() override;
  const mm::mtl::RTTI *getRTTI() const override;
  bool execute(BlackBoard &blackBoard) override;
  bool checkCondition(const BlackBoard &blackBoard) override;
};

class SelectorNode : public CompositeNode {
public:
  static mm::mtl::RTTI m_rtti;

  SelectorNode(const char *name, u32 childNum, mm::mtl::IAllocator *allocator) : CompositeNode(name, childNum, allocator) {}
  ~SelectorNode() override;
  const mm::mtl::RTTI *getRTTI() const override;
  bool execute(BlackBoard &blackBoard) override;
  bool checkCondition(const BlackBoard &blackBoard) override;
};

class SequenceNode : public CompositeNode {
public:
  static mm::mtl::RTTI m_rtti;

  SequenceNode(const char *name, u32 childNum, mm::mtl::IAllocator *allocator) : CompositeNode(name, childNum, allocator) {}
  ~SequenceNode() override;
  const mm::mtl::RTTI *getRTTI() const override;
  bool execute(BlackBoard &blackBoard) override;
  bool checkCondition(const BlackBoard &blackBoard) override;
};

static_assert(sizeof(ParallelNode) == 0x60, "");
static_assert(sizeof(SelectorNode) == 0x60, "");
static_assert(sizeof(SequenceNode) == 0x60, "");

} // namespace ai
