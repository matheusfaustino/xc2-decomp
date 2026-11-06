// Original file name from the game string C:/Monolithsoft/BF2/prog/libs/wndlib/src/wndlib/composite/wnd_area.cpp.
#include "wnd/wnd_area.h" // IWYU pragma: keep

#include "mm/mm_std_base.h"
#include "wnd/wnd_object.h" // IWYU pragma: keep
#include "wnd/wnd_object_pool_memory.h"

namespace wnd {

AreaBlock::AreaBlock() : unknown10(nullptr), unknown58(0), unknown5c(0) {
  for (int i = 0; i < 8; i++) {
    unknown18[i] = nullptr;
  }
}

AreaBlock::~AreaBlock() {
#line 30
  MM_ASSERT(m_winds.empty());
}

AreaBlock *AreaBlock::createChild(u32 index, PoolObjectMemory<AreaBlock> &pool) {
  AreaBlock *child = pool.alloc();
  unknown18[index] = child;
  child->unknown10 = this;
  unknown58 |= 1u << index;
  return child;
}

void AreaBlock::destroyChild(u32 index, PoolObjectMemory<AreaBlock> &pool) {
  AreaBlock *child = unknown18[index];
  if (child != nullptr) {
    for (u32 i = 0; i < 8; i++) {
      child->destroyChild(i, pool);
    }
    // The recursive calls release the block, so the child is read again from the cell.
    child = unknown18[index];
    pool.m_useList.erase(child);
    pool.m_freeList.push_back(child);
    unknown18[index] = nullptr;
  }
  unknown58 &= ~(1u << index);
}

} // namespace wnd
