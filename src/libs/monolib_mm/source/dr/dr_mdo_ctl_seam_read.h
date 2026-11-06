#pragma once

#include "dr/dr_mdo_ctl_seam_light.h"

namespace ml {
struct unknown064;

class DrMdoCtlSeamRead : public DrMdoCtlSeamLight {
public:
  DrMdoCtlSeamRead(int viewNum);
  void updateReadList();
  void updateReadCall();
  unknown064 *getSeamReadCall();
};

} // namespace ml
