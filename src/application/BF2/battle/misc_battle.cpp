#include "battle/misc_battle.h"

#include <math.h>

#include "battle/utility_battle.h"
#include "bdat.h"
#include "fw/fw_coli_check.h"
#include "fw/fw_model_object.h"
#include "fw/fw_transform.h"
#include "gf/data/gf_bdat_id.h"
#include "gf/data/gf_data_bdat.h"
#include "gf/game/gf_game_party.h"
#include "gf/object/component/model/gf_com_model.h"
#include "gf/object/gf_obj.h"
#include "gf/object/gf_obj_acc.h"
#include "gf/object/gf_obj_handle.h"
#include "gf/object/gf_obj_util.h"
#include "ml/rand.h"
#include "mm_math/mm_math.h"
#include "mm_math/mm_pnt.h"
#include "mm_math/mm_quat.h"
#include "mm_math/mm_vec3.h"
#include "scn/scn_obj_acc.h"

namespace btl {

constexpr f32 NORMALIZE_LENGTH_MIN = 0.000001f;
constexpr int KIZUNA_STAGE_TO_ID = 7;

void Misc::GetCrossDirection(const mm::Vec3 &direction, mm::Vec3 &cross, CROSS_MODE mode) {
  f32 z = direction.z;
  f32 x = direction.x;
  cross.x = mode == CROSS_MODE_LEFT ? -z : z;
  cross.z = mode == CROSS_MODE_LEFT ? x : -x;
  cross.y = 0.0f;
}

bool Misc::CalcDirection(const mm::Vec3 &from, const mm::Vec3 &to, mm::Vec3 &direction) {
  mm::Vec3 vector;
  vector.x = to.x - from.x;
  vector.y = to.y - from.y;
  vector.z = to.z - from.z;
  if (CalcLength(from, to) <= NORMALIZE_LENGTH_MIN) {
    direction = mm::Vec3::zero;
    return false;
  }

  Normalize(vector);
  direction = vector;
  return true;
}

void Misc::GetPostureDirection(const mm::Quat &rotation, mm::Vec3 &direction) {
  const mm::Vec3 &forward = mm::Vec3::unitZ;
  f32 tx = rotation.y * forward.z - rotation.z * forward.y + rotation.w * forward.x;
  f32 ty = rotation.z * forward.x - forward.z * rotation.x + forward.y * rotation.w;
  f32 tz = forward.y * rotation.x - rotation.y * forward.x + forward.z * rotation.w;
  f32 tw = -forward.x * rotation.x - rotation.y * forward.y - forward.z * rotation.z;
  direction.x = -(rotation.z * ty) + rotation.y * tz - rotation.x * tw + rotation.w * tx;
  direction.y = -(rotation.x * tz) + rotation.z * tx - rotation.y * tw + rotation.w * ty;
  direction.z = -(rotation.y * tx) + rotation.x * ty - rotation.z * tw + rotation.w * tz;
}

f32 Misc::CalcAngle(const mm::Vec3 &from, const mm::Vec3 &to, bool isSigned) {
  mm::Vec3 left = from;
  mm::Vec3 right = to;
  if (!(fabsf(sqrtf(left.x * left.x + left.y * left.y + left.z * left.z) - 1.0f) <= NORMALIZE_LENGTH_MIN)) {
    Normalize(left);
  }
  if (!(fabsf(sqrtf(right.x * right.x + right.y * right.y + right.z * right.z) - 1.0f) <= NORMALIZE_LENGTH_MIN)) {
    Normalize(right);
  }
  f32 dot = left.x * right.x + left.y * right.y + left.z * right.z;
  f32 angle = mm::acos(mm::clamp(dot, -1.0f, 1.0f));
  if (isSigned) {
    f32 cross = left.z * right.x - left.x * right.z;
    return cross < 0.0f ? -angle : angle;
  }
  return angle;
}

f32 Misc::CalcRotY(const mm::Vec3 &direction) { return CalcAngle(mm::Vec3::unitZ, direction, true); }

f32 Misc::CalcSlipDirection(const mm::Vec3 &direction, mm::Vec3 &slip, f32 angle) {
  f32 result = CalcAngle(mm::Vec3::unitY, direction, false);
  if (result < angle) {
    return 0.0f;
  }

  f32 length = CalcLengthXZ(direction);
  f32 rate = direction.y / length;
  slip.x = rate * direction.x;
  slip.z = rate * direction.z;
  slip.y = direction.y < 0.0f ? length : -length;
  return result;
}

bool Misc::NormalizeXZ(mm::Vec3 &vector) {
  f32 length = sqrtf(vector.x * vector.x + vector.z * vector.z);
  if (length <= NORMALIZE_LENGTH_MIN) {
    vector = mm::Vec3::unitZ;
    return false;
  }

  f32 scale = 1.0f / length;
  vector.y = 0.0f;
  vector.x = scale * vector.x;
  vector.z = scale * vector.z;
  return true;
}

void Misc::Normalize(mm::Vec3 &vector) {
  f32 length = sqrtf(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
  if (length > NORMALIZE_LENGTH_MIN) {
    f32 scale = 1.0f / length;
    vector.x = scale * vector.x;
    vector.y = scale * vector.y;
    vector.z = scale * vector.z;
    return;
  }

  if (length == 0.0f) {
    vector = mm::Vec3::unitZ;
    return;
  }

  vector *= 1000000.0f;
  length = sqrtf(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
  if (length <= NORMALIZE_LENGTH_MIN) {
    vector = mm::Vec3::zero;
    return;
  }

  f32 scale = 1.0f / length;
  vector.x = scale * vector.x;
  vector.y = scale * vector.y;
  vector.z = scale * vector.z;
}

f32 Misc::CalcLength(const mm::Vec3 &from, const mm::Vec3 &to) {
  f32 x = to.x - from.x;
  f32 y = to.y - from.y;
  f32 z = to.z - from.z;
  return sqrtf(x * x + y * y + z * z);
}

f32 Misc::CalcSquareLength(const mm::Vec3 &from, const mm::Vec3 &to) {
  f32 x = to.x - from.x;
  f32 y = to.y - from.y;
  f32 z = to.z - from.z;
  return x * x + y * y + z * z;
}

f32 Misc::CalcLengthXZ(const mm::Vec3 &from, const mm::Vec3 &to) {
  f32 x = to.x - from.x;
  f32 z = to.z - from.z;
  return sqrtf(x * x + z * z);
}

f32 Misc::CalcSquareLengthXZ(const mm::Vec3 &from, const mm::Vec3 &to) {
  f32 x = to.x - from.x;
  f32 z = to.z - from.z;
  return x * x + z * z;
}

f32 Misc::CalcLengthXZ(const mm::Vec3 &vector) { return sqrtf(vector.x * vector.x + vector.z * vector.z); }

f32 Misc::CalcSquareLengthXZ(const mm::Vec3 &vector) { return vector.x * vector.x + vector.z * vector.z; }

bool Misc::CalcDirectionXZ(const mm::Vec3 &from, const mm::Vec3 &to, mm::Vec3 &direction) {
  direction.x = to.x - from.x;
  direction.z = to.z - from.z;
  return NormalizeXZ(direction);
}

f32 Misc::InnerProductXZ(const mm::Vec3 &left, const mm::Vec3 &right) { return left.x * right.x + left.z * right.z; }

f32 Misc::CalcOuterProductY(const mm::Vec3 &left, const mm::Vec3 &right) { return left.z * right.x - left.x * right.z; }

void Misc::RandomVector(mm::Vec3 &vector) {
  vector.x = 0.5f - ml::mtRandf1();
  vector.y = 0.5f - ml::mtRandf1();
  vector.z = 0.5f - ml::mtRandf1();
  Normalize(vector);
}

void Misc::RandomVectorXZ(mm::Vec3 &vector) {
  vector.x = 0.5f - ml::mtRandf1();
  vector.z = 0.5f - ml::mtRandf1();
  NormalizeXZ(vector);
}

bool Misc::GetObjectTransform(gf::GF_OBJ_HANDLE *handle, mm::Vec3 &transform, TRANS_TYPE type) {
  if (handle == gf::INVALID_HANDLE) {
    return false;
  }

  return GetObjectTransform(gf::GfObjUtil::getObj(handle), transform, type);
}

bool Misc::GetObjectDirection(const gf::GfObj *obj, mm::Vec3 &direction, bool isXZ) {
  if (!GetObjectTransform(obj, direction, TRANS_TYPE_DIRECTION)) {
    return false;
  }

  if (isXZ) {
    NormalizeXZ(direction);
  }

  return true;
}

bool Misc::GetNodePosition(gf::GF_OBJ_HANDLE *handle, const char *node, mm::Vec3 &position) {
  gf::GfObj *obj = gf::GfObjUtil::getObj(handle);
  if (obj == nullptr) {
    return false;
  }

  return GetNodePosition(obj, node, position);
}

bool Misc::GetNodePosition(gf::GfObj *obj, const char *node, mm::Vec3 &position) {
  gf::GfObjAcc acc(obj);
  int bone = acc.getBoneIdx(node);
  if (bone < 0) {
    return false;
  }

  fw::Transform transform;
  acc.getBoneTransform(transform, bone);
  position = transform.unknown00;
  return true;
}

int Misc::GetPartyID(const gf::GfObj *obj) {
  if (obj == nullptr) {
    return -1;
  }

  return GetPartyID(obj->unknownf8);
}

int Misc::GetPartyID(gf::GF_OBJ_HANDLE *handle) {
  if (!Utility::IsThirdCharacter(handle)) {
    return gf::GfGameParty::getPartyId(handle);
  }

  for (int i = 0; i < 3; i++) {
    gf::GF_OBJ_HANDLE *driver = gf::GfGameParty::getHandleDriver(i);
    if (driver == gf::INVALID_HANDLE) {
      continue;
    }
    gf::GfObjAcc acc(driver);
    if (acc.getBdatId() == 1) {
      return i;
    }
  }

  return -1;
}

void Misc::GetKizunaMinMax(int stage, int &minimum, int &maximum) {
  int id = CovertKizunaStageToID(stage);
  u8 *bdat = gf::GfDataBdat::getFP(gf::BDAT_BTL_KIZUNA);
  minimum = static_cast<u16>(Bdat::getValCheck(bdat, "KizunaMin", id, 2));
  maximum = static_cast<u16>(Bdat::getValCheck(bdat, "KizunaMax", id, 2));
}

int Misc::CovertKizunaStageToID(int stage) { return KIZUNA_STAGE_TO_ID - stage; }

bool Misc::CheckOnSegment2D(const mm::Pnt<f32> &point, const Misc::SEGMENT_2D &segment) {
  if (NORMALIZE_LENGTH_MIN < segment.unknown08.x - segment.unknown00.x) {
    f32 rate = (point.x - segment.unknown00.x) / (segment.unknown08.x - segment.unknown00.x);
    if (rate < 0.0f || 1.0f < rate) {
      return false;
    }
  } else {
    if (point.x != segment.unknown00.x) {
      return false;
    }
  }

  if (NORMALIZE_LENGTH_MIN < segment.unknown08.y - segment.unknown00.y) {
    f32 rate = (point.y - segment.unknown00.y) / (segment.unknown08.y - segment.unknown00.y);
    if (rate < 0.0f || 1.0f < rate) {
      return false;
    }
  } else {
    if (point.y != segment.unknown00.y) {
      return false;
    }
  }
  return true;
}

bool Misc::CheckInRange(const mm::Vec3 &position, const mm::Vec3 &target, f32 range, bool isXZ) {
  f32 length = isXZ ? CalcLengthXZ(position, target) : CalcLength(target, position);
  return length <= range;
}

bool Misc::GetObjectDirection(gf::GF_OBJ_HANDLE *handle, mm::Vec3 &direction, bool isXZ) {
  if (handle == gf::INVALID_HANDLE) {
    return false;
  }

  bool isFound = GetObjectTransform(gf::GfObjUtil::getObj(handle), direction, TRANS_TYPE_DIRECTION);
  if (isFound && isXZ) {
    NormalizeXZ(direction);
  }

  return isFound;
}

f32 Misc::GetLandHeight(const mm::Vec3 &position, f32 distance, bool isUnder) {
  mm::Vec3 pos = position;
  pos.y += distance;
  mm::Vec3 result;
  if (isUnder) {
    if (!fw::ColiCheck::getSnapPos(result, pos, distance + 15.0f, 3)) {
      return position.y - 3000.0f;
    }
    return result.y;
  }

  u32 mask = 0x80000060;
  f32 range = distance + 5.0f;
  if (fw::ColiCheck::getSnapPos(result, pos, range, mask)) {
    return result.y;
  }
  pos.y -= range;
  if (!fw::ColiCheck::getSnapPos(result, pos, 10.0f, mask)) {
    return position.y - 3000.0f;
  }

  return result.y;
}

f32 Misc::GetVolumeFadeAlphaRate(gf::GF_OBJ_HANDLE *handle) {
  f32 rate = 0.0f;
  gf::GfObj *obj = gf::GfObjUtil::getObj(handle);
  if (obj != nullptr) {
    gf::GfComModel *model = obj->getComponent<gf::GfComModel>();
    if (model != nullptr && model->unknown18 != nullptr) {
      ml::ScnObjAccVolumeFade fade(model->unknown18->getInterface());
      rate = 1.0f - fade.getRatio(0);
    }
  }
  return rate;
}

const char *Misc::GetArtsTypeName(int) { return ""; }

void Misc::TurnDirectionAngle(const mm::Vec3 &from, const mm::Vec3 &to, mm::Vec3 &direction, f32 angle, bool isRadian) {
  mm::Vec3 vector;
  vector.x = to.x - from.x;
  vector.y = to.y - from.y;
  vector.z = to.z - from.z;
  Normalize(vector);
  TurnDirectionAngle(direction, vector, angle, isRadian);
}

} // namespace btl
