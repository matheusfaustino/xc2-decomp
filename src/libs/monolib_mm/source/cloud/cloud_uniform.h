#pragma once
#include "grlib/cglib_dbl_uniform_block.h"
#include "types.h"

namespace mm {
struct Vec3;
} // namespace mm

namespace cloud {

class CloudMdlParts;
struct CMLObjData;
struct CloudWrkParticles;

enum CloudUniformType {};

// A double-buffered uniform block of the cloud shaders. The create* functions upload values read
// from CloudCalcParams (CloudMan::get()->unknown50->unknown38) into the mapped buffer of the block.
class CloudUniform : public grlib::CGLibDblUniformBlock {
public:
  CloudUniform();
  ~CloudUniform();
  void createUniform(CloudUniformType type, u32 num);
  void updateData();
  void releaseUniform();

  void createFogUniform();
  void createFlowMdlUniform(f32 speed);
  void createFlowShadow();
  void createMdlColUniform();
  void createDefUniform(u32 a, u32 b, f32 c, f32 d);
  void createBlurUniform(bool horizontal);
  void createColorUniform(const mm::Vec3 &a, const mm::Vec3 &b, const mm::Vec3 &c, const mm::Vec3 &d, const mm::Vec3 &e, f32 f, f32 g, f32 h, u32 i, u32 j, f32 k);
  void createLightUniform();
  void createDetailUniform(const mm::Vec3 &detail);
  void createGlbFogUniform();
  void createSkyUniform(const mm::Vec3 &sky);
  void createSkyVertexUniform();
  void createParticlesUniform(const CloudWrkParticles *particles, u32 count);
  void createMdlPolyUniform(const CloudMdlParts *parts, const mm::Vec3 &a, const mm::Vec3 &b, const CMLObjData *data);
  void createPartCalcUniform(u32 width, u32 height);
  void createDownDepthUniform(u32 width, u32 height);
  static f32 gaussianDistribution(f32 x, f32 y, f32 z);
  static void getSampleOffsets_GaussBlur_Horizontally(f32 a, f32 b, int count, f32 *offsets, f32 c, f32 d);
  static void getSampleOffsets_GaussBlur_Verticaly(f32 a, f32 b, int count, f32 *offsets, f32 c, f32 d);

  static u32 s_nUniformBlockSize[18];
  static f32 s_fPlaneSideRot[2];
};

static_assert(sizeof(CloudUniform) == 0x38, "");

} // namespace cloud
