
#include "ai/ai_behavior_node.h"
#include "mm_mtl/vector.h"

namespace ai {

SelectorNode::~SelectorNode() {}

const mm::mtl::RTTI *SelectorNode::getRTTI() const { return &m_rtti; }

bool SelectorNode::checkCondition(const BlackBoard &blackBoard) { return true; }

bool SelectorNode::execute(BlackBoard &blackBoard) {
  bool result = false;
  BehaviorNode **it = m_childNodes.begin();
  BehaviorNode **end = m_childNodes.end();
  for (; it != end; it++) {
    BehaviorNode *pNode = *it;
    if (pNode != nullptr) {
      bool active = false;
      if (!result && pNode->checkCondition(blackBoard)) {
        active = pNode->execute(blackBoard);
        pNode->setActive(blackBoard, active);
      } else {
        pNode->setActiveNotiryChildren(blackBoard, false);
      }
      result |= active;
    }
  }
  return result;
}

} // namespace ai
