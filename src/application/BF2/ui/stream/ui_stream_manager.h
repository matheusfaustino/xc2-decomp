#pragma once
#include <stddef.h>

#include "mm_mtl/intrusive_list.h"
#include "mm_mtl/singleton.h"
#include "types.h"

namespace fw {
class RenderResource;
} // namespace fw

namespace mm {
namespace mtl {
class IAllocator;
} // namespace mtl
} // namespace mm

namespace ui {

class UIStreamManager : public mm::mtl::PtrSingleton<UIStreamManager> {
public:
  struct Content : mm::mtl::IntrusiveListNodeBase {
    u32 unknown10; // Inferred role: stream hash; original member name unknown.
    u32 unknown14; // Inferred role: stream data; original member name unknown.
    u8 *unknown18; // Inferred role: bdat; original member name unknown.
  };

  UIStreamManager(mm::mtl::IAllocator *allocator);
  virtual ~UIStreamManager();
  u32 regist(u8 *bdat);
  void start(u32 streamId, u32 handle, u32 index);
  u32 getContentsNum(u32 streamId);

  mm::mtl::IAllocator *unknown08;            // Inferred role: allocator; original member name unknown.
  mm::mtl::IntrusiveList<Content> unknown10; // Inferred role: contents; original member name unknown.
  mm::mtl::IntrusiveListBase unknown28;      // Inferred role: list; original member name unknown.
  mm::mtl::IntrusiveListBase unknown40;      // Inferred role: list; original member name unknown.
  fw::RenderResource *unknown58;             // Inferred role: dummy image resource; original member name unknown.
};

static_assert(offsetof(UIStreamManager::Content, unknown10) == 0x10, "");
static_assert(offsetof(UIStreamManager::Content, unknown18) == 0x18, "");
static_assert(offsetof(UIStreamManager, unknown10) == 0x10, "");

} // namespace ui
