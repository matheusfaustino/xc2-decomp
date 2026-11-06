#pragma once
#include <stddef.h>

#include "cloud/cloud_obj.h"
#include "cloud/cloud_uniform.h"
#include "types.h"

namespace cloud {

// A cloud plane that flows along the wind. It owns the two uniform blocks uploaded in update:
// unknown20 holds the world/view/projection matrices and the fog parameters, unknown58 holds the
// shadow parameters.
class CloudObjFlow : public CloudObj {
public:
  CloudObjFlow();
  ~CloudObjFlow() override = default;
  void update() override;
  void alphaRender() override;
  void flowRender();
  void calcSamplingPos();

  static f32 FLOW_CYCLE;
  static f32 FLOW_HALF_CYCLE;
  static f32 FLOW_TEX_SIZE;

  CloudUniform unknown20; // Inferred role: flow uniform; original member name unknown.
  CloudUniform unknown58; // Inferred role: shadow uniform; original member name unknown.
};

static_assert(offsetof(CloudObjFlow, unknown20) == 0x20, "");
static_assert(offsetof(CloudObjFlow, unknown58) == 0x58, "");
static_assert(sizeof(CloudObjFlow) == 0x90, "");

} // namespace cloud
