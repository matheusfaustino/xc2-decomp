#include "cloud/cloud_calc_params.h"

#include "cloud/cloud_man.h"
#include "cloud/cloud_obj_man.h"
#include "cloud/cloud_obj_reduction.h"
#include "dr/dr_draw_work_info.h"
#include "grlib/cglib_color_ctl.h"
#include "grlib/cglib_depth_target.h"
#include "grlib/cglib_gpu_cache.h"
#include "grlib/cglib_render_target.h"
#include "grlib/cglib_state.h"
#include "grlib/shd_util.h"

namespace cloud {

CloudCalcParams::~CloudCalcParams() { unknown460.release(); }

// Same stores as CloudUniform::createFogUniform; the game keeps both functions as separate symbols.
void CloudCalcParams::parmUpdate() {
  CloudCalcParams *params = CloudMan::get()->unknown50->unknown38;
  const f32 *fog = params->unknown00->unknown1d4;
  f32 *buff = static_cast<f32 *>(unknown460.getBuff());
  grlib::ShdF32SetF32Array(buff, fog, 0x18);
  buff[0x18] = params->unknown194;
  buff[0x19] = 1.0f / params->unknown380;
  buff[0x1a] = params->unknown384;
  buff[0x1b] = 0.0f;
  unknown460.updateData();
}

const f32 *CloudCalcParams::getFogTbl() const { return unknown00->unknown1d4; }

f32 CloudCalcParams::getShadowOfs(u32 index) const { return unknown00->unknown4e0[index]; }

void *CloudCalcParams::getShadowTex(u32 index) { return unknown00->unknown90[index]; }

const mm::Mat44 &CloudCalcParams::getShadowMatrix(u32 index) const { return unknown00->unknown420[index]; }

void CloudCalcParams::setMainRenderTarget() { unknown420->setRenderTarget(*unknown438); }

void CloudCalcParams::end() { setMainRenderTarget(); }

void CloudCalcParams::setMainRenderTargetAndDepth(bool isDepth, bool isSecondDepth) {
  if (isSecondDepth) {
    unknown420->setRenderTarget(*unknown438);
  } else if (isDepth) {
    unknown420->setRenderTarget(*unknown438, *unknown440, *unknown458);
  } else {
    unknown420->setRenderTarget(*unknown438, *unknown440);
  }
}

void CloudCalcParams::setPlnReductionTarget(bool isDepth) {
  CloudObjReduction *reduction = CloudMan::get()->unknown50->unknown40;
  if (isDepth) {
    reduction->unknown70.setRenderTarget(*reduction->unknown1ae8.unknown120, reduction->unknown438, reduction->unknownbc8);
  } else {
    reduction->unknown70.setRenderTarget(reduction->unknown438, reduction->unknownbc8);
  }
}

void CloudCalcParams::setMdlReductionTarget() {
  CloudObjReduction *reduction = CloudMan::get()->unknown50->unknown40;
  grlib::CGLibRenderTarget &target = reduction->unknown1358;
  grlib::LibMainStateViewPort viewPort(0.0f, 0.0f, static_cast<f32>(target.unknownbc), static_cast<f32>(target.unknownc0), 0.0f, 1.0f);
  grlib::CGLibState::setViewport(viewPort);
  grlib::CGLibState::beginRegisterRenderTarget();
  grlib::CGLibState::setRegisterRenderTarget(target, grlib::GRL_MRT_TYPE_0);
  grlib::CGLibState::setRegisterDepthTarget(*reduction->unknown22e8.unknown120);
  grlib::CGLibState::endRegisterRenderTarget();
}

void CloudCalcParams::setSkyReductionTarget() {
  CloudObjReduction *reduction = CloudMan::get()->unknown50->unknown40;
  reduction->unknownf90.setRenderTarget(*reduction->unknown1ae8.unknown120);
}

void CloudCalcParams::setMdlZBuffReductionTarget(bool isSky) {
  CloudObjReduction *reduction = CloudMan::get()->unknown50->unknown40;
  if (isSky) {
    reduction->unknown1ae8.setRenderTarget();
  } else {
    reduction->unknown22e8.setRenderTarget();
  }
}

void CloudCalcParams::beginTile(CldTileType type) {
  unknown001 &state = unknown49c[unknown498][type];
  if (state.unknown08) {
    grlib::CGLibGPUCache::setTiledCacheSize(state.unknown00, state.unknown04);
    grlib::CGLibGPUCache::setTiledCacheAction(GRL_TILED_CACHE_ACTION_UNKNOWN_1);
  }
}

void CloudCalcParams::endTile(CldTileType type) {
  unknown001 &state = unknown49c[unknown498][type];
  if (state.unknown08) {
    grlib::CGLibGPUCache::setTiledCacheAction(GRL_TILED_CACHE_ACTION_UNKNOWN_3);
    grlib::CGLibGPUCache::setTiledCacheAction(GRL_TILED_CACHE_ACTION_UNKNOWN_2);
  }
}

} // namespace cloud
