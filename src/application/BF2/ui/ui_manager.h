#pragma once
#include <stddef.h>

#include "mm_mtl/fixed_vector.h"
#include "mm_mtl/singleton.h"
#include "types.h"
#include "ui/ui_group.h"

namespace mtl {
class ALLOC_HANDLE;
} // namespace mtl

namespace ui {

class UIGameListener;
class UIPadRead;
class UIUpdateCallback;
struct EventData;

class UIManager : public mm::mtl::PtrSingleton<UIManager> {
public:
  enum {
    GROUP_MAX = 8,
  };

  struct LayerInfo {
    u16 unknown00; // Inferred role: group; original member name unknown.
    u16 unknown02; // Inferred role: layer; original member name unknown.
  };

  UIManager(const UIPadRead *padRead, mtl::ALLOC_HANDLE handle);
  void terminate();
  void resetData();
  u32 createLayer(u32 layer);
  bool isLayerCreated(u32 layer);
  u32 searchObject(const char *name, u16 group);
  void initializeLayer(u32 layer);
  void terminateLayer(u32 layer, bool isImmediate);
  void releaseLayer(u32 layer);
  bool preloadLayer(u32 layer);
  void terminateObject(const char *name, u8 group, bool isImmediate);
  void requestObjectEvent(u32 handle, const EventData &event, f32 delay);
  void requestGameEvent(u32 handle, const EventData &event);
  void setGameEventLisenter(UIGameListener *listener);
  void setUpdateCallback(UIUpdateCallback *callback);
  void registObjectTimer(u32 handle, u16 id, u64 time, bool repeat);
  void releaseObjectTimer(u32 handle);
  void setDisp(u16 group, bool disp);
  bool isLoading() const;
  void reloadText();
  u32 getLoadedResourceSize();
  bool isTerminated() const;

  UIGroup unknown08[GROUP_MAX];                   // Inferred role: groups; original member name unknown.
  mm::mtl::FixedVector<LayerInfo, 32> unknown348; // Inferred role: layers; original member name unknown.
  u8 _3d0[0x3d8 - 0x3d0];
  const UIPadRead *unknown3d8; // Inferred role: pad read; original member name unknown.
  UIUpdateCallback *m_updateCallback;
};

static_assert(offsetof(UIManager, unknown08) == 0x08, "");
static_assert(offsetof(UIManager, unknown348) == 0x348, "");
static_assert(offsetof(UIManager, unknown3d8) == 0x3d8, "");
static_assert(offsetof(UIManager, m_updateCallback) == 0x3e0, "");
static_assert(sizeof(UIManager) == 0x3e8, "");

} // namespace ui
