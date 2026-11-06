#include "dr/dr_mdo_mpf_man.h"

#include <string.h>

#include "dr/dr_mpf_work_auto_inf.h"
#include "grlib/cglib_ctl_vertex.h"
#include "mtl/mem_manager.h"

namespace ml {

DrMdoMpfMan::~DrMdoMpfMan() {
  unknown18.release();
  unknown60.release();
  unknown98.release();
  if (unknown3d28 != nullptr) {
    mtl::MemManager::deallocate(unknown3d28, 0, false);
    unknown3d28 = nullptr;
  }
}

void DrMdoMpfMan::begin() {
  unknown3d24 = 1.0f;
  unknown3d20 = 0;
  unknownd0 = 0;
}

void DrMdoMpfMan::beginBitTbl(int num) {
  int size = num + (0x20 - num % 0x20) % 0x20;
  memset(unknown3d00, 0, size);
}

int DrMdoMpfMan::getMpfWindMax() { return 0x40; }

void DrMdoMpfMan::updateInstanceBuff(u32 offset, u32 size) { unknown98.updateData(offset, size); }

u32 DrMdoMpfMan::advanceInstanceBuffOsf(u32 size) {
  u32 offset = unknownd0 + size;
  if ((offset & 0x1f) != 0) {
    offset = offset + (-offset & 0x1f);
  }
  unknownd0 = offset;
  return offset;
}

void DrMdoMpfMan::bindInstanceBuffAsVertex(u32 stream, u32 offset, u32 stride, u32 divisor) { grlib::CGLibCtlVertex::setUniformVertex(unknown98, stride, offset, stream, divisor); }

void DrMdoMpfMan::bindInstanceBuffAsSSBO(u32 index, u32 offset, u32 size) { unknown98.setStorageBuffer(index, offset, size); }

void DrMdoMpfMan::addMpfObj() { unknown08++; }

bool DrMdoMpfMan::delMpfObj() {
  int count = unknown08;
  unknown08 = count - 1;
  return count < 2;
}

u8 *DrMdoMpfMan::getTempMemTbl(int size) {
  u8 *table = unknown3d10 + unknown3d20;
  unknown3d20 += size;
  return table;
}

void DrMdoMpfMan::releaseList(_MPFWorkAutoInf *work) {
  _MPFWorkAutoInf *first = work->unknown28;
  _MPFWorkAutoInf *second = work->unknown30;
  if (first == nullptr) {
    if (second != nullptr) {
      unknowne0 = second;
      second->unknown28 = nullptr;
      return;
    }
    unknownd8 = nullptr;
    unknowne0 = nullptr;
    return;
  }
  if (second != nullptr) {
    first->unknown30 = second;
    work->unknown30->unknown28 = work->unknown28;
    return;
  }
  unknownd8 = first;
  first->unknown30 = nullptr;
}

void DrMdoMpfMan::debugListCheck(_MPFWorkAutoInf *work, int type) {
  switch (type) {
  case 0:
    for (_MPFWorkAutoInf *current = unknownd8; current != nullptr && current != work; current = current->unknown28) {
    }
    for (_MPFWorkAutoInf *current = unknowne0; current != nullptr && current != work; current = current->unknown30) {
    }
    break;
  case 2:
    for (_MPFWorkAutoInf *current = unknowne8; current != nullptr; current = current->unknown38) {
      if (current == work) {
        break;
      }
      if (current->unknown38 == work) {
        break;
      }
    }
    break;
  case 3: {
    _MPFWorkAutoInf *current = unknowne8;
    if (current != nullptr) {
      while (current->unknown38 != nullptr) {
        current = current->unknown38;
      }
    }
    break;
  }
  }
}

void DrMdoMpfMan::freeMemWork(_MPFWorkAutoInf *work) {
  bool isTail = true;
  _MPFWorkAutoInf *preceding = work->unknown40;
  if (preceding != nullptr && (preceding->unknown02 & 2) == 0) {
    preceding->unknown1c += work->unknown1c;
    _MPFWorkAutoInf *next = work->unknown38;
    preceding->unknown38 = next;
    if (next != nullptr) {
      next->unknown40 = preceding;
    }
    memset(work, 0, 0x58);
    isTail = false;
    work = preceding;
  }
  _MPFWorkAutoInf *following = work->unknown38;
  if (following != nullptr && (following->unknown02 & 2) == 0) {
    work->unknown1c += following->unknown1c;
    _MPFWorkAutoInf *next = following->unknown38;
    work->unknown38 = next;
    if (next != nullptr) {
      next->unknown40 = work;
    }
    releaseList(following);
  }
  if (isTail) {
    work->unknown02 = 0;
    if (unknowne0 == nullptr) {
      unknownd8 = work;
      work->unknown30 = nullptr;
    } else {
      work->unknown30 = unknowne0;
      unknowne0->unknown28 = work;
    }
    work->unknown28 = nullptr;
    unknowne0 = work;
  }
}

} // namespace ml
