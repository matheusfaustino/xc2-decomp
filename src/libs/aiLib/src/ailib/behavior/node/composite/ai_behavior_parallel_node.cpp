
#include "ai/ai_behavior_node.h"
#include "mm_mtl/vector.h"

namespace ai {

ParallelNode::~ParallelNode() {}

const mm::mtl::RTTI *ParallelNode::getRTTI() const { return &m_rtti; }

bool ParallelNode::checkCondition(const BlackBoard &blackBoard) { return true; }

bool ParallelNode::execute(BlackBoard &blackBoard) {
  bool result = false;
  BehaviorNode **it = m_childNodes.begin();
  BehaviorNode **end = m_childNodes.end();
  for (; it != end; it++) {
    BehaviorNode *pNode = *it;
    if (pNode != nullptr) {
      bool active = false;
      if (pNode->checkCondition(blackBoard)) {
        active = pNode->execute(blackBoard);
      }
      pNode->setActive(blackBoard, active);
      result |= active;
    }
  }
  return result;
}

} // namespace ai
