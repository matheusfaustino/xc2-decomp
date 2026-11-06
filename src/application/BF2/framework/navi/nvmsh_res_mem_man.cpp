#include "navi/nvmsh_res_mem_man.h"

#include "mtl/mem_manager.h"

namespace navi {

// The game stores these fields in this order.
NvmshResMemObj::NvmshResMemObj() {
  unknown18 = nullptr;
  unknown20 = 0;
  unknown10 = 0;
  unknown00 = nullptr;
  unknown08 = nullptr;
}

NvmshResMemMan::~NvmshResMemMan() {
  if (unknown00 != nullptr) {
    mtl::MemManager::deallocate(unknown00, 0, false);
    unknown00 = nullptr;
  }
}

void NvmshResMemMan::memLcMove(void *destination, void *source, u32 size) { __builtin_memcpy(destination, source, size); }

void NvmshResMemMan::releaseFreeObj(NvmshResMemObj *object) {
  for (NvmshResMemObj *entry = unknown18; entry != nullptr; entry = entry->unknown08) {
    if (entry == object) {
      clearMemObj(entry);
      return;
    }
  }
}

void NvmshResMemMan::moveMemoryObject(NvmshResMemObj *destination, NvmshResMemObj *source) {
  memLcMove(destination->unknown18, source->unknown18, source->unknown20);
  source->unknown10 |= 4;
  void *data = destination->unknown18;
  source->unknown18 = data;
  destination->unknown18 = static_cast<u8 *>(data) + source->unknown20;
}

} // namespace navi
