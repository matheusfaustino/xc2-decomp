#include "navi/nvmsh_res_obj.h"

#include "navi/nvmsh_res_mem_man.h"

namespace navi {

void NvmshResObj::releaseNavi() {
  unknown00 = 0;
  unknown08 = nullptr;
}

void NvmshResObj::resetNavi() {
  unknown00 = 0;
  unknown08 = nullptr;
}

void NvmshResData::setReadData(const _NvMhAllReadObj *readData, int offset) {
  unknown08 = readData;
  unknownc0 = offset;
}

void NvmshResData::releaseNavi(NvmshResMemMan *memMan) {
  NvmshResMemObj *object = unknownc8;
  unknown00 = 0;
  unknown04 = 0;
  unknown10 = 0;
  unknown18 = nullptr;
  unknown68 = 0;
  unknown70 = nullptr;

  if (object != nullptr) {
    for (NvmshResMemObj *entry = memMan->unknown18; entry != nullptr; entry = entry->unknown08) {
      if (entry == object) {
        memMan->clearMemObj(object);
        break;
      }
    }

    unknownc8 = nullptr;
  }
}

void NvmshResData::releaseCall() {
  unknown00 |= 4;
  unknown04 = 0;
}

} // namespace navi
