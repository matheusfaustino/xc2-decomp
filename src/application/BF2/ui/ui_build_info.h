#pragma once
#include "types.h"

namespace ui {

class UIBuildInfo {
public:
  struct ObjectData {
    u8 unknown00[0xc0]; // Inferred role: object build data; original member names unknown.
  };

  virtual ~UIBuildInfo() {}
  virtual void update();
  virtual bool isReady() const;
  virtual bool isValid() const;
  virtual u32 getObjectNum() const = 0;
  virtual void getObjectData(u32 index, ObjectData &data) const = 0;
  virtual const char *getName() const = 0;
};

static_assert(sizeof(UIBuildInfo::ObjectData) == 0xc0, "");

} // namespace ui
