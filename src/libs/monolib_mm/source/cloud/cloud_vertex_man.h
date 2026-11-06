#pragma once
#include <stddef.h>

namespace grlib {
class CGLibVtxIndex;
class CGLibVtxVertex;
} // namespace grlib

namespace cloud {

enum CloudVertexType {};

class CloudVertexMan {
public:
  CloudVertexMan();
  ~CloudVertexMan();
  void createVertexList();
  void releaseVertexList();
  void createSphere();
  void createCapsule();
  void createHalfSphere();
  void bindVertexList(CloudVertexType type);
  void renderIndexList(CloudVertexType type);

  grlib::CGLibVtxVertex *unknown00[9]; // Inferred role: vertex lists indexed by CloudVertexType; original member name unknown.
  grlib::CGLibVtxIndex *unknown48[9];  // Inferred role: index lists indexed by CloudVertexType; original member name unknown.
};

static_assert(offsetof(CloudVertexMan, unknown00) == 0x00, "");
static_assert(offsetof(CloudVertexMan, unknown48) == 0x48, "");
static_assert(sizeof(CloudVertexMan) == 0x90, "");

} // namespace cloud
