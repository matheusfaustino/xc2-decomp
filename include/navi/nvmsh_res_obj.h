#pragma once
#include <stddef.h>

#include "types.h"

namespace navi {

class NvmshResMemMan;
class NvmshResMemObj;

// Header of a loaded .nvmh resource; only the offsets NvmshResObj::createNavi reads are known.
struct _NvMhAllReadObj {
  u32 unknown00; // Inferred role: resource flags, bit 0 and bit 1 gate the two data blocks; original member name unknown.
  u8 _04[0x0c - 0x04];
  u32 unknown0c; // Inferred role: offset of the second data block; original member name unknown.
  u8 _10[0x28 - 0x10];
  u32 unknown28;
  u8 _2c[0x30 - 0x2c];
  u32 unknown30;
  u8 _34[0x40 - 0x34];
  u32 unknown40;
  u32 unknown44;
  u32 unknown48;
  u32 unknown4c;
  u32 unknown50;
  u32 unknown54;
  u8 _58[0x6c - 0x58];
  u32 unknown6c;
  u32 unknown70;
  u32 unknown74;
};

static_assert(offsetof(_NvMhAllReadObj, unknown00) == 0x00, "");
static_assert(offsetof(_NvMhAllReadObj, unknown0c) == 0x0c, "");
static_assert(offsetof(_NvMhAllReadObj, unknown28) == 0x28, "");
static_assert(offsetof(_NvMhAllReadObj, unknown30) == 0x30, "");
static_assert(offsetof(_NvMhAllReadObj, unknown40) == 0x40, "");
static_assert(offsetof(_NvMhAllReadObj, unknown48) == 0x48, "");
static_assert(offsetof(_NvMhAllReadObj, unknown50) == 0x50, "");
static_assert(offsetof(_NvMhAllReadObj, unknown54) == 0x54, "");
static_assert(offsetof(_NvMhAllReadObj, unknown6c) == 0x6c, "");
static_assert(offsetof(_NvMhAllReadObj, unknown74) == 0x74, "");

// Holds the pointers into one navigation mesh resource file.
class NvmshResObj {
public:
  NvmshResObj();
  ~NvmshResObj();
  void releaseNavi();
  void resetNavi();
  void createNavi(const u8 *data);

  u32 unknown00; // Inferred role: flags, bit 0 set once the resource is created; original member name unknown.
  u8 _04[4];
  const u8 *unknown08; // Inferred role: resource data start; original member name unknown.
  const u8 *unknown10; // Inferred role: world header; original member name unknown.
  const u8 *unknown18; // Inferred role: mesh table; original member name unknown.
  const u8 *unknown20; // Inferred role: polygon table; original member name unknown.
  const u8 *unknown28; // Inferred role: read from the header offset at 0x54; original member name unknown.
  const u8 *unknown30; // Inferred role: read from the header offset at 0x48; original member name unknown.
  const u8 *unknown38; // Inferred role: read from the header offset at 0x6c; original member name unknown.
  const u8 *unknown40; // Inferred role: read from the header offset at 0x74; original member name unknown.
  const u8 *unknown48; // Inferred role: read from the header offset at 0x40; original member name unknown.
  const u8 *unknown50; // Inferred role: read from the header offset at 0x50; original member name unknown.
};

static_assert(offsetof(NvmshResObj, unknown08) == 0x08, "");
static_assert(offsetof(NvmshResObj, unknown10) == 0x10, "");
static_assert(offsetof(NvmshResObj, unknown50) == 0x50, "");
static_assert(sizeof(NvmshResObj) == 0x58, "");

// Holds the state of one loaded navigation mesh resource.
class NvmshResData {
public:
  NvmshResData();
  ~NvmshResData();
  void setReadData(const _NvMhAllReadObj *readData, int offset);
  void releaseCall();
  void releaseNavi(NvmshResMemMan *memMan);

  u32 unknown00; // Inferred role: state flags, bit 2 set when the resource is released; original member name unknown.
  u16 unknown04;
  u8 _06[2];
  const _NvMhAllReadObj *unknown08; // Inferred role: read object; original member name unknown.
  u32 unknown10;
  u8 _14[4];
  const u8 *unknown18; // Inferred role: start of the first data block; original member name unknown.
  const u8 *unknown20;
  const u8 *unknown28;
  const u8 *unknown30;
  const u8 *unknown38;
  const u8 *unknown40;
  const u8 *unknown48;
  const u8 *unknown50;
  const u8 *unknown58;
  const u8 *unknown60;
  u32 unknown68; // Inferred role: bit 0 set once the second data block is created; original member name unknown.
  u8 _6c[4];
  const u8 *unknown70; // Inferred role: start of the second data block; original member name unknown.
  const u8 *unknown78;
  const u8 *unknown80;
  const u8 *unknown88;
  const u8 *unknown90;
  const u8 *unknown98;
  const u8 *unknowna0;
  const u8 *unknowna8;
  const u8 *unknownb0;
  const u8 *unknownb8;
  int unknownc0; // Inferred role: read data offset; original member name unknown.
  u8 _c4[4];
  NvmshResMemObj *unknownc8; // Inferred role: allocated memory object; original member name unknown.
};

static_assert(offsetof(NvmshResData, unknown04) == 0x04, "");
static_assert(offsetof(NvmshResData, unknown08) == 0x08, "");
static_assert(offsetof(NvmshResData, unknown10) == 0x10, "");
static_assert(offsetof(NvmshResData, unknown18) == 0x18, "");
static_assert(offsetof(NvmshResData, unknown60) == 0x60, "");
static_assert(offsetof(NvmshResData, unknown78) == 0x78, "");
static_assert(offsetof(NvmshResData, unknownb8) == 0xb8, "");
static_assert(offsetof(NvmshResData, unknown68) == 0x68, "");
static_assert(offsetof(NvmshResData, unknown70) == 0x70, "");
static_assert(offsetof(NvmshResData, unknownc0) == 0xc0, "");
static_assert(offsetof(NvmshResData, unknownc8) == 0xc8, "");
static_assert(sizeof(NvmshResData) == 0xd0, "");

} // namespace navi
