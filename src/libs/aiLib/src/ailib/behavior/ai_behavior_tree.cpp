// Original file name from the game string C:/Monolithsoft/BF2/prog/libs/aiLib/src/ailib/behavior/ai_behavior_tree.cpp.
#include "ai/ai_behavior_tree.h"

#include "ai/ai_behavior_decorator.h"
#include "mm_mtl/fixstr.h"
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

BehaviorTree::BehaviorTree() : unknown00(), unknown40(), m_pNodes(), m_pAllocator(nullptr), m_setup(false) {}

BehaviorTree::~BehaviorTree() {}

BlackBoard::BlackBoard() {}

BlackBoard::~BlackBoard() {}

const mm::mtl::RTTI *BlackBoard::getRTTI() const { return &m_rtti; }

BehaviorNode::BehaviorNode(const char *name, u32 flags) : unknown30(false), unknown34(flags) { unknown0c = name; }

BehaviorNode::~BehaviorNode() {}

const mm::mtl::RTTI *BehaviorNode::getRTTI() const { return &m_rtti; }

bool BehaviorNode::execute(BlackBoard &blackBoard) { return true; }

bool BehaviorNode::checkCondition(const BlackBoard &blackBoard) { return true; }

void BehaviorTree::beginSetup(u32 nodeNum, mm::mtl::IAllocator *allocator) {
  m_pAllocator = allocator;
  m_pNodes.setAllocator(allocator);
  m_pNodes.resize(nodeNum, nullptr);
  m_setup = true;
}

void BehaviorTree::endSetup() { m_setup = false; }

void BehaviorTree::attachRoot(u32 nodeID) {
  BehaviorNode *pNode = m_pNodes[nodeID].get();
#line 88
  MM_ASSERT(pNode);
  unknown00.attachChild(pNode);
}

void BehaviorTree::attachComposite(u32 parentID, u32 childID, u32 index) {
  BehaviorNode *pNode = m_pNodes[parentID].get();
  CompositeNode *pComposite = nullptr;
  if (pNode != nullptr && pNode->getRTTI()->isKindOf(&CompositeNode::m_rtti)) {
    pComposite = static_cast<CompositeNode *>(pNode);
  }
#line 104
  MM_ASSERT(pComposite);
  BehaviorNode *pChildNode = m_pNodes[childID].get();
#line 107
  MM_ASSERT(pChildNode);
  pComposite->attachChild(index, pChildNode);
}

void BehaviorTree::attachDecorator(u32 nodeID, u32 childID) {
  BehaviorNode *pNode = m_pNodes[nodeID].get();
  DecoratorNode *pDecorator = nullptr;
  if (pNode != nullptr && pNode->getRTTI()->isKindOf(&DecoratorNode::m_rtti)) {
    pDecorator = static_cast<DecoratorNode *>(pNode);
  }
#line 121
  MM_ASSERT(pDecorator);
  BehaviorNode *pChildNode = m_pNodes[childID].get();
#line 124
  MM_ASSERT(pChildNode);
  pDecorator->attachChild(pChildNode);
}

void BehaviorTree::execute() {
  BlackBoard *pBB = unknown40.get();
#line 157
  MM_ASSERT(pBB);
  bool result = unknown00.execute(*pBB);
  unknown00.setActive(*pBB, result);
}

void BehaviorTree::queryNode(mm::mtl::IAllocator *allocator, BehaviorNodeInfo *info) const {
  BehaviorNode *pChildNode = unknown00.m_pChildNode;
  if (pChildNode == nullptr) {
    return;
  }
  info->unknown10 = pChildNode->unknown0c.c_str();
  info->unknown18 = pChildNode->unknown30;
  info->unknown20.setAllocator(allocator);
  collectChildNode(pChildNode, allocator, info);
}

} // namespace ai
