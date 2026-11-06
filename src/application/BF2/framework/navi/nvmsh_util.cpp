#include "navi/nvmsh_util.h"

namespace navi {

const int NvmshUtil::NAVI_UTIL_OFFX[4] = {1, 0, -1, 0};
const int NvmshUtil::NAVI_UTIL_OFFY[4] = {0, 1, 0, -1};

void NvmshUtil::getConectDirXY(int &x, int &y, const _NvMhMeshAdjacency *adjacency) {
  int index = (adjacency->unknown02 >> 4) & 3;
  x = NAVI_UTIL_OFFX[index];
  y = NAVI_UTIL_OFFY[index];
}

// The game computes both branch indices and selects between them; each triangular term is an arithmetic shift
// right by one in the game's code (`asr #1`).
int NvmshUtil::getConectPriIdx(int x, int y, int z, const u8 *table) {
  s32 index;
  if (x > y) {
    index = z * y + (x - y - 1 - ((y * (y + 1)) >> 1));
  } else {
    index = z * x + (y - x - 1 - ((x * (x + 1)) >> 1));
  }

  return table[index] + 1;
}

NvmshUtilConect::NvmshUtilConect() { unknown09 = false; }

void NvmshUtilConect::seachConectAStar(NvmshConectResult *result, bool unknown) {
  unknown00 = result;
  unknown08 = unknown;
  calcConectAStar();
}

void NvmshUtilConect::seachSeamConectAStar(NvmshConectResult *result, bool unknown) {
  unknown00 = result;
  unknown08 = unknown;
  calcSeamAStar();
}

NvmshUtilSeach::NvmshUtilSeach() { unknown08 = 0; }

void NvmshUtilSeach::seachAStar(NvmshAStarResult *result) {
  unknown00 = result;
  result->unknown48 = 0;
  calcAStar();
}

void NvmshUtilColi::setAttr(bool enable, u32 attr) {
  if (!enable) {
    unknown80 |= attr;
  } else {
    unknown80 &= ~attr;
  }
}

} // namespace navi
