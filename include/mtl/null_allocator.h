#pragma once
#include <stddef.h>

#include "mm_mtl/fixstr.h"
#include "mtl/mem_allocator.h"
#include "mtl/mem_manager.h"
#include "types.h"

namespace mtl {

class NullAllocator : public IMemAllocator {
public:
  ~NullAllocator() override;
  void create(void *buffer, size_t size, const char *name) override;
  void destroy() override;
  AllocatorType getType() const override;
  void *allocate(size_t size, size_t alignment) override;
  void *allocate(size_t size, size_t alignment, const char *file, int line) override;
  void *allocate_tail(size_t size, size_t alignment, const char *file, int line) override;
  void deallocate(void *pointer, size_t size) override;
  void *reallocate(void *pointer, size_t size) override;
  void log(MemoryInfo *info, bool isDetail) const override;
  const char *getName() const override;
  bool hasMemory(const void *pointer) const override;
  size_t getAllocSize(const void *pointer) const override;
  void memoryInfo(MemoryInfo *info) const override;
  void getMemImage(MEMIMAGE_HEAD *head) const override;
  bool getMemInfo1(MemoryInfo1 *info, u32 num) const override;
  bool empty() const override;
  bool valid() const override;
  bool dummy() const override;
  u32 getMemSize(const void *pointer) const override;

  mm::mtl::FixStr<32> m_name;
};

static_assert(sizeof(NullAllocator) == 0x30, "");

} // namespace mtl
