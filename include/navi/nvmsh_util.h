#pragma once
#include <stddef.h>

#include "types.h"

namespace navi {

class NvmshConectResult;

// Adjacency entry of a navigation mesh; only the field read by NvmshUtil is known.
struct _NvMhMeshAdjacency {
  u16 unknown00;
  u16 unknown02; // Inferred role: direction in bits 4-5; original member name unknown.
};

// Navigation mesh geometry helpers.
class NvmshUtil {
public:
  static void getConectDirXY(int &x, int &y, const _NvMhMeshAdjacency *adjacency);
  static int getConectPriIdx(int x, int y, int z, const u8 *table);

  static const int NAVI_UTIL_OFFX[4];
  static const int NAVI_UTIL_OFFY[4];
};

// Result of an A* search; the fields are only known through the code that reads them.
struct NvmshAStarResult {
  u8 _00[0x48];
  u32 unknown48; // Inferred role: cleared by NvmshUtilSeach::seachAStar; original member name unknown.
};

// Runs the connection A* searches over the navigation mesh.
class NvmshUtilConect {
public:
  NvmshUtilConect();
  void seachConectAStar(NvmshConectResult *result, bool unknown);
  void seachSeamConectAStar(NvmshConectResult *result, bool unknown);
  void calcConectAStar();
  void calcSeamAStar();

  NvmshConectResult *unknown00;
  bool unknown08;
  bool unknown09;
};

static_assert(offsetof(NvmshUtilConect, unknown08) == 0x08, "");
static_assert(offsetof(NvmshUtilConect, unknown09) == 0x09, "");

// Runs the mesh A* search.
class NvmshUtilSeach {
public:
  NvmshUtilSeach();
  void seachAStar(NvmshAStarResult *result);
  void calcAStar();

  NvmshAStarResult *unknown00;
  u16 unknown08;
};

static_assert(offsetof(NvmshUtilSeach, unknown08) == 0x08, "");

// Collision tests against the navigation mesh.
class NvmshUtilColi {
public:
  void setAttr(bool enable, u32 attr);

  u8 _00[0x70];
  f32 unknown70; // Inferred role: 3.0f; original member name unknown.
  u8 _74[4];
  int unknown78; // Inferred role: -1; original member name unknown.
  u16 unknown7c; // Inferred role: 0xffff; original member name unknown.
  u8 _7e[2];
  u32 unknown80; // Inferred role: attribute bits changed by setAttr; original member name unknown.
  u32 unknown84;
  u16 unknown88;
  u16 unknown8a;
};

static_assert(offsetof(NvmshUtilColi, unknown70) == 0x70, "");
static_assert(offsetof(NvmshUtilColi, unknown78) == 0x78, "");
static_assert(offsetof(NvmshUtilColi, unknown7c) == 0x7c, "");
static_assert(offsetof(NvmshUtilColi, unknown80) == 0x80, "");
static_assert(offsetof(NvmshUtilColi, unknown88) == 0x88, "");
static_assert(offsetof(NvmshUtilColi, unknown8a) == 0x8a, "");

} // namespace navi
