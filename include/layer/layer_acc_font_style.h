#pragma once
#include <stddef.h>

#include "mm_mtl/vector.h"
#include "types.h"

namespace layer {

class LayerBase;
class LayerFontStyleObj;
class LayerStrInfo;

// Inferred layout of a font style row; original type and member names unknown.
struct LyrFontStyleData {
  u8 _00[0x04];
  u32 unknown04; // Inferred role: style name offset; original member name unknown.
  u32 unknown08; // Inferred role: font name offset; original member name unknown.
  f32 unknown0c;
  f32 unknown10;
  u8 _14[0x18 - 0x14];
  u16 unknown18; // Inferred role: line width limit; original member name unknown.
  u16 unknown1a; // Inferred role: line count limit; original member name unknown.
  u16 unknown1c; // Inferred role: line spacing; original member name unknown.
  u16 unknown1e; // Inferred role: character spacing; original member name unknown.
  u8 _20[0x28 - 0x20];
};

static_assert(offsetof(LyrFontStyleData, unknown04) == 0x04, "");
static_assert(offsetof(LyrFontStyleData, unknown08) == 0x08, "");
static_assert(offsetof(LyrFontStyleData, unknown18) == 0x18, "");
static_assert(offsetof(LyrFontStyleData, unknown1e) == 0x1e, "");
static_assert(sizeof(LyrFontStyleData) == 0x28, "");

// Inferred role: pair returned by LayerAccFontStyle::getStyleInfo; original type name unknown.
struct unknown085 {
  const void *unknown00;
  const LyrFontStyleData *unknown08;
};

class LayerAccFontStyle {
public:
  LayerAccFontStyle(LayerBase *layer);
  u32 getStyleDataMax() const;
  unknown085 getStyleInfo(u32 index) const;
  s32 getStyleIndex(const char *name) const;
  const char *getStyleName(u32 index) const;
  const char *getFontName(u32 index) const;
  mm::mtl::Vector<LayerStrInfo> *createColorInfo(u32 num);
  void releaseColorInfo();
  void *getColorInfo();
  mm::mtl::Vector<LayerStrInfo> *createIconInfo(u32 num);
  void releaseIconInfo();
  void *getIconInfo();
  f32 getRubyScale() const;
  void setRubyScale(f32 scale);

  LayerFontStyleObj *m_style;
};

} // namespace layer
