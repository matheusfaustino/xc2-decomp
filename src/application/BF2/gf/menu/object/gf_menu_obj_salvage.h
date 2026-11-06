#pragma once
#include <stddef.h>

#include "gf/menu/gf_menu_object.h"
#include "gf/menu/gf_menu_sequence.h"
#include "types.h"
#include "ui/event/ui_object_listener.h"

namespace gf {

class GfMenuCallContext;

class GfMenuObjSalvage : public GfMenuObject {
public:
  class ResultListener : public ui::UIObjectListener {
  public:
    ResultListener(GfMenuObjSalvage *owner) : unknown0c(0), _10(), unknown20(owner) {}
    void reciveEvent(const ui::EventData &event, u32 handle) override;

    int unknown0c; // Inferred role: state, -1 once closed; original member name unknown.
    u8 _10[0x1c - 0x10];
    GfMenuObjSalvage *unknown20; // Inferred role: owner; original member name unknown.
  };

  struct ResultParam {
    u32 unknown00; // Inferred role: result rank; original member name unknown.
  };

  GfMenuObjSalvage(u32 handle);
  ~GfMenuObjSalvage() override;
  void initialize() override;
  void finalize() override;
  void update() override;
  bool seqResultAppear();
  bool seqResultLeave();
  bool seqResultEnd();
  static void openResult(GfMenuCallContext *context, const ResultParam &param);

  u32 unknown24;                                 // Inferred role: result rank; original member name unknown.
  GfMenuSequence<GfMenuObjSalvage, 8> unknown28; // Inferred role: result steps; original member name unknown.
  bool unknown2b8;                               // Inferred role: animation or control started flag; original member name unknown.
};

static_assert(sizeof(GfMenuObjSalvage::ResultParam) == 0x04, "");
static_assert(offsetof(GfMenuObjSalvage::ResultListener, unknown20) == 0x20, "");
static_assert(sizeof(GfMenuObjSalvage::ResultListener) == 0x28, "");
static_assert(offsetof(GfMenuObjSalvage, unknown24) == 0x24, "");
static_assert(offsetof(GfMenuObjSalvage, unknown28) == 0x28, "");
static_assert(offsetof(GfMenuObjSalvage, unknown2b8) == 0x2b8, "");
static_assert(sizeof(GfMenuObjSalvage) == 0x2c0, "");

} // namespace gf
