#include "dr/dr_pixl_post_parm.h"

#include "dr/dr_calc_man.h"
#include "dr/dr_calc_view_work.h"
#include "dr/dr_man.h"
#include "dr/dr_pixl_man.h"
#include "dr/dr_pixl_post_man.h"

namespace ml {

bool DrPixlPostParm::isShwRaytrace() const { return m_shwRaytrace; }

void DrPixlPostParm::calcReductionTransBuffer(int &width, int &height) const {
  width = width * unknownfdc;
  height = height * unknownfe0;
}

void DrPixlPostParm::setDOF(bool dof) { unknown514[DrMan::s_nDblIdx].unknown221 = dof; }

bool DrPixlPostParm::getTMAAOn() const { return m_TMAAOn; }

bool DrPixlPostParm::isBlm2PassExtract() const { return unknownf72 && m_TMAAOn; }

bool DrPixlPostParm::isShwRaytraceTemporalWithOthers() const {
  if (!DrMan::getIns()->isIBLHalfRes()) {
    return false;
  }
  return unknown1024;
}

f32 DrPixlPostParm::getTMAASharpness(u32 index) const {
  if (DrMan::getIns()->unknown1c60 == 0) {
    return unknown12c[index];
  }
  return unknown114[index];
}

bool DrPixlPostParm::isMotBlur2() const { return m_motBlur2; }

bool DrPixlPostParm::isMotBlur() const { return unknown1ca || unknown1cb; }

bool DrPixlPostParm::getBloomOn() const { return m_bloomOn; }

bool DrPixlPostParm::getToneMapOn() const { return m_toneMapOn; }

void DrPixlPostParm::update() {
  if (unknown10fc) {
    unknown10fc = false;
  }
}

bool DrPixlPostParm::getAAOn() const { return m_AAOn; }

bool DrPixlPostParm::getColTblOn() const { return m_colTblOn; }

f32 DrPixlPostParm::getBorderBlmMul() const { return m_borderBlmMul; }

DrPixlPostParm *DrPixlPostParm::get() { return s_pIns; }

void DrPixlPostParm::setInst() { s_pIns = this; }

DrPixlPostParm::DrPixlPostParm() { reset(); }

DrPixlPostParm::~DrPixlPostParm() {}

DrPixlPostParm &DrPixlPostParm::operator=(const DrPixlPostParm &parm) {
  __builtin_memcpy(this, &parm, sizeof(DrPixlPostParm));
  return *this;
}

void DrPixlPostParm::calcOfsRenderCheck(f32 x, f32 y) { unknownfc8 = static_cast<u32>(x * 100.0f) | static_cast<u32>(y * 100.0f) << 8; }

void DrPixlPostParm::calcReductionBuffer(int &width, int &height) {
  if (unknownfb3) {
    width = static_cast<int>(width * unknownfc0);
    height = static_cast<int>(height * unknownfc4);
    width += -width & 7;
    height += -height & 7;
  }
}

void DrPixlPostParm::setOverrideToneMap(f32 overrideToneMap) {
  unknown039 &parm = unknown514[DrMan::s_nDblIdx];
  parm.unknown04 = true;
  parm.unknown08 = overrideToneMap;
}

f32 DrPixlPostParm::getToonDirLightScale() const {
  if (DrMan::getIns()->unknown1e08) {
    return unknown2a0;
  }
  return 1.0f;
}

void DrPixlPostParm::getRanDropLayerInfo(u32 layer, stRainDropLayerInfo &info) const {
  info.unknown00 = unknown40c[layer];
  info.unknown04 = unknown418[layer];
  info.unknown08 = unknown424[layer];
  info.unknown0c = unknown43c[layer];
  info.unknown10 = unknown430[layer];
}

void DrPixlPostParm::OnCloseAttriMenu() { unknown10fc = true; }

void DrPixlPostParm::resetScene() { unknown1105[DrMan::s_nDblStockIdx] = true; }

void DrPixlPostParm::setWeatherFrame(f32 weatherFrame) { unknown1088[DrMan::s_nDblStockIdx] = weatherFrame; }

f32 DrPixlPostParm::getWeatherFrame() const { return unknown1088[DrMan::s_nDblIdx]; }

void DrPixlPostParm::resetWeatherFrame() { unknown1088[DrMan::s_nDblIdx] = 60.0f; }

void DrPixlPostParm::writeParameter(const char *path, const char *name) {}

void DrPixlPostParm::readParameter(const char *path, bool isCreate, const char *name) {}

void DrPixlPostParm::updateParameter(const char *name) {}

void DrPixlPostParm::setOverrideMotBlur(bool overrideMotBlur) { unknown514[DrMan::s_nDblIdx].unknown204 = overrideMotBlur; }

void DrPixlPostParm::setMotBlur2(bool motBlur2) { unknown514[DrMan::s_nDblIdx].unknown205 = motBlur2; }

void DrPixlPostParm::setOverrideDOF(bool overrideDOF) { unknown514[DrMan::s_nDblIdx].unknown220 = overrideDOF; }

void DrPixlPostParm::setDOFHlv(bool dofHlv) { unknown514[DrMan::s_nDblIdx].unknown223 = dofHlv; }

void DrPixlPostParm::setDOFDownSec(bool dofDownSec) { unknown514[DrMan::s_nDblIdx].unknown224 = dofDownSec; }

void DrPixlPostParm::setDOFAutoFocus(bool dofAutoFocus) { unknown514[DrMan::s_nDblIdx].unknown225 = dofAutoFocus; }

void DrPixlPostParm::setDOFAutoFocusDebug(bool dofAutoFocusDebug) { unknown21e = dofAutoFocusDebug; }

void DrPixlPostParm::setDoFPixlRatio(f32 dofPixlRatio) { unknown514[DrMan::s_nDblIdx].unknown228 = dofPixlRatio; }

void DrPixlPostParm::setDoFDistance(f32 dofDistance) { unknown514[DrMan::s_nDblIdx].unknown22c = dofDistance; }

void DrPixlPostParm::setDoFRange(f32 dofRange) { unknown514[DrMan::s_nDblIdx].unknown230 = dofRange; }

void DrPixlPostParm::setDoFStrength(f32 dofStrength) { unknown514[DrMan::s_nDblIdx].unknown234 = dofStrength; }

void DrPixlPostParm::setDoFLens(f32 dofLens) { unknown514[DrMan::s_nDblIdx].unknown238 = dofLens; }

void DrPixlPostParm::setDoFBlend(f32 dofBlend) { unknown514[DrMan::s_nDblIdx].unknown23c = dofBlend; }

void DrPixlPostParm::setDoFPixlLv(s32 dofPixlLv) { unknown514[DrMan::s_nDblIdx].unknown240 = dofPixlLv; }

void DrPixlPostParm::setDoFocusNum(s32 dofFocusNum) { unknown514[DrMan::s_nDblIdx].unknown244 = dofFocusNum; }

void DrPixlPostParm::setDoFDofFocusRat(f32 dofFocusRatio) { unknown514[DrMan::s_nDblIdx].unknown248 = dofFocusRatio; }

void DrPixlPostParm::setDofFocusMin(f32 dofFocusMin) { unknown514[DrMan::s_nDblIdx].unknown24c = dofFocusMin; }

bool DrPixlPostParm::isOverrideDOF() const { return unknown514[DrMan::s_nDblIdx].unknown220; }

bool DrPixlPostParm::isOverrideColorFilter() const { return unknown514[DrMan::s_nDblIdx].unknown272; }

void DrPixlPostParm::setOverrideColorFilter(bool overrideColorFilter) { unknown514[DrMan::s_nDblIdx].unknown272 = overrideColorFilter; }

void DrPixlPostParm::setColorFilterNum(s32 colorFilterNum) { unknown514[DrMan::s_nDblIdx].unknown274 = colorFilterNum; }

void DrPixlPostParm::setColorFilterFrm(f32 colorFilterFrm) { unknown514[DrMan::s_nDblIdx].unknown278 = colorFilterFrm; }

void DrPixlPostParm::setColorFilterTransZ(bool colorFilterTransZ) { this->unknown274 = colorFilterTransZ; }

void DrPixlPostParm::setColorFilterFarNum(s32 colorFilterFarNum) { unknown514[DrMan::s_nDblIdx].unknown280 = colorFilterFarNum; }

bool DrPixlPostParm::getRainDropOn() const { return m_rainDropOn; }

bool DrPixlPostParm::getSSRWithTrans() const { return !DrMan::getIns()->unknown1cc1; }

bool DrPixlPostParm::getSSROn() const {
  if (DrMan::getIns()->unknown78->unknown7f0->unknown180->unknown108 < 0.01f) {
    return false;
  }
  if (DrMan::getIns()->unknown580->unknown10->unknown9aa && unknown1104) {
    return false;
  }
  return unknown48a != 0;
}

} // namespace ml
