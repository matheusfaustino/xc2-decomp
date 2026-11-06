#include "cloud/cloud_obj_reduction.h"

#include "cloud/cloud_uniform.h"
#include "mm_math/mm_col4.h"

namespace cloud {

CloudObjReduction::CloudObjReduction() {
  unknown00.release();
  unknown00.setup(CloudUniform::s_nUniformBlockSize[15]);
  unknown38.release();
  unknown38.setup(CloudUniform::s_nUniformBlockSize[16]);
  unknown2b4c = -1;
  unknown2b48 = -1;
  unknown2b54 = false;
}

void CloudObjReduction::skyBufferClear() { unknownf90.clearTexture(mm::Col4(0.0f, 0.0f, 0.0f, 0.0f), static_cast<GrlClaerMask>(0xf)); }

void CloudObjReduction::mdlBufferClear() { unknown1358.clearTexture(mm::Col4(0.0f, 0.0f, 0.0f, 0.0f), static_cast<GrlClaerMask>(0xf)); }

void CloudObjReduction::planeBufferClear() {
  unknown438.clearTexture(mm::Col4(0.0f, 0.0f, 0.0f, 0.0f), static_cast<GrlClaerMask>(0xf));
  unknown800.clearTexture(mm::Col4(0.0f, 0.0f, 0.0f, 0.0f), static_cast<GrlClaerMask>(0xf));
  unknownbc8.clearTexture(mm::Col4(0.0f, 0.0f, 0.0f, 0.0f), static_cast<GrlClaerMask>(0xf));
}

} // namespace cloud
