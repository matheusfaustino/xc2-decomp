#include "ai/ai_behavior_node.h"

namespace ai {

SequenceNode::~SequenceNode() {}

const mm::mtl::RTTI *SequenceNode::getRTTI() const { return &m_rtti; }

bool SequenceNode::checkCondition(const BlackBoard &blackBoard) { return true; }

bool SequenceNode::execute(BlackBoard &blackBoard) {
  return unknown001(true, [&](BehaviorNode *pNode, bool result) {
    bool active = false;
    if (result && pNode->checkCondition(blackBoard)) {
      active = pNode->execute(blackBoard);
      pNode->setActive(blackBoard, active);
    } else {
      pNode->setActiveNotiryChildren(blackBoard, false);
    }
    return result & active;
  });
}

} // namespace ai
