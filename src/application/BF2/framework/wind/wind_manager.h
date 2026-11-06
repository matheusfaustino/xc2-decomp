#pragma once
#include <stddef.h>

#include "framework/manager_object.h"
#include "framework/wind/wind_impluse.h" // IWYU pragma: keep
#include "framework/wind/wind_probe.h"   // IWYU pragma: keep
#include "mm_mtl/intrusive_list.h"
#include "mm_mtl/intrusive_ptr.h"
#include "mm_mtl/singleton.h"
#include "types.h"
#include "wnd/wnd.h" // IWYU pragma: keep

namespace mm {
struct Vec3;
} // namespace mm

namespace ml {
class WinProc;
} // namespace ml

namespace wnd {
struct ResWind;
struct Query;
class WindWorld;
} // namespace wnd

namespace fw {

class CommonResource;
struct UpdateInfo;
class ProbeDrawer;
class ParamList;

class WindManager : public ManagerObject, public mm::mtl::PtrSingleton<WindManager> {
public:
  WindManager();
  ~WindManager() override;
  bool initialize() override;
  const char *getName() const override;
  virtual void update(const UpdateInfo &info);
  bool isDebugMode();
  void activateDebugWindow(ml::WinProc *window);
  int getGlobalWind() const;
  bool setGlobalWind(int wind);
  void addProbe(WindProbe *probe);
  void removeProbe(WindProbe *probe);
  void addDebugDrawer(ProbeDrawer &drawer);
  void removeDebugDrawer(ProbeDrawer &drawer);
  void addImpluse(WindImpluse *impluse);
  void removeImpluse(WindImpluse *impluse);
  static void setDebugMode(bool enable, u32 mask);
  static bool query(wnd::Query *query);
  void buildWorld(const wnd::ResWind *resWind);
  void destroyWorld();
  static const mm::Vec3 &getGlobalWindPower();

  wnd::WindWorld *unknown18;                     // Inferred role: wind world, reference counted by hand; original member name unknown.
  mm::mtl::IntrusiveList<WindProbe> unknown20;   // Inferred role: probes; original member name unknown.
  mm::mtl::IntrusiveList<ProbeDrawer> unknown38; // Inferred role: debug drawers; original member name unknown.
  mm::mtl::IntrusiveList<WindImpluse> unknown50; // Inferred role: impluses; original member name unknown.
  CommonResource *unknown68;                     // Inferred role: wind parameter resource; original member name unknown.
  mm::mtl::IntrusivePtr<ParamList> unknown70;    // Inferred role: wind parameter list; original member name unknown.
  mm::mtl::IntrusivePtr<ParamList> unknown78;    // Inferred role: second parameter list; original member name unknown.
  void *unknown80;                               // Inferred role: unknown pointer; original member name unknown.
  mm::mtl::IntrusivePtr<WindProbe> unknown88;    // Inferred role: global probe; original member name unknown.
  mm::mtl::IntrusivePtr<WindProbe> unknown90;    // Inferred role: follow frame probe; original member name unknown.
};

static_assert(offsetof(WindManager, unknown18) == 0x18, "");
static_assert(offsetof(WindManager, unknown20) == 0x20, "");
static_assert(offsetof(WindManager, unknown38) == 0x38, "");
static_assert(offsetof(WindManager, unknown50) == 0x50, "");
static_assert(offsetof(WindManager, unknown68) == 0x68, "");
static_assert(offsetof(WindManager, unknown70) == 0x70, "");
static_assert(offsetof(WindManager, unknown78) == 0x78, "");
static_assert(offsetof(WindManager, unknown80) == 0x80, "");
static_assert(offsetof(WindManager, unknown88) == 0x88, "");
static_assert(offsetof(WindManager, unknown90) == 0x90, "");

} // namespace fw
