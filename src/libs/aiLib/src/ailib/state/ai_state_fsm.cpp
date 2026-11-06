// Original file name from the game string C:/Monolithsoft/BF2/prog/libs/aiLib/src/ailib/state/ai_state_fsm.cpp.
#include "ai/ai_state_fsm.h"

#include "mm_mtl/unique_ptr.h" // IWYU pragma: keep

template void mm::mtl::Vector<ai::Fsm::CreateFunc>::expand(size_t, size_t);
template void mm::mtl::Vector<ai::Fsm::CreateFunc>::extend(size_t, size_t, const ai::Fsm::CreateFunc &);
template void mm::mtl::Vector<ai::Fsm::CreateFunc>::resize(size_t, const ai::Fsm::CreateFunc &);
template void mm::mtl::UniquePtr<ai::Fsm>::_safe_bool_helper::safe_bool_function() const;

namespace ai {

bool State::onEvent(void *owner, void *event) { return false; }

} // namespace ai
