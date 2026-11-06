#include "ai/ai_behavior_node.h"

namespace ai {

ParallelNode::~ParallelNode() {}

const mm::mtl::RTTI *ParallelNode::getRTTI() const { return &m_rtti; }

bool ParallelNode::checkCondition(const BlackBoard &blackBoard) { return true; }

bool ParallelNode::execute(BlackBoard &blackBoard) {
  return unknown001(false, [&](BehaviorNode *pNode, bool result) {
    bool active = false;
    if (pNode->checkCondition(blackBoard)) {
      active = pNode->execute(blackBoard);
    }
    pNode->setActive(blackBoard, active);
    return result | active;
  });
}

} // namespace ai
