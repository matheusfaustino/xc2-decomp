#pragma once
#include <stddef.h>

#include "types.h"
#include "ui/ui_build_info.h"

namespace ui {

class UIBuildInfoBdat : public UIBuildInfo {
public:
  UIBuildInfoBdat(const char *name);
  virtual ~UIBuildInfoBdat();
  virtual const char *getName() const;
  virtual u32 getObjectNum() const;
  virtual void getObjectData(u32 index, ObjectData &data) const;

  const char *m_name;
};

static_assert(offsetof(UIBuildInfoBdat, m_name) == 0x08, "");
static_assert(sizeof(UIBuildInfoBdat) == 0x10, "");

} // namespace ui
