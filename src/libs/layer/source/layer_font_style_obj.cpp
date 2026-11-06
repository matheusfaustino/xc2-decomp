#include "layer/layer_font_style_obj.h"

#include <string.h>

#include "layer/layer_acc_font_style.h"
#include "layer/layer_font_manager.h"
#include "layer/layer_tag.h"
#include "mm_mtl/vector.h"

namespace layer {

namespace {

struct unknown005 {
  u8 _00[0x08];
  u32 unknown08; // Inferred role: offset of the style data table; original member name unknown.
  u32 unknown0c; // Inferred role: style data count; original member name unknown.
};

static_assert(offsetof(unknown005, unknown08) == 0x08, "");
static_assert(offsetof(unknown005, unknown0c) == 0x0c, "");

} // namespace

const f32 LayerFontStyleObj::RUBY_SCALE = 0.4f;

void LayerFontStyleObj::setStyleData(const void *data) {
  const unknown005 *header = static_cast<const unknown005 *>(data);
  unknown20 = data;
  unknown28 = header->unknown0c;
  unknown30 = reinterpret_cast<const LyrFontStyleData *>(static_cast<const u8 *>(data) + header->unknown08);
}

const LyrFontStyleData *LayerFontStyleObj::getStyleData(u32 index) const {
  if (index < unknown28) {
    return unknown30 + index;
  }
  return unknown30;
}

s32 LayerFontStyleObj::getStyleIndex(const char *name) const {
  for (u32 i = 0; i < unknown28; i++) {
    if (strcmp(static_cast<const char *>(unknown20) + unknown30[i].unknown04, name) == 0) {
      return i;
    }
  }
  return -1;
}

const char *LayerFontStyleObj::getStyleName(u32 index) const { return static_cast<const char *>(unknown20) + unknown30[index].unknown04; }

const char *LayerFontStyleObj::getFontName(u32 index) const { return static_cast<const char *>(unknown20) + unknown30[index].unknown08; }

LayerFontStyleObj::LayerFontStyleObj(LayerFontManager *manager) {
  unknown38 = manager;
  unknown18 = LAYER_OBJ_TYPE_FONT_STYLE;
  unknown40 = RUBY_SCALE;
  unknown48 = nullptr;
  unknown50 = nullptr;
}

LayerFontStyleObj::~LayerFontStyleObj() {
  releaseColorInfo();
  releaseIconInfo();
}

void LayerFontStyleObj::releaseColorInfo() {
  if (unknown48 != nullptr) {
    delete unknown48;
    unknown48 = nullptr;
  }
}

void LayerFontStyleObj::releaseIconInfo() {
  if (unknown50 != nullptr) {
    delete unknown50;
    unknown50 = nullptr;
  }
}

void LayerFontStyleObj::release() {
  unknown1c = 0x22;
  releaseColorInfo();
  releaseIconInfo();
  unknown38->releaseStyleObj(this);
}

} // namespace layer
