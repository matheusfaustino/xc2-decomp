#pragma once
#include "mm/mm_std_base.h"
#include "mm_mtl/intrusive_list.h"

namespace mm {
namespace mtl {
class IAllocator;
} // namespace mtl
} // namespace mm

namespace wnd {

// Element pool of the broad phase octree; the member names come from the assertions of its constructor.
template <typename T> class PoolObjectMemory {
public:
  explicit PoolObjectMemory(mm::mtl::IAllocator *allocator);

  T *alloc();

  mm::mtl::IAllocator *m_allocator;
  mm::mtl::IntrusiveList<T> m_useList;
  mm::mtl::IntrusiveList<T> m_freeList;
};

static_assert(sizeof(PoolObjectMemory<void>) == 0x38, "");

// __LINE__ must be 102 and 103 as in the original header: they are the line arguments of the asserts.
#line 101
template <typename T> inline PoolObjectMemory<T>::PoolObjectMemory(mm::mtl::IAllocator *allocator) {
  MM_ASSERT(m_freeList.empty());
  MM_ASSERT(m_useList.empty());
  m_allocator = allocator;
}

} // namespace wnd
