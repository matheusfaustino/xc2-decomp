#pragma once

#include "types.h"

namespace mm {
struct Mat44;
} // namespace mm

namespace effect {

// Interface inferred from the base of fw::EffectCasterSend (1.0.0: _ZTVN2fw16EffectCasterSendE).
// The virtual functions are listed in vtable order after the destructor. getBindData returns the bind-data
// array of fw::EffectCasterSend, whose element type does not belong to this interface, so it is void *.
class IEffectCallBack {
public:
  virtual ~IEffectCallBack() {}
  virtual void *getMdl() = 0;
  virtual const char *getStateName() = 0;
  virtual void *getBindData(u32 &count) = 0;
  virtual f32 getMdlFrame() = 0;
  virtual f32 getMdlScl() = 0;
  virtual void getMdlMat(mm::Mat44 &matrix) = 0;
  virtual void getEfpMat(mm::Mat44 &matrix, const char *name) = 0;
};

} // namespace effect
