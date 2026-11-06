#pragma once
#include <stddef.h>

#include "mm_mtl/singleton.h"
#include "mtl/alloc_handle.h"
#include "types.h"

namespace mm {
struct Col3;
} // namespace mm

namespace ml {
class DrMdoZSortMan;
class DrResMdoSortCloud;
class IDrDrawWorkInfo;
class Scn;
class WinProc;
} // namespace ml

namespace cloud {

class CloudDataMan;
class CloudObjMan;
class CloudShaderMan;
class CloudTextureMan;
class CloudVertexMan;

class CloudMan : public mm::mtl::PtrSingleton<CloudMan> {
public:
  static int s_nDblDfIdx;
  static int s_nDblStIdx;

  CloudMan(ml::Scn *scn, mtl::ALLOC_HANDLE handle, u32 size);
  ~CloudMan() override;
  void dispOn(bool isOn);
  void planeOn(bool isOn);
  void planeSideOn(bool isOn);
  void skyOn(bool isOn);
  void mdlOn(bool isOn);
  void setHeight(f32 height);
  f32 getHeight() const;
  bool isHole() const;
  void setCldNum(u32 cldNum);
  u32 getCldNum() const;
  void beginSetup();
  void begin(f32 speed);
  void end();
  void apply(const ml::IDrDrawWorkInfo *workInfo, ml::DrMdoZSortMan *sortMan);
  void alphaRender(const ml::IDrDrawWorkInfo *workInfo);
  void finalRender(const ml::IDrDrawWorkInfo *workInfo);
  void tail();
  static void getPixelMem(u32 &size, u32 width, u32 height);
  const mm::Col3 &getPlaneCol(u32 index) const;
  void openDebugWindow(ml::WinProc *window);

  mtl::ALLOC_HANDLE unknown08;      // Inferred role: mem top; original member name unknown.
  mtl::ALLOC_HANDLE unknown0c;      // Inferred role: mem data; original member name unknown.
  mtl::ALLOC_HANDLE unknown10;      // Inferred role: mem work; original member name unknown.
  ml::Scn *unknown18;               // Inferred role: scn; original member name unknown.
  bool unknown20;                   // Inferred role: is setup request; original member name unknown.
  u32 *unknown28;                   // Inferred role: culling group bits; original member name unknown.
  CloudDataMan *unknown30;          // Inferred role: data man; original member name unknown.
  CloudVertexMan *unknown38;        // Inferred role: vertex man; original member name unknown.
  CloudTextureMan *unknown40;       // Inferred role: texture man; original member name unknown.
  CloudShaderMan *unknown48;        // Inferred role: shader man; original member name unknown.
  CloudObjMan *unknown50;           // Inferred role: object man; original member name unknown.
  ml::DrResMdoSortCloud *unknown58; // Inferred role: sort cloud; original member name unknown.
  f32 unknown60;                    // Inferred role: speed; original member name unknown.
  f32 _64;
};

static_assert(offsetof(CloudMan, unknown08) == 0x08, "");
static_assert(offsetof(CloudMan, unknown18) == 0x18, "");
static_assert(offsetof(CloudMan, unknown20) == 0x20, "");
static_assert(offsetof(CloudMan, unknown28) == 0x28, "");
static_assert(offsetof(CloudMan, unknown30) == 0x30, "");
static_assert(offsetof(CloudMan, unknown58) == 0x58, "");
static_assert(offsetof(CloudMan, unknown60) == 0x60, "");

} // namespace cloud
