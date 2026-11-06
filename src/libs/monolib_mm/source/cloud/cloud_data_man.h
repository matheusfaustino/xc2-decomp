#pragma once
#include <stddef.h>

#include "ml/work_event.h"
#include "types.h"

namespace ml {
class FileHandleTh;
} // namespace ml

namespace cloud {

enum CloudDataType {
  CLOUD_DATA_TYPE_0,
  CLOUD_DATA_TYPE_1,
  CLOUD_DATA_TYPE_2,
  CLOUD_DATA_TYPE_3,
  CLOUD_DATA_TYPE_4,
};

// Owns the five cloud files read from the device: the shader list and four textures. The data
// pointer, loaded size and pending handle arrays are indexed by CloudDataType.
class CloudDataMan : public ml::IWorkEvent {
public:
  CloudDataMan();
  ~CloudDataMan() override;
  bool isReadOk() const;
  bool OnFileEvent(ml::EventFileTh *event) override;
  void reLoadData(CloudDataType type);
  void reLoadShaderData();
  void reLoadTextureData();

  void *unknown08[5];             // Inferred role: file data per CloudDataType; original member name unknown.
  u32 unknown30[5];               // Inferred role: loaded size per CloudDataType; original member name unknown.
  ml::FileHandleTh *unknown48[5]; // Inferred role: pending handle per CloudDataType; original member name unknown.
};

static_assert(offsetof(CloudDataMan, unknown08) == 0x08, "");
static_assert(offsetof(CloudDataMan, unknown30) == 0x30, "");
static_assert(offsetof(CloudDataMan, unknown48) == 0x48, "");
static_assert(sizeof(CloudDataMan) == 0x70, "");

} // namespace cloud
