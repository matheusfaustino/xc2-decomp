#include "framework/wind/wind_probe.h"

#include "framework/memory/framework_memory.h"
#include "framework/reference_frame.h"
#include "framework/wind/wind_manager.h"
#include "fw/fw_transform.h"
#include "ml/rand.h"
#include "mm_math/mm_quat.h"
#include "mm_math/mm_vec3.h"
#include "wnd/wnd_object.h"
#include "wnd/wnd_probe.h"
#include "wnd/wnd_shape.h"

namespace fw {

WindProbe::WindProbe(u32 group, f32 radius, mm::mtl::IntrusivePtr<ReferenceFrame> frame, bool isFollowFrame) : unknown28(frame), unknown30(radius), unknown34(isFollowFrame) {
  wnd::ProbeCreateInfo info = {};
  info.unknown14 = group;
  if (radius > 0.0f) {
#line 52
    info.pShape = new (FrameworkMemory::getAllocator(5), 16, __FILE__, __LINE__) wnd::SphereShape(radius);
  } else {
#line 56
    info.pShape = new (FrameworkMemory::getAllocator(5), 16, __FILE__, __LINE__) wnd::SphereShape(0.01f);
  }

  wnd::Transform transform;
  if (unknown28.get() == nullptr) {
    transform.unknown00 = mm::Vec3::zero;
    transform.unknown10 = mm::Quat::identity;
  } else {
    ReferenceFrame *ref = unknown28.get();
    mm::Vec3 offset = ref->unknown40.unknown10.rotate(mm::Vec3(0.0f, unknown30, 0.0f));
    transform.unknown00 = ref->unknown40.unknown00 + offset;
    transform.unknown10 = ref->unknown40.unknown10;
  }

  info.pTransform = &transform;
  info.unknown10 = 1;
  u32 seed = ml::mtRand(12);
#line 71
  unknown20 = new (FrameworkMemory::getAllocator(5), 16, __FILE__, __LINE__) wnd::WindProbe(info, seed, FrameworkMemory::getAllocator(5));
  unknown20->addRef();
  if (WindManager::get() != nullptr) {
    WindManager::get()->addProbe(this);
  }
}

WindProbe::~WindProbe() {
  if (WindManager::get() != nullptr) {
    WindManager::get()->removeProbe(this);
  }

  if (unknown20 != nullptr) {
    unknown20->release();
    unknown20 = nullptr;
  }
}

} // namespace fw
