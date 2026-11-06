#pragma once

#include "types.h"

namespace mtl {
class ALLOC_HANDLE;
} // namespace mtl

// Reconstructed from ms::MsWrapper::getColor, which reads one 32-bit value (4 channels, little endian). The symbol
// _ZN2ms9MsWrapper8getColorEjP8LMSColor names LMSColor in the global namespace.
struct LMSColor {
  u8 unknown00; // Inferred role: first channel; original member name unknown.
  u8 unknown01; // Inferred role: second channel; original member name unknown.
  u8 unknown02; // Inferred role: third channel; original member name unknown.
  u8 unknown03; // Inferred role: fourth channel; original member name unknown.
};

static_assert(sizeof(LMSColor) == 4, "");

namespace ms {

// Reconstructed from the symbols and the LMS_SetMemFuncs call of ms::MsWrapper (m_allocHandle global, 0x71014a5080).
class MsWrapper {
public:
  static void init(mtl::ALLOC_HANDLE handle);
  static void regist(const char *path);
  static void unregist();
  static u32 getColorNum();
  static bool getColorName(u32 index, char *name, u32 size);
  static bool getColor(u32 index, LMSColor *color);
  static s32 getTagNum(s32 group);
  static const char *getTagName(s32 group, s32 tag);
  static s32 getTagParamListItemNum(s32 group, s32 tag, s32 param);
  static const char *getTagParamListItemName(s32 group, s32 tag, s32 param, s32 item);
};

} // namespace ms
