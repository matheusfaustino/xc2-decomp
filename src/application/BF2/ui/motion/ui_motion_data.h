#pragma once
#include <stddef.h>

#include "mm_mtl/vector.h"
#include "types.h"

namespace mm {
namespace mtl {
class IAllocator;
} // namespace mtl
} // namespace mm

namespace ui {

class UIMotionData {
public:
  enum ParamType {
    PARAM_TYPE_POS_X = 0,
    PARAM_TYPE_POS_Y = 1,
    PARAM_TYPE_SCALE_X = 2,
    PARAM_TYPE_SCALE_Y = 3,
    PARAM_TYPE_ROT = 4,
    PARAM_TYPE_ALPHA = 5,
  };

  struct MotionParam {
    MotionParam() : unknown00(0), unknown04(0.0f), unknown08(0.0f), unknown0c(0.0f), unknown10(0.0f), unknown14(0.0f), unknown18(0.0f) {}

    bool has(ParamType type) const { return (unknown00 & (1 << type)) != 0; }

    u32 unknown00; // Inferred role: flags; original member name unknown.
    f32 unknown04; // Inferred role: pos x; original member name unknown.
    f32 unknown08; // Inferred role: pos y; original member name unknown.
    f32 unknown0c; // Inferred role: scale x; original member name unknown.
    f32 unknown10; // Inferred role: scale y; original member name unknown.
    f32 unknown14; // Inferred role: rot; original member name unknown.
    f32 unknown18; // Inferred role: alpha; original member name unknown.
  };

  struct Parameter {
    u16 unknown00; // Inferred role: parameter type; original member name unknown.
    u16 unknown02; // Inferred role: value kind flags; original member name unknown.
    u32 unknown04; // Inferred role: value or name index; original member name unknown.
  };

  UIMotionData(mm::mtl::IAllocator *allocator);
  virtual ~UIMotionData();

  struct MotionData {
    mm::mtl::Vector<mm::mtl::Vector<Parameter>> unknown00; // Inferred role: frames of parameter values; original member name unknown.
    u32 unknown28;                                         // Inferred role: name index; original member name unknown.
  };

  void initialize();
  void clear();
  u32 getMotionNum() const;
  u32 getParamType(const char *name) const;
  u32 searchMotion(const char *name) const;
  f32 getMotionLength(u32 motion) const;
  const char *getMotionName(u32 motion) const;
  bool getMotionParamDefault(u32 motion, MotionParam &param) const;
  bool getMotionParamFrame(u32 motion, u32 frame, u32 &outFrame, MotionParam &param) const;
  void getMotionParamDelta(u32 motion, f32 begin, f32 end, MotionParam &param) const;

  mm::mtl::Vector<MotionData> unknown08;   // Inferred role: motions; original member name unknown.
  mm::mtl::Vector<const char *> unknown30; // Inferred role: motion names; original member name unknown.
  mm::mtl::IAllocator *unknown58;          // Inferred role: allocator; original member name unknown.
};

static_assert(offsetof(UIMotionData::MotionData, unknown28) == 0x28, "");
static_assert(sizeof(UIMotionData::MotionData) == 0x30, "");
static_assert(sizeof(UIMotionData::Parameter) == 0x08, "");
static_assert(offsetof(UIMotionData, unknown08) == 0x08, "");
static_assert(offsetof(UIMotionData, unknown30) == 0x30, "");
static_assert(offsetof(UIMotionData, unknown58) == 0x58, "");
static_assert(sizeof(UIMotionData) == 0x60, "");

static_assert(offsetof(UIMotionData::MotionParam, unknown18) == 0x18, "");
static_assert(sizeof(UIMotionData::MotionParam) == 0x1c, "");

} // namespace ui
