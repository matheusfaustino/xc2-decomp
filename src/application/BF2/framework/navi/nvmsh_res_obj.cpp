#include "navi/nvmsh_res_obj.h"

#include "navi/nvmsh_res_mem_man.h"

namespace navi {

NvmshResObj::NvmshResObj()
    : unknown00(0), unknown08(nullptr), unknown10(nullptr), unknown18(nullptr), unknown20(nullptr), unknown28(nullptr), unknown30(nullptr), unknown38(nullptr), unknown40(nullptr), unknown48(nullptr),
      unknown50(nullptr) {}

NvmshResObj::~NvmshResObj() {}

NvmshResData::~NvmshResData() {}

void NvmshResObj::createNavi(const u8 *data) {
  // Evidence: the game reads the header fields at these offsets directly from the file image.
  const _NvMhAllReadObj *header = reinterpret_cast<const _NvMhAllReadObj *>(data);
  unknown08 = data;
  unknown10 = data;
  unknown18 = data + header->unknown28;
  unknown20 = data + header->unknown30;
  unknown28 = data + header->unknown54;
  unknown48 = data + header->unknown40;
  unknown30 = data + header->unknown48;
  unknown50 = data + header->unknown50;
  unknown38 = data + header->unknown6c;
  unknown40 = data + header->unknown74;
  unknown00 |= 1;
}

// The game has both functions, with the same body.
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
    // The game inlines releaseFreeObj's scan here and calls clearMemObj directly; calling
    // releaseFreeObj emits a bl to it instead (checked against the game).
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
