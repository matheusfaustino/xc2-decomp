#pragma once
#include <stddef.h>

#include "types.h"
#include "ui/event/ui_event_data.h"
#include "ui/input/ui_input_layer.h"

namespace mm {
namespace mtl {
class IAllocator;
} // namespace mtl
} // namespace mm

namespace fw {
struct PadData;
}

namespace ui {

class UIInputPad : public UIInputLayer {
public:
  struct PadSetup;

  UIInputPad(u32 handle, u32 priority, const PadSetup &setup);
  static UIInputPad *create(u32 handle, u32 priority, const PadSetup &setup, mm::mtl::IAllocator *allocator);
  virtual ~UIInputPad();
  void setLineSelectionMax(u32 max);
  void setLineSelectLoop(bool loop);
  void setPageSize(u32 size);
  void setLoop(bool loop);
  virtual const EventData *getInputEventExt();
  virtual bool hasEventExt() const;
  virtual void clearExt();
  virtual UIInputPad *castUIInputPad();
  virtual bool updateImpl();
  void moveSelect(const fw::PadData &pad, u32 trigger, int index);
  void fixSelectNo(int index, bool up, bool wrap);

  u8 unknown1e0[0x360 - 0x1e0]; // Inferred role: pad state; original member names unknown.
  u64 unknown360;               // Role and original member name unknown.
  bool unknown368;              // Inferred role: loop; original member name unknown.
  u8 unknown369[0x380 - 0x369];
  u32 unknown380;  // Role and original member name unknown.
  u32 unknown384;  // Role and original member name unknown.
  u32 unknown388;  // Role and original member name unknown.
  u32 unknown38c;  // Role and original member name unknown.
  u32 unknown390;  // Inferred role: line selection max; original member name unknown.
  u32 unknown394;  // Inferred role: line count; original member name unknown.
  bool unknown398; // Inferred role: line select loop; original member name unknown.
  u8 unknown399[0x39c - 0x399];
  u32 unknown39c; // Inferred role: page size; original member name unknown.
  u32 unknown3a0; // Role and original member name unknown.
  u8 unknown3a4[0x3a8 - 0x3a4];
  EventData unknown3a8; // Inferred role: input event; original member name unknown.
};

static_assert(offsetof(UIInputPad, unknown360) == 0x360, "");
static_assert(offsetof(UIInputPad, unknown368) == 0x368, "");
static_assert(offsetof(UIInputPad, unknown390) == 0x390, "");
static_assert(offsetof(UIInputPad, unknown394) == 0x394, "");
static_assert(offsetof(UIInputPad, unknown398) == 0x398, "");
static_assert(offsetof(UIInputPad, unknown39c) == 0x39c, "");
static_assert(offsetof(UIInputPad, unknown3a8) == 0x3a8, "");
static_assert(sizeof(UIInputPad) == 0x3c0, "");

} // namespace ui
