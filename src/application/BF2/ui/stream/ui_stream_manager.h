#pragma once
#include <stddef.h>

#include "mm_mtl/fixstr.h"
#include "mm_mtl/intrusive_list.h"
#include "mm_mtl/singleton.h"
#include "types.h"

namespace fw {
class CommonResource;
class RenderResource;
struct UpdateInfo;
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

  struct StreamContent : mm::mtl::IntrusiveListNodeBase {
    u32 unknown10;                 // Inferred role: object handle; original member name unknown.
    u32 unknown14;                 // Inferred role: unused; original member name unknown.
    fw::CommonResource *unknown18; // Inferred role: stream resource; original member name unknown.
  };

  struct ImageContent : mm::mtl::IntrusiveListNodeBase {
    u32 unknown10;                 // Inferred role: object handle; original member name unknown.
    u32 unknown14;                 // Inferred role: child handle; original member name unknown.
    mm::mtl::FixStr<32> unknown18; // Inferred role: child name; original member name unknown.
    u32 unknown3c;                 // Inferred role: mask object handle; original member name unknown.
    fw::CommonResource *unknown40; // Inferred role: current resource; original member name unknown.
    fw::CommonResource *unknown48; // Inferred role: pending resource; original member name unknown.
    bool unknown50;                // Inferred role: loaded; original member name unknown.
  };

  UIStreamManager(mm::mtl::IAllocator *allocator);
  virtual ~UIStreamManager();
  void update(const fw::UpdateInfo &info);
  u32 regist(u8 *bdat);
  void start(u32 streamId, u32 handle, u32 index);
  void deleteImage(const mm::mtl::FixStr<32> &name, u32 object);
  u32 getContentsNum(u32 streamId);

  mm::mtl::IAllocator *unknown08;                  // Inferred role: allocator; original member name unknown.
  mm::mtl::IntrusiveList<Content> unknown10;       // Inferred role: contents; original member name unknown.
  mm::mtl::IntrusiveList<StreamContent> unknown28; // Inferred role: stream objects; original member name unknown.
  mm::mtl::IntrusiveList<ImageContent> unknown40;  // Inferred role: stream images; original member name unknown.
  fw::RenderResource *unknown58;                   // Inferred role: dummy image resource; original member name unknown.
};

static_assert(offsetof(UIStreamManager::Content, unknown10) == 0x10, "");
static_assert(offsetof(UIStreamManager::ImageContent, unknown3c) == 0x3c, "");
static_assert(offsetof(UIStreamManager::ImageContent, unknown40) == 0x40, "");
static_assert(offsetof(UIStreamManager::ImageContent, unknown50) == 0x50, "");
static_assert(sizeof(UIStreamManager::ImageContent) == 0x58, "");
static_assert(offsetof(UIStreamManager, unknown10) == 0x10, "");

} // namespace ui
