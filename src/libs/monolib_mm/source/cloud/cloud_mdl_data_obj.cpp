#include "cloud/cloud_mdl_data_obj.h"

#include "cloud/cloud_man.h"

namespace cloud {

CloudMdlDataObj::CloudMdlDataObj()
    : unknown00(nullptr), unknown08(nullptr), unknown10(nullptr), unknown48(nullptr), unknown60(nullptr), unknown68(nullptr), unknown70(nullptr), unknown78(0), unknown7c(0), unknown80(0) {}

CloudMdlDataObj::~CloudMdlDataObj() { releaseData(); }

void CloudMdlDataObj::setCallDisp(bool isCallDisp) {
  u32 flags = unknown80;
  u32 cleared = flags & ~3u;
  u32 set = flags | 2u;
  unknown80 = isCallDisp ? set : cleared;
}

void CloudMdlDataObj::release() { unknown80 |= 0x40u << CloudMan::s_nDblStIdx; }

} // namespace cloud
