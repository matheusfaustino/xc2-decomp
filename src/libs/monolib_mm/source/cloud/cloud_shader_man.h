#pragma once
#include <stddef.h>

#include "grlib/cglib_dec_shader.h"
#include "grlib/cglib_sampler.h"
#include "grlib/cglib_shd_list.h"
#include "grlib/cglib_vtx_fetch.h"
#include "types.h"

namespace grlib {
class CGLibTextureBuffer;
} // namespace grlib

namespace cloud {

enum CloudShaderType {};
enum CloudSamplerType {};

class CloudShaderMan {
public:
  // Inferred role: member function pointer of the select functions; original type name unknown.
  typedef grlib::CGLibDecShader *(CloudShaderMan::*unknown001)(CloudShaderType type, u32 a, u32 b, u32 c);

  CloudShaderMan();
  ~CloudShaderMan();
  void createShader();
  void update();
  void releaseShader();
  void setTexture(u32 index, const grlib::CGLibTextureBuffer &texture, CloudSamplerType sampler);
  void setShader(CloudShaderType type, u32 a, u32 b, u32 c);

  // The select functions bind one decoded shader to a vertex fetch or, for selectParticles, to the
  // compute shader, and set the shader declarations whose indices come from the caller.
  grlib::CGLibDecShader *selectRenderPlane(CloudShaderType type, u32 a, u32 b, u32 c);
  grlib::CGLibDecShader *selectDebug(CloudShaderType type, u32 a, u32 b, u32 c);
  grlib::CGLibDecShader *selectColor(CloudShaderType type, u32 a, u32 b, u32 c);
  grlib::CGLibDecShader *selectParticles(CloudShaderType type, u32 a, u32 b, u32 c);
  grlib::CGLibDecShader *selectShadow(CloudShaderType type, u32 a, u32 b, u32 c);
  grlib::CGLibDecShader *selectRenderSky(CloudShaderType type, u32 a, u32 b, u32 c);
  grlib::CGLibDecShader *selectMdlSphere(CloudShaderType type, u32 a, u32 b, u32 c);
  grlib::CGLibDecShader *selectDetailMap(CloudShaderType type, u32 a, u32 b, u32 c);
  grlib::CGLibDecShader *selectGlbFog(CloudShaderType type, u32 a, u32 b, u32 c);
  grlib::CGLibDecShader *selectOutput(CloudShaderType type, u32 a, u32 b, u32 c);
  grlib::CGLibDecShader *selectDownDepth(CloudShaderType type, u32 a, u32 b, u32 c);
  grlib::CGLibDecShader *selectBlur(CloudShaderType type, u32 a, u32 b, u32 c);
  grlib::CGLibDecShader *selectCounter(CloudShaderType type, u32 a, u32 b, u32 c);
  grlib::CGLibDecShader *selectPlnDepth(CloudShaderType type, u32 a, u32 b, u32 c);
  grlib::CGLibDecShader *selectUpScale(CloudShaderType type, u32 a, u32 b, u32 c);

  static unknown001 s_CallSelect[16];
  static u32 s_reShdFtxTbl[16];

  void *unknown00;                   // Inferred role: shader buffer freed by the destructor; original member name unknown.
  grlib::CGLibShdListData unknown08; // Inferred role: decoded shader list; original member name unknown.
  bool unknown60;                    // Inferred role: is created; original member name unknown.
  u8 _61[0x68 - 0x61];
  grlib::CGLibVtxFetch unknown68[4]; // Inferred role: vertex fetches indexed by s_reShdFtxTbl; original member name unknown.
  grlib::CGLibSampler unknowna8[8];  // Inferred role: samplers indexed by CloudSamplerType; original member name unknown.
};

static_assert(offsetof(CloudShaderMan, unknown08) == 0x08, "");
static_assert(offsetof(CloudShaderMan, unknown60) == 0x60, "");
static_assert(offsetof(CloudShaderMan, unknown68) == 0x68, "");
static_assert(offsetof(CloudShaderMan, unknowna8) == 0xa8, "");
static_assert(sizeof(CloudShaderMan) == 0x1a8, "");

} // namespace cloud
