// Original file name from the game string C:/Monolithsoft/BF2/prog/application/BF2/minigame/tora/minigame_tora.cpp.
#include "minigame/tora/minigame_tora.h"

#include "gf/gf_allocator.h"
#include "mm_mtl/singleton.h" // IWYU pragma: keep

namespace minigame {
namespace tora {

class minigame; // IWYU pragma: keep

mytask::~mytask() { gf::getAllocator(gf::ALLOCATOR_DEFAULT)->deallocate(unknown00); }

void *mytask::entryNext(void *task) {
  unknown001 *node = unknown10;
  if (node != nullptr) {
    unknown10 = node->unknown00;
    unknown001 *previous = static_cast<unknown001 *>(task);
    node->unknown08 = previous;
    if (previous == nullptr) {
      node->unknown00 = nullptr;
      unknown18 = node;
      unknown20 = node;
    } else {
      node->unknown00 = previous->unknown00;
      if (node->unknown00 != nullptr)
        node->unknown00->unknown08 = node;
      else
        unknown20 = node;
      previous->unknown00 = node;
    }
  }
  return node;
}

void *mytask::entryPrev(void *task) {
  unknown001 *node = unknown10;
  if (node != nullptr) {
    unknown10 = node->unknown00;
    unknown001 *next = static_cast<unknown001 *>(task);
    node->unknown00 = next;
    if (next == nullptr) {
      node->unknown08 = nullptr;
      unknown18 = node;
      unknown20 = node;
    } else {
      node->unknown08 = next->unknown08;
      if (node->unknown08 != nullptr)
        node->unknown08->unknown00 = node;
      else
        unknown18 = node;
      next->unknown08 = node;
    }
  }
  return node;
}

void *mytask::entry() { return entryNext(unknown20); }
void *mytask::entryTop() { return entryPrev(unknown18); }
void *mytask::entryEnd() { return entryNext(unknown20); }

void mytask::remove(void *task) {
  unknown001 *node = static_cast<unknown001 *>(task);
  if (node == unknown28)
    unknown28 = node->unknown00;
  if (node->unknown00 != nullptr)
    node->unknown00->unknown08 = node->unknown08;
  else
    unknown20 = node->unknown08;
  if (node->unknown08 != nullptr)
    node->unknown08->unknown00 = node->unknown00;
  else
    unknown18 = node->unknown00;
  node->unknown00 = unknown10;
  unknown10 = node;
  node->unknown10 = nullptr;
  node->unknown18 = nullptr;
}

void mytask::executeMove() {
  for (unknown001 *node = unknown18; node != nullptr; node = unknown28) {
    unknown28 = node->unknown00;
    if (node->unknown10 != nullptr)
      node->unknown10(node);
  }
}

void mytask::executeDraw() {
  for (unknown001 *node = unknown18; node != nullptr; node = unknown28) {
    unknown28 = node->unknown00;
    if (node->unknown18 != nullptr)
      node->unknown18(node);
  }
}

void *mytask::enum_next(void *task) { return task != nullptr ? static_cast<unknown001 *>(task)->unknown00 : unknown18; }

} // namespace tora
} // namespace minigame

template mm::mtl::PtrSingleton<minigame::tora::minigame>::~PtrSingleton();
