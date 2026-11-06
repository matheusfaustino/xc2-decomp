#include "ml/file_res_th.h"

#include "ml/dev_file_th.h"
#include "ml/file_handle_th.h"
#include "ml/path_util.h"
#include "mm_mtl/allocator.h"
#include "mtl/alloc_handle.h"
#include "mtl/mem_manager.h"

namespace ml {

FileResTh::~FileResTh() { release(); }

bool FileResTh::isExist() const { return unknownc8 != nullptr; }

bool FileResTh::load(mtl::ALLOC_HANDLE handle, const char *path, u32 option, FILE_PRIO priority) {
  release();
  DevFileParamTh param;
  param._08 = handle;
  param.unknown00 = path;
  param.unknown60 = this;
  param._54 = option;
  param.unknown68 = priority;
  param._6d = true;
  SetName(PathUtil::getFilePtrFromPath(path));
  unknownc0 = DevFileTh::readFile(param);
  if (unknownc0 != nullptr) {
    unknownf4 = 1;
  } else {
    unknownd0 = -2;
    unknownf4 = 3;
  }
  return unknownc0 != nullptr;
}

bool FileResTh::load(mm::mtl::IAllocator *allocator, const char *path, u32 option, FILE_PRIO priority) {
  release();
  DevFileParamTh param;
  param._10 = allocator;
  param.unknown00 = path;
  param.unknown60 = this;
  param._54 = option;
  param.unknown68 = priority;
  param._6d = true;
  SetName(PathUtil::getFilePtrFromPath(path));
  unknownc0 = DevFileTh::readFile(param);
  if (unknownc0 != nullptr) {
    unknownf4 = 1;
  } else {
    unknownd0 = -2;
    unknownf4 = 3;
  }
  return unknownc0 != nullptr;
}

void FileResTh::release() {
  if (unknownc0 != nullptr) {
    unknownc0->unknown60 = nullptr;
    if (unknownc0 != nullptr) {
      DevFileTh::cancel(unknownc0, false);
      unknownc0 = nullptr;
    }
  }
  if (unknownf0 && unknownc8 != nullptr) {
    if (unknownf8 != nullptr) {
      unknownf8->deallocate(unknownc8);
      unknownc8 = nullptr;
      unknownf8 = nullptr;
    } else {
      mtl::MemManager::deallocate(unknownc8, 0, false);
      unknownc8 = nullptr;
    }
  }
  unknownd0 = -1;
  unknownf4 = 0;
  unknowne0 = 0;
}

void FileResTh::setMem(void *buffer, int size, const char *name, bool isBufferOwner) {
  unknownf0 = isBufferOwner;
  unknownc8 = buffer;
  unknownd0 = size;
  SetName(PathUtil::getFilePtrFromPath(name));
}

bool FileResTh::OnFileEvent(EventFileTh *event) {
  if (event->type == EventFileTh::TYPE_COMPLETE) {
    void *buffer = unknownc0->unknown08;
    unknownc0->unknown08 = nullptr;
    unknownc8 = buffer;
    unknownd0 = unknownc0->unknown18;
    unknowne8 = unknownc0->unknown1c8;
    unknowne0 = unknownc0->m_time;
    unknownf8 = static_cast<mm::mtl::IAllocator *>(unknownc0->unknown30);
    unknownf4 = 2;
  } else {
    unknownd0 = -2;
    unknownf4 = 3;
  }
  unknownc0 = nullptr;
  return true;
}

} // namespace ml
