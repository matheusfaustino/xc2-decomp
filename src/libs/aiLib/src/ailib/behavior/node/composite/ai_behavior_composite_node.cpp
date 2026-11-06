// Original file name from the game string C:/Monolithsoft/BF2/prog/libs/aiLib/src/ailib/behavior/node/composite/ai_behavior_composite_node.cpp.
#include "ai/ai_behavior_node.h"
#include "mm/mm_std_base.h"
#include "mm_mtl/vector.h" // IWYU pragma: keep
#include "types.h"

namespace mm {
namespace mtl {
class IAllocator;
} // namespace mtl
} // namespace mm

template void mm::mtl::Vector<ai::BehaviorNode *>::expand(size_t, size_t);
template void mm::mtl::Vector<ai::BehaviorNode *>::extend(size_t, size_t, ai::BehaviorNode *const &);
template void mm::mtl::Vector<ai::BehaviorNode *>::resize(size_t, ai::BehaviorNode *const &);

namespace ai {

CompositeNode::CompositeNode(const char *name, u32 childNum, mm::mtl::IAllocator *allocator) : BehaviorNode(name, 0), m_childNodes(allocator) { m_childNodes.resize(childNum, nullptr); }

CompositeNode::~CompositeNode() {}

const mm::mtl::RTTI *CompositeNode::getRTTI() const { return &m_rtti; }

void CompositeNode::setActiveNotiryChildren(BlackBoard &blackBoard, bool isActive) {
  setActive(blackBoard, isActive);
  BehaviorNode **it = m_childNodes.begin();
  BehaviorNode **end = m_childNodes.end();
  for (; it != end; it++) {
    if (*it != nullptr) {
      (*it)->setActiveNotiryChildren(blackBoard, isActive);
    }
  }
}

void CompositeNode::attachChild(u32 index, BehaviorNode *node) {
#line 68
  MM_ASSERT(m_childNodes[index] == nullptr);
  m_childNodes[index] = node;
}

void CompositeNode::detachChild(u32 index) { m_childNodes[index] = nullptr; }

void CompositeNode::detachChildAll() {
  for (u32 i = 0; i < m_childNodes.size(); i++) {
    m_childNodes[i] = nullptr;
  }
}

} // namespace ai
