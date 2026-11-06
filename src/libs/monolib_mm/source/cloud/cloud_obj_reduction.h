#pragma once
#include <stddef.h>

#include "grlib/cglib_dbl_uniform_block.h"
#include "grlib/cglib_depth_target.h"
#include "grlib/cglib_pixl_counter.h"
#include "grlib/cglib_render_target.h"
#include "types.h"

namespace cloud {

// Off-screen pass chain of the sea-of-clouds renderer. The constructor initialises these members in
// declaration order (0x70, 0x438, 0x800, 0xbc8, 0xf90, 0x1358, 0x1720 are CGLibRenderTarget
// constructors, 0x1ae8 and 0x22e8 CGLibDepthTarget, 0x2ae8.. CGLibPixlCounter).
class CloudObjReduction {
public:
  CloudObjReduction();
  static void getPixelMem(u32 &size, u32 width, u32 height);
  void alphaFastRender();
  void skyBufferClear();
  void mdlBufferClear();
  void planeBufferClear();

  grlib::CGLibDblUniformBlock unknown00; // Inferred role: original member name unknown.
  grlib::CGLibDblUniformBlock unknown38; // Inferred role: original member name unknown.
  grlib::CGLibRenderTarget unknown70;    // Inferred role: original member name unknown.
  grlib::CGLibRenderTarget unknown438;   // Inferred role: original member name unknown.
  grlib::CGLibRenderTarget unknown800;   // Inferred role: original member name unknown.
  grlib::CGLibRenderTarget unknownbc8;   // Inferred role: original member name unknown.
  grlib::CGLibRenderTarget unknownf90;   // Inferred role: original member name unknown.
  grlib::CGLibRenderTarget unknown1358;  // Inferred role: original member name unknown.
  grlib::CGLibRenderTarget unknown1720;  // Inferred role: original member name unknown.
  grlib::CGLibDepthTarget unknown1ae8;   // Inferred role: original member name unknown.
  grlib::CGLibDepthTarget unknown22e8;   // Inferred role: original member name unknown.
  grlib::CGLibPixlCounter unknown2ae8;   // Inferred role: original member name unknown.
  grlib::CGLibPixlCounter unknown2b00;   // Inferred role: original member name unknown.
  grlib::CGLibPixlCounter unknown2b18;   // Inferred role: original member name unknown.
  grlib::CGLibPixlCounter unknown2b30;   // Inferred role: original member name unknown.
  int unknown2b48;                       // Inferred role: original member name unknown.
  int unknown2b4c;                       // Inferred role: original member name unknown.
  u8 _2b50[0x2b54 - 0x2b50];
  bool unknown2b54; // Inferred role: original member name unknown.
};

static_assert(offsetof(CloudObjReduction, unknown00) == 0x00, "");
static_assert(offsetof(CloudObjReduction, unknown38) == 0x38, "");
static_assert(offsetof(CloudObjReduction, unknown70) == 0x70, "");
static_assert(offsetof(CloudObjReduction, unknown438) == 0x438, "");
static_assert(offsetof(CloudObjReduction, unknown800) == 0x800, "");
static_assert(offsetof(CloudObjReduction, unknownbc8) == 0xbc8, "");
static_assert(offsetof(CloudObjReduction, unknownf90) == 0xf90, "");
static_assert(offsetof(CloudObjReduction, unknown1358) == 0x1358, "");
static_assert(offsetof(CloudObjReduction, unknown1720) == 0x1720, "");
static_assert(offsetof(CloudObjReduction, unknown1ae8) == 0x1ae8, "");
static_assert(offsetof(CloudObjReduction, unknown22e8) == 0x22e8, "");
static_assert(offsetof(CloudObjReduction, unknown2ae8) == 0x2ae8, "");
static_assert(offsetof(CloudObjReduction, unknown2b48) == 0x2b48, "");
static_assert(offsetof(CloudObjReduction, unknown2b4c) == 0x2b4c, "");
static_assert(offsetof(CloudObjReduction, unknown2b54) == 0x2b54, "");
static_assert(sizeof(CloudObjReduction) == 0x2b58, "");

} // namespace cloud
