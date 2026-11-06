#include "dr/dr_mdo_ctl_mpf.h"

#include "dr/dr_man.h"
#include "dr/dr_mdl_man.h"
#include "dr/dr_mdo_mpf_man.h"
#include "dr/dr_pixl_tile_parm.h"
#include "mm_math/mm_vec3.h"

namespace ml {

void DrMdoCtlMpf::createDisplaList() {}

void DrMdoCtlMpf::updateMpf() {
  m_AABBMax = mm::Vec3(1.0f, 1.0f, 1.0f);
  m_AABBMin = mm::Vec3(-1.0f, -1.0f, -1.0f);
  unknown160 = 100000.0f;
  unknownf8 = 0;
  unknown168 = 0;
  unknown130 = mm::Vec3(0.0f, 0.0f, 0.0f);
  DrMdoMpfMan *mpfMan = m_drMan->unknown88->unknown020;
  if (mpfMan != nullptr) {
    mpfMan->unknown3d24 = unknowndc;
  }
  calcMpfRenderList();
  updateMpfMatUniformBlock();
}

void DrMdoCtlMpf::callZPrepassRender() {
  if ((unknownf8 & unknown028_0) == 0) {
    return;
  }
  defMpfZPreRender();
}

void DrMdoCtlMpf::callRenderMpf() {
  if ((unknownf8 & unknown028_0) == 0) {
    return;
  }
  DrMan::get()->unknown180->begin(DrPixlTileParm::TILE_TYPE_UNKNOWN_3);
  unknownf4 = 0;
  unknownf2 = 0;
  defMpfrender();
  DrMan::get()->unknown180->end(DrPixlTileParm::TILE_TYPE_UNKNOWN_3);
}

} // namespace ml
