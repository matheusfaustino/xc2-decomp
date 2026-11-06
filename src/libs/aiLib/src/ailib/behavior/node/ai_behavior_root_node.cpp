// Original file name from the game string C:/Monolithsoft/BF2/prog/libs/aiLib/src/ailib/behavior/node/ai_behavior_root_node.cpp.
#include "ai/ai_behavior_root_node.h"
#include "ai/ai_behavior_node.h"
#include "mm/mm_std_base.h"

namespace ai {

RootNode::RootNode() : BehaviorNode("RootNode", 0) { m_pChildNode = nullptr; }

RootNode::~RootNode() {}

const mm::mtl::RTTI *RootNode::getRTTI() const { return &m_rtti; }

// The game has an out-of-line copy of this body for RootNode and another for DecoratorNode.
bool RootNode::execute(BlackBoard &blackBoard) {
  if (!m_pChildNode->checkCondition(blackBoard)) {
    m_pChildNode->setActiveNotiryChildren(blackBoard, false);
    return false;
  }
  bool result = m_pChildNode->execute(blackBoard);
  m_pChildNode->setActive(blackBoard, result);
  return result;
}

void RootNode::detachChild() { m_pChildNode = nullptr; }

void RootNode::attachChild(BehaviorNode *node) {
#line 42
  MM_ASSERT(m_pChildNode == nullptr);
  m_pChildNode = node;
}

} // namespace ai
