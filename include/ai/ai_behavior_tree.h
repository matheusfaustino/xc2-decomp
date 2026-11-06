#pragma once
#include <new>
#include <stddef.h>

#include "ai/ai_behavior_node.h"
#include "ai/ai_behavior_root_node.h"
#include "mm/mm_std_base.h"
#include "mm_mtl/allocator.h"
#include "mm_mtl/intrusive_ptr.h"
#include "mm_mtl/rtti.h"
#include "mm_mtl/unique_ptr.h"
#include "mm_mtl/vector.h"
#include "types.h"

namespace ai {

// Layout evidence: BehaviorTree::BehaviorTree (0x7100941b2c) constructs the root node at 0x00 and clears the black board
// at 0x40, the node vector at 0x48, the allocator at 0x70 and the setup flag at 0x78; the game has no BehaviorTree vtable.
// The member names m_pNodes, m_pAllocator and m_setup come from the asserts of the createTask template.
class BehaviorTree {
public:
  BehaviorTree();
  ~BehaviorTree();
  void beginSetup(u32 nodeNum, mm::mtl::IAllocator *allocator);
  void endSetup();
  void cleanup();
  void attachRoot(u32 nodeID);
  void attachComposite(u32 parentID, u32 childID, u32 index);
  void attachDecorator(u32 nodeID, u32 childID);
  void execute();
  void queryNode(mm::mtl::IAllocator *allocator, BehaviorNodeInfo *info) const;
  void collectChildNode(BehaviorNode *node, mm::mtl::IAllocator *allocator, BehaviorNodeInfo *info) const;

  // __LINE__ must be 47 as in the original header: it is the line argument of the first assert.
  // Inferred role: creates the black board of the given type; original name unknown.
#line 46
  template <typename T> void unknown003() {
    MM_ASSERT(m_setup == true);
    MM_ASSERT(m_pAllocator);
    void *buffer = m_pAllocator->allocate(sizeof(T), alignof(T));
    unknown40.reset(new (buffer) T());
  }

  // __LINE__ must be 60 as in the original header: it is the line argument of the first assert.
#line 59
  template <typename T> void createTask(u32 nodeID, const char *name) {
    MM_ASSERT(m_setup == true);
    MM_ASSERT(m_pAllocator);
    MM_ASSERT(m_pNodes[nodeID] == nullptr);
    void *buffer = m_pAllocator->allocate(sizeof(T), alignof(T));
    m_pNodes[nodeID] = new (buffer) T(name);
    if ((m_pNodes[nodeID]->unknown34 & 2) != 0) {
      CreateEvent event;
      m_pNodes[nodeID]->onEvent(*unknown40, event);
    }
  }

  // __LINE__ must be 79 as in the original header: it is the line argument of the first assert.
#line 78
  template <typename T> void createComposite(u32 nodeID, const char *name, u32 childNum) {
    MM_ASSERT(m_setup == true);
    MM_ASSERT(m_pAllocator);
    MM_ASSERT(m_pNodes[nodeID] == nullptr);
    void *buffer = m_pAllocator->allocate(sizeof(T), alignof(T));
    m_pNodes[nodeID] = new (buffer) T(name, childNum, m_pAllocator);
  }

  // Inferred role: black board of the given type, null when it has another type; original name unknown.
  template <typename T> T *unknown001() const {
    BlackBoard *blackBoard = unknown40.get();
    if (blackBoard == nullptr) {
      return nullptr;
    }
    return mm::mtl::dynamicCast<T>(blackBoard);
  }

  RootNode unknown00;                       // Inferred role: root node; original member name unknown.
  mm::mtl::UniquePtr<BlackBoard> unknown40; // Inferred role: black board; original member name unknown.
  mm::mtl::Vector<mm::mtl::IntrusivePtr<BehaviorNode>> m_pNodes;
  mm::mtl::IAllocator *m_pAllocator;
  bool m_setup;
};

static_assert(offsetof(BehaviorTree, unknown40) == 0x40, "");
static_assert(offsetof(BehaviorTree, m_pNodes) == 0x48, "");
static_assert(offsetof(BehaviorTree, m_pAllocator) == 0x70, "");
static_assert(offsetof(BehaviorTree, m_setup) == 0x78, "");
static_assert(sizeof(BehaviorTree) == 0x80, "");

} // namespace ai
