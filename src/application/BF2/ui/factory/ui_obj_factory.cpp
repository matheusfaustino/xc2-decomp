#include "ui/factory/ui_obj_factory.h"

#include <string.h>

#include "framework/memory/allocator.h"
#include "framework/memory/framework_memory.h"
#include "mm_mtl/fixstr.h"
#include "mm_mtl/reference_object.h"
#include "ui/ui_build_info.h"
#include "ui/ui_build_info_bdat.h"
#include "ui/ui_build_info_uio.h"
#include "ui/ui_obj_build_data.h"
#include "ui/ui_resource.h"
#include "ui/ui_util.h"

namespace ui {

UIObjFactory::UIObjFactory() {
  UIUtil::initObjectMap(0x1fa0);
  UIObjBuildData::initialize();
  unknown08.setAllocator(UIUtil::getProgAllocator());
  unknown08.reserve(0x20);
}

bool UIObjFactory::getPreloadResource(const char *name, PreloadResource &resource) {
  for (const PreloadResource *it = unknown30.begin(); it != unknown30.end(); ++it) {
    if (it->unknown08 != nullptr && strcmp(it->unknown08->unknown0c.c_str(), name) == 0) {
      resource = *it;
      return true;
    }
  }
  return false;
}

void UIObjFactory::registPreloadResource(u32 type, const char *name) {
  for (const PreloadResource *it = unknown30.begin(); it != unknown30.end(); ++it) {
    if (it->unknown00 != nullptr && strcmp(it->unknown00->getName(), name) == 0) {
      return;
    }
  }

  UIBuildInfo *buildInfo;
  switch (type) {
  case 0:
    // __LINE__ must be 212 and 216 as in the original file: they are the line arguments of operator new.
#line 212
    buildInfo = new (fw::Allocator::getProcAllocator(), 16, __FILE__, __LINE__) UIBuildInfoBdat(name);
    break;
  case 1:
#line 216
    buildInfo = new (fw::Allocator::getProcAllocator(), 16, __FILE__, __LINE__) UIBuildInfoUIO(name);
    break;
  default:
    return;
  }
  if (buildInfo != nullptr) {
    PreloadResource resource = {buildInfo, nullptr};
    unknown30.push_back(resource);
  }
}

void UIObjFactory::releasePreloadResource(const char *name) {
  for (PreloadResource *it = unknown30.begin(); it != unknown30.end(); ++it) {
    if (strcmp(it->unknown08->unknown0c.c_str(), name) == 0) {
      if (it->unknown00 != nullptr) {
        delete it->unknown00;
        it->unknown00 = nullptr;
      }
      mm::mtl::safeRelease(it->unknown08);
      unknown30.erase(it);
      return;
    }
  }
}

bool UIObjFactory::isLoading() const {
  if (unknown08.size() != 0) {
    return true;
  }

  for (const PreloadResource *it = unknown30.begin(); it != unknown30.end(); ++it) {
    if (it->unknown08->isLoading()) {
      return true;
    }
  }
  return false;
}

} // namespace ui
