#pragma once
#include <stddef.h>

#include "ml/critical_section.h"
#include "mtl/alloc_handle.h"
#include "types.h"

namespace mm {
namespace mtl {
class IAllocator;
} // namespace mtl
} // namespace mm

namespace mtl {

class IMemAllocator;
struct MEMIMAGE_HEAD;
struct MemoryInfo1;

struct MemoryInfo {
  MemoryInfo() : unknown00(0), unknown08(0), unknown10(0), unknown18(0), unknown20(0), unknown28(0), unknown30(0), unknown38(0), unknown40(0) {}

  u64 unknown00; // Inferred role: total size; original member name unknown.
  u64 unknown08; // Role and original member name unknown.
  u64 unknown10; // Inferred role: used size; original member name unknown.
  u64 unknown18; // Role and original member name unknown.
  u64 unknown20; // Role and original member name unknown.
  int unknown28; // Inferred role: allocation count; original member name unknown.
  u64 unknown30; // Role and original member name unknown.
  u64 unknown38; // Role and original member name unknown.
  u64 unknown40; // Role and original member name unknown.
};

static_assert(offsetof(MemoryInfo, unknown10) == 0x10, "");
static_assert(offsetof(MemoryInfo, unknown28) == 0x28, "");
static_assert(offsetof(MemoryInfo, unknown30) == 0x30, "");
static_assert(sizeof(MemoryInfo) == 0x48, "");

enum AllocatorType {
  ALLOCATOR_TYPE_PROGRAM = 1,
  ALLOCATOR_TYPE_RESOURCE = 2,
};

class MemManager {
public:
  class Region {
  public:
    Region *getArea(const void *pointer);
    void log(MemoryInfo *info, bool isDetail);
    void log_callSTack(void *callStack);
    void checkLeakMemory();
    void memoryInfo(MemoryInfo *info);
    void getMemImage(MEMIMAGE_HEAD *head);
    bool getMemInfo1(MemoryInfo1 *info, u32 num) const;
    int getAllocs() const;
    u64 getTopAdr() const;
    bool empty() const;
    bool hasMemory(const void *pointer) const;
    size_t getAllocSize(void *pointer);
    void *alloc(size_t size);
    void *alloc(size_t size, size_t alignment, const char *file, int line);

    IMemAllocator *unknown00;        // 0x00, inferred role: backing allocator; original member name unknown.
    u64 unknown08;                   // Inferred role: buffer start; original member name unknown.
    u64 unknown10;                   // Inferred role: buffer size; original member name unknown.
    u64 m_allocs;                    // 0x18, read by getAllocs().
    bool unknown20;                  // Inferred role: logging enabled; original member name unknown.
    u8 unknown21[0x4f];              // 0x21..0x6f, roles and original member names unknown.
    u64 m_topAdr;                    // 0x70, read by getTopAdr().
    u8 unknown78[8];                 // Role and original member name unknown.
    ml::CriticalSectionNC unknown80; // 0x80
  };

  static ALLOC_HANDLE create(void *buffer, size_t size, const char *name, bool);
  static ALLOC_HANDLE create(void *buffer, size_t size, const char *name, bool, AllocatorType type);
  static ALLOC_HANDLE create(ALLOC_HANDLE parent, size_t size, const char *name, bool, const char *file = "no name", int line = 0);
  static ALLOC_HANDLE create(ALLOC_HANDLE parent, size_t size, const char *name, bool, AllocatorType type, size_t alignment, const char *file = "no name", int line = 0);
  static void erase(ALLOC_HANDLE handle);
  static bool valid(const ALLOC_HANDLE &handle, bool);
  static mm::mtl::IAllocator *getAllocator(const ALLOC_HANDLE &handle);
  static void *allocate(ALLOC_HANDLE handle, size_t size, size_t alignment, bool isBack, const char *name, int line);
  static void deallocate(void *pointer, size_t size, bool isBack);
  static void *reallocate(void *pointer, size_t size, bool isBack);
  static size_t maxAllocSize(ALLOC_HANDLE handle);
  static bool isArea(ALLOC_HANDLE handle, const void *pointer);
  static u64 getTopAdr(const ALLOC_HANDLE &handle);
  static size_t getTotalSize(ALLOC_HANDLE handle);
  static void log(ALLOC_HANDLE handle, bool isDetail, MemoryInfo *info);
  static bool getMemoryInfo(ALLOC_HANDLE handle, MemoryInfo *info);
  static const char *getName(ALLOC_HANDLE handle);
  static bool isInitialized();
  static bool isCollectCallStack();
  static void setCollectCallStack(bool collectCallStack);
  static void tchk();

  static bool s_bInitialize;
  static bool m_collectCallstack;
};

static_assert(offsetof(MemManager::Region, m_allocs) == 0x18, "");
static_assert(offsetof(MemManager::Region, unknown20) == 0x20, "");
static_assert(offsetof(MemManager::Region, m_topAdr) == 0x70, "");
static_assert(offsetof(MemManager::Region, unknown80) == 0x80, "");

} // namespace mtl
