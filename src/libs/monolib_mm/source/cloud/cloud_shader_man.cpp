#include "cloud/cloud_shader_man.h"

#include "grlib/cglib_ctl_shader.h"
#include "grlib/cglib_ctl_texture.h"
#include "mtl/mem_manager.h"

namespace cloud {

// Dispatches a shader type to the function that binds its decoded shader; indexed by
// CloudShaderType.
CloudShaderMan::unknown001 CloudShaderMan::s_CallSelect[16] = {
    &CloudShaderMan::selectRenderPlane,
    &CloudShaderMan::selectDebug,
    &CloudShaderMan::selectColor,
    &CloudShaderMan::selectParticles,
    &CloudShaderMan::selectColor,
    &CloudShaderMan::selectShadow,
    &CloudShaderMan::selectRenderSky,
    &CloudShaderMan::selectMdlSphere,
    &CloudShaderMan::selectDetailMap,
    &CloudShaderMan::selectGlbFog,
    &CloudShaderMan::selectOutput,
    &CloudShaderMan::selectDownDepth,
    &CloudShaderMan::selectBlur,
    &CloudShaderMan::selectCounter,
    &CloudShaderMan::selectPlnDepth,
    &CloudShaderMan::selectUpScale,
};

// Vertex fetch of each shader type; evidence: the value is the index into unknown68 used by every
// select function.
u32 CloudShaderMan::s_reShdFtxTbl[16] = {2, 1, 1, 0, 0, 1, 1, 2, 1, 2, 2, 1, 1, 1, 2, 1};

CloudShaderMan::CloudShaderMan() {
  unknown00 = nullptr;
  unknown60 = true;
}

CloudShaderMan::~CloudShaderMan() { releaseShader(); }

void CloudShaderMan::releaseShader() {
  unknown08.release();
  if (unknown00 != nullptr) {
    mtl::MemManager::deallocate(unknown00, 0, false);
    unknown00 = nullptr;
  }
}

void CloudShaderMan::update() {}
void CloudShaderMan::setTexture(u32 index, const grlib::CGLibTextureBuffer &texture, CloudSamplerType sampler) { grlib::CGLibCtlTexture::setTexture(texture, unknowna8[sampler], index); }

void CloudShaderMan::setShader(CloudShaderType type, u32 a, u32 b, u32 c) { (this->*s_CallSelect[type])(type, a, b, c); }

grlib::CGLibDecShader *CloudShaderMan::selectRenderPlane(CloudShaderType type, u32 a, u32 b, u32 c) {
  grlib::CGLibDecShader *shader = &unknown08.unknown40[type];
  u32 levelNum = shader->getDeclarationOfs("CLD_LV_NUM");
  u32 holeMode = shader->getDeclarationOfs("HOLE_MODE");
  u32 blendMode = shader->getDeclarationOfs("BLND_MODE");
  shader->setDeclarationIdx(levelNum, a);
  shader->setDeclarationIdx(holeMode, b);
  shader->setDeclarationIdx(blendMode, c);
  grlib::CGLibCtlShader::setShader(*shader, unknown68[s_reShdFtxTbl[type]]);
  return shader;
}

grlib::CGLibDecShader *CloudShaderMan::selectDebug(CloudShaderType type, u32 a, u32 b, u32 c) {
  grlib::CGLibDecShader *shader = &unknown08.unknown40[type];
  shader->setDeclarationIdx(shader->getDeclarationOfs("DEBUG_MODE"), a);
  grlib::CGLibCtlShader::setShader(*shader, unknown68[s_reShdFtxTbl[type]]);
  return shader;
}

grlib::CGLibDecShader *CloudShaderMan::selectColor(CloudShaderType type, u32 a, u32 b, u32 c) {
  grlib::CGLibDecShader *shader = &unknown08.unknown40[type];
  grlib::CGLibCtlShader::setShader(*shader, unknown68[s_reShdFtxTbl[type]]);
  return shader;
}

grlib::CGLibDecShader *CloudShaderMan::selectParticles(CloudShaderType type, u32 a, u32 b, u32 c) {
  grlib::CGLibDecShader *shader = &unknown08.unknown40[type];
  grlib::CGLibCtlShader::setComputeShader(*shader);
  return shader;
}

grlib::CGLibDecShader *CloudShaderMan::selectShadow(CloudShaderType type, u32 a, u32 b, u32 c) {
  grlib::CGLibDecShader *shader = &unknown08.unknown40[type];
  shader->setDeclarationIdx(shader->getDeclarationOfs("CLD_SHADOW_ON"), a);
  grlib::CGLibCtlShader::setShader(*shader, unknown68[s_reShdFtxTbl[type]]);
  return shader;
}

grlib::CGLibDecShader *CloudShaderMan::selectRenderSky(CloudShaderType type, u32 a, u32 b, u32 c) {
  grlib::CGLibDecShader *shader = &unknown08.unknown40[type];
  shader->setDeclarationIdx(shader->getDeclarationOfs("CLD_LV_NUM"), a);
  grlib::CGLibCtlShader::setShader(*shader, unknown68[s_reShdFtxTbl[type]]);
  return shader;
}

grlib::CGLibDecShader *CloudShaderMan::selectMdlSphere(CloudShaderType type, u32 a, u32 b, u32 c) {
  grlib::CGLibDecShader *shader = &unknown08.unknown40[type];
  grlib::CGLibCtlShader::setShader(*shader, unknown68[s_reShdFtxTbl[type]]);
  return shader;
}

grlib::CGLibDecShader *CloudShaderMan::selectDetailMap(CloudShaderType type, u32 a, u32 b, u32 c) {
  grlib::CGLibDecShader *shader = &unknown08.unknown40[type];
  grlib::CGLibCtlShader::setShader(*shader, unknown68[s_reShdFtxTbl[type]]);
  return shader;
}

grlib::CGLibDecShader *CloudShaderMan::selectGlbFog(CloudShaderType type, u32 a, u32 b, u32 c) {
  grlib::CGLibDecShader *shader = &unknown08.unknown40[type];
  grlib::CGLibCtlShader::setShader(*shader, unknown68[s_reShdFtxTbl[type]]);
  return shader;
}

grlib::CGLibDecShader *CloudShaderMan::selectOutput(CloudShaderType type, u32 a, u32 b, u32 c) {
  grlib::CGLibDecShader *shader = &unknown08.unknown40[type];
  u32 outMode = shader->getDeclarationOfs("OUT_MODE");
  shader->setDeclarationIdx(outMode, a);
  u32 earlyZ = shader->getDeclarationOfs("EARLY_Z_ON");
  shader->setDeclarationIdx(earlyZ, b);
  // Inferred role: vertex fetch, mode 3 uses a different one; original table name unknown.
  grlib::CGLibCtlShader::setShader(*shader, unknown68[a == 3 ? 1 : 2]);
  return shader;
}

grlib::CGLibDecShader *CloudShaderMan::selectDownDepth(CloudShaderType type, u32 a, u32 b, u32 c) {
  grlib::CGLibDecShader *shader = &unknown08.unknown40[type];
  shader->setDeclarationIdx(shader->getDeclarationOfs("DONW_SAMPLE_TYPE"), a);
  grlib::CGLibCtlShader::setShader(*shader, unknown68[s_reShdFtxTbl[type]]);
  return shader;
}

grlib::CGLibDecShader *CloudShaderMan::selectBlur(CloudShaderType type, u32 a, u32 b, u32 c) {
  grlib::CGLibDecShader *shader = &unknown08.unknown40[type];
  grlib::CGLibCtlShader::setShader(*shader, unknown68[s_reShdFtxTbl[type]]);
  return shader;
}

grlib::CGLibDecShader *CloudShaderMan::selectCounter(CloudShaderType type, u32 a, u32 b, u32 c) {
  grlib::CGLibDecShader *shader = &unknown08.unknown40[type];
  u32 calcMode = shader->getDeclarationOfs("CLD_MDL_CALC");
  shader->setDeclarationIdx(calcMode, a);
  // Inferred role: vertex fetch per counter mode; original name unknown. The game indexes the
  // 8-byte table with the sign-extended value of a - 1.
  static const u64 unknown002[4] = {1, 2, 1, 1};
  s32 index = static_cast<s32>(a) - 1;
  u64 fetch = 2;
  if (static_cast<u32>(index) <= 3) {
    fetch = unknown002[index];
  }
  grlib::CGLibCtlShader::setShader(*shader, unknown68[fetch]);
  return shader;
}

grlib::CGLibDecShader *CloudShaderMan::selectPlnDepth(CloudShaderType type, u32 a, u32 b, u32 c) {
  grlib::CGLibDecShader *shader = &unknown08.unknown40[type];
  grlib::CGLibCtlShader::setShader(*shader, unknown68[s_reShdFtxTbl[type]]);
  return shader;
}

grlib::CGLibDecShader *CloudShaderMan::selectUpScale(CloudShaderType type, u32 a, u32 b, u32 c) {
  grlib::CGLibDecShader *shader = &unknown08.unknown40[type];
  grlib::CGLibCtlShader::setShader(*shader, unknown68[s_reShdFtxTbl[type]]);
  return shader;
}

} // namespace cloud
