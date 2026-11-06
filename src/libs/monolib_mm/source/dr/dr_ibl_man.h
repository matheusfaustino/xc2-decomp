#pragma once
#include <stddef.h>

#include "types.h"

namespace grlib {
class CGLibTextureBuffer;
} // namespace grlib

namespace ml {

class DrIBLObj;

class DrIBLMan {
public:
  bool isValid() const;
  void releaseObj(DrIBLObj *obj);
  grlib::CGLibTextureBuffer *getAlbedoTexture() const;
  grlib::CGLibTextureBuffer *getNormalTexture() const;

  u8 _000[0x910];
  DrIBLObj *unknown910; // Inferred role: current IBL object; original member name unknown.
};

static_assert(offsetof(DrIBLMan, unknown910) == 0x910, "");

} // namespace ml
