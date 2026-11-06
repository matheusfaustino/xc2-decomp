#pragma once
#include <stddef.h>

#include "mm_mtl/fixed_vector.h"
#include "mm_mtl/singleton.h"
#include "mm_mtl/vector.h"
#include "types.h"

namespace fw {
struct UpdateInfo;
}

namespace ui {

class UIBuildInfo;
class UIGroup;
class UIResource;

class UIObjFactory : public mm::mtl::PtrSingleton<UIObjFactory> {
public:
  struct PreloadResource {
    UIBuildInfo *unknown00; // Inferred role: build info; original member name unknown.
    UIResource *unknown08;  // Inferred role: resource; original member name unknown.
  };

  struct OrderParam {
    u8 unknown00;  // Inferred role: group; original member name unknown.
    u8 unknown01;  // Inferred role: priority; original member name unknown.
    u16 unknown02; // Inferred role: layer; original member name unknown.
    u16 unknown04; // Inferred role: resource type; original member name unknown.
    u8 _06[0x08 - 0x06];
    UIGroup *unknown08;    // Inferred role: group; original member name unknown.
    const char *unknown10; // Inferred role: object name; original member name unknown.
  };

  struct OrderInfo {
    u8 unknown00;  // Inferred role: group; original member name unknown.
    u8 unknown01;  // Inferred role: priority; original member name unknown.
    u16 unknown02; // Inferred role: layer; original member name unknown.
    u8 _04[0x08 - 0x04];
    UIGroup *unknown08;     // Inferred role: group; original member name unknown.
    const char *unknown10;  // Inferred role: object name; original member name unknown.
    UIBuildInfo *unknown18; // Inferred role: build info; original member name unknown.
    UIResource *unknown20;  // Inferred role: resource; original member name unknown.
    bool unknown28;         // Inferred role: preloaded; original member name unknown.
    u8 _29[0x30 - 0x29];
  };

  UIObjFactory();
  virtual ~UIObjFactory();
  void order(const OrderParam &param);
  void update(const fw::UpdateInfo &info);
  void registPreloadResource(u32 type, const char *name);
  void releasePreloadResource(const char *name);
  bool getPreloadResource(const char *name, PreloadResource &resource);
  bool isLoading() const;

  mm::mtl::Vector<OrderInfo> unknown08;               // Inferred role: orders; original member name unknown.
  mm::mtl::FixedVector<PreloadResource, 8> unknown30; // Inferred role: preload resources; original member name unknown.
};

static_assert(sizeof(UIObjFactory::PreloadResource) == 0x10, "");
static_assert(sizeof(UIObjFactory::OrderParam) == 0x18, "");
static_assert(sizeof(UIObjFactory::OrderInfo) == 0x30, "");
static_assert(offsetof(UIObjFactory, unknown08) == 0x08, "");
static_assert(offsetof(UIObjFactory, unknown30) == 0x30, "");
static_assert(offsetof(UIObjFactory, unknown30) + sizeof(mm::mtl::FixedVector<UIObjFactory::PreloadResource, 8>) == 0xb8, "");
static_assert(sizeof(UIObjFactory) == 0xb8, "");

} // namespace ui
