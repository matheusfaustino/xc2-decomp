#pragma once
#include <stddef.h>

#include "types.h"

namespace navi {

class NvmshConectResult;

// Adjacency entry of a navigation mesh; only the field read by NvmshUtil is known.
struct _NvMhMeshAdjacency {
  u16 unknown00; // Inferred role: unknown; original member name unknown.
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

static_assert(offsetof(NvmshAStarResult, unknown48) == 0x48, "");

// Runs the connection A* searches over the navigation mesh.
class NvmshUtilConect {
public:
  NvmshUtilConect();
  void seachConectAStar(NvmshConectResult *result, bool unknown);
  void seachSeamConectAStar(NvmshConectResult *result, bool unknown);
  void calcConectAStar();
  void calcSeamAStar();

  NvmshConectResult *unknown00; // Inferred role: search result; original member name unknown.
  bool unknown08;               // Inferred role: search parameter; original member name unknown.
  bool unknown09;               // Inferred role: cleared by the constructor; original member name unknown.
};

static_assert(offsetof(NvmshUtilConect, unknown00) == 0x00, "");
static_assert(offsetof(NvmshUtilConect, unknown08) == 0x08, "");
static_assert(offsetof(NvmshUtilConect, unknown09) == 0x09, "");
static_assert(sizeof(NvmshUtilConect) == 0x10, "");

// Runs the mesh A* search.
class NvmshUtilSeach {
public:
  NvmshUtilSeach();
  void seachAStar(NvmshAStarResult *result);
  void calcAStar();

  NvmshAStarResult *unknown00; // Inferred role: search result; original member name unknown.
  u16 unknown08;               // Inferred role: cleared by the constructor; original member name unknown.
};

static_assert(offsetof(NvmshUtilSeach, unknown00) == 0x00, "");
static_assert(offsetof(NvmshUtilSeach, unknown08) == 0x08, "");
static_assert(sizeof(NvmshUtilSeach) == 0x10, "");

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
  u32 unknown84; // Inferred role: unknown; original member name unknown.
  u16 unknown88; // Inferred role: unknown; original member name unknown.
  u16 unknown8a; // Inferred role: unknown; original member name unknown.
};

static_assert(offsetof(NvmshUtilColi, unknown70) == 0x70, "");
static_assert(offsetof(NvmshUtilColi, unknown78) == 0x78, "");
static_assert(offsetof(NvmshUtilColi, unknown7c) == 0x7c, "");
static_assert(offsetof(NvmshUtilColi, unknown80) == 0x80, "");
static_assert(offsetof(NvmshUtilColi, unknown84) == 0x84, "");
static_assert(offsetof(NvmshUtilColi, unknown88) == 0x88, "");
static_assert(offsetof(NvmshUtilColi, unknown8a) == 0x8a, "");
static_assert(sizeof(NvmshUtilColi) == 0x8c, "");

} // namespace navi
