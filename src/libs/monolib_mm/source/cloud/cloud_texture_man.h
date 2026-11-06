#pragma once
#include <stddef.h>

namespace grlib {
class CGLibTextureBuffer;
} // namespace grlib

namespace cloud {

class CloudTextureMan {
public:
  CloudTextureMan();
  ~CloudTextureMan();
  void createTexture();
  void releaseTexture();

  grlib::CGLibTextureBuffer *unknown00[4]; // Inferred role: cloud textures; original member name unknown.
};

static_assert(offsetof(CloudTextureMan, unknown00) == 0x00, "");
static_assert(sizeof(CloudTextureMan) == 0x20, "");

} // namespace cloud
