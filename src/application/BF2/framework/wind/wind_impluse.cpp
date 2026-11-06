#include "framework/wind/wind_impluse.h"

#include "framework/memory/framework_memory.h"
#include "framework/wind/wind_manager.h"
#include "fw/fw_update_info.h"
#include "mm_math/mm_quat.h"
#include "mm_math/mm_vec3.h"
#include "wnd/wnd_object.h"
#include "wnd/wnd_probe.h"
#include "wnd/wnd_shape.h"

namespace fw {

WindImpluse::WindImpluse(const mm::Vec3 &position, f32 radius, f32 param3, f32 param4, f32 param5) : unknown28(0.0f), unknown2c(param5), unknown30(param4) {
  wnd::LocalWindCreateInfo info;
  info.unknown14 = param4;
  info.unknown18 = param3;
  info.unknown1c = radius;
#line 38
  wnd::SphereShape *shape = new (FrameworkMemory::getAllocator(5), 16, __FILE__, __LINE__) wnd::SphereShape(radius);
  info.pShape = shape;
  wnd::Transform transform;
  transform.unknown00 = position;
  transform.unknown10 = mm::Quat::identity;
  info.unknown10 = 0;
  info.pTransform = &transform;
#line 44
  unknown20 = new (FrameworkMemory::getAllocator(5), 16, __FILE__, __LINE__) wnd::LocalWind(info);
  if (WindManager::get() != nullptr) {
    WindManager::get()->addImpluse(this);
  }
}

WindImpluse::~WindImpluse() {
  if (WindManager::get() != nullptr) {
    WindManager::get()->removeImpluse(this);
  }
}

bool WindImpluse::update(const UpdateInfo &info) {
  unknown28 += info.unknown04;
  if (unknown20 != nullptr) {
    f32 ratio = unknown28 / unknown2c;
    f32 clamped = ratio;
    if (1.0f < ratio) {
      clamped = 1.0f;
    }

    f32 fade = 1.0f - clamped;
    if (ratio < 0.0f) {
      fade = 1.0f;
    }

    unknown20->unknown88 = unknown30 * fade;
  }

  return unknown28 < unknown2c;
}

} // namespace fw
