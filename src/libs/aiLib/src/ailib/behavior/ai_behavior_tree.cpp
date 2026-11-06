// Original file name from the game string C:/Monolithsoft/BF2/prog/libs/aiLib/src/ailib/behavior/ai_behavior_tree.cpp.
#include "ai/ai_behavior_tree.h"

#include "mm_mtl/intrusive_ptr.h" // IWYU pragma: keep
#include "mm_mtl/vector.h"        // IWYU pragma: keep

template void mm::mtl::Vector<mm::mtl::IntrusivePtr<ai::BehaviorNode>>::destroy();
template void mm::mtl::Vector<mm::mtl::IntrusivePtr<ai::BehaviorNode>>::expand(size_t, size_t);
template void mm::mtl::Vector<mm::mtl::IntrusivePtr<ai::BehaviorNode>>::extend(size_t, size_t, const mm::mtl::IntrusivePtr<ai::BehaviorNode> &);
template void mm::mtl::Vector<mm::mtl::IntrusivePtr<ai::BehaviorNode>>::push_back(const mm::mtl::IntrusivePtr<ai::BehaviorNode> &);
template void mm::mtl::Vector<mm::mtl::IntrusivePtr<ai::BehaviorNode>>::resize(size_t, const mm::mtl::IntrusivePtr<ai::BehaviorNode> &);
template void mm::mtl::Vector<mm::mtl::IntrusivePtr<ai::BehaviorNodeInfo>>::destroy();
template void mm::mtl::Vector<mm::mtl::IntrusivePtr<ai::BehaviorNodeInfo>>::expand(size_t, size_t);

namespace ai {

BlackBoard::BlackBoard() {}

BlackBoard::~BlackBoard() {}

const mm::mtl::RTTI *BlackBoard::getRTTI() const { return &m_rtti; }

BehaviorNode::~BehaviorNode() {}

const mm::mtl::RTTI *BehaviorNode::getRTTI() const { return &m_rtti; }

bool BehaviorNode::execute(BlackBoard &blackBoard) { return true; }

bool BehaviorNode::checkCondition(const BlackBoard &blackBoard) { return true; }

void BehaviorTree::endSetup() { m_setup = false; }

} // namespace ai
