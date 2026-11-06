#pragma once
#include "types.h"

namespace cloud {

class CloudObjReduction {
public:
  static void getPixelMem(u32 &size, u32 width, u32 height);
  void alphaFastRender();
};

} // namespace cloud
