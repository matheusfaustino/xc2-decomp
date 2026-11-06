#include "ui/ui_build_info_uio.h"

#include "framework/memory/framework_memory.h"
#include "framework/resource/common_resource.h"
#include "mm_mtl/fixstr.h"
#include "mm_mtl/reference_object.h"
#include "ui/ui_obj_build_data.h"
#include "ui/ui_util.h"

namespace ui {

UIBuildInfoUIO::UIBuildInfoUIO(const char *name) : m_name(name), unknown10(nullptr) {
  mm::mtl::FixStr<128> fileName;
  UIUtil::makeMenuUIOFileName(fileName, name);
  fw::CommonResource *resource = fw::CommonResource::request(UIUtil::getResourceAllocator(), fileName.c_str(), 10, false);
  if (resource != nullptr) {
    unknown10 = resource;
  }
  unknown18.setAllocator(UIUtil::getProgAllocator());
}

UIBuildInfoUIO::~UIBuildInfoUIO() {
  if (unknown10 != nullptr) {
    unknown10->release();
    unknown10 = nullptr;
  }
}

const char *UIBuildInfoUIO::getName() const { return m_name; }

void UIBuildInfoUIO::update() {
  if (unknown18.size() != 0) {
    return;
  }
  if (unknown10 == nullptr) {
    return;
  }
  if (!unknown10->valid()) {
    return;
  }
  // __LINE__ must be 61 as in the original source: it is the line argument of operator new.
#line 61
  UIObjBuildData *buildData = new (UIUtil::getProgAllocator(), 16, __FILE__, __LINE__) UIObjBuildData(UIUtil::getProgAllocator());
  buildData->deserialize(unknown10->getAssetData(), unknown10->getAssetSize());
  u32 objectNum = buildData->getObjectNum();
  if (objectNum == 0) {
    delete buildData;
    mm::mtl::safeRelease(unknown10);
    return;
  }
  unknown18.reserve(objectNum);
  for (u32 i = 0; i < objectNum; i++) {
    getObjectDataUIO(buildData, i, unknown18[i]);
  }
  delete buildData;
}

u32 UIBuildInfoUIO::getObjectNum() const { return isReady() ? static_cast<u32>(unknown18.size()) : 0; }

void UIBuildInfoUIO::getObjectData(u32 index, ObjectData &data) const {
  if (!isReady()) {
    return;
  }
  data = unknown18[index];
}

bool UIBuildInfoUIO::isReady() const { return unknown18.size() != 0; }

bool UIBuildInfoUIO::isValid() const { return unknown10 != nullptr; }

} // namespace ui
