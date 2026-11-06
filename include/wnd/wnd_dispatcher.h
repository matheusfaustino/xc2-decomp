#pragma once
#include <stddef.h>

#include "mm_mtl/intrusive_list.h"
#include "mm_mtl/reference_object.h"
#include "types.h"

namespace wnd {

class WindObject;
class WindProbe;
struct HitCache;
struct UpdateInfo;

// Settings of the collision dispatcher; the game reads member 0.
struct CollisionInfo {
  u32 unknown00; // Inferred role: collision mask; original member name unknown.
};

static_assert(sizeof(CollisionInfo) == 0x04, "");

class Dispatcher;

class SphereSphereAgent {
public:
  static void registerAgent(Dispatcher &dispatcher);
  static void processCollision(const WindObject &wind0, const WindObject &wind1, f32 radius, HitCache *cache);
};

class SphereBoxAgent {
public:
  static void registerAgent(Dispatcher &dispatcher);
  static void processCollision(const WindObject &wind0, const WindObject &wind1, f32 radius, HitCache *cache);
};

class Dispatcher : public mm::mtl::ReferenceObject {
public:
  explicit Dispatcher(const CollisionInfo &info);

  bool dispatchAll(mm::mtl::IntrusiveList<WindProbe> &probes, const UpdateInfo &info);

  void (*unknown10)(const WindObject &, const WindObject &, f32, HitCache *); // Inferred role: sphere/sphere test.
  void (*unknown18)(const WindObject &, const WindObject &, f32, HitCache *); // Inferred role: sphere/box test.
  void *unknown20;                                                            // Role and original member name unknown.
  void *unknown28;                                                            // Role and original member name unknown.
  CollisionInfo unknown30;                                                    // Inferred role: settings; original member name unknown.
  u8 _34[0x38 - 0x34];
};

static_assert(offsetof(Dispatcher, unknown10) == 0x10, "");
static_assert(offsetof(Dispatcher, unknown18) == 0x18, "");
static_assert(offsetof(Dispatcher, unknown20) == 0x20, "");
static_assert(offsetof(Dispatcher, unknown28) == 0x28, "");
static_assert(offsetof(Dispatcher, unknown30) == 0x30, "");

} // namespace wnd
