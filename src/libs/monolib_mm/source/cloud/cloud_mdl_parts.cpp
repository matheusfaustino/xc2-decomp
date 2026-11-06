#include "cloud/cloud_mdl_parts.h"

#include "cloud/cloud_man.h"
#include "cloud/cloud_mdl_data_obj.h"

namespace cloud {

bool CloudMdlParts::isDisp() const {
  if ((unknown68->unknown80 & 1) == 0) {
    return false;
  }
  u32 group = unknown74;
  if (group != 0) {
    u32 bits = CloudMan::get()->unknown28[static_cast<u32>(group) >> 5];
    if ((bits & (1u << (group & 0x1f))) != 0) {
      return false;
    }
  }
  return true;
}

const char *CloudMdlParts::getName() const {
  const CloudMdlDataObj *data = unknown68;
  return data->unknown48 + data->unknown50[unknown70].unknownb0;
}

} // namespace cloud
