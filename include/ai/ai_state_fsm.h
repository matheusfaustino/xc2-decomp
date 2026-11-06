#pragma once
#include <stddef.h>

#include "ai/ai_state.h"
#include "mm/mm_std_base.h"
#include "mm_math/mm_math.h"
#include "mm_mtl/fixed_queue.h"
#include "mm_mtl/vector.h"
#include "types.h"

namespace mm {
namespace mtl {
class IAllocator;
}
} // namespace mm

namespace ai {

class Fsm {
public:
  typedef State *(*CreateFunc)(void *buffer);

  Fsm(mm::mtl::IAllocator *allocator);
  void beginSetup(void *owner, u32 stateCount);

  // __LINE__ must be 31 as in the original header: it is the line argument of the assert.
#line 30
  template <typename T> void registerState(u32 stateID) {
    MM_ASSERT(m_setup);
    m_createFuncs[stateID] = &T::create;
    unknown68 = (mm::max(sizeof(T), unknown68) + 15) & ~static_cast<size_t>(15);
  }

  void endSetup();
  void cleanup();
  void clearState();
  void setState(u32 stateID);
  void update(const void *param);
  bool execEvent(void *event);
  State *getState() const { return m_pState; }

private:
  // The game defines it out of line; not reconstructed yet.
  void setStateImpl(u32 stateID);

  void *unknown00; // Inferred role: p owner; original member name unknown.
  State *m_pState;
  mm::mtl::Vector<CreateFunc> m_createFuncs;
  mm::mtl::FixedQueue<void *, 2> m_stateBufferQueue;
  mm::mtl::IAllocator *m_allocator;
  void *unknown60;  // Inferred role: state buffer; original member name unknown.
  size_t unknown68; // Inferred role: state size; original member name unknown.
  bool m_setup;
  bool m_inside;
};

static_assert(sizeof(Fsm) == 0x78, "");

} // namespace ai
