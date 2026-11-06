#include "dr/dr_mdo_ctl_seam_read.h"

#include "dr/dr_mdo_ctl_seam.h"
#include "types.h"

namespace ml {

DrMdoCtlSeamRead::DrMdoCtlSeamRead(int viewNum) : DrMdoCtlSeamLight(viewNum) {}

unknown064 *DrMdoCtlSeamRead::getSeamReadCall() {
  s32 index = unknown14b0;
  unknown064 *call = &unknown1370[index];
  unknown14b0 = index + 1;
  if (index >= 7) {
    unknownc7c |= 0x80000;
  }
  return call;
}

} // namespace ml
