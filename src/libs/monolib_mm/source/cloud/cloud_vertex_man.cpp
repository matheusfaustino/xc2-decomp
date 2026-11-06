#include "cloud/cloud_vertex_man.h"

#include <string.h>

#include "grlib/cglib_ctl_vertex.h"
#include "grlib/cglib_vtx_index.h"
#include "grlib/cglib_vtx_vertex.h"
#include "types.h"

namespace cloud {

CloudVertexMan::CloudVertexMan() {
  memset(this, 0, sizeof(*this));
  createVertexList();
}

CloudVertexMan::~CloudVertexMan() { releaseVertexList(); }

void CloudVertexMan::releaseVertexList() {
  for (u32 i = 0; i < 9; i++) {
    if (unknown00[i] != nullptr) {
      delete unknown00[i];
      unknown00[i] = nullptr;
    }
    if (unknown48[i] != nullptr) {
      delete unknown48[i];
      unknown48[i] = nullptr;
    }
  }
}

void CloudVertexMan::bindVertexList(CloudVertexType type) { grlib::CGLibCtlVertex::setVertex(*unknown00[type]); }

void CloudVertexMan::renderIndexList(CloudVertexType type) { unknown48[type]->drawIndex(); }

} // namespace cloud
