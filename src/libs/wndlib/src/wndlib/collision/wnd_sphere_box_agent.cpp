// Original file name from the game string .../wndlib/src/wndlib/collision/wnd_sphere_box_agent.cpp.
#include "wnd/wnd_dispatcher.h" // IWYU pragma: keep

namespace wnd {

void SphereBoxAgent::registerAgent(Dispatcher &dispatcher) { dispatcher.unknown18 = &SphereBoxAgent::processCollision; }

} // namespace wnd
