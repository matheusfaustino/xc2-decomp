#pragma once
#include <stddef.h>

#include "anm/anm_array-inl.h"
#include "anm/anm_job.h"
#include "anm/anm_reference_object.h"
#include "mm_math/mm_quat.h"
#include "mm_math/mm_vec3.h"
#include "mm_math/mm_vec4.h"
#include "mm_mtl/vector.h"
#include "skd/skd_packet.h"
#include "types.h"

namespace anm {
class Pose;
class ThreadPool;
class XmlInterface;
class JobQueue;
struct QsTransform;
} // namespace anm

namespace mm {
namespace mtl {
class IAllocator;
} // namespace mtl
} // namespace mm

namespace skd {

class CollionArray;
class CollisionObject;
class ForceInterface;
struct ResHingeConstraint;
struct ResNode;
struct ResNodeCollision;
struct ResSpringConstraint;
struct ResStickConstraint;
struct DetectInfo;
struct DetectInfoBuffer;

struct ResSolver {
  const char *unknown00;                     // Inferred role: name; original member name unknown.
  anm::Array<ResNode> unknown08;             // Inferred role: nodes; original member name unknown.
  anm::Array<ResNodeCollision> unknown18;    // Inferred role: node collisions; original member name unknown.
  anm::Array<ResStickConstraint> unknown28;  // Inferred role: stick constraints; original member name unknown.
  anm::Array<ResSpringConstraint> unknown38; // Inferred role: spring constraints; original member name unknown.
  anm::Array<ResHingeConstraint> unknown48;  // Inferred role: hinge constraints; original member name unknown.
};

static_assert(offsetof(ResSolver, unknown08) == 0x08, "");
static_assert(offsetof(ResSolver, unknown18) == 0x18, "");
static_assert(offsetof(ResSolver, unknown28) == 0x28, "");
static_assert(offsetof(ResSolver, unknown38) == 0x38, "");
static_assert(offsetof(ResSolver, unknown48) == 0x48, "");
static_assert(sizeof(ResSolver) == 0x58, "");

// Layout evidence for the resource structs below: setResCollision (0x710097570c), setResConstraint (0x7100976094) and
// DynamicsImporter::importXML (0x7100976704) fill them, and the copies of their Vector push_back and expand show which
// members are copied one by one (the Vec4 members) and which are copied as raw bytes.

// Collision shape read from the dynamics xml; the shapes derive from it (setResCollision takes a ResCollision *).
struct ResCollision {
  u32 unknown00;         // Inferred role: shape type (0 sphere, 1 capsule, 2 plane); original member name unknown.
  const char *unknown08; // Inferred role: name; original member name unknown.
  const char *unknown10; // Inferred role: bound joint name ("bind_joint"); original member name unknown.
  u32 unknown18;         // Inferred role: collision type ("col_type"); original member name unknown.
  mm::Vec4 unknown20;    // Inferred role: local translation, w = 1; original member name unknown.
  mm::Quat unknown30;    // Inferred role: local rotation; original member name unknown.
};

static_assert(offsetof(ResCollision, unknown08) == 0x08, "");
static_assert(offsetof(ResCollision, unknown10) == 0x10, "");
static_assert(offsetof(ResCollision, unknown18) == 0x18, "");
static_assert(offsetof(ResCollision, unknown20) == 0x20, "");
static_assert(offsetof(ResCollision, unknown30) == 0x30, "");
static_assert(sizeof(ResCollision) == 0x40, "");

struct ResCollisionShpere : ResCollision {
  f32 unknown40; // Inferred role: radius; original member name unknown.
};

static_assert(offsetof(ResCollisionShpere, unknown40) == 0x40, "");
static_assert(sizeof(ResCollisionShpere) == 0x50, "");

struct ResCollisionCapsule : ResCollision {
  f32 unknown40; // Inferred role: radius; original member name unknown.
  f32 unknown44; // Inferred role: height; original member name unknown.
};

static_assert(offsetof(ResCollisionCapsule, unknown40) == 0x40, "");
static_assert(offsetof(ResCollisionCapsule, unknown44) == 0x44, "");
static_assert(sizeof(ResCollisionCapsule) == 0x50, "");

struct ResCollisionPlane : ResCollision {};

static_assert(sizeof(ResCollisionPlane) == 0x40, "");

// Layout evidence: CollionArray::CollionArray (0x710097bb08) reads the element count at 0x8 and walks the array at 0x0
// with a stride of 0x50, the size of the largest collision shape.
struct ResCollisionArray {
  const u8 *unknown00; // Inferred role: collision shapes, 0x50 bytes each; original member name unknown.
  u32 unknown08;       // Inferred role: shape count; original member name unknown.
  u8 _0c[0x10 - 0x0c];
};

static_assert(offsetof(ResCollisionArray, unknown08) == 0x08, "");

struct ResNode {
  const char *unknown00; // Inferred role: name; original member name unknown.
  const char *unknown08; // Inferred role: bound joint name ("bind_joint"); original member name unknown.
  f32 unknown10;         // Inferred role: model space restore rate ("restoreMS"); original member name unknown.
  f32 unknown14;         // Inferred role: local space restore rate ("restoreLS"); original member name unknown.
  f32 unknown18;         // Inferred role: damping ("damping"); original member name unknown.
  f32 unknown1c;         // Inferred role: power gain ("powergain"); original member name unknown.
  f32 unknown20;         // Inferred role: float attribute of the node; original member name unknown.
  u16 unknown24;         // Inferred role: flag set from an integer attribute; original member name unknown.
};

static_assert(offsetof(ResNode, unknown20) == 0x20, "");
static_assert(offsetof(ResNode, unknown24) == 0x24, "");
static_assert(sizeof(ResNode) == 0x28, "");

struct ResNodeCollision {
  u16 unknown00; // Inferred role: first node index; original member name unknown.
  u16 unknown02; // Inferred role: second node index; original member name unknown.
  f32 unknown04; // Inferred role: position ratio between the two nodes; original member name unknown.
  f32 unknown08; // Inferred role: radius ("radius"); original member name unknown.
  u32 unknown0c; // Inferred role: attribute copied from the constraint xml; original member name unknown.
};

static_assert(offsetof(ResNodeCollision, unknown0c) == 0x0c, "");
static_assert(sizeof(ResNodeCollision) == 0x10, "");

// Constraint between two nodes; the constraints derive from it (setResConstraint takes a ResConstraint *).
struct ResConstraint {
  const char *unknown00; // Inferred role: name; original member name unknown.
  f32 unknown08;         // Inferred role: distance ("distance"); original member name unknown.
  u16 unknown0c;         // Inferred role: first node index ("bind_node0"); original member name unknown.
  u16 unknown0e;         // Inferred role: second node index ("bind_node1"); original member name unknown.
};

static_assert(offsetof(ResConstraint, unknown0c) == 0x0c, "");
static_assert(offsetof(ResConstraint, unknown0e) == 0x0e, "");
static_assert(sizeof(ResConstraint) == 0x10, "");

// Angle limit; the name comes from skd::limitCone(ResConstraint const &, ResConeLimit const &, NodeArray *), which
// applyStickConstraints calls with the member at 0x10 of each ResStickConstraint. The 8-byte copy at 0x20 in
// Vector<ResHingeConstraint>::push_back shows the hinge constraint holds one too.
struct ResConeLimit {
  f32 unknown00;  // Inferred role: limit angle in radians ("angle"); original member name unknown.
  bool unknown04; // Inferred role: limit enabled ("use_limit"); original member name unknown.
};

static_assert(offsetof(ResConeLimit, unknown04) == 0x04, "");
static_assert(sizeof(ResConeLimit) == 0x08, "");

struct ResStickConstraint : ResConstraint {
  ResConeLimit unknown10; // Inferred role: angle limit; original member name unknown.
};

static_assert(offsetof(ResStickConstraint, unknown10) == 0x10, "");
static_assert(sizeof(ResStickConstraint) == 0x18, "");

struct ResSpringConstraint : ResConstraint {
  f32 unknown10; // Inferred role: spring rate ("spring"); original member name unknown.
  f32 unknown14; // Inferred role: maximum extension ("extendMax"); original member name unknown.
  f32 unknown18; // Inferred role: maximum shrink ("shrinkMax"); original member name unknown.
};

static_assert(offsetof(ResSpringConstraint, unknown18) == 0x18, "");
static_assert(sizeof(ResSpringConstraint) == 0x20, "");

struct ResHingeConstraint : ResConstraint {
  mm::Vec4 unknown10;     // Inferred role: hinge axis, w = 1; original member name unknown.
  ResConeLimit unknown20; // Inferred role: angle limit; original member name unknown.
};

static_assert(offsetof(ResHingeConstraint, unknown10) == 0x10, "");
static_assert(offsetof(ResHingeConstraint, unknown20) == 0x20, "");
static_assert(sizeof(ResHingeConstraint) == 0x30, "");

// Simulated node, one per ResNode: NodeArray::NodeArray (0x710098234c) resizes its Vector<Node> to the ResNode count.
// Layout evidence: the element copy of Vector<Node>::extend (copy_construct_n at 0x71009825b8) copies the Vec3 members
// one by one and the Quat members as raw bytes, and resize zero-fills new nodes; applySpringConstraints reads and
// moves the position at 0x50.
struct Node {
  mm::Vec3 unknown00; // Inferred role: translation; original member name unknown.
  mm::Quat unknown10; // Inferred role: rotation; original member name unknown.
  mm::Vec3 unknown20; // Inferred role: vector, use not determined; original member name unknown.
  mm::Vec3 unknown30; // Inferred role: vector, use not determined; original member name unknown.
  mm::Quat unknown40; // Inferred role: rotation; original member name unknown.
  mm::Vec3 unknown50; // Inferred role: current position; original member name unknown.
  mm::Vec3 unknown60; // Inferred role: vector, use not determined; original member name unknown.
  mm::Vec3 unknown70; // Inferred role: blended position target; original member name unknown.
  mm::Vec3 unknown80; // Inferred role: vector, use not determined; original member name unknown.
  mm::Vec3 unknown90; // Inferred role: vector, use not determined; original member name unknown.
  u16 unknowna0;      // Inferred role: index, use not determined; original member name unknown.
};

static_assert(offsetof(Node, unknown10) == 0x10, "");
static_assert(offsetof(Node, unknown40) == 0x40, "");
static_assert(offsetof(Node, unknown50) == 0x50, "");
static_assert(offsetof(Node, unknowna0) == 0xa0, "");
static_assert(sizeof(Node) == 0xb0, "");

// Layout evidence: NodeArray::NodeArray (0x710098234c) stores the two resource arrays at 0x0 and 0x8 and constructs
// the node vector at 0x10 with the allocator at 0x30; DynamicsSolver stores it at 0x10.
class NodeArray {
public:
  NodeArray(mm::mtl::IAllocator *allocator, const anm::Array<ResNode> &resNodes, const anm::Array<ResNodeCollision> &resNodeCollisions);

  const anm::Array<ResNode> *unknown00;          // Inferred role: resource nodes; original member name unknown.
  const anm::Array<ResNodeCollision> *unknown08; // Inferred role: resource node collisions; original member name unknown.
  mm::mtl::Vector<Node> unknown10;               // Inferred role: simulated nodes; original member name unknown.
};

static_assert(offsetof(NodeArray, unknown08) == 0x08, "");
static_assert(offsetof(NodeArray, unknown10) == 0x10, "");
static_assert(sizeof(NodeArray) == 0x38, "");

// Solver for one ResSolver. Layout evidence: the constructor at 0x710097d378 stores the resource at 0x8, constructs
// the NodeArray at 0x10, the force vector at 0x50 and the blend weight at 0x7c; `m_pResSolver` is named by the
// assertion in skd_dynamics.cpp, and the vtable at 0x71011b9b18 lists the destructor then solve and solveMT.
class DynamicsSolver {
public:
  DynamicsSolver(mm::mtl::IAllocator *allocator, const ResSolver &resSolver, u32 index);
  virtual ~DynamicsSolver() = default;
  virtual void solve(f32 deltaTime, const CollionArray &collisions, const Packet &packet);
  virtual void solveMT(f32 deltaTime, const CollionArray &collisions, const Packet &packet, anm::ThreadPool &threadPool);

  void init(const Packet &packet);
  void convertCoordinateSystem(const anm::QsTransform &transform);
  void resetNodes(const Packet &packet);

  const ResSolver *m_pResSolver;
  NodeArray unknown10;                 // Inferred role: simulated nodes; original member name unknown.
  f32 unknown48;                       // Inferred role: last delta time; original member name unknown.
  mm::mtl::Vector<mm::Vec3> unknown50; // Inferred role: forces; original member name unknown.
  u32 unknown78;                       // Inferred role: solver index; original member name unknown.
  f32 m_solverBlendWeight;             // Name from Dynamics::getSolverBlendWeight/setSolverBlendWeight.
  bool unknown80;                      // Inferred role: initialized; original member name unknown.
};

static_assert(offsetof(DynamicsSolver, m_pResSolver) == 0x08, "");
static_assert(offsetof(DynamicsSolver, unknown10) == 0x10, "");
static_assert(offsetof(DynamicsSolver, unknown48) == 0x48, "");
static_assert(offsetof(DynamicsSolver, unknown50) == 0x50, "");
static_assert(offsetof(DynamicsSolver, unknown78) == 0x78, "");
static_assert(offsetof(DynamicsSolver, m_solverBlendWeight) == 0x7c, "");
static_assert(offsetof(DynamicsSolver, unknown80) == 0x80, "");
static_assert(sizeof(DynamicsSolver) == 0x88, "");

class ApplyDetectJob : public anm::Job {
public:
  static const char *_TYPE_NAME;

  ApplyDetectJob(u16 index, NodeArray *nodes);

  u16 unknown10; // Inferred role: node index; original member name unknown.
  u8 _12[0x06];
  DetectInfo *unknown18[0x20]; // Inferred role: hits of the node; original member name unknown.
  size_t unknown118;           // Inferred role: hit count; original member name unknown.
  NodeArray *unknown120;       // Inferred role: simulated nodes; original member name unknown.
};

static_assert(offsetof(ApplyDetectJob, unknown10) == 0x10, "");
static_assert(offsetof(ApplyDetectJob, unknown18) == 0x18, "");
static_assert(offsetof(ApplyDetectJob, unknown118) == 0x118, "");
static_assert(offsetof(ApplyDetectJob, unknown120) == 0x120, "");
static_assert(sizeof(ApplyDetectJob) == 0x128, "");

class DetectSphereJob : public anm::Job {
public:
  static const char *_TYPE_NAME;

  DetectSphereJob(const class ShpereCollObj *object, const ResNodeCollision *nodeCollision, const mm::Vec3 &position, DetectInfo *unknown30, DetectInfo *unknown38);

  const class ShpereCollObj *unknown10; // Inferred role: collision shape; original member name unknown.
  const ResNodeCollision *unknown18;    // Inferred role: node collision; original member name unknown.
  mm::Vec3 unknown20;                   // Inferred role: detection position; original member name unknown.
  DetectInfo *unknown30;                // Inferred role: first hit output; original member name unknown.
  DetectInfo *unknown38;                // Inferred role: second hit output; original member name unknown.
};

static_assert(offsetof(DetectSphereJob, unknown18) == 0x18, "");
static_assert(offsetof(DetectSphereJob, unknown20) == 0x20, "");
static_assert(offsetof(DetectSphereJob, unknown30) == 0x30, "");
static_assert(offsetof(DetectSphereJob, unknown38) == 0x38, "");
static_assert(sizeof(DetectSphereJob) == 0x40, "");

class DetectCapsuleJob : public anm::Job {
public:
  static const char *_TYPE_NAME;

  DetectCapsuleJob(const class CapsuleCollObj *object, const ResNodeCollision *nodeCollision, const mm::Vec3 &position, DetectInfo *unknown30, DetectInfo *unknown38);

  const class CapsuleCollObj *unknown10; // Inferred role: collision shape; original member name unknown.
  const ResNodeCollision *unknown18;     // Inferred role: node collision; original member name unknown.
  mm::Vec3 unknown20;                    // Inferred role: detection position; original member name unknown.
  DetectInfo *unknown30;                 // Inferred role: first hit output; original member name unknown.
  DetectInfo *unknown38;                 // Inferred role: second hit output; original member name unknown.
};

static_assert(offsetof(DetectCapsuleJob, unknown18) == 0x18, "");
static_assert(offsetof(DetectCapsuleJob, unknown20) == 0x20, "");
static_assert(offsetof(DetectCapsuleJob, unknown30) == 0x30, "");
static_assert(offsetof(DetectCapsuleJob, unknown38) == 0x38, "");
static_assert(sizeof(DetectCapsuleJob) == 0x40, "");

class DetectPlaneJob : public anm::Job {
public:
  static const char *_TYPE_NAME;

  DetectPlaneJob(const class PlaneCollObj *object, const ResNodeCollision *nodeCollision, const mm::Vec3 &position, DetectInfo *unknown30, DetectInfo *unknown38);

  const class PlaneCollObj *unknown10; // Inferred role: collision shape; original member name unknown.
  const ResNodeCollision *unknown18;   // Inferred role: node collision; original member name unknown.
  mm::Vec3 unknown20;                  // Inferred role: detection position; original member name unknown.
  DetectInfo *unknown30;               // Inferred role: first hit output; original member name unknown.
  DetectInfo *unknown38;               // Inferred role: second hit output; original member name unknown.
};

static_assert(offsetof(DetectPlaneJob, unknown18) == 0x18, "");
static_assert(offsetof(DetectPlaneJob, unknown20) == 0x20, "");
static_assert(offsetof(DetectPlaneJob, unknown30) == 0x30, "");
static_assert(offsetof(DetectPlaneJob, unknown38) == 0x38, "");
static_assert(sizeof(DetectPlaneJob) == 0x40, "");

// Layout evidence: import (0x71009756a4) placement-constructs the object at header+8, whose constructor only sets the
// vptr (the serialized data keeps its members); DynamicResDynamics's destructor reads the allocator at 0x10 and the
// collisions and solvers at 0x18 and 0x20.
struct ResDynamics {
  virtual ~ResDynamics() = default;

  u8 _08[0x08];
  mm::mtl::IAllocator *unknown10;         // Inferred role: allocator of owned resources; original member name unknown.
  const ResCollisionArray *unknown18;     // Inferred role: collisions; original member name unknown.
  const anm::Array<ResSolver> *unknown20; // Inferred role: solvers; original member name unknown.
};

static_assert(offsetof(ResDynamics, unknown10) == 0x10, "");
static_assert(offsetof(ResDynamics, unknown18) == 0x18, "");
static_assert(offsetof(ResDynamics, unknown20) == 0x20, "");
static_assert(sizeof(ResDynamics) == 0x28, "");

// Layout evidence: the destructor at 0x7100978d54 deallocates each pointer of the Vector at 0x28 with the
// ReferenceObject allocator at 0x10, then destroys the vector; the vtable at 0x71011b9ab8 lists the destructor only.
class DynamicResDynamics : public ResDynamics {
public:
  virtual ~DynamicResDynamics() = default;

  mm::mtl::Vector<void *> unknown28; // Inferred role: owned resources; original member name unknown.
};

static_assert(offsetof(DynamicResDynamics, unknown28) == 0x28, "");
static_assert(sizeof(DynamicResDynamics) == 0x50, "");

class DynamicsImporter {
public:
  static ResDynamics *import(void *data);
  static bool restore(void *data);
  static class DynamicResDynamics *importXML(const char *path, anm::XmlInterface &xml, mm::mtl::IAllocator *allocator);
};

class Dynamics : public anm::ReferenceObject {
public:
  Dynamics(CollisionObject &collisionObject, const anm::Array<ResSolver> &solvers, anm::Pose &pose, ForceInterface *force, mm::mtl::IAllocator *allocator);
  ~Dynamics() override;

  void createSolver();
  void destroySolver();
  void update(f32 deltaTime);
  void updateMT(f32 deltaTime, anm::ThreadPool &threadPool);
  void notifyMovement(const mm::Vec3 &position, const mm::Quat &rotation, const mm::Vec3 &previousPosition, const mm::Quat &previousRotation);
  void resetDynamics();
  s32 getSolverIndex(const char *name) const;
  f32 getSolverBlendWeight(u32 index) const;
  void setSolverBlendWeight(u32 index, f32 weight);

  const anm::Array<ResSolver> *m_pResSolver;   // Name from the "m_pResSolver" assertion in skd_dynamics.cpp.
  mm::mtl::Vector<DynamicsSolver *> m_solvers; // Name from the "m_solvers[i]" assertion in skd_dynamics.cpp.
  Packet unknown48;                            // Inferred role: packet; original member name unknown.
  CollisionObject *unknown190;                 // Inferred role: collision object; original member name unknown.
  f32 unknown198;                              // Inferred role: max move distance; original member name unknown.
  f32 unknown19c;                              // Inferred role: max rotation; original member name unknown.
};

static_assert(offsetof(Dynamics, m_pResSolver) == 0x18, "");
static_assert(offsetof(Dynamics, m_solvers) == 0x20, "");
static_assert(offsetof(Dynamics, unknown48) == 0x48, "");
static_assert(offsetof(Dynamics, unknown190) == 0x190, "");
static_assert(offsetof(Dynamics, unknown198) == 0x198, "");
static_assert(offsetof(Dynamics, unknown19c) == 0x19c, "");
static_assert(sizeof(Dynamics) == 0x1a0, "");

class CalcUtil {
public:
  static bool initNodes(anm::Pose &pose, NodeArray *nodes);
  static void cacheNode(anm::Pose &pose, NodeArray *nodes);
  static void addForceTransform(const NodeArray &nodes, mm::mtl::Vector<mm::Vec3> *forces);
  static void addForceSpring(const NodeArray &nodes, const anm::Array<ResSpringConstraint> &constraints, mm::mtl::Vector<mm::Vec3> *forces);
  static void applyAccel(mm::mtl::Vector<mm::Vec3> &forces, NodeArray *nodes);
  static void integrate(f32 deltaTime, f32 step, NodeArray *nodes);
  static void applyStickConstraints(const anm::Array<ResStickConstraint> &constraints, NodeArray *nodes);
  static void applySpringConstraints(const anm::Array<ResSpringConstraint> &constraints, NodeArray *nodes);
  static void applyHingeConstraints(const anm::Array<ResHingeConstraint> &constraints, NodeArray *nodes);
  static void detectCollision(const CollionArray &collisions, NodeArray *nodes);
  static void buildDetectCollisionJobs(const CollionArray &collisions, const NodeArray &nodes, DetectInfoBuffer *buffer, anm::JobQueue *queue);
  static void blendNode(f32 weight, NodeArray *nodes);
  static void applyPose(NodeArray &nodes, anm::Pose *pose);
  static void writeBackNode(anm::Pose &pose, NodeArray *nodes);
  static void convertCoordinateSystem(const anm::QsTransform &transform, NodeArray *nodes);
};

void limitCone(const ResConstraint &constraint, const ResConeLimit &limit, NodeArray *nodes);
void limitDistance(const ResConstraint &constraint, NodeArray *nodes);
void limitHinge(const ResHingeConstraint &constraint, NodeArray *nodes);

void executeApplyDetectJob(anm::Job &job);
void executeDetectSphereJob(anm::Job &job);
void executeDetectCapsuleJob(anm::Job &job);
void executeDetectPlaneJob(anm::Job &job);
void registerApplyDetectJobExecuter(anm::JobQueue *queue);
void registerDetectSphereJobExecuter(anm::JobQueue *queue);
void registerDetectCapsuleJobExecuter(anm::JobQueue *queue);
void registerDetectPlaneJobExecuter(anm::JobQueue *queue);
void addApplyDetectJobs(DetectInfoBuffer &buffer, NodeArray *nodes, anm::JobQueue *queue);
void addDetectSphereJob(const class ShpereCollObj *object, const ResNodeCollision *nodeCollision, const NodeArray *nodes, const mm::Vec3 &position, DetectInfoBuffer *buffer, anm::JobQueue *queue);
void addDetectCapsuleJob(const class CapsuleCollObj *object, const ResNodeCollision *nodeCollision, const NodeArray *nodes, const mm::Vec3 &position, DetectInfoBuffer *buffer, anm::JobQueue *queue);
void addDetectPlaneJob(const class PlaneCollObj *object, const ResNodeCollision *nodeCollision, const NodeArray *nodes, const mm::Vec3 &position, DetectInfoBuffer *buffer, anm::JobQueue *queue);

} // namespace skd
