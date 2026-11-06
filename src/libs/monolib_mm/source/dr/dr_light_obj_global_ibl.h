#pragma once
#include <stddef.h>

#include "grlib/cglib_dbl_uniform_block.h"
#include "grlib/cglib_pixl_counter.h"
#include "grlib/cglib_texture_buffer.h"
#include "grlib/cglib_vtx_index.h"
#include "grlib/cglib_vtx_vertex.h"
#include "mm_math/mm_col3.h"
#include "mm_math/mm_mat44.h"
#include "mm_mtl/vector.h"
#include "types.h"

namespace ml {

class DrResUseQueryObj;

class DrLightObjGlobalIBL {
public:
  static const int TEXTURE_MAX = 7;
  static const int SH_TERM_NUM = 4;

  struct MeshData {
    // Evidence: the destructor of the elements released by Vector<MeshData>::destroy (1.0.0: 0x71007da9b0) calls
    // CGLibPixlCounter::release on this + 0x1c0, then destroys the CGLibDblUniformBlock at 0x188 and 0x150.
    ~MeshData() { unknown1c0.release(); }

    grlib::CGLibVtxVertex unknown00; // Inferred role: vertex; original member name unknown.
    grlib::CGLibVtxIndex unknown28;  // Inferred role: index; original member name unknown.
    u8 _50[0x110 - 0x50];
    DrResUseQueryObj *unknown110; // Inferred role: occlusion query; original member name unknown.
    u8 _118[0x150 - 0x118];
    grlib::CGLibDblUniformBlock unknown150; // Inferred role: uniform block; original member name unknown.
    grlib::CGLibDblUniformBlock unknown188; // Inferred role: uniform block; original member name unknown.
    grlib::CGLibPixlCounter unknown1c0;     // Inferred role: pixel counter; original member name unknown.
    u8 _1d8[0x1e0 - 0x1d8];
  };

  ~DrLightObjGlobalIBL();
  void update();
  int getPriority() const;
  void bindVertex(u32 index);
  void drawVertex();
  void beginOccQuery();
  void endOccQuery();
  f32 calcSimilarity(const DrLightObjGlobalIBL &other) const;

  u8 _00[0x08 - 0x00];
  int unknown08; // Inferred role: version; original member name unknown.
  int m_priority;
  u8 _10[0x14 - 0x10];
  int unknown14; // Inferred role: texture num; original member name unknown.
  u8 _18[0x20 - 0x18];
  mm::mtl::Vector<MeshData> unknown20; // Inferred role: meshes; original member name unknown.
  u8 _48[0x70 - 0x48];
  u32 unknown70; // Inferred role: current mesh index; original member name unknown.
  u8 _74[0x78 - 0x74];
  grlib::CGLibTextureBuffer unknown78[TEXTURE_MAX]; // Inferred role: textures; original member name unknown.
  u8 _388[0x3a0 - 0x388];
  mm::Col3 unknown3a0; // Inferred role: additional ambient; original member name unknown.
  u8 _3ac[0x3b0 - 0x3ac];
  f32 unknown3b0; // Inferred role: sky intensity; original member name unknown.
  f32 unknown3b4; // Inferred role: dir light intensity; original member name unknown.
  u8 _3b8[0x3d0 - 0x3b8];
  mm::Mat44 unknown3d0;                            // Inferred role: world to proxy space matrix (DrShdLgtGlobalIBL::updateUB_ToProxySpace); original member name unknown.
  f32 unknown410[16];                              // Inferred role: similarity values; original member name unknown.
  f32 unknown450[TEXTURE_MAX][SH_TERM_NUM][9];     // Inferred role: sh terms; original member name unknown.
  mm::Mat44 unknown840[TEXTURE_MAX * SH_TERM_NUM]; // Inferred role: sh matrices; original member name unknown.
};

static_assert(offsetof(DrLightObjGlobalIBL, unknown08) == 0x08, "");
static_assert(offsetof(DrLightObjGlobalIBL, m_priority) == 0x0c, "");
static_assert(offsetof(DrLightObjGlobalIBL, unknown14) == 0x14, "");
static_assert(sizeof(DrLightObjGlobalIBL::MeshData) == 0x1e0, "");
static_assert(offsetof(DrLightObjGlobalIBL::MeshData, unknown28) == 0x28, "");
static_assert(offsetof(DrLightObjGlobalIBL::MeshData, unknown110) == 0x110, "");
static_assert(offsetof(DrLightObjGlobalIBL, unknown20) == 0x20, "");
static_assert(offsetof(DrLightObjGlobalIBL, unknown70) == 0x70, "");
static_assert(offsetof(DrLightObjGlobalIBL, unknown78) == 0x78, "");
static_assert(offsetof(DrLightObjGlobalIBL, unknown3a0) == 0x3a0, "");
static_assert(offsetof(DrLightObjGlobalIBL, unknown3b0) == 0x3b0, "");
static_assert(offsetof(DrLightObjGlobalIBL, unknown3b4) == 0x3b4, "");
static_assert(offsetof(DrLightObjGlobalIBL, unknown3d0) == 0x3d0, "");
static_assert(offsetof(DrLightObjGlobalIBL, unknown410) == 0x410, "");
static_assert(offsetof(DrLightObjGlobalIBL, unknown450) == 0x450, "");
static_assert(offsetof(DrLightObjGlobalIBL, unknown840) == 0x840, "");

} // namespace ml
