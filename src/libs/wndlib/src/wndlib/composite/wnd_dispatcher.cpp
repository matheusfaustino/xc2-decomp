// Original file name from the game string C:/Monolithsoft/BF2/prog/libs/wndlib/src/wndlib/composite/wnd_dispatcher.cpp.
#include "wnd/wnd_dispatcher.h" // IWYU pragma: keep

namespace wnd {

Dispatcher::Dispatcher(const CollisionInfo &info) : unknown30(info) {
  unknown10 = nullptr;
  unknown18 = nullptr;
  unknown20 = nullptr;
  unknown28 = nullptr;
  SphereSphereAgent::registerAgent(*this);
  SphereBoxAgent::registerAgent(*this);
}

} // namespace wnd
