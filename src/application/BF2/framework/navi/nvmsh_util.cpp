#include "navi/nvmsh_util.h"

namespace navi {

const int NvmshUtil::NAVI_UTIL_OFFX[4] = {1, 0, -1, 0};
const int NvmshUtil::NAVI_UTIL_OFFY[4] = {0, 1, 0, -1};

void NvmshUtil::getConectDirXY(int &x, int &y, const _NvMhMeshAdjacency *adjacency) {
  int index = (adjacency->unknown02 >> 4) & 3;
  x = NAVI_UTIL_OFFX[index];
  y = NAVI_UTIL_OFFY[index];
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
