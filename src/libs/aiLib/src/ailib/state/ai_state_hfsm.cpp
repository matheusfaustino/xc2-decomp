// Original file name from the game string C:/Monolithsoft/BF2/prog/libs/aiLib/src/ailib/state/ai_state_hfsm.cpp.
#include "ai/ai_state_hfsm.h"

#include "mm_mtl/allocator.h"

template void mm::mtl::Vector<ai::Hfsm::StateInfo>::extend(size_t, size_t);
template void mm::mtl::Vector<ai::Hfsm::StateInfo>::resize(size_t);
template void mm::mtl::Queue<void *>::expand(size_t, size_t);
template void mm::mtl::Queue<void *>::push_back(void *const &);

namespace ai {

Hfsm::Hfsm(mm::mtl::IAllocator *allocator)
    : unknown00(nullptr), m_pState(nullptr), m_stateInfos(allocator), unknown38(allocator), unknown68(allocator), unknown70(nullptr), unknown78(0), m_setup(false), m_inside(false) {}

void Hfsm::endSetup() {
  m_setup = false;
  u32 capacity = unknown38.capacity();
  u8 *buffer = static_cast<u8 *>(unknown68->allocate(unknown78 * capacity, 16));
  unknown70 = buffer;
  for (u32 i = 0; i < capacity; i++) {
    unknown38.unknown001(buffer);
    buffer += unknown78;
  }
}

void Hfsm::beginSetup(void *owner, u32 stateCount, u32 maxDepth) {
#line 39
  MM_ASSERT(m_pState == nullptr);
#line 40
  MM_ASSERT(maxDepth <= MAX_DEPTH);
  unknown00 = owner;
  m_stateInfos.resize(stateCount);
  unknown38.reserve(maxDepth * 2);
  m_setup = true;
}

void Hfsm::setState(u32 state) {
#line 87
  MM_ASSERT(!m_inside);
  setStateImpl(state);
}

void Hfsm::cleanup() {
  clearState();
  unknown38.clear();
  unknown68->deallocate(unknown70);
}

void Hfsm::clearState() {
  for (State *state = m_pState; state != nullptr; state = state->unknown10) {
    state->onLeave(unknown00, nullptr);
  }
  State *state = m_pState;
  while (state != nullptr) {
    State *next = state->unknown10;
    state->~State();
    unknown38.push_back(state);
    state = next;
  }
  m_pState = nullptr;
}

bool Hfsm::execEvent(void *event) {
  m_inside = true;
  for (State *state = m_pState; state != nullptr; state = state->unknown10) {
    if (state->onEvent(unknown00, event)) {
      if (state->m_reqState != INVALID_STATE_ID) {
        setStateImpl(state->m_reqState);
      }
      m_inside = false;
      return true;
    }
  }
  m_inside = false;
  return false;
}

} // namespace ai
