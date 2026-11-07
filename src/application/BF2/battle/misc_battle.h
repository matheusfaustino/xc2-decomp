#pragma once
#include "mm_math/mm_pnt.h"
#include "types.h"

namespace mm {
struct Quat;
struct Vec3;
} // namespace mm

namespace gf {
struct GF_OBJ_HANDLE;
class GfObj;
} // namespace gf

namespace btl {

class BattleCharacter;

inline f32 NextFloat(f32 value) {
  u32 bits = *reinterpret_cast<u32 *>(&value) + 1;
  return *reinterpret_cast<f32 *>(&bits);
}

class Misc {
public:
  enum CROSS_MODE {
    CROSS_MODE_LEFT,
    CROSS_MODE_RIGHT,
  };

  enum TRANS_TYPE {
    TRANS_TYPE_POSITION,
    TRANS_TYPE_DIRECTION,
  };

  struct SEGMENT_2D {
    mm::Pnt<f32> unknown00; // Inferred role: start point; original member name unknown.
    mm::Pnt<f32> unknown08; // Inferred role: end point; original member name unknown.
  };

  static void Normalize(mm::Vec3 &vector);
  static void TurnDirectionAngle(mm::Vec3 &direction, const mm::Vec3 &axis, f32 angle, bool isRadian);
  static void TurnDirectionAngle(const mm::Vec3 &from, const mm::Vec3 &to, mm::Vec3 &direction, f32 angle, bool isRadian);
  static void GetCrossDirection(const mm::Vec3 &direction, mm::Vec3 &cross, CROSS_MODE mode);
  static void GetPostureDirection(const mm::Quat &rotation, mm::Vec3 &direction);
  static bool CalcDirection(const mm::Vec3 &from, const mm::Vec3 &to, mm::Vec3 &direction);
  static f32 CalcAngle(const mm::Vec3 &from, const mm::Vec3 &to, bool isSigned);
  static f32 CalcRotY(const mm::Vec3 &direction);
  static f32 CalcSlipDirection(const mm::Vec3 &direction, mm::Vec3 &slip, f32 angle);
  static bool NormalizeXZ(mm::Vec3 &vector);
  static f32 CalcLength(const mm::Vec3 &from, const mm::Vec3 &to);
  static f32 CalcSquareLength(const mm::Vec3 &from, const mm::Vec3 &to);
  static f32 CalcLengthXZ(const mm::Vec3 &from, const mm::Vec3 &to);
  static f32 CalcSquareLengthXZ(const mm::Vec3 &from, const mm::Vec3 &to);
  static f32 CalcLengthXZ(const mm::Vec3 &vector);
  static f32 CalcSquareLengthXZ(const mm::Vec3 &vector);
  static bool CalcDirectionXZ(const mm::Vec3 &from, const mm::Vec3 &to, mm::Vec3 &direction);
  static f32 InnerProductXZ(const mm::Vec3 &left, const mm::Vec3 &right);
  static f32 CalcOuterProductY(const mm::Vec3 &left, const mm::Vec3 &right);
  static void RandomVector(mm::Vec3 &vector);
  static void RandomVectorXZ(mm::Vec3 &vector);
  static bool GetObjectTransform(gf::GF_OBJ_HANDLE *handle, mm::Vec3 &transform, TRANS_TYPE type);
  static bool GetObjectTransform(const gf::GfObj *obj, mm::Vec3 &transform, TRANS_TYPE type);
  static bool GetObjectDirection(gf::GF_OBJ_HANDLE *handle, mm::Vec3 &direction, bool isXZ);
  static bool GetObjectDirection(const gf::GfObj *obj, mm::Vec3 &direction, bool isXZ);
  static bool GetNodePosition(gf::GF_OBJ_HANDLE *handle, const char *node, mm::Vec3 &position);
  static bool GetNodePosition(gf::GfObj *obj, const char *node, mm::Vec3 &position);
  static int GetPartyID(gf::GF_OBJ_HANDLE *handle);
  static int GetPartyID(const gf::GfObj *obj);
  static void GetKizunaMinMax(int stage, int &minimum, int &maximum);
  static int CovertKizunaStageToID(int stage);
  static int GetKizunaStage(int kizuna, f32 *rate);
  static bool CheckOnSegment2D(const mm::Pnt<f32> &point, const SEGMENT_2D &segment);
  static bool CheckInRange(const mm::Vec3 &position, const mm::Vec3 &target, f32 range, bool isXZ);
  static bool CheckInRange(const mm::Vec3 &position, BattleCharacter *character, f32 range, bool isXZ);
  static f32 GetLandHeight(const mm::Vec3 &position, f32 distance, bool isUnder);
  static f32 GetVolumeFadeAlphaRate(gf::GF_OBJ_HANDLE *handle);
  static bool IsInWater(const mm::Vec3 &position, f32 depth);
  static const char *GetArtsTypeName(int type);
};

static_assert(sizeof(Misc::SEGMENT_2D) == 0x10, "");

} // namespace btl
