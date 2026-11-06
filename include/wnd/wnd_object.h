#pragma once
#include <stddef.h>

#include "mm_mtl/intrusive_list.h"
#include "mm_mtl/reference_object.h"
#include "types.h"
#include "wnd/wnd_shape.h"

namespace wnd {

struct ResConpoundWind;

// ObjectType values passed to WindObject: LocalWind passes 0, WindProbe 1 and AreaWind 2; the original
// enumerator names are unknown.
enum ObjectType {
  unknown002_0 = 0,
  unknown002_1 = 1,
  unknown002_2 = 2,
};

// Member names come from the assertions of WindObject::WindObject ("info.pTransform", "info.pShape").
// The game stores the members of the derived create infos from 0x14 on, so this prefix is 4-byte packed.
#pragma pack(push, 4)
struct WindObjectCreateInfo {
  const Transform *pTransform;
  Shape *pShape;
  u32 unknown10; // Inferred role: group; original member name unknown.
};
#pragma pack(pop)

static_assert(offsetof(WindObjectCreateInfo, pShape) == 0x08, "");
static_assert(offsetof(WindObjectCreateInfo, unknown10) == 0x10, "");
static_assert(sizeof(WindObjectCreateInfo) == 0x14, "");

struct ProbeCreateInfo : WindObjectCreateInfo {
  u32 unknown14; // Inferred role: group; original member name unknown.
};

static_assert(offsetof(ProbeCreateInfo, unknown14) == 0x14, "");
static_assert(sizeof(ProbeCreateInfo) == 0x18, "");

struct LocalWindCreateInfo : WindObjectCreateInfo {
  f32 unknown14; // Inferred role: power; original member name unknown.
  f32 unknown18; // Inferred role: minimum distance; original member name unknown.
  f32 unknown1c; // Inferred role: maximum distance; original member name unknown.
};

static_assert(offsetof(LocalWindCreateInfo, unknown14) == 0x14, "");

static_assert(offsetof(LocalWindCreateInfo, unknown18) == 0x18, "");
static_assert(offsetof(LocalWindCreateInfo, unknown1c) == 0x1c, "");
static_assert(sizeof(LocalWindCreateInfo) == 0x20, "");

struct AreaWindCreateInfo : WindObjectCreateInfo {
  u32 unknown14;                     // Role and original member name unknown.
  const ResConpoundWind *m_pResWind; // Inferred role: wind resource, named by the assertion of AreaWind::AreaWind.
  f32 unknown20;                     // Role and original member name unknown.
};

static_assert(offsetof(AreaWindCreateInfo, m_pResWind) == 0x18, "");
static_assert(offsetof(AreaWindCreateInfo, unknown20) == 0x20, "");
static_assert(sizeof(AreaWindCreateInfo) == 0x28, "");

class WindObject;

// Inferred helper: intrusive list node that holds the object it belongs to, used by the lists that only receive
// nodes and need the object back (WindWorld::delayErace); the original name is unknown.
template <typename T> class unknown003 : public mm::mtl::IntrusiveListNodeBase {
public:
  T *m_pOwner;
};

static_assert(sizeof(unknown003<WindObject>) == 0x18, "");

class WindObject : public mm::mtl::ReferenceObject, public mm::mtl::IntrusiveListNodeBase {
public:
  WindObject(ObjectType type, const WindObjectCreateInfo &info);
  virtual ~WindObject();

  ObjectType unknown20;             // Inferred role: object type; original member name unknown.
  Transform unknown30;              // Inferred role: transform, passed to Shape::calcAABB; original member name unknown.
  Shape *m_pShape;                  // Inferred role: shape, named by the assertion of wnd_broad_phaze_interface.h.
  u32 unknown58;                    // Inferred role: group, copied from WindObjectCreateInfo::unknown10; original member name unknown.
  void *m_pProxy;                   // Inferred role: broad phase proxy, named by the assertion of wnd_object.cpp.
  u32 unknown68;                    // Role and original member name unknown.
  unknown003<WindObject> unknown70; // Inferred role: node of the delayed erase list; original member name unknown.
};

static_assert(offsetof(WindObject, unknown20) == 0x20, "");
static_assert(offsetof(WindObject, unknown30) == 0x30, "");
static_assert(offsetof(WindObject, m_pShape) == 0x50, "");
static_assert(offsetof(WindObject, unknown58) == 0x58, "");
static_assert(offsetof(WindObject, m_pProxy) == 0x60, "");
static_assert(offsetof(WindObject, unknown68) == 0x68, "");
static_assert(offsetof(WindObject, unknown70) == 0x70, "");
static_assert(sizeof(WindObject) == 0x90, "");
// The subclasses place their first member at 0x88, in the tail padding of the Transform member.

} // namespace wnd
