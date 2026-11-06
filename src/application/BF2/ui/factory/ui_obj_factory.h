#pragma once
#include <stddef.h>

#include "mm_mtl/fixed_vector.h"
#include "mm_mtl/singleton.h"
#include "types.h"

namespace ui {

class UIBuildInfo;
class UIResource;

class UIObjFactory : public mm::mtl::PtrSingleton<UIObjFactory> {
public:
  struct PreloadResource {
    UIBuildInfo *unknown00; // Inferred role: build info; original member name unknown.
    UIResource *unknown08;  // Inferred role: resource; original member name unknown.
  };

  void registPreloadResource(u32 type, const char *name);
  void releasePreloadResource(const char *name);
  bool getPreloadResource(const char *name, PreloadResource &resource);
  bool isLoading() const;

  u8 _08[0x10 - 0x08];
  void *unknown10; // Role and original member name unknown.
  u8 _18[0x30 - 0x18];
  mm::mtl::FixedVector<PreloadResource, 8> unknown30; // Inferred role: preload resources; original member name unknown.
};

static_assert(sizeof(UIObjFactory::PreloadResource) == 0x10, "");
static_assert(offsetof(UIObjFactory, unknown10) == 0x10, "");
static_assert(offsetof(UIObjFactory, unknown30) == 0x30, "");
static_assert(offsetof(UIObjFactory, unknown30) + sizeof(mm::mtl::FixedVector<UIObjFactory::PreloadResource, 8>) == 0xb8, "");

} // namespace ui
