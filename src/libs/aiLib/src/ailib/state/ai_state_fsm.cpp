// Original file name from the game string C:/Monolithsoft/BF2/prog/libs/aiLib/src/ailib/state/ai_state_fsm.cpp.
#include "ai/ai_state_fsm.h"

#include "mm/mm_std_base.h"
#include "mm_mtl/allocator.h"
#include "mm_mtl/unique_ptr.h" // IWYU pragma: keep

template void mm::mtl::Vector<ai::Fsm::CreateFunc>::expand(size_t, size_t);
template void mm::mtl::Vector<ai::Fsm::CreateFunc>::extend(size_t, size_t, const ai::Fsm::CreateFunc &);
template void mm::mtl::Vector<ai::Fsm::CreateFunc>::resize(size_t, const ai::Fsm::CreateFunc &);
template void mm::mtl::UniquePtr<ai::Fsm>::_safe_bool_helper::safe_bool_function() const;

namespace ai {

bool State::onEvent(void *owner, void *event) { return false; }

Fsm::Fsm(mm::mtl::IAllocator *allocator)
    : unknown00(nullptr), m_pState(nullptr), m_createFuncs(allocator), m_stateBufferQueue(), m_allocator(allocator), unknown60(nullptr), unknown68(0), m_setup(false), m_inside(false) {}

void Fsm::beginSetup(void *owner, u32 stateCount) {
#line 37
  MM_ASSERT(m_pState == nullptr);
  unknown00 = owner;
  m_createFuncs.resize(stateCount, nullptr);
  m_setup = true;
}

void Fsm::endSetup() {
  m_setup = false;
  unknown60 = m_allocator->allocate(unknown68 * 2, 16);
  m_stateBufferQueue.push(unknown60);
  m_stateBufferQueue.push(static_cast<void *>(static_cast<u8 *>(unknown60) + unknown68));
}

void Fsm::cleanup() {
  clearState();
  m_stateBufferQueue.clear();
  m_allocator->deallocate(unknown60);
}

void Fsm::clearState() {
  if (m_pState != nullptr) {
    m_pState->onLeave(unknown00, nullptr);
    m_pState->~State();
    m_stateBufferQueue.push(m_pState);
    m_pState = nullptr;
  }
}

void Fsm::setState(u32 stateID) {
#line 79
  MM_ASSERT(!m_inside);
  setStateImpl(stateID);
}

void Fsm::update(const void *param) {
  m_inside = true;
  if (m_pState != nullptr) {
    m_pState->onStep(unknown00, param);
    if (m_pState->m_reqState != INVALID_STATE_ID) {
      setStateImpl(m_pState->m_reqState);
    }
  }
  m_inside = false;
}

bool Fsm::execEvent(void *event) {
  m_inside = true;
  bool result = false;
  if (m_pState->onEvent(unknown00, event)) {
    if (m_pState->m_reqState != INVALID_STATE_ID) {
      setStateImpl(m_pState->m_reqState);
    }
    result = true;
  }
  m_inside = false;
  return result;
}

} // namespace ai
