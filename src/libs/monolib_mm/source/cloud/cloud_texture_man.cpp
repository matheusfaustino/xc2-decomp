#include "cloud/cloud_texture_man.h"

#include "grlib/cglib_texture_buffer.h"
#include "types.h"

namespace cloud {

CloudTextureMan::CloudTextureMan() : unknown00{nullptr, nullptr, nullptr, nullptr} {}

CloudTextureMan::~CloudTextureMan() { releaseTexture(); }

void CloudTextureMan::releaseTexture() {
  for (u32 i = 0; i < 4; i++) {
    if (unknown00[i] != nullptr) {
      delete unknown00[i];
      unknown00[i] = nullptr;
    }
  }
}

} // namespace cloud
