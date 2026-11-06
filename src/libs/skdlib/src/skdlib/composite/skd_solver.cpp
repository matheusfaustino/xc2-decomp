// Original file name from the game string C:/Monolithsoft/BF2/prog/libs/skdlib/src/skdlib/composite/skd_solver.cpp.
#include "anm/anm_array-inl.h"
#include "mm_math/mm_vec3.h"
#include "mm_mtl/vector.h"
#include "skd/skd_dynamics.h"
#include "skd/skd_packet.h"
#include "types.h"

namespace anm {
struct QsTransform;
} // namespace anm
namespace mm {
namespace mtl {
class IAllocator;
} // namespace mtl
} // namespace mm

namespace skd {

DynamicsSolver::DynamicsSolver(mm::mtl::IAllocator *allocator, const ResSolver &resSolver, u32 index)
    : m_pResSolver(&resSolver), unknown10(allocator, resSolver.unknown08, resSolver.unknown18), unknown48(1.0f / 30.0f), unknown50(allocator), unknown78(index), m_solverBlendWeight(0.0f),
      unknown80(false) {
  unknown50.resize(resSolver.unknown08.size(), mm::Vec3::zero);
}

void DynamicsSolver::init(const Packet &packet) { unknown80 = CalcUtil::initNodes(*packet.m_pBindPose, &unknown10); }

void DynamicsSolver::convertCoordinateSystem(const anm::QsTransform &transform) { CalcUtil::convertCoordinateSystem(transform, &unknown10); }

void DynamicsSolver::resetNodes(const Packet &packet) { CalcUtil::initNodes(*packet.m_pBindPose, &unknown10); }

} // namespace skd
