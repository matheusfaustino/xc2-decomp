#pragma once
#include <stddef.h>

#include "mm_mtl/array.h"
#include "types.h"
#include "ui/ui_build_info.h"

namespace fw {
class CommonResource;
} // namespace fw

namespace ui {

class UIObjBuildData;

class UIBuildInfoUIO : public UIBuildInfo {
public:
  UIBuildInfoUIO(const char *name);
  virtual ~UIBuildInfoUIO();
  virtual void update();
  virtual bool isReady() const;
  virtual bool isValid() const;
  virtual const char *getName() const;
  virtual u32 getObjectNum() const;
  virtual void getObjectData(u32 index, ObjectData &data) const;
  void getObjectDataUIO(UIObjBuildData *buildData, u32 index, ObjectData &data) const;

  const char *m_name;
  fw::CommonResource *unknown10;        // Inferred role: resource; original member name unknown.
  mm::mtl::Array<ObjectData> unknown18; // Inferred role: object data; original member name unknown.
};

static_assert(offsetof(UIBuildInfoUIO, m_name) == 0x08, "");
static_assert(offsetof(UIBuildInfoUIO, unknown10) == 0x10, "");
static_assert(offsetof(UIBuildInfoUIO, unknown18) == 0x18, "");
static_assert(sizeof(UIBuildInfoUIO) == 0x30, "");

} // namespace ui
