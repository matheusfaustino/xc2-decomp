#include "cloud/cloud_uniform.h"

#include <math.h>

#include "cloud/cloud_calc_params.h"
#include "cloud/cloud_man.h"
#include "cloud/cloud_obj_man.h"
#include "dr/dr_draw_work_info.h"
#include "grlib/shd_util.h"
#include "mm_math/mm_mat44.h"
#include "mm_math/mm_vec3.h"

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

// 24-entry fog table, height, inverse of unknown380, and unknown384; the cloud shaders read it as
// the per-frame fog block.
void CloudUniform::createFogUniform() {
  CloudCalcParams *params = CloudMan::get()->unknown50->unknown38;
  const f32 *fog = params->unknown00->unknown1d4;
  f32 *buff = static_cast<f32 *>(getBuff());
  grlib::ShdF32SetF32Array(buff, fog, 0x18);
  buff[0x18] = params->unknown194;
  buff[0x19] = 1.0f / params->unknown380;
  buff[0x1a] = params->unknown384;
  buff[0x1b] = 0.0f;
}

// Transposed global fog matrix followed by the same 24-entry fog table.
void CloudUniform::createGlbFogUniform() {
  CloudCalcParams *params = CloudMan::get()->unknown50->unknown38;
  const f32 *fog = params->unknown00->unknown1d4;
  f32 *buff = static_cast<f32 *>(getBuff());
  grlib::ShdMat44TransposeMat44(buff, params->unknown110);
  grlib::ShdF32SetF32Array(buff + 0x10, fog, 0x18);
}

// Shadow offsets, cloud density and its complement.
void CloudUniform::createFlowShadow() {
  f32 *buff = static_cast<f32 *>(getBuff());
  CloudCalcParams *params = CloudMan::get()->unknown50->unknown38;
  buff[0] = params->unknown00->unknown4e0[0];
  buff[1] = params->unknown00->unknown4e0[1];
  buff[2] = params->unknown00->unknown4e0[2];
  buff[3] = 0.0f;
  buff[4] = params->unknown170;
  buff[5] = 1.0f - params->unknown170;
  buff[6] = 0.0f;
  buff[7] = 0.0f;
}

// Detail colour, its three parameters and the current cloud speed product.
void CloudUniform::createDetailUniform(const mm::Vec3 &detail) {
  CloudMan *man = CloudMan::get();
  CloudCalcParams *params = man->unknown50->unknown38;
  f32 speed = man->unknown60 * man->_64;
  f32 *buff = static_cast<f32 *>(getBuff());
  buff[0] = detail.x;
  buff[1] = detail.y;
  buff[2] = detail.z;
  buff[3] = 0.0f;
  buff[4] = params->unknown334;
  buff[5] = params->unknown330;
  buff[6] = params->unknown32c;
  buff[7] = speed;
}

// Sky sphere vertex uniform: two matrices (unknown90 and unknownd0, the latter twice), the sky
// colour at 0x30 and unknown3d0 scaled by 0.9.
void CloudUniform::createSkyVertexUniform() {
  CloudCalcParams *params = CloudMan::get()->unknown50->unknown38;
  f32 *buff = static_cast<f32 *>(getBuff());
  grlib::ShdMat44TransposeMat44(buff, params->unknown90);
  grlib::ShdMat44TransposeMat44(buff + 0x10, params->unknownd0);
  grlib::ShdMat44TransposeMat44(buff + 0x20, params->unknownd0);
  buff[0x30] = params->unknown150;
  buff[0x31] = params->unknown154;
  buff[0x32] = params->unknown158;
  buff[0x33] = params->unknown3d0 * 0.9f;
}

// Flow model vertex uniform: view projection, the three shadow matrices, the flow speed and the
// height/cloud parameters.
void CloudUniform::createFlowMdlUniform(f32 speed) {
  f32 *buff = static_cast<f32 *>(getBuff());
  CloudCalcParams *params = CloudMan::get()->unknown50->unknown38;
  grlib::ShdMat44TransposeMat44(buff, params->unknown50);
  grlib::ShdMat44TransposeMat44(buff + 0x10, params->unknown00->unknown420[0]);
  grlib::ShdMat44TransposeMat44(buff + 0x20, params->unknown00->unknown420[1]);
  grlib::ShdMat44TransposeMat44(buff + 0x30, params->unknown00->unknown420[2]);
  buff[0x40] = speed;
  buff[0x41] = 0.0f;
  buff[0x42] = 0.0f;
  buff[0x43] = speed;
  buff[0x44] = params->unknown198;
  buff[0x45] = 1.0f;
  buff[0x46] = params->unknown198;
  buff[0x47] = 0.0f;
  buff[0x48] = params->unknown180;
  buff[0x49] = 0.0f;
  buff[0x4a] = params->unknown188;
  buff[0x4b] = 0.0f;
}

// Gaussian weight used by the blur kernels: exp((x^2 + y^2) / (-2 z^2)) / sqrt(2 pi z^2).
f32 CloudUniform::gaussianDistribution(f32 x, f32 y, f32 z) {
  f32 g = 1.0f / sqrtf(z * 6.2831855f * z);
  g *= expf((x * x + y * y) / (z * -2.0f * z));
  return g;
}

// The light direction (unknown400..408) transformed by the view matrix and normalized, then the
// three light colours at 0x3ec..0x3f4.
void CloudUniform::createLightUniform() {
  CloudCalcParams *params = CloudMan::get()->unknown50->unknown38;
  f32 *buff = static_cast<f32 *>(getBuff());
  const mm::Mat44 &mat = params->unknown50;
  mm::Vec3 dir;
  dir.x = params->unknown400 * mat.rows[0][0] + params->unknown404 * mat.rows[1][0] + params->unknown408 * mat.rows[2][0];
  dir.y = params->unknown400 * mat.rows[0][1] + params->unknown404 * mat.rows[1][1] + params->unknown408 * mat.rows[2][1];
  dir.z = params->unknown400 * mat.rows[0][2] + params->unknown404 * mat.rows[1][2] + params->unknown408 * mat.rows[2][2];
  dir.unknown001();
  buff[0] = dir.x;
  buff[1] = dir.y;
  buff[2] = dir.z;
  buff[3] = 1.0f;
  buff[4] = params->unknown3ec;
  buff[5] = params->unknown3f0;
  buff[6] = params->unknown3f4;
  buff[7] = 1.0f;
}

// Two model colours, the sky colour, then three model colours.
void CloudUniform::createMdlColUniform() {
  CloudCalcParams *params = CloudMan::get()->unknown50->unknown38;
  f32 *buff = static_cast<f32 *>(getBuff());
  buff[0] = params->unknown360;
  buff[1] = params->unknown364;
  buff[2] = params->unknown368;
  buff[3] = 1.0f;
  buff[4] = params->unknown36c;
  buff[5] = params->unknown370;
  buff[6] = params->unknown374;
  buff[7] = 1.0f;
  buff[8] = params->unknown150;
  buff[9] = params->unknown154;
  buff[0xa] = params->unknown158;
  buff[0xb] = 0.0f;
  buff[0xc] = params->unknown3e0;
  buff[0xd] = params->unknown3e4;
  buff[0xe] = params->unknown3e8;
  buff[0xf] = 0.0f;
  buff[0x10] = params->unknown3ec;
  buff[0x11] = params->unknown3f0;
  buff[0x12] = params->unknown3f4;
  buff[0x13] = 0.0f;
  buff[0x14] = params->unknown400;
  buff[0x15] = params->unknown404;
  buff[0x16] = params->unknown408;
  buff[0x17] = 0.0f;
}

} // namespace cloud
