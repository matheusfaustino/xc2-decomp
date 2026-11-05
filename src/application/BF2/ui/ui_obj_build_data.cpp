#include "ui/ui_obj_build_data.h"

#include <string.h>

#include "mm_mtl/hash.h"

namespace ui {

// Original names unknown: the object parameter names read from the UI build data, and their hashes.
static const char *unknown001[] = {
    "type",
    "resource",
    "name",
    "parent",
    "attach_layout",
    "text_id",
    "text_align_h",
    "text_align_v",
    "text_property",
    "priority",
    "def_undisp",
    "offs_x",
    "offs_y",
    "input_type",
    "input_id",
    "rot",
    "alpha",
    "col_name_mul",
    "col_name_add",
    "screen_w",
    "screen_h",
    "border_weight",
    "slope",
    "input_disable",
    "screen_on_mem",
    "img_stream_list",
    "img_index",
    "img_capture",
    "img_capture_w",
    "img_capture_h",
    "poly_v_num",
    "scale_x",
    "scale_y",
    "img_hflip",
    "img_vflip",
    "localize",
    "text_face_ptn",
    "poly_shape_type",
    "poly_face_v_num",
    "poly_ring_div",
    "poly_ring_w",
    "blend_type",
    "poly_particle_file",
    "save_def_col",
    "col_link_name",
    "ignore_sizing",
    "mask_link_name",
    "img_mask",
};
static u32 unknown002[48];

UIObjBuildData::UIObjBuildData(mm::mtl::IAllocator *allocator) : unknown58(allocator) {
  if (allocator != nullptr) {
    unknown08.setAllocator(allocator);
    unknown30.setAllocator(unknown58);
  }
}

UIObjBuildData::~UIObjBuildData() { clear(); }

void UIObjBuildData::clear() {
  for (u32 i = 0; i < unknown08.size(); i++) {
    if (unknown08[i].unknown00.capacity() != 0) {
      unknown08[i].unknown00.destroy();
    }
  }
  if (unknown08.capacity() != 0) {
    unknown08.destroy();
  }
  if (unknown30.capacity() != 0) {
    unknown30.destroy();
  }
}

u32 UIObjBuildData::getObjectNum() const { return unknown08.size(); }

u32 UIObjBuildData::searchObject(const char *name) {
  for (u32 i = 0; i < unknown08.size(); i++) {
    if (unknown08[i].unknown28 < static_cast<u32>(unknown30.size())) {
      if (strcmp(unknown30[unknown08[i].unknown28], name) == 0) {
        return i;
      }
    }
  }
  return static_cast<u32>(-1);
}

void UIObjBuildData::initialize() {
  for (u32 i = 0; i < 48; i++) {
    unknown002[i] = mm::mtl::hashStrCrc(unknown001[i]);
  }
}

u32 UIObjBuildData::getParamType(const char *name) const {
  u32 hash = mm::mtl::hashStrCrc(name);
  for (s64 i = 0; i < 48; i++) {
    if (unknown002[i] == hash) {
      return i;
    }
  }
  return 0xffff;
}

} // namespace ui
