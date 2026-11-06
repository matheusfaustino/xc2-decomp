#pragma once
#include <stddef.h>

#include "mm_mtl/fixstr.h"
#include "mm_mtl/reference_object.h"
#include "mm_mtl/vector.h"
#include "scn/scn_render_dr_sys.h"
#include "types.h"

namespace fw {
class CommonResource;
} // namespace fw

namespace grlib {
class CGLibTextureBuffer;
} // namespace grlib

namespace mm {
namespace mtl {
class IAllocator;
} // namespace mtl
} // namespace mm

namespace ui {

class UIBuildInfo;

class UIResource : public mm::mtl::ReferenceObject {
public:
  struct RenderView {
    ml::ScnRenderDrViewCtrl *unknown00; // Inferred role: render view; original member name unknown.
    u32 unknown08;                      // Inferred role: name hash; original member name unknown.
    u8 _0c[0x10 - 0x0c];
  };

  struct Capture {
    grlib::CGLibTextureBuffer *unknown00; // Inferred role: capture buffer; original member name unknown.
    ml::ScnRenderDrCapture unknown08;     // Inferred role: capture; original member name unknown.
    u8 _10[0x20 - 0x10];
    u32 unknown20; // Inferred role: name hash; original member name unknown.
    u8 _24[0x28 - 0x24];
  };

  UIResource(const char *name);
  virtual ~UIResource();
  void load(const UIBuildInfo *buildInfo, mm::mtl::IAllocator *allocator);
  void releaseRenderViewList();
  void releaseResourceList();
  void releaseCaptureList();
  bool isValid() const;
  bool isLoading() const;
  fw::CommonResource *search(const char *name) const;
  ml::ScnRenderDrViewCtrl *searchRednerView(const char *name) const;
  ml::ScnRenderDrCapture *searchCapture(const char *name);

  mm::mtl::FixStr<32> unknown0c;                   // Inferred role: name; original member name unknown.
  mm::mtl::Vector<fw::CommonResource *> unknown30; // Inferred role: resources; original member name unknown.
  mm::mtl::Vector<RenderView> unknown58;           // Inferred role: render views; original member name unknown.
  mm::mtl::Vector<Capture> unknown80;              // Inferred role: captures; original member name unknown.
};

static_assert(sizeof(UIResource::RenderView) == 0x10, "");
static_assert(sizeof(UIResource::Capture) == 0x28, "");
static_assert(offsetof(UIResource, unknown0c) == 0x0c, "");
static_assert(offsetof(UIResource, unknown30) == 0x30, "");
static_assert(offsetof(UIResource, unknown58) == 0x58, "");
static_assert(offsetof(UIResource, unknown80) == 0x80, "");
static_assert(sizeof(UIResource) == 0xa8, "");

} // namespace ui
