#pragma once
#include <stddef.h>

#include "mm_mtl/singleton.h"
#include "mm_mtl/vector.h"
#include "types.h"

namespace fw {
class CommonResource;
struct UpdateInfo;
} // namespace fw

namespace ui {

class UIMotionData;

class UIMotionLibrary : public mm::mtl::PtrSingleton<UIMotionLibrary> {
public:
  struct MotionInfo {
    u32 unknown00; // Inferred role: name hash; original member name unknown.
    u8 _04[0x08 - 0x04];
    fw::CommonResource *unknown08; // Inferred role: resource; original member name unknown.
    UIMotionData *unknown10;       // Inferred role: motion data; original member name unknown.
  };

  UIMotionLibrary();
  virtual ~UIMotionLibrary();
  UIMotionData *find(const char *name);
  bool add(const char *name);
  bool isLoading() const;
  void update(const fw::UpdateInfo &info);
  void reset();

  mm::mtl::Vector<MotionInfo> unknown08; // Inferred role: motions; original member name unknown.
};

static_assert(sizeof(UIMotionLibrary::MotionInfo) == 0x18, "");
static_assert(offsetof(UIMotionLibrary::MotionInfo, unknown10) == 0x10, "");
static_assert(offsetof(UIMotionLibrary, unknown08) == 0x08, "");
static_assert(sizeof(UIMotionLibrary) == 0x30, "");

} // namespace ui
