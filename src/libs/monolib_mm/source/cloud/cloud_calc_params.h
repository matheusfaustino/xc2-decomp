#pragma once
#include <stddef.h>

#include "grlib/cglib_dbl_uniform_block.h"
#include "mm_math/mm_col3.h"
#include "mm_math/mm_mat44.h"
#include "types.h"

namespace grlib {
class CGLibDepthBuffer;
class CGLibRenderTarget;
} // namespace grlib

namespace ml {
class IDrDrawWorkInfo;
} // namespace ml

namespace cloud {

enum CldTileType {};

class CloudCalcParams {
public:
  // One tiled-cache state per (buffer, tile type); beginTile/endTile select [unknown498][type].
  struct CldTileState {
    u32 unknown00; // Inferred role: tile width; original member name unknown.
    u32 unknown04; // Inferred role: tile height; original member name unknown.
    u8 unknown08;  // Inferred role: tiled cache enabled; original member name unknown.
    u8 _09[0x0c - 0x09];
  };

  static_assert(offsetof(CldTileState, unknown08) == 0x08, "");
  static_assert(sizeof(CldTileState) == 0x0c, "");

  CloudCalcParams();
  ~CloudCalcParams() { unknown460.release(); }
  void apply(const ml::IDrDrawWorkInfo *workInfo);
  void parmUpdate();
  void end();
  void setMainRenderTarget();
  const f32 *getFogTbl() const;
  f32 getShadowOfs(u32 index) const;
  void *getShadowTex(u32 index);
  const mm::Mat44 &getShadowMatrix(u32 index) const;
  void setMainRenderTargetAndDepth(bool isDepth, bool isSecondDepth);
  void setPlnReductionTarget(bool isDepth);
  void setMdlReductionTarget();
  void setSkyReductionTarget();
  void setMdlZBuffReductionTarget(bool isSky);
  void beginTile(CldTileType type);
  void endTile(CldTileType type);

  ml::IDrDrawWorkInfo *unknown00; // Inferred role: draw work info; original member name unknown.
  mm::Mat44 unknown10;            // Inferred role: projection copy; original member name unknown.
  mm::Mat44 unknown50;            // Inferred role: view projection; original member name unknown.
  mm::Mat44 unknown90;            // Inferred role: copy of unknown50; original member name unknown.
  u8 _d0[0x150 - 0x0d0];
  f32 unknown150; // Inferred role: original member name unknown.
  f32 unknown154; // Inferred role: original member name unknown.
  f32 unknown158; // Inferred role: original member name unknown.
  u8 _15c[0x170 - 0x15c];
  f32 unknown170; // Inferred role: cloud density; original member name unknown.
  u8 _174[0x180 - 0x174];
  f32 unknown180; // Inferred role: original member name unknown.
  u8 _184[0x188 - 0x184];
  f32 unknown188; // Inferred role: original member name unknown.
  u8 _18c[0x190 - 0x18c];
  f32 unknown190;  // Inferred role: original member name unknown.
  f32 unknown194;  // Inferred role: height; original member name unknown.
  f32 unknown198;  // Inferred role: original member name unknown.
  u32 unknown19c;  // Inferred role: cld num; original member name unknown.
  bool unknown1a0; // Inferred role: original member name unknown.
  u8 _1a1[0x1b8 - 0x1a1];
  mm::Col3 unknown1b8[2]; // Inferred role: plane cols; original member name unknown.
  u8 _1d0[0x420 - 0x1d0];
  grlib::CGLibRenderTarget *unknown420; // Inferred role: main render target; original member name unknown.
  u8 _428[0x438 - 0x428];
  grlib::CGLibDepthBuffer *unknown438;  // Inferred role: main depth buffer; original member name unknown.
  grlib::CGLibRenderTarget *unknown440; // Inferred role: second render target; original member name unknown.
  u8 _448[0x458 - 0x448];
  grlib::CGLibDepthBuffer *unknown458; // Inferred role: second depth buffer; original member name unknown.

  grlib::CGLibDblUniformBlock unknown460; // Inferred role: calc uniform; original member name unknown.
  u16 unknown498;                         // Inferred role: tile state index; original member name unknown.
  u8 _49a[0x49c - 0x49a];
  CldTileState unknown49c[2][3]; // Inferred role: tiled cache states; original member name unknown.
};

static_assert(offsetof(CloudCalcParams, unknown00) == 0x00, "");
static_assert(offsetof(CloudCalcParams, unknown10) == 0x10, "");
static_assert(offsetof(CloudCalcParams, unknown50) == 0x50, "");
static_assert(offsetof(CloudCalcParams, unknown90) == 0x90, "");
static_assert(offsetof(CloudCalcParams, unknown150) == 0x150, "");
static_assert(offsetof(CloudCalcParams, unknown170) == 0x170, "");
static_assert(offsetof(CloudCalcParams, unknown180) == 0x180, "");
static_assert(offsetof(CloudCalcParams, unknown188) == 0x188, "");
static_assert(offsetof(CloudCalcParams, unknown194) == 0x194, "");
static_assert(offsetof(CloudCalcParams, unknown19c) == 0x19c, "");
static_assert(offsetof(CloudCalcParams, unknown1b8) == 0x1b8, "");
static_assert(offsetof(CloudCalcParams, unknown420) == 0x420, "");
static_assert(offsetof(CloudCalcParams, unknown438) == 0x438, "");
static_assert(offsetof(CloudCalcParams, unknown440) == 0x440, "");
static_assert(offsetof(CloudCalcParams, unknown458) == 0x458, "");
static_assert(offsetof(CloudCalcParams, unknown460) == 0x460, "");
static_assert(offsetof(CloudCalcParams, unknown498) == 0x498, "");
static_assert(offsetof(CloudCalcParams, unknown49c) == 0x49c, "");

} // namespace cloud
