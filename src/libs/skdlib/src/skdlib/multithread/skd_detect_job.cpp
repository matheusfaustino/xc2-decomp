// Original file name inferred from the job type strings and the skd_detect_info.h assertion file path.
#include "anm/anm_job.h"
#include "anm/anm_job_queue.h"
#include "mm_math/mm_vec3.h"
#include "skd/skd_collision_object.h"
#include "skd/skd_detect_info.h"
#include "skd/skd_dynamics.h"
#include "skd/skd_packet.h"
#include "types.h"

namespace skd {

const char *ApplyDetectJob::_TYPE_NAME = "ApplyDetectJob";
const char *DetectCapsuleJob::_TYPE_NAME = "DetectCapsuleJob";
const char *DetectPlaneJob::_TYPE_NAME = "DetectPlaneJob";
const char *DetectSphereJob::_TYPE_NAME = "DetectSphereJob";

ApplyDetectJob::ApplyDetectJob(u16 index, NodeArray *nodes) : anm::Job(_TYPE_NAME, sizeof(ApplyDetectJob), 3), unknown10(index), unknown118(0), unknown120(nodes) {}

DetectSphereJob::DetectSphereJob(const ShpereCollObj *object, const ResNodeCollision *nodeCollision, const mm::Vec3 &position, DetectInfo *unknown30, DetectInfo *unknown38)
    : anm::Job(_TYPE_NAME, sizeof(DetectSphereJob), 0), unknown10(object), unknown18(nodeCollision), unknown20(position), unknown30(unknown30), unknown38(unknown38) {}

DetectCapsuleJob::DetectCapsuleJob(const CapsuleCollObj *object, const ResNodeCollision *nodeCollision, const mm::Vec3 &position, DetectInfo *unknown30, DetectInfo *unknown38)
    : anm::Job(_TYPE_NAME, sizeof(DetectCapsuleJob), 1), unknown10(object), unknown18(nodeCollision), unknown20(position), unknown30(unknown30), unknown38(unknown38) {}

DetectPlaneJob::DetectPlaneJob(const PlaneCollObj *object, const ResNodeCollision *nodeCollision, const mm::Vec3 &position, DetectInfo *unknown30, DetectInfo *unknown38)
    : anm::Job(_TYPE_NAME, sizeof(DetectPlaneJob), 2), unknown10(object), unknown18(nodeCollision), unknown20(position), unknown30(unknown30), unknown38(unknown38) {}

void executeDetectSphereJob(anm::Job &job) {
  DetectSphereJob &detectJob = static_cast<DetectSphereJob &>(job);
  const ResNodeCollision *nodeCollision = detectJob.unknown18;
  mm::Vec3 normal;
  if (!detectJob.unknown10->detect(nodeCollision->unknown08, detectJob.unknown20, &normal)) {
    return;
  }
  if (detectJob.unknown30 != nullptr) {
    detectJob.unknown30->unknown10 = normal * (1.0f - nodeCollision->unknown04);
    detectJob.unknown30->unknown20 = true;
  }
  if (detectJob.unknown38 != nullptr) {
    detectJob.unknown38->unknown10 = normal * nodeCollision->unknown04;
    detectJob.unknown38->unknown20 = true;
  }
}

void executeDetectCapsuleJob(anm::Job &job) {
  DetectCapsuleJob &detectJob = static_cast<DetectCapsuleJob &>(job);
  const ResNodeCollision *nodeCollision = detectJob.unknown18;
  mm::Vec3 normal;
  if (!detectJob.unknown10->detect(nodeCollision->unknown08, detectJob.unknown20, &normal)) {
    return;
  }
  if (detectJob.unknown30 != nullptr) {
    detectJob.unknown30->unknown10 = normal * (1.0f - nodeCollision->unknown04);
    detectJob.unknown30->unknown20 = true;
  }
  if (detectJob.unknown38 != nullptr) {
    detectJob.unknown38->unknown10 = normal * nodeCollision->unknown04;
    detectJob.unknown38->unknown20 = true;
  }
}

void executeDetectPlaneJob(anm::Job &job) {
  DetectPlaneJob &detectJob = static_cast<DetectPlaneJob &>(job);
  const ResNodeCollision *nodeCollision = detectJob.unknown18;
  mm::Vec3 normal;
  if (!detectJob.unknown10->detect(nodeCollision->unknown08, detectJob.unknown20, &normal)) {
    return;
  }
  if (detectJob.unknown30 != nullptr) {
    detectJob.unknown30->unknown10 = normal * (1.0f - nodeCollision->unknown04);
    detectJob.unknown30->unknown20 = true;
  }
  if (detectJob.unknown38 != nullptr) {
    detectJob.unknown38->unknown10 = normal * nodeCollision->unknown04;
    detectJob.unknown38->unknown20 = true;
  }
}

void registerApplyDetectJobExecuter(anm::JobQueue *queue) { queue->registerJobExecuter(3, executeApplyDetectJob); }
void registerDetectSphereJobExecuter(anm::JobQueue *queue) { queue->registerJobExecuter(0, executeDetectSphereJob); }
void registerDetectCapsuleJobExecuter(anm::JobQueue *queue) { queue->registerJobExecuter(1, executeDetectCapsuleJob); }
void registerDetectPlaneJobExecuter(anm::JobQueue *queue) { queue->registerJobExecuter(2, executeDetectPlaneJob); }

} // namespace skd
