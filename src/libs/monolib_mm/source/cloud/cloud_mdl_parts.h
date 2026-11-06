#pragma once
#include <stddef.h>

#include "cloud/cloud_uniform.h"
#include "types.h"

namespace mm {
struct Vec3;
} // namespace mm

namespace cloud {

class CloudMdlDataObj;

class CloudMdlParts {
public:
  CloudMdlParts() {
    unknown00 = nullptr;
    unknown08 = 0;
    unknown10 = nullptr;
    unknown18 = 0;
    unknown20 = nullptr;
    unknown28 = 0;
    // The game initialises the next three members in this order: unknown72, unknown78, unknown74.
    unknown72 = 0;
    unknown78 = 0;
    unknown74 = 0;
    unknownc0 = nullptr;
    unknownc8 = nullptr;
    unknown30.createUniform(UNKNOWN001_UNKNOWN_10, 0);
  }
  ~CloudMdlParts() { unknown30.releaseUniform(); }
  void setMdl(CloudMdlDataObj *mdlData, u32 index);
  void createUniform(const mm::Vec3 &pos);
  void bindVertex();
  void render();
  bool isDisp() const;
  const char *getName() const;

  void *unknown00;            // Inferred role: original member name unknown.
  u32 unknown08;              // Inferred role: original member name unknown.
  void *unknown10;            // Inferred role: original member name unknown.
  u32 unknown18;              // Inferred role: original member name unknown.
  void *unknown20;            // Inferred role: original member name unknown.
  u32 unknown28;              // Inferred role: original member name unknown.
  CloudUniform unknown30;     // Inferred role: part uniform; original member name unknown.
  CloudMdlDataObj *unknown68; // Inferred role: model data; original member name unknown.
  u16 unknown70;              // Inferred role: part index; original member name unknown.
  u16 unknown72;              // Inferred role: z sort key; original member name unknown.
  u16 unknown74;              // Inferred role: culling group; original member name unknown.
  u8 _76[0x78 - 0x76];
  u32 unknown78; // Inferred role: original member name unknown.
  u8 _7c[0xc0 - 0x7c];
  CloudMdlParts *unknownc0; // Inferred role: next part; original member name unknown.
  CloudMdlParts *unknownc8; // Inferred role: previous part; original member name unknown.
};

static_assert(offsetof(CloudMdlParts, unknown30) == 0x30, "");
static_assert(offsetof(CloudMdlParts, unknown08) == 0x08, "");
static_assert(offsetof(CloudMdlParts, unknown10) == 0x10, "");
static_assert(offsetof(CloudMdlParts, unknown18) == 0x18, "");
static_assert(offsetof(CloudMdlParts, unknown20) == 0x20, "");
static_assert(offsetof(CloudMdlParts, unknown28) == 0x28, "");
static_assert(offsetof(CloudMdlParts, unknown68) == 0x68, "");
static_assert(offsetof(CloudMdlParts, unknown70) == 0x70, "");
static_assert(offsetof(CloudMdlParts, unknown72) == 0x72, "");
static_assert(offsetof(CloudMdlParts, unknown74) == 0x74, "");
static_assert(offsetof(CloudMdlParts, unknown78) == 0x78, "");
static_assert(offsetof(CloudMdlParts, unknownc0) == 0xc0, "");
static_assert(offsetof(CloudMdlParts, unknownc8) == 0xc8, "");
static_assert(sizeof(CloudMdlParts) == 0xd0, "");

} // namespace cloud
