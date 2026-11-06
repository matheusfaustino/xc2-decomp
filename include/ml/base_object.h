#pragma once
#include <stddef.h>

#include "ml/critical_section.h"
#include "mm_mtl/fixstr.h"

namespace ml {

class BaseObjectM {
public:
  virtual ~BaseObjectM() {}
  virtual int AddRef();
  virtual int Release();
  virtual void SetName(const char *name);

  int unknown08;                  // Inferred role: ref count; original member name unknown.
  mm::mtl::FixStr<128> unknown0c; // Inferred role: name; original member name unknown.
  CriticalSection unknown90;      // Inferred role: critical section; original member name unknown.
};

static_assert(offsetof(BaseObjectM, unknown0c) == 0x0c, "");
static_assert(offsetof(BaseObjectM, unknown90) == 0x90, "");
static_assert(sizeof(BaseObjectM) == 0xb8, "");

} // namespace ml
