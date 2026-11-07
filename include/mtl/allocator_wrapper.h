#pragma once
#include <stddef.h>

#include "mm_mtl/fixstr.h"
#include "types.h"

namespace mtl {
class IMemAllocator;
struct MEMIMAGE_HEAD;
struct MemoryInfo;
struct MemoryInfo1;

class AllocatorWrapper {
public:
  AllocatorWrapper();
  ~AllocatorWrapper();
  void init(IMemAllocator *allocator, void *buffer, size_t size, const char *name);
  void destroy();
  const char *getName() const;
  void *allocate(size_t size);
  void *allocate(size_t size, size_t alignment, const char *file, int line);
  void *allocate_tail(size_t size, size_t alignment, const char *file, int line);
  void deallocate(void *pointer, size_t size);
  void *reallocate(void *pointer, size_t size);
  void log(MemoryInfo *info, bool isDetail) const;
  void log_callSTack(void *callStack) const;
  void checkLeakMemory();
  bool hasMemory(const void *pointer) const;
  size_t getAllocSize(const void *pointer) const;
  bool isArea(const void *pointer) const;
  size_t getCapacity() const;
  void memoryInfo(MemoryInfo *info) const;
  void getMemImage(MEMIMAGE_HEAD *head) const;
  bool getMemInfo1(MemoryInfo1 *info, u32 num) const;
  bool valid() const;
  bool empty() const;
  bool dummy() const;
  void clear();

  IMemAllocator *m_allocator;
  void *m_buffer;
  size_t m_capacity;
  size_t unknown18; // Inferred role: alloc num; original member name unknown.
  mm::mtl::FixStr<64> m_name;
};

static_assert(offsetof(AllocatorWrapper, m_name) == 0x20, "");
static_assert(sizeof(AllocatorWrapper) == 0x68, "");

} // namespace mtl
