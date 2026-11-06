// Original file name from the game string C:/Monolithsoft/BF2/prog/libs/skdlib/src/skdlib/composite/skd_calc_util.cpp.
#include "anm/anm_array-inl.h"
#include "anm/anm_pose.h"
#include "anm/anm_qs_transform.h"
#include "anm/anm_skeleton.h"
#include "mm_math/mm_quat.h"
#include "mm_math/mm_vec3.h"
#include "mm_math/mm_vec4.h"
#include "mm_mtl/vector.h"
#include "skd/skd_dynamics.h"
#include "types.h"

namespace skd {

void CalcUtil::writeBackNode(anm::Pose &pose, NodeArray *nodes) {
  const anm::Skeleton *skeleton = pose.unknown18;
  u32 count = nodes->unknown10.size();
  for (u32 i = 0; i < count; i++) {
    Node &node = nodes->unknown10[i];
    u16 bone = node.unknowna0;
    node.unknown20 = node.unknown00;
    node.unknown00 = node.unknown50;
    u16 modelBone = skeleton->unknown20[bone];
    if (modelBone != anm::Skeleton::INVALID_BONE) {
      const anm::QsTransform &transform = pose.getBoneModelSpace(modelBone);
      node.unknown30 = mm::Vec3(transform.unknown00.x, transform.unknown00.y, transform.unknown00.z);
      node.unknown40 = transform.unknown10;
    }
  }
}

void CalcUtil::blendNode(f32 weight, NodeArray *nodes) {
  u32 count = nodes->unknown10.size();
  for (u32 i = 0; i < count; i++) {
    Node &node = nodes->unknown10[i];
    node.unknown50 = node.unknown50 * (1.0f - weight) + node.unknown70 * weight;
  }
}

void CalcUtil::applyStickConstraints(const anm::Array<ResStickConstraint> &constraints, NodeArray *nodes) {
  u32 count = constraints.size();
  for (u32 i = 0; i < count; i++) {
    const ResStickConstraint &constraint = constraints[i];
    limitCone(constraint, constraint.unknown10, nodes);
  }
  for (u32 i = 0; i < count; i++) {
    limitDistance(constraints[i], nodes);
  }
}

void CalcUtil::applyHingeConstraints(const anm::Array<ResHingeConstraint> &constraints, NodeArray *nodes) {
  u32 count = constraints.size();
  for (u32 i = 0; i < count; i++) {
    limitHinge(constraints[i], nodes);
  }
  for (u32 i = 0; i < count; i++) {
    limitDistance(constraints[i], nodes);
  }
}

} // namespace skd
