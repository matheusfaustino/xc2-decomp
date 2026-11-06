#include "cloud/cloud_uniform.h"

namespace cloud {

// Uniform block byte size of each CloudUniformType; evidence: createUniform multiplies this value
// by the instance count before setup, and CloudObjFlow's constructor uses entries 6 and 7.
u32 CloudUniform::s_nUniformBlockSize[18] = {0x150, 0x150, 0x70, 0x20, 0xe0, 0x20, 0x3000, 0x20, 0xd0, 0xa0, 0xf0, 0x60, 0xa0, 0xe0, 0x20, 0xa0, 0x140, 0x60};

// Left and right rotations of the cloud planes; evidence: CloudUniform::createPartCalcUniform.
f32 CloudUniform::s_fPlaneSideRot[2] = {-1.5707964f, 1.5707964f};

CloudUniform::CloudUniform() {}

CloudUniform::~CloudUniform() { release(); }

void CloudUniform::updateData() { grlib::CGLibDblUniformBlock::updateData(); }

void CloudUniform::releaseUniform() { release(); }

void CloudUniform::createUniform(CloudUniformType type, u32 num) {
  release();
  u32 size = s_nUniformBlockSize[type];
  u32 count = num == 0 ? 1 : num;
  setup(size * count);
}

} // namespace cloud
