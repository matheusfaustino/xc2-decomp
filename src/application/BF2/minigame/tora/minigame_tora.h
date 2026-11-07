#pragma once
#include <stddef.h>

namespace fw {
class UpdatableObject;
}

namespace ui {
class UIObjectAcc;
}

namespace minigame {
namespace tora {

// Fixed-stride task pool and active list, as used at 0x71006a0fdc–0x71006a1368.
class mytask {
public:
  // Inferred role: task link/callback header; original type name unknown.
  struct unknown001 {
    unknown001 *unknown00;     // Inferred role: next link; original member name unknown.
    unknown001 *unknown08;     // Inferred role: previous link; original member name unknown.
    void (*unknown10)(void *); // Inferred role: move callback; original member name unknown.
    void (*unknown18)(void *); // Inferred role: draw callback; original member name unknown.
  };

  mytask(int size, int count);
  ~mytask();
  void reset();
  void *entry();
  void *entryNext(void *task);
  void *entryTop();
  void *entryPrev(void *task);
  void *entryEnd();
  void remove(void *task);
  void executeMove();
  void executeDraw();
  void *enum_next(void *task);

  void *unknown00;       // Inferred role: allocated pool; original member name unknown.
  int unknown08;         // Inferred role: slot stride; original member name unknown.
  int unknown0c;         // Inferred role: slot count; original member name unknown.
  unknown001 *unknown10; // Inferred role: free-list head; original member name unknown.
  unknown001 *unknown18; // Inferred role: active-list head; original member name unknown.
  unknown001 *unknown20; // Inferred role: active-list tail; original member name unknown.
  unknown001 *unknown28; // Inferred role: traversal successor; original member name unknown.
};

static_assert(offsetof(mytask::unknown001, unknown08) == 0x08, "");
static_assert(offsetof(mytask::unknown001, unknown10) == 0x10, "");
static_assert(offsetof(mytask::unknown001, unknown18) == 0x18, "");
static_assert(sizeof(mytask::unknown001) == 0x20, "");
static_assert(offsetof(mytask, unknown08) == 0x08, "");
static_assert(offsetof(mytask, unknown0c) == 0x0c, "");
static_assert(offsetof(mytask, unknown10) == 0x10, "");
static_assert(offsetof(mytask, unknown18) == 0x18, "");
static_assert(offsetof(mytask, unknown20) == 0x20, "");
static_assert(offsetof(mytask, unknown28) == 0x28, "");
static_assert(sizeof(mytask) == 0x30, "");

namespace factory {

fw::UpdatableObject *create(ui::UIObjectAcc *parent);

} // namespace factory
} // namespace tora
} // namespace minigame
