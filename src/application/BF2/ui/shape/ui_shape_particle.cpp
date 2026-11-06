#include "ui/shape/ui_shape_particle.h"

#include "ui/shape/ui_particle_data.h"
#include "ui/shape/ui_particle_manager.h"

namespace ui {

UIShapeParticle::UIShapeParticle(void *data, u32 size, u32 width, u32 height) : unknown40(-1), unknown44(width), unknown48(height) {
  UIParticleData particleData;
  particleData.deserialize(data, size);
  unknown40 = UIParticleManager::get()->create(particleData);
  if (unknown40 != -1) {
    u32 quadNum = static_cast<u32>((particleData.unknown6c + particleData.unknown70) * particleData.unknown74);
    setupBuffer(quadNum * 4, quadNum * 6);
    for (u32 i = 0; i < quadNum; i++) {
      u32 index[6] = {i * 4, i * 4 + 1, i * 4 + 2, i * 4 + 1, i * 4 + 2, i * 4 + 3};
      writeIndex(index, 6, i * 6);
    }
  }
}

UIShapeParticle::~UIShapeParticle() {
  if (unknown40 != -1) {
    UIParticleManager::get()->remove(unknown40);
  }
}

void UIShapeParticle::activate(bool activate) { UIParticleManager::get()->setActivate(unknown40, activate); }

bool UIShapeParticle::isActive() const { return UIParticleManager::get()->isActive(unknown40); }

const mm::mtl::RTTI *UIShapeParticle::getRTTI() const { return &m_rtti; }

} // namespace ui
