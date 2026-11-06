#pragma once
#include <stddef.h>

#include "layer/layer_obj.h"
#include "types.h"

namespace layer {

struct LyrActionData {
  u32 unknown00; // Inferred role: flags; original member name unknown.
  u16 unknown04; // Inferred role: next idx; original member name unknown.
  u16 unknown06; // Inferred role: end type; original member name unknown.
  u32 unknown08; // Inferred role: id; original member name unknown.
  u32 unknown0c; // Inferred role: name offset; original member name unknown.
  s16 unknown10; // Inferred role: command count; original member name unknown.
  u8 _12[0x14 - 0x12];
  u16 unknown14; // Inferred role: name idx; original member name unknown.
  u8 _16[0x18 - 0x16];
  f32 unknown18; // Inferred role: sec; original member name unknown.
  u8 _1c[0x2c - 0x1c];
};

static_assert(offsetof(LyrActionData, unknown10) == 0x10, "");
static_assert(sizeof(LyrActionData) == 0x2c, "");

// Inferred role: layer action header; original type name unknown.
struct unknown083 {
  u32 unknown00; // Inferred role: flags; original member name unknown.
  u32 unknown04; // Inferred role: offset of the actions; original member name unknown.
  s32 unknown08; // Inferred role: action count; original member name unknown.
  s32 actionNum;
  u32 unknown10; // Inferred role: first action idx; original member name unknown.
};

// Inferred role: layer act work; original type name unknown.
struct unknown084 {
  const u8 *unknown00;            // Inferred role: data; original member name unknown.
  const LyrActionData *unknown08; // Inferred role: actions; original member name unknown.
  s32 unknown10;                  // Inferred role: action num; original member name unknown.
  f32 unknown14;                  // Inferred role: sec; original member name unknown.
  f32 unknown18;                  // Inferred role: delta sec; original member name unknown.
  f32 unknown1c;                  // Inferred role: inv delta sec; original member name unknown.
  u32 unknown20;                  // Inferred role: action idx; original member name unknown.
  u32 unknown24;                  // Inferred role: flags; original member name unknown.
  u32 *unknown28;                 // Inferred role: action works; original member name unknown.
};

static_assert(offsetof(unknown084, unknown20) == 0x20, "");
static_assert(sizeof(unknown084) == 0x30, "");

class LayerActBase : public LayerObj {
public:
  LayerActBase();
  virtual void updatePartsAction(const LyrActionData *action);
  void updateAction();
  void setupAction(u8 *work, const u8 *data);
  void startSetupAction();
  void checkActionEnd(const LyrActionData *action, bool isCmdEnd);
  u32 calcActionWorkSize(const u8 *data);
  void actionIdx(u32 index);
  void actionEnd();
  bool actcmd_no(const u8 *&cmd);
  bool actcmd_rot(const u8 *&cmd);
  bool actcmd_numrot(const u8 *&cmd);
  bool actcmd_priority(const u8 *&cmd);
  bool actcmd_wait(const u8 *&cmd);
  bool actcmd_ctrlspd(const u8 *&cmd);
  bool actcmd_pos(const u8 *&cmd);
  bool actcmd_scl(const u8 *&cmd);
  bool actcmd_addcol(const u8 *&cmd);
  bool actcmd_mulcol(const u8 *&cmd);
  bool actend_no(const LyrActionData *action, bool isCmdEnd);
  bool actend_cmdend(const LyrActionData *action, bool isCmdEnd);
  bool actend_on(const LyrActionData *action, bool isCmdEnd);

  static bool (LayerActBase::*s_pCallActEnd[])(const LyrActionData *action, bool isCmdEnd);

  unknown084 *unknown250; // Inferred role: act work; original member name unknown.
};

static_assert(offsetof(LayerActBase, unknown250) == 0x250, "");

} // namespace layer
