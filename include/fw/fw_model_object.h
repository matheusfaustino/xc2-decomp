#pragma once
#include <stddef.h>

#include "mm_math/mm_vec3.h"
#include "mm_mtl/intrusive_list.h"
#include "mm_mtl/intrusive_ptr.h"
#include "mm_mtl/reference_object.h"
#include "types.h"

namespace ml {
class Scn;
class ScnObjModel;
} // namespace ml

namespace fw {

class ReferenceFrame;

// The layer list node is the IntrusiveListNode base at 0x10: ModelObject::~ModelObject and
// fw::ModelManager::update (1.0.0: 0x710018e530) reach it by subtracting 0x10 from the list node.
class ModelObject : public mm::mtl::ReferenceObject, public mm::mtl::IntrusiveListNode<ModelObject> {
public:
  ModelObject();
  virtual ~ModelObject();
  virtual void update();
  void setup();
  void changeLayer(u32 layer);
  bool isRender() const;
  ml::ScnObjModel *getInterface();
  void setActive(bool isActive);
  bool isActive() const;
  void setAddOffsetPos(const mm::Vec3 &offset);
  void setScale(f32 scale);

  u64 unknown20;      // Role and original member name unknown.
  ml::Scn *unknown28; // Inferred role: scn; original member name unknown.
  ml::ScnObjModel *m_interface;
  u32 unknown38; // Inferred role: layer; original member name unknown.
  bool m_active;
  bool unknown3d; // Inferred role: offset dirty; original member name unknown.
  u8 _3e[0x40 - 0x3e];
  mm::mtl::IntrusivePtr<ReferenceFrame> unknown40; // Inferred role: reference frame; original member name unknown.
  u8 _48[0x50 - 0x48];
  mm::Vec3 unknown50; // Inferred role: add offset pos; original member name unknown.
  f32 unknown60;      // Inferred role: scale; original member name unknown.
};

static_assert(offsetof(ModelObject, unknown28) == 0x28, "");
static_assert(offsetof(ModelObject, m_interface) == 0x30, "");
static_assert(offsetof(ModelObject, unknown38) == 0x38, "");
static_assert(offsetof(ModelObject, unknown40) == 0x40, "");
static_assert(offsetof(ModelObject, m_active) == 0x3c, "");
static_assert(offsetof(ModelObject, unknown3d) == 0x3d, "");
static_assert(offsetof(ModelObject, unknown50) == 0x50, "");
static_assert(offsetof(ModelObject, unknown60) == 0x60, "");

} // namespace fw
