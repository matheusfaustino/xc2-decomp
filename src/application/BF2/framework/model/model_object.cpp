#include "framework/model/model_manager.h"
#include "framework/reference_frame.h"
#include "fw/fw_model_object.h"
#include "ml/scn.h"
#include "ml/scn_obj_model.h"
#include "mm_math/mm_vec3.h"
#include "mm_mtl/fixed_array.h"
#include "mm_mtl/intrusive_list.h"
#include "mm_mtl/intrusive_ptr.h"
#include "scn/scn_obj_acc.h"
#include "types.h"

namespace mm {
struct Mat44;
} // namespace mm

namespace fw {

ModelObject::~ModelObject() {
  if (unknown28 != nullptr) {
    if (m_interface != nullptr) {
      unknown28->releaseItem(m_interface);
      m_interface = nullptr;
    }
    unknown28 = nullptr;
  }

  ModelManager::get()->unknown18[unknown38].unknown00.erase(this);
}

ModelObject::ModelObject() : unknown20(0), m_interface(nullptr), unknown38(0), m_active(true), unknown3d(false), unknown50(0.0f, 0.0f, 0.0f), unknown60(1.0f) {}

void ModelObject::update() {
  if (m_active && unknown40.get() != nullptr) {
    if (m_interface != nullptr) {
      const mm::Mat44 &matrix = unknown40.get()->unknown60;
      m_interface->setMat(matrix);
      if (unknown3d) {
        m_interface->getOffsetInfo(true)->unknown00 = unknown50;
      }
    }
  }
}

void ModelObject::setScale(f32 scale) {
  unknown60 = scale;
  if (m_interface != nullptr) {
    mm::Vec3 vector(scale, scale, scale);
    m_interface->setScl(vector);
  }
}

void ModelObject::setup() { unknown3d = false; }

bool ModelObject::isRender() const {
  ml::ScnObjAccResMdlInfo info(m_interface);
  return info.isGpuRender() != 0;
}

ml::ScnObjModel *ModelObject::getInterface() { return m_interface; }

void ModelObject::setActive(bool isActive) { m_active = isActive; }

bool ModelObject::isActive() const { return m_active; }

void ModelObject::setAddOffsetPos(const mm::Vec3 &offset) { unknown50 = offset; }

} // namespace fw
