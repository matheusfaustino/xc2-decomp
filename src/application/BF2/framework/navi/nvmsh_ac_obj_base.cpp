#include "navi/nvmsh_ac_obj_base.h"

namespace navi {

const mm::Vec3 &NvmshAcObjBase::getMoveVec() const { return m_moveVec; }

void NvmshAcObjBase::setBeginPos(const mm::Vec3 &position) { m_beginPos = position; }

void NvmshAcObjBase::setEndPos(const mm::Vec3 &position) { m_endPos = position; }

void NvmshAcObjBase::setBeginRadius(f32 radius) { m_beginRadius = radius; }

void NvmshAcObjBase::setEndRadius(f32 radius) { m_endRadius = radius; }

void NvmshAcObjBase::setBeginSeachRadius(f32 radius) { m_beginSeachRadius = radius; }

void NvmshAcObjBase::setEndSeachRadius(f32 radius) { m_endSeachRadius = radius; }

void NvmshAcObjBase::setBeginHeight(f32 param1, f32 param2) {
  m_beginHeight[0] = param1;
  m_beginHeight[1] = param2;
}

void NvmshAcObjBase::setEndHeight(f32 param1, f32 param2) {
  m_endHeight[0] = param1;
  m_endHeight[1] = param2;
}

void NvmshAcObjBase::setMeshNum(int num) { unknown64 = num > 0; }

int NvmshAcObjBase::getMeshNum() const { return m_meshNum; }

void NvmshAcObjBase::setMoveSpd(f32 speed) { m_moveSpd = speed; }

void NvmshAcObjBase::rootReset() {
  unknown50 = 0;
  unknown52 = 0;
  unknown5e = 0;
  unknown54 = 0;
  unknown5a = 0;
  unknown5c = 0xffff;
}

void NvmshAcObjBase::conectReset() {
  unknown56 = 0;
  unknown58 = 0;
  unknown62 = 0;
}

void NvmshAcObjBase::setRootListBuff(u8 *buffer, int size) {
  unknown30 = buffer;
  unknown38 = size;
  unknown40 = buffer;
  unknown48 = buffer;
}

void NvmshAcObjBase::calcNextVec(const _NaviCalcResTbl *resTable, const _NaviTempColiResl &res) {
  if (unknown52 != unknown50) {
    calcRootMeshVec(resTable, res);
    return;
  }

  calcTargetVecLine(unknown110, unknown120);
}

void NvmshAcObjBase::resetCache() { unknown28 &= 0xff4fe3e0; }

} // namespace navi
