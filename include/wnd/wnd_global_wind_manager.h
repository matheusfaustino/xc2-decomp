#pragma once
#include <stddef.h>

#include "mm/mm_std_base.h"
#include "mm_mtl/intrusive_list.h"
#include "mm_mtl/reference_object.h"
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

struct Query;
class AreaWind;
class LocalWind;
class WindProbe;
class BroadPhazeInterface;
class Dispatcher;
class WindObject;
struct ResConpoundWind;
struct UpdateInfo;

// Array of pointers with the assertions of the original wnd_array-inl.h.
template <typename T> class Array {
public:
  // __LINE__ must be 231 and 232 as in the original header: they are the line arguments of the asserts.
#line 230
  T &operator[](size_t index) {
    MM_ASSERT(m_pElements);
    MM_ASSERT(index < m_size);
    return m_pElements[index];
  }

  // __LINE__ must be 231 and 232 as in the original header: they are the line arguments of the asserts.
#line 230
  const T &operator[](size_t index) const {
    MM_ASSERT(m_pElements);
    MM_ASSERT(index < m_size);
    return m_pElements[index];
  }

  size_t size() const { return m_size; }

  T *m_pElements;
  size_t m_size;
};

static_assert(sizeof(Array<const ResConpoundWind *>) == 0x10, "");

class GlobalWindManager {
public:
  GlobalWindManager();
  void setup(const Array<const ResConpoundWind *> &resWind, mm::mtl::IAllocator *allocator);
  bool setCurrentWind(int index);
  void setDebugWind(bool enable, u32 mask);
  void cleanup();

  mm::mtl::Vector<ConpoundWind> unknown00; // Inferred role: winds built by setup; original member name unknown.
  const ResConpoundWind **unknown28;       // Inferred role: wind resources, copied by setup; original member name unknown.
  u32 unknown30;                           // Inferred role: number of wind resources; original member name unknown.
  u8 _34[0x40 - 0x34];
  GlobalWind unknown40; // Inferred role: global wind; original member name unknown.
  int unknown140;       // Inferred role: current wind index; original member name unknown.
  int unknown144;       // Inferred role: debug wind index; original member name unknown.
  u8 _148[0x150 - 0x148];
};

static_assert(offsetof(GlobalWindManager, unknown28) == 0x28, "");
static_assert(offsetof(GlobalWindManager, unknown30) == 0x30, "");
static_assert(offsetof(GlobalWindManager, unknown40) == 0x40, "");
static_assert(offsetof(GlobalWindManager, unknown140) == 0x140, "");
static_assert(offsetof(GlobalWindManager, unknown144) == 0x144, "");
static_assert(sizeof(GlobalWindManager) == 0x150, "");

class WindWorld : public mm::mtl::ReferenceObject {
public:
  WindWorld(Dispatcher *dispatcher, BroadPhazeInterface *broadPhaze, const Array<const ResConpoundWind *> &resWind, mm::mtl::IAllocator *allocator);
  ~WindWorld() override;

  void update(f32 deltaTime);
  bool query(Query *query);
  bool addProbe(WindProbe &probe);
  void removeProbe(WindProbe &probe);
  bool addAreaWind(AreaWind &wind);
  void removeAreaWind(AreaWind &wind);
  bool addLocalWind(LocalWind &wind);
  void removeLocalWind(LocalWind &wind);
  void setDebugMode(bool enable, u32 mask);
  void delayErace();
  void updateProbe(const UpdateInfo &info);
  void updateGlobalWind(const UpdateInfo &info);
  void createOverlappingPairCache(WindProbe &probe);
  void destroyOverlappingPairCache(WindProbe &probe);

  u32 unknown0c;                                             // Inferred role: accumulated time; original member name unknown.
  GlobalWindManager unknown10;                               // Inferred role: global wind; original member name unknown.
  BroadPhazeInterface *m_pBroadPhaze;                        // Named by the assertion "m_pBroadPhaze" of wnd_world.cpp.
  Dispatcher *m_pDispatcher;                                 // Inferred role: dispatcher; original member name unknown.
  mm::mtl::IntrusiveList<WindProbe> unknown170;              // Inferred role: probes; original member name unknown.
  mm::mtl::IntrusiveList<unknown003<AreaWind>> unknown188;   // Inferred role: area winds; original member name unknown.
  mm::mtl::IntrusiveList<unknown003<LocalWind>> unknown1a0;  // Inferred role: local winds; original member name unknown.
  mm::mtl::IntrusiveList<unknown003<WindObject>> unknown1b8; // Inferred role: objects to erase; original member name unknown.
  bool unknown1d0;                                           // Inferred role: debug mode; original member name unknown.
  u8 _1d1[0x1d8 - 0x1d1];
};

static_assert(offsetof(WindWorld, unknown10) == 0x10, "");
static_assert(offsetof(WindWorld, m_pBroadPhaze) == 0x160, "");
static_assert(offsetof(WindWorld, m_pDispatcher) == 0x168, "");
static_assert(offsetof(WindWorld, unknown170) == 0x170, "");
static_assert(offsetof(WindWorld, unknown188) == 0x188, "");
static_assert(offsetof(WindWorld, unknown1a0) == 0x1a0, "");
static_assert(offsetof(WindWorld, unknown1b8) == 0x1b8, "");
static_assert(offsetof(WindWorld, unknown1d0) == 0x1d0, "");

} // namespace wnd
