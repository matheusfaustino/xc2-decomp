#pragma once
#include <stddef.h>

namespace mm {
struct Vec3;
} // namespace mm

namespace navi {

class NvmshManager;

class NvmshAccUtil {
public:
  NvmshAccUtil();
  bool testNaviColi(mm::Vec3 &position, const mm::Vec3 &target, float radius, float height, int layer = 0);
  bool testNaviColiRange(mm::Vec3 &position, const mm::Vec3 &target, float radius, float height, float range, int layer = 0);
  int testNaviStraightLine(mm::Vec3 &position, const mm::Vec3 &start, const mm::Vec3 &end, float radius, float height, int layer = 0);
  int checkNaviFindPos(const mm::Vec3 &position, float radius, float height, float range, float offset, const mm::Vec3 &target, float targetRadius, float targetHeight, float targetRange,
                       float targetOffset, int layer = 0);
  bool isNaviMeshReadCheck(const mm::Vec3 &position);

  NvmshManager *unknown00; // Inferred role: manager singleton; original member name unknown.
  bool unknown08;          // Inferred role: avoid attribute; original member name unknown.
  unsigned char _09[0x10 - 0x09];
};

static_assert(offsetof(NvmshAccUtil, unknown08) == 0x08, "");
static_assert(sizeof(NvmshAccUtil) == 0x10, "");

} // namespace navi
