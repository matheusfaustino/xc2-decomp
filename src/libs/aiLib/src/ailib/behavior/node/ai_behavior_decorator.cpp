// Original file name from the game string C:/Monolithsoft/BF2/prog/libs/aiLib/src/ailib/behavior/node/ai_behavior_decorator.cpp.
#include "ai/ai_behavior_decorator.h"
#include "mm/mm_std_base.h"

namespace ai {

DecoratorNode::DecoratorNode(const char *name) : BehaviorNode(name, 0) { m_pChildNode = nullptr; }

DecoratorNode::~DecoratorNode() {}

const mm::mtl::RTTI *DecoratorNode::getRTTI() const { return &m_rtti; }

bool DecoratorNode::execute(BlackBoard &blackBoard) {
  if (!m_pChildNode->checkCondition(blackBoard)) {
    m_pChildNode->setActiveNotiryChildren(blackBoard, false);
    return false;
  }
  bool result = m_pChildNode->execute(blackBoard);
  m_pChildNode->setActive(blackBoard, result);
  return result;
}

void DecoratorNode::attachChild(BehaviorNode *node) {
#line 43
  MM_ASSERT(m_pChildNode == nullptr);
  m_pChildNode = node;
}

void DecoratorNode::detachChild() { m_pChildNode = nullptr; }

} // namespace ai
