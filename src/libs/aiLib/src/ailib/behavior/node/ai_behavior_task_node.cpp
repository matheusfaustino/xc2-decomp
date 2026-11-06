#include "ai/ai_behavior_node.h"
#include "types.h"

namespace ai {

TaskNode::TaskNode(const char *name, u32 flags) : BehaviorNode(name, flags) {}

TaskNode::~TaskNode() {}

const mm::mtl::RTTI *TaskNode::getRTTI() const { return &m_rtti; }

} // namespace ai
