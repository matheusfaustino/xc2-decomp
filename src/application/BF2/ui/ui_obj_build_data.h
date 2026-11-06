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

class UIObjBuildData {
public:
  struct Parameter {
    u16 unknown00; // Inferred role: parameter type; original member name unknown.
    u16 unknown02; // Inferred role: value kind flags; original member name unknown.
    u32 unknown04; // Inferred role: value or string index; original member name unknown.
  };

  struct ObjData {
    mm::mtl::Vector<Parameter> unknown00; // Inferred role: parameters; original member name unknown.
    u32 unknown28;                        // Inferred role: name index; original member name unknown.
  };

  UIObjBuildData(mm::mtl::IAllocator *allocator);
  virtual ~UIObjBuildData();

  static void initialize();
  void clear();
  void deserialize(const void *data, u32 size);
  u32 getObjectNum() const;
  u32 searchObject(const char *name);
  u32 getParamType(const char *name) const;

  mm::mtl::Vector<ObjData> unknown08;      // Inferred role: objects; original member name unknown.
  mm::mtl::Vector<const char *> unknown30; // Inferred role: object and string names; original member name unknown.
  mm::mtl::IAllocator *unknown58;          // Inferred role: allocator; original member name unknown.
};

static_assert(sizeof(UIObjBuildData::Parameter) == 0x08, "");
static_assert(offsetof(UIObjBuildData::ObjData, unknown28) == 0x28, "");
static_assert(sizeof(UIObjBuildData::ObjData) == 0x30, "");
static_assert(offsetof(UIObjBuildData, unknown08) == 0x08, "");
static_assert(offsetof(UIObjBuildData, unknown30) == 0x30, "");
static_assert(offsetof(UIObjBuildData, unknown58) == 0x58, "");
static_assert(sizeof(UIObjBuildData) == 0x60, "");

} // namespace ui
