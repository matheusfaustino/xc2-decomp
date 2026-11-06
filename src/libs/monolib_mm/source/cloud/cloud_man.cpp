#include "cloud/cloud_man.h"

#include "cloud/cloud_calc_params.h"
#include "cloud/cloud_data_man.h"
#include "cloud/cloud_obj_man.h"
#include "cloud/cloud_obj_reduction.h"
#include "cloud/cloud_shader_man.h"
#include "cloud/cloud_texture_man.h"
#include "cloud/cloud_vertex_man.h"
#include "dr/dr_draw_work_info.h"
#include "dr/dr_man.h"
#include "dr/dr_mdo_zsort.h"
#include "dr/dr_res_mdo_sort.h"
#include "mm_math/mm_col3.h"
#include "mtl/mem_manager.h"

namespace cloud {

CloudMan::CloudMan(ml::Scn *scn, mtl::ALLOC_HANDLE handle, u32 size) : unknown18(scn), unknown28(nullptr) {
  s_nDblDfIdx = 0;
  s_nDblStIdx = 1;
  unknown08 = mtl::ALLOC_HANDLE();
  unknown0c = mtl::ALLOC_HANDLE();
  unknown10 = mtl::ALLOC_HANDLE();
  unknown08 = mtl::MemManager::create(handle, size + 0x1000, "CloudTop", false);
  unknown0c = mtl::MemManager::create(unknown08, 0x380000, "CloudData", false);
  u32 workSize = size - 0x380000;
  unknown10 = mtl::MemManager::create(unknown08, workSize, "CloudWork", false);
  unknown20 = true;
  unknown58 = nullptr;
  unknown40 = nullptr;
  unknown48 = nullptr;
  unknown38 = nullptr;
  unknown60 = 1.0f;
  _64 = 1.0f;
  unknown30 = new (unknown10) CloudDataMan();
  unknown50 = new (unknown10) CloudObjMan();
  unknown58 = new (unknown10) ml::DrResMdoSortCloud(this);
}

CloudMan::~CloudMan() {
  if (unknown58 != nullptr) {
    delete unknown58;
    unknown58 = nullptr;
  }

  if (unknown50 != nullptr) {
    delete unknown50;
    unknown50 = nullptr;
  }

  if (unknown38 != nullptr) {
    delete unknown38;
    unknown38 = nullptr;
  }

  if (unknown40 != nullptr) {
    delete unknown40;
    unknown40 = nullptr;
  }

  if (unknown48 != nullptr) {
    delete unknown48;
    unknown48 = nullptr;
  }

  if (unknown30 != nullptr) {
    delete unknown30;
    unknown30 = nullptr;
  }

  if (unknown0c.isValid()) {
    mtl::MemManager::erase(unknown0c);
    unknown0c = mtl::ALLOC_HANDLE();
  }

  if (unknown10.isValid()) {
    mtl::MemManager::erase(unknown10);
    unknown10 = mtl::ALLOC_HANDLE();
  }

  if (unknown08.isValid()) {
    mtl::MemManager::erase(unknown08);
    unknown08 = mtl::ALLOC_HANDLE();
  }
}

void CloudMan::dispOn(bool isOn) { unknown50->unknown58 = isOn; }

void CloudMan::planeOn(bool isOn) { unknown50->unknown59 = isOn; }

void CloudMan::planeSideOn(bool isOn) { unknown50->unknown5a = isOn; }

void CloudMan::skyOn(bool isOn) { unknown50->unknown5c = isOn; }

void CloudMan::mdlOn(bool isOn) { unknown50->unknown61 = isOn; }

void CloudMan::setHeight(f32 height) { unknown50->unknown38->unknown194 = height; }

f32 CloudMan::getHeight() const { return unknown50->unknown38->unknown194; }

bool CloudMan::isHole() const { return unknown50->unknown66; }

void CloudMan::setCldNum(u32 cldNum) {
  if (cldNum < 2) {
    unknown50->unknown38->unknown19c = cldNum;
  }
}

u32 CloudMan::getCldNum() const { return unknown50->unknown38->unknown19c; }

void CloudMan::beginSetup() {
  if (!unknown30->isReadOk()) {
    return;
  }

  unknown20 = false;
  unknown38 = new (unknown10) CloudVertexMan();
  unknown40 = new (unknown10) CloudTextureMan();
  unknown48 = new (unknown10) CloudShaderMan();
  unknown48->createShader();
  unknown40->createTexture();
  unknown50->creataFastSetup();
}

void CloudMan::begin(f32 speed) {
  unknown60 = speed;
  if (unknown20) {
    beginSetup();
  } else {
    unknown48->update();
  }
}

void CloudMan::end() {}

void CloudMan::apply(const ml::IDrDrawWorkInfo *workInfo, ml::DrMdoZSortMan *sortMan) {
  if (!unknown20 && workInfo->unknown560 == 0 && unknown50->update(workInfo) && unknown50->isRender()) {
    sortMan->setViewCloud(unknown58);
  }
}

void CloudMan::alphaRender(const ml::IDrDrawWorkInfo *workInfo) {
  unknown50->alphaRender(workInfo);
  ml::DrMan::getIns()->setAlphaRenderTarget();
}

void CloudMan::finalRender(const ml::IDrDrawWorkInfo *workInfo) {
  if (!unknown20) {
    unknown50->isRender();
  }
}

void CloudMan::tail() {
  s_nDblDfIdx = 1 - s_nDblDfIdx;
  s_nDblStIdx = 1 - s_nDblStIdx;
}

void CloudMan::getPixelMem(u32 &size, u32 width, u32 height) { CloudObjReduction::getPixelMem(size, width, height); }

const mm::Col3 &CloudMan::getPlaneCol(u32 index) const {
  if (index == 0) {
    return unknown50->unknown38->unknown1b8[0];
  }

  return unknown50->unknown38->unknown1b8[1];
}

void CloudMan::openDebugWindow(ml::WinProc *window) {}

} // namespace cloud
