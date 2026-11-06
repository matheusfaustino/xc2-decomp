#pragma once
#include <stddef.h>

#include "ai/ai_behavior_node.h"
#include "mm/mm_std_base.h"

namespace mm {
namespace mtl {
class RTTI;
} // namespace mtl
} // namespace mm

namespace ai {

// Layout evidence: RootNode::RootNode (0x71009434e0) clears the child node at 0x38, named by the assert of
// RootNode::attachChild.
class RootNode : public BehaviorNode {
public:
  static mm::mtl::RTTI m_rtti;

  RootNode();
  ~RootNode() override;
  const mm::mtl::RTTI *getRTTI() const override;
  bool execute(BlackBoard &blackBoard) override;
  // __LINE__ must be 39 as in the original header: it is the line argument of the assert.
#line 38
  void setActiveNotiryChildren(BlackBoard &blackBoard, bool isActive) override {
    setActive(blackBoard, isActive);
    MM_ASSERT(m_pChildNode);
    m_pChildNode->setActiveNotiryChildren(blackBoard, isActive);
  }
  void attachChild(BehaviorNode *node);
  void detachChild();

  BehaviorNode *m_pChildNode;
};

static_assert(offsetof(RootNode, m_pChildNode) == 0x38, "");
static_assert(sizeof(RootNode) == 0x40, "");

} // namespace ai
