#pragma once
#include <stddef.h>

#include "types.h"

namespace ui {

class UIBuildInfo {
public:
  // One object of a UI layout, as read from a bdat sheet or from a UIO file. The
  // original member names are unknown: they are named after their byte offset and
  // the bdat column that fills them.
  struct ObjectData {
    ObjectData()
        : unknown00(nullptr), unknown08(nullptr), unknown10(nullptr), unknown18(nullptr), unknown20(nullptr), unknown28(nullptr), unknown30(0), unknown32(0), unknown34(0), unknown38(0),
          unknown3c(1.0f), unknown40(1.0f), unknown44(1.0f), unknown48(0), unknown4a(0), unknown4b(false), unknown4c(false), unknown4d(false), unknown4e(false), unknown50(nullptr), unknown58(nullptr),
          unknown60(0), unknown62(0), unknown64(0), unknown68(nullptr), unknown70(-1), unknown74(0), unknown78(0), unknown7c(0), unknown80(false), unknown81(false), unknown82(false), unknown84(0),
          unknown88(0), unknown89(0), unknown8a(0), unknown8c(0.0f), unknown90(0), unknown98(nullptr), unknowna0(0), unknowna4(0), unknowna8(false), unknownaa(0), unknownac(0), unknownae(0),
          unknownb0(0), unknownb8(nullptr) {}

    const char *unknown00; // Inferred role: object name, "name" column; original member name unknown.
    const char *unknown08; // Inferred role: parent object, "parent" column; original member name unknown.
    const char *unknown10; // Inferred role: attach layout, "attach_layout" column; original member name unknown.
    const char *unknown18; // Inferred role: multiply color, "col_name_mul" column; original member name unknown.
    const char *unknown20; // Inferred role: add color, "col_name_add" column; original member name unknown.
    const char *unknown28; // Inferred role: resource, "resource" column; original member name unknown.
    u8 unknown30;          // Inferred role: object type, "type" column; original member name unknown.
    u16 unknown32;         // Inferred role: x offset, "offs_x" column; original member name unknown.
    u16 unknown34;         // Inferred role: y offset, "offs_y" column; original member name unknown.
    s32 unknown38;         // Inferred role: rotation, "rot" column; original member name unknown.
    f32 unknown3c;         // Inferred role: x scale, "scale_x" column; original member name unknown.
    f32 unknown40;         // Inferred role: y scale, "scale_y" column; original member name unknown.
    f32 unknown44;         // Inferred role: alpha, "alpha" column divided by 255; original member name unknown.
    u16 unknown48;         // Inferred role: priority, "priority" column; original member name unknown.
    u8 unknown4a;          // Inferred role: blend type, "blend_type" column; original member name unknown.
    bool unknown4b;        // Inferred role: "def_undisp" column; original member name unknown.
    bool unknown4c;        // Inferred role: "localize" column; original member name unknown.
    bool unknown4d;        // Inferred role: "save_def_col" column; original member name unknown.
    bool unknown4e;        // Inferred role: "ignore_sizing" column; original member name unknown.
    const char *unknown50; // Inferred role: color link, "col_link_name" column; original member name unknown.
    const char *unknown58; // Inferred role: mask link, "mask_link_name" column; original member name unknown.
    u8 unknown60;          // Inferred role: input type, "input_type" column; original member name unknown.
    u16 unknown62;         // Inferred role: input id, "input_id" column; original member name unknown.
    u8 unknown64;          // Inferred role: unknown; original member name unknown.
    const char *unknown68; // Inferred role: stream list, "img_stream_list" column; original member name unknown.
    s32 unknown70;         // Inferred role: image index, "img_index" column; original member name unknown.
    u8 unknown74;          // Inferred role: "img_capture" column; original member name unknown.
    u32 unknown78;         // Inferred role: "img_capture_w" column; original member name unknown.
    u32 unknown7c;         // Inferred role: "img_capture_h" column; original member name unknown.
    bool unknown80;        // Inferred role: "img_hflip" column; original member name unknown.
    bool unknown81;        // Inferred role: "img_vflip" column; original member name unknown.
    bool unknown82;        // Inferred role: "img_mask" column; original member name unknown.
    u32 unknown84;         // Inferred role: text id, "text_id" column; original member name unknown.
    u8 unknown88;          // Inferred role: "text_align_h" column; original member name unknown.
    u8 unknown89;          // Inferred role: "text_align_v" column; original member name unknown.
    u8 unknown8a;          // Inferred role: unknown bdat column; original member name unknown.
    f32 unknown8c;         // Inferred role: "border_weight" column; original member name unknown.
    u32 unknown90;         // Inferred role: slope, "slope" column; original member name unknown.
    const char *unknown98; // Inferred role: text face pattern, "text_face_ptn" column; original member name unknown.
    u32 unknowna0;         // Inferred role: screen width, "screen_w" column; original member name unknown.
    u32 unknowna4;         // Inferred role: screen height, "screen_h" column; original member name unknown.
    bool unknowna8;        // Inferred role: "screen_on_mem" column; original member name unknown.
    u16 unknownaa;         // Inferred role: "poly_shape_type" column; original member name unknown.
    u16 unknownac;         // Inferred role: "poly_face_v_num" column; original member name unknown.
    u16 unknownae;         // Inferred role: "poly_ring_div" column; original member name unknown.
    u16 unknownb0;         // Inferred role: "poly_ring_w" column; original member name unknown.
    const char *unknownb8; // Inferred role: "poly_particle_file" column; original member name unknown.
  };

  virtual ~UIBuildInfo() {}
  virtual void update();
  virtual bool isReady() const;
  virtual bool isValid() const;
  virtual u32 getObjectNum() const = 0;
  virtual void getObjectData(u32 index, ObjectData &data) const = 0;
  virtual const char *getName() const = 0;
};

static_assert(offsetof(UIBuildInfo::ObjectData, unknown00) == 0x00, "");
static_assert(offsetof(UIBuildInfo::ObjectData, unknown08) == 0x08, "");
static_assert(offsetof(UIBuildInfo::ObjectData, unknown10) == 0x10, "");
static_assert(offsetof(UIBuildInfo::ObjectData, unknown18) == 0x18, "");
static_assert(offsetof(UIBuildInfo::ObjectData, unknown20) == 0x20, "");
static_assert(offsetof(UIBuildInfo::ObjectData, unknown28) == 0x28, "");
static_assert(offsetof(UIBuildInfo::ObjectData, unknown30) == 0x30, "");
static_assert(offsetof(UIBuildInfo::ObjectData, unknown32) == 0x32, "");
static_assert(offsetof(UIBuildInfo::ObjectData, unknown34) == 0x34, "");
static_assert(offsetof(UIBuildInfo::ObjectData, unknown38) == 0x38, "");
static_assert(offsetof(UIBuildInfo::ObjectData, unknown3c) == 0x3c, "");
static_assert(offsetof(UIBuildInfo::ObjectData, unknown40) == 0x40, "");
static_assert(offsetof(UIBuildInfo::ObjectData, unknown44) == 0x44, "");
static_assert(offsetof(UIBuildInfo::ObjectData, unknown48) == 0x48, "");
static_assert(offsetof(UIBuildInfo::ObjectData, unknown4a) == 0x4a, "");
static_assert(offsetof(UIBuildInfo::ObjectData, unknown4b) == 0x4b, "");
static_assert(offsetof(UIBuildInfo::ObjectData, unknown4c) == 0x4c, "");
static_assert(offsetof(UIBuildInfo::ObjectData, unknown4d) == 0x4d, "");
static_assert(offsetof(UIBuildInfo::ObjectData, unknown4e) == 0x4e, "");
static_assert(offsetof(UIBuildInfo::ObjectData, unknown50) == 0x50, "");
static_assert(offsetof(UIBuildInfo::ObjectData, unknown58) == 0x58, "");
static_assert(offsetof(UIBuildInfo::ObjectData, unknown60) == 0x60, "");
static_assert(offsetof(UIBuildInfo::ObjectData, unknown62) == 0x62, "");
static_assert(offsetof(UIBuildInfo::ObjectData, unknown64) == 0x64, "");
static_assert(offsetof(UIBuildInfo::ObjectData, unknown68) == 0x68, "");
static_assert(offsetof(UIBuildInfo::ObjectData, unknown70) == 0x70, "");
static_assert(offsetof(UIBuildInfo::ObjectData, unknown74) == 0x74, "");
static_assert(offsetof(UIBuildInfo::ObjectData, unknown78) == 0x78, "");
static_assert(offsetof(UIBuildInfo::ObjectData, unknown7c) == 0x7c, "");
static_assert(offsetof(UIBuildInfo::ObjectData, unknown80) == 0x80, "");
static_assert(offsetof(UIBuildInfo::ObjectData, unknown81) == 0x81, "");
static_assert(offsetof(UIBuildInfo::ObjectData, unknown82) == 0x82, "");
static_assert(offsetof(UIBuildInfo::ObjectData, unknown84) == 0x84, "");
static_assert(offsetof(UIBuildInfo::ObjectData, unknown88) == 0x88, "");
static_assert(offsetof(UIBuildInfo::ObjectData, unknown89) == 0x89, "");
static_assert(offsetof(UIBuildInfo::ObjectData, unknown8a) == 0x8a, "");
static_assert(offsetof(UIBuildInfo::ObjectData, unknown8c) == 0x8c, "");
static_assert(offsetof(UIBuildInfo::ObjectData, unknown90) == 0x90, "");
static_assert(offsetof(UIBuildInfo::ObjectData, unknown98) == 0x98, "");
static_assert(offsetof(UIBuildInfo::ObjectData, unknowna0) == 0xa0, "");
static_assert(offsetof(UIBuildInfo::ObjectData, unknowna4) == 0xa4, "");
static_assert(offsetof(UIBuildInfo::ObjectData, unknowna8) == 0xa8, "");
static_assert(offsetof(UIBuildInfo::ObjectData, unknownaa) == 0xaa, "");
static_assert(offsetof(UIBuildInfo::ObjectData, unknownac) == 0xac, "");
static_assert(offsetof(UIBuildInfo::ObjectData, unknownae) == 0xae, "");
static_assert(offsetof(UIBuildInfo::ObjectData, unknownb0) == 0xb0, "");
static_assert(offsetof(UIBuildInfo::ObjectData, unknownb8) == 0xb8, "");
static_assert(sizeof(UIBuildInfo::ObjectData) == 0xc0, "");

} // namespace ui
