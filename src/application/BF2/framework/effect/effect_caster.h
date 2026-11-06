#pragma once
#include <stddef.h>

#include "eflib/effect_callback.h"
#include "framework/reference_frame.h"
#include "mm_math/mm_mat44.h"
#include "mm_math/mm_vec3.h"
#include "mm_mtl/delegate.h"
#include "mm_mtl/fixed_vector.h"
#include "mm_mtl/fixstr.h"
#include "mm_mtl/intrusive_list.h"
#include "mm_mtl/intrusive_ptr.h"
#include "mm_mtl/reference_object.h"
#include "mm_mtl/vector.h"
#include "types.h"

namespace mm {
namespace mtl {
class IAllocator;
} // namespace mtl
} // namespace mm

namespace fw {

class ModelObject;
struct UpdateInfo;

// Base class from the ctor (1.0.0: 0x7100178128; the u32 at 0x08 is zeroed before the vtable store), the vtable of two
// destructor slots, and EffectCasterBase incrementing the u32 at 0x08 of the point it keeps. The members from 0x60 are
// copied by the ctor from its EfpInitData argument, whose caller GfComEffectCaster binds the delegates.
class EffectPoint : public mm::mtl::ReferenceObject {
public:
  struct Param {
    mm::Mat44 unknown00;                             // Inferred role: offset matrix; original member name unknown.
    mm::Vec3 unknown40;                              // Inferred role: scale; original member name unknown.
    mm::mtl::IntrusivePtr<ReferenceFrame> unknown50; // Inferred role: parent reference frame; original member name unknown.
    mm::mtl::FixStr<32> unknown58;                   // Inferred role: name; original member name unknown.
    u32 unknown7c;                                   // Inferred role: name hash; original member name unknown.
    mm::mtl::FixStr<32> unknown80;                   // Inferred role: parent name; original member name unknown.
    s32 unknowna4;                                   // Inferred role: parent bone index; original member name unknown.
    bool unknowna8;                                  // Inferred role: dirty; original member name unknown.
  };

  ~EffectPoint() override;
  int getPointCount() const;
  int getPointIndex(const char *name) const;
  const char *getPointName(int index) const;
  const mm::Mat44 &getPointTransform(int index) const;
  const mm::Vec3 &getScale(int index) const;
  bool isDirty(u32 index);
  void setDirty(u32 index, bool isDirty);
  const char *getParentName(const char *name);
  mm::Mat44 getOffset(const char *name);

  u32 unknown0c;                        // Inferred role: layer; original member name unknown.
  mm::mtl::Vector<Param> unknown10;     // Inferred role: points; original member name unknown.
  mm::mtl::Vector<mm::Mat44> unknown38; // Inferred role: point transforms; original member name unknown.
  ModelObject *unknown60;               // Inferred role: model; original member name unknown.
  // Inferred role: returns the reference frame of a parent name; original member name unknown.
  mm::mtl::Delegate<mm::mtl::IntrusivePtr<ReferenceFrame>(const char *)> unknown68;
  // Inferred role: releases the reference frame of a parent name; original member name unknown.
  mm::mtl::Delegate<void(const char *)> unknown88;
};

static_assert(sizeof(EffectPoint::Param) == 0xb0, "");
static_assert(offsetof(EffectPoint::Param, unknown50) == 0x50, "");
static_assert(offsetof(EffectPoint::Param, unknown58) == 0x58, "");
static_assert(offsetof(EffectPoint::Param, unknown7c) == 0x7c, "");
static_assert(offsetof(EffectPoint::Param, unknown80) == 0x80, "");
static_assert(offsetof(EffectPoint::Param, unknowna4) == 0xa4, "");
static_assert(offsetof(EffectPoint::Param, unknowna8) == 0xa8, "");
static_assert(offsetof(EffectPoint, unknown0c) == 0x0c, "");
static_assert(offsetof(EffectPoint, unknown10) == 0x10, "");
static_assert(offsetof(EffectPoint, unknown38) == 0x38, "");
static_assert(offsetof(EffectPoint, unknown60) == 0x60, "");
static_assert(offsetof(EffectPoint, unknown68) == 0x68, "");
static_assert(offsetof(EffectPoint, unknown88) == 0x88, "");
static_assert(sizeof(EffectPoint) == 0xa8, "");

class EffectCasterBase;

class EffectCasterSend : public effect::IEffectCallBack, public mm::mtl::IntrusiveListNode<EffectCasterSend> {
public:
  static const u32 BUFFER_NUM = 2;

  struct MtxAndName {
    u32 unknown00;               // Inferred role: hash; original member name unknown.
    char unknown04[0x20 - 0x04]; // Inferred role: name; original member name unknown.
    mm::Mat44 unknown20;         // Inferred role: matrix; original member name unknown.
  };

  explicit EffectCasterSend(mm::mtl::IAllocator *allocator);
  virtual ~EffectCasterSend();
  virtual void *getMdl();
  virtual const char *getStateName();
  virtual void *getBindData(u32 &count);
  virtual f32 getMdlFrame();
  virtual f32 getMdlScl();
  virtual void getMdlMat(mm::Mat44 &matrix);
  virtual void getEfpMat(mm::Mat44 &matrix, const char *name);

  void tailCopy();
  EffectCasterBase *getCaster();
  bool checkCaster(EffectCasterBase *caster);
  void sendData(mm::Mat44 &matrix, const char *name, int index);
  void sendData(f32 scale);
  void sendDataFrame(f32 frame);
  void sendDataStateName(const char *name);

  mm::mtl::Vector<MtxAndName> unknown18[BUFFER_NUM]; // Inferred role: bind datas; original member name unknown.
  EffectCasterBase *m_caster;
  void *unknown70;                           // Inferred role: point; original member name unknown.
  mm::mtl::IAllocator *unknown78;            // Inferred role: allocator; original member name unknown.
  void *unknown80;                           // Inferred role: model; original member name unknown.
  f32 unknown88[BUFFER_NUM];                 // Inferred role: scales; original member name unknown.
  f32 unknown90[BUFFER_NUM];                 // Inferred role: frames; original member name unknown.
  mm::mtl::FixStr<32> unknown98[BUFFER_NUM]; // Inferred role: state names; original member name unknown.
};

static_assert(offsetof(EffectCasterSend::MtxAndName, unknown20) == 0x20, "");
static_assert(sizeof(EffectCasterSend::MtxAndName) == 0x60, "");
static_assert(offsetof(EffectCasterSend, unknown18) == 0x18, "");
static_assert(offsetof(EffectCasterSend, m_caster) == 0x68, "");
static_assert(offsetof(EffectCasterSend, unknown70) == 0x70, "");
static_assert(offsetof(EffectCasterSend, unknown78) == 0x78, "");
static_assert(offsetof(EffectCasterSend, unknown80) == 0x80, "");
static_assert(offsetof(EffectCasterSend, unknown98) == 0x98, "");
static_assert(sizeof(EffectCasterSend) == 0xe0, "");

class EffectCasterBase {
public:
  // Inferred role: per frame update; original name unknown.
  virtual void unknown001(const UpdateInfo &info) = 0;
  // Inferred role: update after the effect system update; original name unknown.
  virtual void unknown002(const UpdateInfo &info) = 0;
  virtual ~EffectCasterBase();
  void setScl(f32 scale);
  void setMotionFrame(f32 frame);
  void setStateName(const char *name);
  void setEfpMat(mm::Mat44 &matrix, const char *name, int index);
  void changeLayer(u32 layer, u32 newLayer);
  static void tail();

  static mm::mtl::FixedVector<EffectCasterBase *, 256> s_casterList;

  u8 _08[0x10 - 0x08];
  EffectPoint *unknown10;                  // Inferred role: point; original member name unknown.
  void *unknown18;                         // Inferred role: model; original member name unknown.
  EffectCasterSend::MtxAndName *unknown20; // Inferred role: bind datas; original member name unknown.
  u32 unknown28;                           // Inferred role: bind data count; original member name unknown.
  mm::mtl::IAllocator *unknown30;          // Inferred role: allocator; original member name unknown.
  u32 unknown38;                           // Inferred role: point count; original member name unknown.
  u32 unknown3c;                           // Inferred role: layer; original member name unknown.
  bool _40;
};

static_assert(offsetof(EffectCasterBase, unknown10) == 0x10, "");
static_assert(offsetof(EffectCasterBase, unknown20) == 0x20, "");
static_assert(offsetof(EffectCasterBase, unknown3c) == 0x3c, "");

} // namespace fw
