// Original file name from the game string C:/Monolithsoft/BF2/prog/libs/wndlib/src/wndlib/object/wnd_object.cpp.
#include "wnd/wnd_object.h" // IWYU pragma: keep

#include "mm/mm_std_base.h"
#include "mm_math/mm_quat.h"
#include "mm_math/mm_vec3.h"

namespace wnd {

WindObject::WindObject(ObjectType type, const WindObjectCreateInfo &info) : unknown20(type), unknown58(info.unknown10), m_pProxy(nullptr), unknown68(0) {
#line 25
  MM_ASSERT(info.pTransform);
  MM_ASSERT(info.pShape);
  const Transform *transform = info.pTransform;
  unknown30.unknown00 = transform->unknown00;
  unknown30.unknown10 = transform->unknown10;
  m_pShape = info.pShape;
  m_pShape->addRef();
  unknown70.m_pOwner = this;
}

WindObject::~WindObject() {
#line 41
  mm::mtl::safeRelease(m_pShape);
  MM_ASSERT(m_pProxy == nullptr);
}

} // namespace wnd
