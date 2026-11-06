#pragma once
#include "dr/dr_mdl_obj.h"

namespace ml {

class DrMdoCtlMpf : public DrMdlObj {
public:
  void createDisplaList();
  void callZPrepassRender();
  void updateMpf();
  void calcMpfRenderList();
  void callRenderMpf();
  void defMpfZPreRender();
  void defMpfrender();
};

} // namespace ml
