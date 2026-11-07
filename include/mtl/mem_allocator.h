#pragma once
#include <stddef.h>

#include "mtl/mem_manager.h"
#include "types.h"

namespace mtl {
struct MEMIMAGE_HEAD;
struct MemoryInfo1;

class IMemAllocator {
public:
  virtual ~IMemAllocator() {}
  virtual void create(void *buffer, size_t size, const char *name) = 0;
  virtual void destroy() = 0;
  virtual AllocatorType getType() const = 0;
  virtual void *allocate(size_t size, size_t alignment) = 0;
  virtual void *allocate(size_t size, size_t alignment, const char *file, int line) = 0;
  virtual void *allocate_tail(size_t size, size_t alignment, const char *file, int line) { return nullptr; }
  virtual void deallocate(void *pointer, size_t size) = 0;
  virtual void *reallocate(void *pointer, size_t size) = 0;
  virtual void log(MemoryInfo *info, bool isDetail) const = 0;
  virtual void log_callSTack(void *callStack) const {}
  virtual void checkLeakMemory() {}
  virtual const char *getName() const = 0;
  virtual bool hasMemory(const void *pointer) const = 0;
  virtual size_t getAllocSize(const void *pointer) const = 0;
  virtual void memoryInfo(MemoryInfo *info) const = 0;
  virtual void getMemImage(MEMIMAGE_HEAD *head) const = 0;
  virtual bool getMemInfo1(MemoryInfo1 *info, u32 num) const = 0;
  virtual bool empty() const = 0;
  virtual bool valid() const = 0;
  virtual bool dummy() const = 0;
  virtual u32 getMemSize(const void *pointer) const = 0;
};

} // namespace mtl
