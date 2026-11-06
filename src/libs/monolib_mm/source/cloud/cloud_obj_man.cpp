#include "cloud/cloud_obj_man.h"

#include "cloud/cloud_calc_params.h"
#include "cloud/cloud_man.h"
#include "cloud/cloud_mdl_data_obj.h"
#include "cloud/cloud_obj.h"
#include "cloud/cloud_obj_reduction.h"
#include "grlib/cglib_render_target.h"
#include "mtl/alloc_handle.h"

namespace cloud {

bool CloudObjMan::isRender() const {
  if (unknown38->unknown420 == nullptr) {
    return false;
  }
  return unknown58;
}

CloudMdlDataObj *CloudObjMan::createMdlData(const void *data) {
  CloudMdlDataObj *object = new (CloudMan::get()->unknown10) CloudMdlDataObj();
  object->create(data);
  // The game takes the address of unknown18 before the branch and reloads unknown20 for the second
  // store.
  CloudMdlDataObj **head = &unknown18;
  if (unknown20 != nullptr) {
    unknown20->unknown08 = object;
    object->unknown00 = unknown20;
    unknown20 = object;
  } else {
    unknown18 = object;
    unknown20 = object;
    object->unknown00 = nullptr;
  }
  object->unknown08 = nullptr;
  object->unknown10 = head;
  unknown28++;
  return object;
}

void CloudObjMan::alphaRender(const ml::IDrDrawWorkInfo *workInfo) {
  unknown40->alphaFastRender();
  for (CloudObj *object = unknown00; object != nullptr; object = object->unknown10) {
    object->alphaRender();
  }
  for (CloudObj *object = unknown00; object != nullptr; object = object->unknown10) {
    object->alphaRenderSec();
  }
  // The game tail-calls CGLibRenderTarget::setRenderTarget here, not CloudCalcParams::setMainRenderTarget.
  unknown38->unknown420->setRenderTarget(*unknown38->unknown438);
}

} // namespace cloud
