#pragma once
#include "gf/menu/gf_menu_object.h"
#include "types.h"

namespace gf {

struct GF_OBJ_HANDLE;

class GfMenuObjChainIcon : public GfMenuObject {
public:
  static void setupScreen(u32 handle, int unknown1, int unknown2, int unknown3, int unknown4);
  void initialize() override;
  void finalize() override;
  void update() override;
  struct OpenParam {
    GF_OBJ_HANDLE *unknown00; // Inferred role: handle; original member name unknown.
  };

  static void open(const OpenParam &param);
  static void close(u32 handle);
};

} // namespace gf
