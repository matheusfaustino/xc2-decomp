// Original file name from the game string .../wndlib/src/wndlib/collision/wnd_sphere_sphere_agent.cpp.
#include "wnd/wnd_dispatcher.h" // IWYU pragma: keep

namespace wnd {

void SphereSphereAgent::registerAgent(Dispatcher &dispatcher) { dispatcher.unknown10 = &SphereSphereAgent::processCollision; }

} // namespace wnd
