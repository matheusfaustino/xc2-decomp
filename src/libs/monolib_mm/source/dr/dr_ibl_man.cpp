#include "dr/dr_ibl_man.h"

#include "dr/dr_ibl_coli_acc.h"
#include "dr/dr_ibl_obj.h"
#include "grlib/cglib_texture_buffer.h"

namespace ml {

bool DrIBLMan::isValid() const { return unknown910 != nullptr; }

void DrIBLMan::releaseObj(DrIBLObj *obj) { obj->unknown58 |= 2; }

grlib::CGLibTextureBuffer *DrIBLMan::getAlbedoTexture() const {
  if (unknown910 == nullptr) {
    return nullptr;
  }
  if (unknown910->unknown38 == nullptr) {
    return nullptr;
  }
  return &unknown910->unknown48[unknown910->unknown38->unknown06];
}

grlib::CGLibTextureBuffer *DrIBLMan::getNormalTexture() const {
  if (unknown910 == nullptr) {
    return nullptr;
  }
  if (unknown910->unknown38 == nullptr) {
    return nullptr;
  }
  return &unknown910->unknown50[unknown910->unknown38->unknown06];
}

} // namespace ml
