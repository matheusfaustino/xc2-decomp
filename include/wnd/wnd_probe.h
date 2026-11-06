#pragma once
#include <stddef.h>

#include "mm_math/mm_vec3.h"
#include "mm_mtl/vector.h"
#include "types.h"
#include "wnd/wnd_object.h"
#include "wnd/wnd_wind.h"

namespace mm {
namespace mtl {
class IAllocator;
} // namespace mtl
} // namespace mm

namespace wnd {
struct UpdateInfo;

// Hit of a wind on a probe, filled by the collision agents.
struct HitCache {
  WindObject *unknown00; // Inferred role: wind; original member name unknown.
  f32 unknown08;         // Inferred role: distance; original member name unknown.
  mm::Vec3 unknown10;    // Inferred role: wind; original member name unknown.
};

static_assert(offsetof(HitCache, unknown08) == 0x08, "");
static_assert(offsetof(HitCache, unknown10) == 0x10, "");
static_assert(sizeof(HitCache) == 0x20, "");

// Wind node of a local wind of the global wind field.
class LocalWind : public WindObject {
public:
  explicit LocalWind(const LocalWindCreateInfo &info);

  void calcEffectiveWind(const mm::Vec3 &position, mm::Vec3 *wind) const;
  f32 calcDampingDistance(f32 distance) const;
  void calcEffectiveWindFormCache(const HitCache &cache, mm::Vec3 *wind) const;

  f32 unknown88;                   // Inferred role: power; original member name unknown.
  f32 unknown8c;                   // Inferred role: minimum distance; original member name unknown.
  f32 unknown90;                   // Inferred role: maximum distance; original member name unknown.
  unknown003<LocalWind> unknown98; // Inferred role: node of the WindWorld list; original member name unknown.
};

static_assert(offsetof(LocalWind, unknown88) == 0x88, "");
static_assert(offsetof(LocalWind, unknown8c) == 0x8c, "");
static_assert(offsetof(LocalWind, unknown90) == 0x90, "");
static_assert(offsetof(LocalWind, unknown98) == 0x98, "");
static_assert(sizeof(LocalWind) == 0xb0, "");

class AreaWind : public WindObject {
public:
  explicit AreaWind(const AreaWindCreateInfo &info);

  void update(const UpdateInfo &info);

  ConpoundWind unknown90;          // Inferred role: wind node, updated by update; original member name unknown.
  f32 unknown260;                  // Inferred role: blend time used by WindProbe::setAreaWind; original member name unknown.
  unknown003<AreaWind> unknown268; // Inferred role: node of the WindWorld list; original member name unknown.
};

static_assert(offsetof(AreaWind, unknown90) == 0x90, "");
static_assert(offsetof(AreaWind, unknown260) == 0x260, "");
static_assert(offsetof(AreaWind, unknown268) == 0x268, "");
static_assert(sizeof(AreaWind) == 0x280, "");

// Local wind hit by a probe. Layout evidence: Dispatcher fills it (area wind, distance, then a Vec3) before
// Vector<LocalWindHitCache>::push_back (0x71009885c4), whose copy moves the first 12 bytes raw and the Vec3 member by
// member; WindProbe keeps the vector at 0x190.
struct LocalWindHitCache {
  AreaWind *unknown00; // Inferred role: wind; original member name unknown.
  f32 unknown08;       // Inferred role: distance; original member name unknown.
  mm::Vec3 unknown10;  // Inferred role: hit vector; original member name unknown.
};

static_assert(offsetof(LocalWindHitCache, unknown08) == 0x08, "");
static_assert(offsetof(LocalWindHitCache, unknown10) == 0x10, "");
static_assert(sizeof(LocalWindHitCache) == 0x20, "");

class WindProbe : public WindObject {
public:
  WindProbe(const ProbeCreateInfo &info, u32 seed, mm::mtl::IAllocator *allocator);
  virtual ~WindProbe();

  void bindGlobalWind(const GlobalWind &wind);
  void unbindGlobalWind();
  void setAreaWind(const AreaWind *wind);
  void calcGlobalWindPower(const UpdateInfo &info);

  u32 unknown88;                                 // Inferred role: wind index used by fw::WindManager::getGlobalWindPower; original member name unknown.
  WindBlender unknown90;                         // Inferred role: blended wind; original member name unknown.
  const GlobalWind *unknown180;                  // Inferred role: bound global wind; original member name unknown.
  const AreaWind *unknown188;                    // Inferred role: current area wind; original member name unknown.
  mm::mtl::Vector<LocalWindHitCache> unknown190; // Inferred role: hits of the local winds; original member name unknown.
  void *m_pOverlappingCache;                     // Named by the assertion "m_pOverlappingCache == nullptr" of the destructor.
};

static_assert(offsetof(WindProbe, unknown88) == 0x88, "");
static_assert(offsetof(WindProbe, unknown90) == 0x90, "");
static_assert(offsetof(WindProbe, unknown180) == 0x180, "");
static_assert(offsetof(WindProbe, unknown188) == 0x188, "");
static_assert(offsetof(WindProbe, unknown190) == 0x190, "");
static_assert(offsetof(WindProbe, m_pOverlappingCache) == 0x1b8, "");
static_assert(sizeof(WindProbe) == 0x1c0, "");

} // namespace wnd
