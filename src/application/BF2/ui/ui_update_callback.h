#pragma once

namespace fw {
struct UpdateInfo;
}

namespace ui {

class UIUpdateCallback {
public:
  virtual void beginUpdate(const fw::UpdateInfo &info) = 0;
  virtual void endUpdate(const fw::UpdateInfo &info);
  virtual void beginApply();
  virtual void endApply();
};

} // namespace ui
