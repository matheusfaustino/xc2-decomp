#pragma once
#include <stddef.h>

#include "grlib/cglib_vtl_mem_pool.h"
#include "grlib/cglib_vtx_index.h"
#include "grlib/cglib_vtx_vertex.h"
#include "types.h"

namespace cloud {

class CloudMdlParts;

class CloudMdlDataObj {
public:
  // Inferred role: per-part table entry; original type name unknown.
  struct unknown001 {
    u8 _000[0xb0];
    u32 unknownb0; // Inferred role: name offset in unknown48; original member name unknown.
    u16 unknownb4; // Inferred role: vertex list index; original member name unknown.
    u8 _b6[0xd0 - 0xb6];
  };

  // Inferred role: vertex index table entry; original type name unknown.
  struct unknown002 {
    u16 unknown00; // Inferred role: vertex list index; original member name unknown.
    u16 unknown02; // Inferred role: index count; original member name unknown.
    u32 unknown04; // Inferred role: index offset; original member name unknown.
  };

  // Inferred role: vertex list entry; original type name unknown.
  struct unknown003 {
    grlib::CGLibVtxVertex unknown00; // Inferred role: vertex buffer; original member name unknown.
    grlib::CGLibVtxIndex unknown28;  // Inferred role: index buffer; original member name unknown.
  };

  CloudMdlDataObj();
  ~CloudMdlDataObj();
  void setCallDisp(bool isCallDisp);
  void release();
  bool update();
  void create(const void *data);
  void releaseData();
  void bindVertex(u32 index);
  void render(u32 index);

  CloudMdlDataObj *unknown00;       // Inferred role: previous model data; original member name unknown.
  CloudMdlDataObj *unknown08;       // Inferred role: next model data; original member name unknown.
  CloudMdlDataObj **unknown10;      // Inferred role: owning list head slot; original member name unknown.
  grlib::CGLibVtlMemPool unknown18; // Inferred role: mem pool; original member name unknown.
  const char *unknown48;            // Inferred role: name buffer; original member name unknown.
  unknown001 *unknown50;            // Inferred role: part table; original member name unknown.
  unknown002 *unknown58;            // Inferred role: vertex index table; original member name unknown.
  void *unknown60;                  // Inferred role: original member name unknown.
  unknown003 *unknown68;            // Inferred role: vertex lists; original member name unknown.
  CloudMdlParts **unknown70;        // Inferred role: parts; original member name unknown.
  s32 unknown78;                    // Inferred role: vertex list num; original member name unknown.
  s32 unknown7c;                    // Inferred role: part num; original member name unknown.
  u32 unknown80;                    // Inferred role: flags; original member name unknown.
};

static_assert(offsetof(CloudMdlDataObj, unknown18) == 0x18, "");
static_assert(offsetof(CloudMdlDataObj, unknown48) == 0x48, "");
static_assert(offsetof(CloudMdlDataObj, unknown50) == 0x50, "");
static_assert(offsetof(CloudMdlDataObj, unknown58) == 0x58, "");
static_assert(offsetof(CloudMdlDataObj, unknown68) == 0x68, "");
static_assert(offsetof(CloudMdlDataObj, unknown70) == 0x70, "");
static_assert(offsetof(CloudMdlDataObj, unknown78) == 0x78, "");
static_assert(offsetof(CloudMdlDataObj, unknown80) == 0x80, "");
static_assert(sizeof(CloudMdlDataObj) == 0x88, "");
static_assert(offsetof(CloudMdlDataObj::unknown001, unknownb0) == 0xb0, "");
static_assert(offsetof(CloudMdlDataObj::unknown001, unknownb4) == 0xb4, "");
static_assert(sizeof(CloudMdlDataObj::unknown001) == 0xd0, "");
static_assert(sizeof(CloudMdlDataObj::unknown002) == 0x08, "");
static_assert(offsetof(CloudMdlDataObj::unknown003, unknown28) == 0x28, "");
static_assert(sizeof(CloudMdlDataObj::unknown003) == 0x50, "");

} // namespace cloud
