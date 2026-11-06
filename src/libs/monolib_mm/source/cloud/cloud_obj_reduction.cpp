#include "cloud/cloud_obj_reduction.h"

#include "cloud/cloud_uniform.h"
#include "mm_math/mm_col4.h"

namespace cloud {

namespace {
// Inferred role: clear mask covering every target; original name unknown.
const GrlClaerMask unknown001 = static_cast<GrlClaerMask>(0xf);
} // namespace

CloudObjReduction::CloudObjReduction() {
  unknown00.release();
  unknown00.setup(CloudUniform::s_nUniformBlockSize[UNKNOWN001_UNKNOWN_15]);
  unknown38.release();
  unknown38.setup(CloudUniform::s_nUniformBlockSize[UNKNOWN001_UNKNOWN_16]);
  unknown2b4c = -1;
  unknown2b48 = -1;
  unknown2b54 = false;
}

void CloudObjReduction::skyBufferClear() { unknownf90.clearTexture(mm::Col4(0.0f, 0.0f, 0.0f, 0.0f), unknown001); }

void CloudObjReduction::mdlBufferClear() { unknown1358.clearTexture(mm::Col4(0.0f, 0.0f, 0.0f, 0.0f), unknown001); }

void CloudObjReduction::planeBufferClear() {
  unknown438.clearTexture(mm::Col4(0.0f, 0.0f, 0.0f, 0.0f), unknown001);
  unknown800.clearTexture(mm::Col4(0.0f, 0.0f, 0.0f, 0.0f), unknown001);
  unknownbc8.clearTexture(mm::Col4(0.0f, 0.0f, 0.0f, 0.0f), unknown001);
}

} // namespace cloud
