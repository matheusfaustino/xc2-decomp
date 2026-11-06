#include "layer/layer_font_tag.h"

#include <strings.h>

namespace layer {

const u16 LayerFontTag::FONT_INSTANCE_SIZE = 0x24;
// Entry 0 is the color tag and entry 1 the ruby tag, as used by callTagEnd.
const char *LayerFontTag::s_resTagTbl[4] = {"System:Color", "System:Ruby", "ML:icon", "ML:wide"};

LayerFontTag::LayerFontTag(const LayerFontResObj *res, const LayerFontStyleObj *style, const LyrFontStyleData *styleData, const LayerFontIconObj *icon, LayerFontSetting setting)
    : unknown10(res), unknown18(icon), unknown20(styleData), unknown28(false), unknown29(false) {
  unknown68 = mm::Col4(1.0f, 1.0f, 1.0f, 0.0f);
  unknown78 = mm::Col4(1.0f, 1.0f, 1.0f, 0.0f);
  unknown494[0] = 0;
  unknown494[1] = 0;
  unknown49c = 0;
  unknown48 = nullptr;
  unknown50 = nullptr;
  unknown58 = nullptr;
  unknown60 = nullptr;
  unknown38 = setting;
  unknown3c = setting != 0;
  unknown08 = style;
  unknown40 = setting == 0;
}

void LayerFontTag::getWorkSize(LayerTagWork &work, const u8 *string) {
  unknown28 = false;
  unknown30 = &work;
  calcParseString(string);
  u32 length = 0;
  while (length < 10000 && string[length] != 0) {
    length++;
  }
  work.unknown10 = length;
  work.unknown14 = length;
}

void LayerFontTag::getWorkSize(LayerTagWork &work, const wchar_t *string) {
  unknown28 = false;
  unknown30 = &work;
  calcParseString(string);
  u32 length = 0;
  while (length < 10000 && string[length] != 0) {
    length++;
  }
  work.unknown10 = length;
  work.unknown14 = length << 2;
}

void LayerFontTag::createWork(const u8 *string) {
  unknown28 = true;
  calcParseString(string);
}

void LayerFontTag::createWork(const wchar_t *string) {
  unknown28 = true;
  calcParseString(string);
}

void LayerFontTag::beginParseString() {
  unknown494[1] = 0;
  unknown49c = 0;
  unknown490 = 0;
  unknown494[0] = 0;
}

void LayerFontTag::calcTagColorEnd() { unknown68 = unknown78; }

bool LayerFontTag::callTagEnd(const char *tag) {
  if (unknown28) {
    if (strcasecmp(s_resTagTbl[0], tag) == 0) {
      calcTagColorEnd();
    }
    if (strcasecmp(s_resTagTbl[1], tag) == 0) {
      calcTagRubyEnd();
    }
  }
  return true;
}

void LayerFontTag::parseNotTag(const u8 *string, u32 &index) {
  u32 position = index;
  s8 code = string[position];
  if (unknown28) {
    if (code == '\n') {
      if (string[position + 1] == '\r') {
        index = position + 2;
      } else {
        index = position + 1;
      }
      createLineBreak();
    } else {
      if (code == '\\') {
        position++;
        index = position;
      }
      u16 result;
      u32 length;
      changeUtf8ToUtf16(result, string + position, length);
      index += length;
      createFontCode(result);
    }
  } else if (code == '\n') {
    if (string[position + 1] == '\r') {
      index = position + 2;
    } else {
      index = position + 1;
    }
    unknown49c++;
  } else {
    if (code < 0) {
      if (static_cast<u32>(code + 0x3e) < 0x1e) {
        position += 2;
      } else if ((code & 0xf0) == 0xe0) {
        position += 3;
      } else if ((code & 0xf8) == 0xf0) {
        position += 4;
      } else {
        position += 1;
      }
    } else {
      position += 1;
    }
    index = position;
    unknown30->unknown00++;
  }
}

void LayerFontTag::calcTagWide() { unknown29 = true; }

} // namespace layer
