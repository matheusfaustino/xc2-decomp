#include "ai/ai_behavior_node.h"

namespace ai {

SelectorNode::~SelectorNode() {}

const mm::mtl::RTTI *SelectorNode::getRTTI() const { return &m_rtti; }

bool SelectorNode::checkCondition(const BlackBoard &blackBoard) { return true; }

bool SelectorNode::execute(BlackBoard &blackBoard) {
  return unknown001(false, [&](BehaviorNode *pNode, bool result) {
    bool active = false;
    if (!result && pNode->checkCondition(blackBoard)) {
      active = pNode->execute(blackBoard);
      pNode->setActive(blackBoard, active);
    } else {
      pNode->setActiveNotiryChildren(blackBoard, false);
    }
    return result | active;
  });
}

} // namespace ai
