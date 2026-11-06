#pragma once
#include <stddef.h>

#include "framework/memory/allocator.h"
#include "mm_mtl/fixstr.h"
#include "mm_mtl/intrusive_list.h"
#include "mm_mtl/reference_object.h"
#include "mm_mtl/vector.h"
#include "types.h"

namespace fw {

enum ValueType : int {};

class ReflectionValue : public mm::mtl::IntrusiveListNode<ReflectionValue>, public mm::mtl::ReferenceObject {
public:
  ReflectionValue(const char *name, ValueType type) : unknown20(name), unknown64(type), unknown68(0), unknown70(0) {}
  ReflectionValue(const char *name, ValueType type, s64 offset) : unknown20(name), unknown64(type), unknown68(0), unknown70(offset) {}
  ~ReflectionValue() override {}
  virtual void writeDefault(u8 *data) const;

  mm::mtl::FixStr<64> unknown20; // Inferred role: member name; original member name unknown.
  ValueType unknown64;           // Inferred role: value type; original member name unknown.
  s64 unknown68;                 // Inferred role: member size; original member name unknown.
  s64 unknown70;                 // Inferred role: member offset in the reflected data; original member name unknown.
};

static_assert(offsetof(ReflectionValue, unknown20) == 0x20, "");
static_assert(offsetof(ReflectionValue, unknown64) == 0x64, "");
static_assert(offsetof(ReflectionValue, unknown68) == 0x68, "");
static_assert(offsetof(ReflectionValue, unknown70) == 0x70, "");

template <typename T, ValueType TYPE> class ReflectionValuePod : public ReflectionValue {
public:
  ReflectionValuePod(const char *name, s64 offset, T minimum, T maximum, T value) : ReflectionValue(name, TYPE, offset), unknown78(minimum), unknown7c(maximum), unknown80(value) {
    unknown68 = sizeof(T);
  }

  void writeDefault(u8 *data) const override;

  T unknown78; // Inferred role: minimum value; original member name unknown.
  T unknown7c; // Inferred role: maximum value; original member name unknown.
  T unknown80; // Inferred role: default value; original member name unknown.
};

typedef ReflectionValuePod<f32, static_cast<ValueType>(6)> ReflectionValuePodF32;

static_assert(offsetof(ReflectionValuePodF32, unknown78) == 0x78, "");
static_assert(offsetof(ReflectionValuePodF32, unknown80) == 0x80, "");

class ReflectionValueF32 : public ReflectionValuePodF32 {
public:
  ReflectionValueF32(const char *name, s64 offset, f32 minimum, f32 maximum, f32 value) : ReflectionValuePodF32(name, offset, minimum, maximum, value) {}
};

class ReflectionValueFlag : public ReflectionValue {
public:
  virtual void writeDefaultFlag(u32 &flags) const;

  u32 unknown78;  // Inferred role: flag mask; original member name unknown.
  bool unknown7c; // Inferred role: default value; original member name unknown.
};

static_assert(offsetof(ReflectionValueFlag, unknown78) == 0x78, "");
static_assert(offsetof(ReflectionValueFlag, unknown7c) == 0x7c, "");

class ReflectionValueEnumElem : public ReflectionValue {};

class ReflectionValueStruct : public ReflectionValue {
public:
  ~ReflectionValueStruct() override {
    while (!unknown78.empty()) {
      ReflectionValue *value = unknown78.begin().get();
      unknown78.pop_front();
      value->release();
    }
  }
  ReflectionValueStruct(const char *name, s64 size) : ReflectionValue(name, static_cast<ValueType>(9)), unknown90(4) { unknown68 = size; }
  void writeDefault(u8 *data) const override;
  void add(ReflectionValue *value) {
    value->addRef();
    unknown78.push_back(value);
  }

  mm::mtl::IntrusiveList<ReflectionValue> unknown78; // Inferred role: members; original member name unknown.
  s64 unknown90;                                     // Inferred role: alignment; original member name unknown.
};

static_assert(offsetof(ReflectionValueStruct, unknown78) == 0x78, "");
static_assert(offsetof(ReflectionValueStruct, unknown90) == 0x90, "");

class ReflectionValueBitset32 : public ReflectionValue {
public:
  ~ReflectionValueBitset32() override {
    while (!unknown78.empty()) {
      ReflectionValueFlag *value = unknown78.begin().get();
      unknown78.pop_front();
      value->release();
    }
  }
  void writeDefault(u8 *data) const override;

  mm::mtl::IntrusiveList<ReflectionValueFlag> unknown78; // Inferred role: flags; original member name unknown.
};

static_assert(offsetof(ReflectionValueBitset32, unknown78) == 0x78, "");

class ReflectionValueEnum : public ReflectionValue {
public:
  ~ReflectionValueEnum() override {
    while (!unknown78.empty()) {
      ReflectionValueEnumElem *value = unknown78.begin().get();
      unknown78.pop_front();
      value->release();
    }
  }
  void writeDefault(u8 *data) const override;

  mm::mtl::IntrusiveList<ReflectionValueEnumElem> unknown78; // Inferred role: elements; original member name unknown.
  int unknown90;                                             // Inferred role: default value; original member name unknown.
};

static_assert(offsetof(ReflectionValueEnum, unknown90) == 0x90, "");

class Reflection : public mm::mtl::ReferenceObject {
public:
  Reflection() : unknown10(Allocator::getDebAllocator()) {}
  ~Reflection() override {
    for (u32 i = 0; i < unknown10.size(); i++) {
      if (unknown10[i] != nullptr) {
        unknown10[i]->release();
        unknown10[i] = nullptr;
      }
    }
  }
  void setReflectionStruct(u32 index, ReflectionValueStruct *value);

  mm::mtl::Vector<ReflectionValueStruct *> unknown10; // Inferred role: reflected structs; original member name unknown.
};

static_assert(offsetof(Reflection, unknown10) == 0x10, "");

} // namespace fw
