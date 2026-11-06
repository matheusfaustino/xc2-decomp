#include "ui/ui_resource.h"

#include <string.h>

#include "framework/resource/common_resource.h"
#include "grlib/cglib_texture_buffer.h"
#include "ml/scn.h"
#include "mm_mtl/hash.h"
#include "mm_mtl/reference_object.h"
#include "scn/scn_render_dr_sys_parm_acc.h"

namespace ui {

UIResource::UIResource(const char *name) : unknown0c(name) {}

UIResource::~UIResource() {
  releaseRenderViewList();
  releaseResourceList();
}

bool UIResource::isValid() const {
  if (unknown30.capacity() == 0 || unknown30.size() == 0) {
    return false;
  }

  for (fw::CommonResource *const *it = unknown30.begin(); it != unknown30.end(); ++it) {
    if ((*it)->valid() && (*it)->getAssetSize() == 0) {
      return false;
    }
  }
  return true;
}

bool UIResource::isLoading() const {
  if (unknown30.capacity() != 0 && unknown30.size() != 0) {
    for (fw::CommonResource *const *it = unknown30.begin(); it != unknown30.end(); ++it) {
      if (!(*it)->valid()) {
        return true;
      }
    }

    if (unknown58.capacity() != 0) {
      for (const RenderView *it = unknown58.begin(); it != unknown58.end(); ++it) {
        if (!it->unknown00->unknown35) {
          return true;
        }
      }
    }
  }
  return false;
}

void UIResource::releaseRenderViewList() {
  if (unknown58.capacity() == 0) {
    return;
  }

  ml::ScnRenderDrSysParmAcc parmAcc(ml::Scn::get()->unknown38, 0);
  for (RenderView *it = unknown58.begin(); it != unknown58.end(); ++it) {
    if (it->unknown00 != nullptr) {
      parmAcc.deleteAddRenderView(it->unknown00);
      if (it->unknown00 != nullptr) {
        delete it->unknown00;
        it->unknown00 = nullptr;
      }
    }
  }
  unknown58.clear();
}

void UIResource::releaseResourceList() {
  if (unknown30.capacity() == 0) {
    return;
  }

  for (fw::CommonResource **it = unknown30.begin(); it != unknown30.end(); ++it) {
    if (*it != nullptr) {
      mm::mtl::safeRelease(*it);
    }
  }
  unknown30.destroy();
}

void UIResource::releaseCaptureList() {
  if (unknown80.capacity() == 0) {
    return;
  }

  for (Capture *it = unknown80.begin(); it != unknown80.end(); ++it) {
    if (it->unknown00 != nullptr) {
      delete it->unknown00;
      it->unknown00 = nullptr;
    }
  }
  unknown80.clear();
}

ml::ScnRenderDrViewCtrl *UIResource::searchRednerView(const char *name) const {
  if (unknown30.capacity() == 0 || unknown30.size() == 0) {
    return nullptr;
  }

  for (fw::CommonResource *const *it = unknown30.begin(); it != unknown30.end(); ++it) {
    if ((*it)->valid() && (*it)->getAssetSize() == 0) {
      return nullptr;
    }
  }

  if (unknown58.capacity() != 0) {
    u32 hash = mm::mtl::hashStrCrc(name);
    for (const RenderView *it = unknown58.begin(); it != unknown58.end(); ++it) {
      if (it->unknown00 != nullptr && it->unknown08 == hash) {
        return it->unknown00;
      }
    }
  }
  return nullptr;
}

ml::ScnRenderDrCapture *UIResource::searchCapture(const char *name) {
  const mm::mtl::Vector<fw::CommonResource *> &resources = unknown30;
  if (resources.capacity() == 0 || resources.size() == 0) {
    return nullptr;
  }

  for (fw::CommonResource *const *it = resources.begin(); it != resources.end(); ++it) {
    if ((*it)->valid() && (*it)->getAssetSize() == 0) {
      return nullptr;
    }
  }

  if (unknown80.capacity() != 0) {
    u32 hash = mm::mtl::hashStrCrc(name);
    for (Capture *it = unknown80.begin(); it != unknown80.end(); ++it) {
      if (it->unknown00 != nullptr && it->unknown20 == hash) {
        return &it->unknown08;
      }
    }
  }
  return nullptr;
}

fw::CommonResource *UIResource::search(const char *name) const {
  if (unknown30.capacity() == 0 || unknown30.size() == 0) {
    return nullptr;
  }

  for (fw::CommonResource *const *it = unknown30.begin(); it != unknown30.end(); ++it) {
    if ((*it)->valid() && (*it)->getAssetSize() == 0) {
      return nullptr;
    }
  }

  for (fw::CommonResource *const *it = unknown30.begin(); it != unknown30.end(); ++it) {
    const char *assetName = (*it)->getAssetName();
    if ((*it)->valid() && assetName != nullptr && strcmp(assetName, name) == 0) {
      return *it;
    }
  }
  return nullptr;
}

} // namespace ui
