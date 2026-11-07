#pragma once
#include <stddef.h>

#include "gf/menu/gf_menu_object.h"
#include "gf/menu/gf_menu_sequence.h"
#include "types.h"
#include "ui/event/ui_object_listener.h"

namespace gf {

class GfMenuCallContext;

class GfMenuObjChapterTelop : public GfMenuObject {
public:
  class ListListener : public ui::UIObjectListener {
  public:
    ListListener(GfMenuObjChapterTelop *owner) : unknown0c(0), unknown10(0), _14(), unknown20(owner) {}
    void reciveEvent(const ui::EventData &event, u32 handle) override;

    u32 unknown0c; // Inferred role: chapter or title number; original member name unknown.
    int unknown10; // Inferred role: telop type, 0 begin, 1 end and 2 avan title; original member name unknown.
    u8 _14[0x1c - 0x14];
    GfMenuObjChapterTelop *unknown20; // Inferred role: owner; original member name unknown.
  };

  GfMenuObjChapterTelop(u32 handle);
  ~GfMenuObjChapterTelop() override;
  void initialize() override;
  void finalize() override;
  void update() override;
  static void openBeginTelop(GfMenuCallContext *context, int chapter);
  static void openEndTelop(GfMenuCallContext *context, int chapter);
  static void openAvanTitle(GfMenuCallContext *context, int title);
  static void close(u16 unknown00);
  bool seqWaitStreamBegin();
  bool seqWaitStreamEnd();
  bool seqOutChapterEnd();
  bool seqWaitChapterEnd();

  u8 _24[0x28 - 0x24];
  GfMenuSequence<GfMenuObjChapterTelop, 4> unknown28; // Inferred role: telop steps; original member name unknown.
};

static_assert(offsetof(GfMenuObjChapterTelop::ListListener, unknown0c) == 0x0c, "");
static_assert(offsetof(GfMenuObjChapterTelop::ListListener, unknown10) == 0x10, "");
static_assert(offsetof(GfMenuObjChapterTelop::ListListener, unknown20) == 0x20, "");
static_assert(sizeof(GfMenuObjChapterTelop::ListListener) == 0x28, "");
static_assert(offsetof(GfMenuObjChapterTelop, unknown28) == 0x28, "");
static_assert(sizeof(GfMenuObjChapterTelop) == 0x188, "");

} // namespace gf
