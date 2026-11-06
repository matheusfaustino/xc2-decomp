#pragma once
#include <stddef.h>

#include "mm_mtl/singleton.h"
#include "types.h"

namespace ml {

class DrPixlMemMan : public mm::mtl::PtrSingleton<DrPixlMemMan> {
public:
  ~DrPixlMemMan();

  u8 _008[0x434 - 0x008];
  u32 unknown434; // Inferred role: mem1 alpha free; original member name unknown.
};

static_assert(offsetof(DrPixlMemMan, unknown434) == 0x434, "");

} // namespace ml
