#pragma once
#include <stddef.h>

#include "types.h"

namespace ml {
class IDrDrawWorkInfo;
} // namespace ml

namespace cloud {

class CloudCalcParams;
class CloudMdlDataObj;
class CloudObj;
class CloudObjReduction;

class CloudObjMan {
public:
  CloudObjMan();
  ~CloudObjMan();
  void creataFastSetup();
  CloudMdlDataObj *createMdlData(const void *data);
  void releaseMdlData(CloudMdlDataObj *mdlData);
  bool update(const ml::IDrDrawWorkInfo *workInfo);
  bool isRender() const;
  void alphaRender(const ml::IDrDrawWorkInfo *workInfo);

  CloudObj *unknown00; // Inferred role: head of the object list; original member name unknown.
  u8 _08[0x38 - 0x08];
  CloudCalcParams *unknown38;   // Inferred role: calc params; original member name unknown.
  CloudObjReduction *unknown40; // Inferred role: reduction renderer; original member name unknown.
  u8 _48[0x58 - 0x48];
  bool unknown58; // Inferred role: is disp; original member name unknown.
  bool unknown59; // Inferred role: is plane; original member name unknown.
  bool unknown5a; // Inferred role: is plane side; original member name unknown.
  u8 _5b;
  bool unknown5c; // Inferred role: is sky; original member name unknown.
  u8 _5d[0x61 - 0x5d];
  bool unknown61; // Inferred role: is mdl; original member name unknown.
  u8 _62[0x66 - 0x62];
  bool unknown66; // Inferred role: is hole; original member name unknown.
  u8 _67[0x70 - 0x67];
};

static_assert(offsetof(CloudObjMan, unknown38) == 0x38, "");
static_assert(offsetof(CloudObjMan, unknown40) == 0x40, "");
static_assert(offsetof(CloudObjMan, unknown58) == 0x58, "");
static_assert(offsetof(CloudObjMan, unknown5c) == 0x5c, "");
static_assert(offsetof(CloudObjMan, unknown61) == 0x61, "");
static_assert(offsetof(CloudObjMan, unknown66) == 0x66, "");
static_assert(sizeof(CloudObjMan) == 0x70, "");

} // namespace cloud
