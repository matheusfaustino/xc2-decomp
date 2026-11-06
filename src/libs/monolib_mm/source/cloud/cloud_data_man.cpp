#include "cloud/cloud_data_man.h"

#include "cloud/cloud_man.h"
#include "ml/dev_file_th.h"
#include "mtl/alloc_handle.h"
#include "mtl/mem_manager.h"

namespace cloud {

// Read order of the five cloud files; entry 0 is the shader list, the four others are blended
// cloud textures. Evidence: CloudDataMan::CloudDataMan passes each path to DevFileTh::readFile in
// this order and reLoadData indexes this table by CloudDataType. Original table name unknown.
static const char *unknown001[5] = {
    "/monolib/shader/cloud/shd_cloud.wishp",
    "/monolib/shader/cloud/volume.witex",
    "/monolib/shader/cloud/volume2.witex",
    "/monolib/shader/cloud/pnoise.witex",
    "/monolib/shader/cloud/tg_smoke_01_n.witex",
};

CloudDataMan::CloudDataMan() {
  for (u32 i = 0; i < 5; i++) {
    unknown08[i] = nullptr;
    unknown30[i] = 0;
    unknown48[i] = nullptr;
  }

  ml::DevFileParamTh param;
  param._08 = CloudMan::get()->unknown0c;
  param.unknown60 = this;
  param._54 = 0x1000;
  param._6d = false;
  for (u32 i = 0; i < 5; i++) {
    param.unknown00 = unknown001[i];
    unknown48[i] = ml::DevFileTh::readFile(param);
    param._54 = 0x1000;
  }
}

CloudDataMan::~CloudDataMan() {
  for (u32 i = 0; i < 5; i++) {
    if (unknown48[i] != nullptr) {
      ml::DevFileTh::cancel(unknown48[i], false);
    }
    if (unknown08[i] != nullptr) {
      mtl::MemManager::deallocate(unknown08[i], 0, false);
      unknown08[i] = nullptr;
    }
  }
}

bool CloudDataMan::isReadOk() const {
  for (u32 i = 0; i < 5; i++) {
    if (unknown08[i] == nullptr) {
      return false;
    }
  }
  return true;
}

bool CloudDataMan::OnFileEvent(ml::EventFileTh *event) {
  if (event->type == ml::EventFileTh::TYPE_COMPLETE) {
    for (u32 i = 0; i < 5; i++) {
      if (unknown48[i] == event->unknown08) {
        unknown08[i] = event->moveFileData();
        unknown30[i] = event->unknown30;
        unknown48[i] = nullptr;
      }
    }
  }
  return true;
}

void CloudDataMan::reLoadData(CloudDataType type) {
  if (unknown08[type] != nullptr) {
    mtl::MemManager::deallocate(unknown08[type], 0, false);
    unknown08[type] = nullptr;
  }

  ml::DevFileParamTh param;
  param._08 = CloudMan::get()->unknown0c;
  param.unknown60 = this;
  param._54 = 0x1000;
  param._6d = false;
  param.unknown00 = unknown001[type];
  unknown48[type] = ml::DevFileTh::readFile(param);
}

void CloudDataMan::reLoadShaderData() { reLoadData(UNKNOWN001_UNKNOWN_0); }

void CloudDataMan::reLoadTextureData() {
  for (u32 type = UNKNOWN001_UNKNOWN_1; type <= UNKNOWN001_UNKNOWN_4; type++) {
    reLoadData(static_cast<CloudDataType>(type));
  }
}

} // namespace cloud
