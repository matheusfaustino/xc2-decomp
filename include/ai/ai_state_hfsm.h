#pragma once
#include <stddef.h>

#include "ai/ai_state.h"
#include "mm/mm_std_base.h"
#include "mm_math/mm_math.h"
#include "mm_mtl/queue.h"
#include "mm_mtl/vector.h"
#include "types.h"

namespace mm {
namespace mtl {
class IAllocator;
} // namespace mtl
} // namespace mm

namespace ai {

// Inferred role: maximum nesting depth of the state hierarchy; original name from the assert of Hfsm::beginSetup.
static const u32 MAX_DEPTH = 8;

class Hfsm {
public:
  typedef State *(*CreateFunc)(void *buffer);

  struct StateInfo {
    // Decoded from the fill loop of Vector<StateInfo>::resize and extend (null create, parent -1).
    StateInfo() : unknown00(nullptr), unknown08(-1) {}

    CreateFunc unknown00; // Inferred role: create; original member name unknown.
    int unknown08;        // Inferred role: parent; original member name unknown.
  };

  // __LINE__ must be 50 as in the original header: it is the line argument of the assert.
#line 49
  template <typename T> void registerState(u32 stateID, int parent) {
    MM_ASSERT(m_setup);
    m_stateInfos[stateID].unknown00 = &T::create;
    m_stateInfos[stateID].unknown08 = parent;
    unknown78 = (mm::max(sizeof(T), unknown78) + 15) & ~static_cast<size_t>(15);
  }

  explicit Hfsm(mm::mtl::IAllocator *allocator);
  void beginSetup(void *owner, u32 stateCount, u32 param3);
  void endSetup();
  void cleanup();
  void clearState();
  void setState(u32 state);
  void update(const void *info);
  bool execEvent(void *event);

private:
  void setStateImpl(u32 state);

public:
  void *unknown00; // Inferred role: p owner; original member name unknown.
  State *m_pState;
  mm::mtl::Vector<StateInfo> m_stateInfos;
  mm::mtl::Queue<void *> unknown38; // Inferred role: destroyed states, pushed by cleanup; original member name unknown.
  mm::mtl::IAllocator *unknown68;   // Inferred role: allocator of the state buffer; original member name unknown.
  void *unknown70;                  // Inferred role: state buffer, released by cleanup; original member name unknown.
  size_t unknown78;                 // Inferred role: state size; original member name unknown.
  bool m_setup;
  bool m_inside; // Inferred role: inside update/event dispatch; original member name unknown.
};

static_assert(sizeof(Hfsm::StateInfo) == 0x10, "");
static_assert(offsetof(Hfsm, m_stateInfos) == 0x10, "");
static_assert(offsetof(Hfsm, unknown38) == 0x38, "");
static_assert(offsetof(Hfsm, unknown68) == 0x68, "");
static_assert(offsetof(Hfsm, unknown78) == 0x78, "");
static_assert(offsetof(Hfsm, m_setup) == 0x80, "");

} // namespace ai
