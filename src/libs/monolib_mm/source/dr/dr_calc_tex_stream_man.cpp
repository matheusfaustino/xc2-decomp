#include "dr/dr_calc_tex_stream_man.h"

#include "ml/critical_section.h"
#include "ml/work_cpu.h"

namespace ml {

bool DrCalcTexStreamMan::callReadType(DrTxtmType type) {
  if (unknown2490 < type) {
    return false;
  }
  unknown2490 = type;
  return unknown2494 >= type;
}

CriticalSection &DrCalcTexStreamMan::getCriticalSection() {
  static CriticalSection sect;
  return sect;
}

int DrCalcTexStreamMan::getUseMax() const {
  const DrCalcTexStmObj *obj = unknown08.unknown00;
  if (obj != nullptr) {
    int count = 0;
    do {
      count += obj->unknown24 & 1;
      obj = obj->unknown10;
    } while (obj != nullptr);
    return count;
  }
  return 0;
}

int DrCalcTexStreamMan::getFreeMax() const {
  const DrCalcTexStmObj *obj = unknown08.unknown00;
  if (obj != nullptr) {
    int count = 0;
    do {
      count += ~obj->unknown24 & 1;
      obj = obj->unknown10;
    } while (obj != nullptr);
    return count;
  }
  return 0;
}

int DrCalcTexStreamMan::getAllFreeSize() const {
  const DrCalcTexStmObj *obj = unknown08.unknown00;
  if (obj != nullptr) {
    int size = 0;
    do {
      if ((obj->unknown24 & 1) == 0) {
        size += obj->unknown20;
      }
      obj = obj->unknown10;
    } while (obj != nullptr);
    return size;
  }
  return 0;
}

int DrCalcTexStreamMan::getAllUseSize() const {
  const DrCalcTexStmObj *obj = unknown08.unknown00;
  if (obj != nullptr) {
    int size = 0;
    do {
      if ((obj->unknown24 & 1) != 0) {
        size += obj->unknown20;
      }
      obj = obj->unknown10;
    } while (obj != nullptr);
    return size;
  }
  return 0;
}

DrCalcTexStmObj *DrCalcTexStreamMan::getTopAheadFreeObj() {
  DrCalcTexStmObj *obj = unknown08.unknown00;
  while (obj != nullptr && (obj->unknown24 & 1) != 0) {
    obj = obj->unknown10;
  }
  return obj;
}

void DrCalcTexStreamMan::entryJob() { WorkCpu::wakeup(unknown2498); }

u32 DrCalcTexStreamMan::jobUpdate(void *argument, void *userData) {
  static_cast<DrCalcTexStreamMan *>(argument)->update();
  return 0;
}

void DrCalcTexStreamMan::calcPieceMem(int &pieceCount, u32 &freeSize, u32 &objectCount) {
  pieceCount = 0;
  freeSize = 0;
  objectCount = 0;
  int state = -1;
  for (DrCalcTexStmObj *obj = unknown08.unknown00; obj != nullptr; obj = obj->unknown10) {
    if ((obj->unknown24 & 1) == 0) {
      freeSize += obj->unknown20;
      if (state == 1) {
        pieceCount++;
      }
      state = 0;
    } else {
      if (state == 0) {
        pieceCount++;
      }
      state = 1;
    }
    objectCount++;
  }
}

void DrCalcTexStreamMan::cntMemUpdate() {
  if (unknown2068 & 1) {
    if (unknown2480++ > 0) {
      unknown2480 = 0;
      unknown2068 &= ~1;
      for (int i = 0; i < unknown2484; i++) {
        if (unknown2080[i] != nullptr) {
          clearMemObj(unknown2080[i]);
          unknown2080[i] = nullptr;
        }
      }
      unknown2484 = 0;
    } else {
      int count = 0;
      for (int i = 0; i < unknown2484; i++) {
        if (unknown2080[i] == nullptr) {
          count++;
        } else if ((unknown2080[i]->unknown24 & 2) == 0) {
          clearMemObj(unknown2080[i]);
          unknown2080[i] = nullptr;
          count++;
        }
      }
      if (count == unknown2484) {
        unknown2480 = 0;
        unknown2484 = 0;
        unknown2068 &= ~1;
      }
    }
  }
}

bool DrCalcTexStreamMan::memSeachOptimization(u32 size) {
  bool isFree = false;
  bool isFound = false;
  for (DrCalcTexStmObj *obj = unknown08.unknown00; obj != nullptr; obj = obj->unknown10) {
    if ((obj->unknown24 & 1) == 0) {
      isFree = true;
    } else if (isFree && (obj->unknown24 & 0x30) == 0) {
      unknown2280[unknown2484++] = obj;
      isFound = true;
      if (unknown2484 >= 64) {
        return true;
      }
    }
  }
  return isFound;
}

DrCalcTexStmObj *DrCalcTexStreamMan::createAheadFreeObj(u32 size) {
  for (DrCalcTexStmObj *obj = unknown08.unknown00; obj != nullptr; obj = obj->unknown10) {
    if ((obj->unknown24 & 1) == 0 && static_cast<u32>(obj->unknown20) >= size) {
      if (!createSizeFreeObj(obj, size)) {
        return nullptr;
      }
      return obj;
    }
  }
  return nullptr;
}

bool DrCalcTexStreamMan::createSizeFreeObj(DrCalcTexStmObj *object, u32 size) {
  int rest = object->unknown20 - size;
  if (rest == 0) {
    object->unknown24 = 0;
  } else {
    DrCalcTexStmObj *child = unknown18.unknown00;
    if (child == nullptr) {
      return false;
    }
    object->unknown20 = size;
    child->unknown18 = object->unknown18 + size;
    child->unknown20 = rest;
    unknown18.unknown001(child);
    child->unknown24 = 0;
    unknown08.unknown003(object, child);
  }
  object->unknown24 |= 1;
  return true;
}

DrCalcTexStmObj *DrCalcTexStreamMan::createChildObj(DrCalcTexStmObj *parent, u32 size) {
  if (static_cast<u32>(parent->unknown20) < size) {
    return nullptr;
  }
  DrCalcTexStmObj *child = unknown18.unknown00;
  if (child == nullptr) {
    return nullptr;
  }
  unknown18.unknown001(child);
  int parentSize = parent->unknown20;
  parent->unknown20 = size;
  child->unknown20 = parentSize - size;
  child->unknown18 = parent->unknown18 + size;
  child->unknown24 = 1;
  unknown08.unknown003(parent, child);
  return child;
}

void DrCalcTexStreamMan::releaseFreeObj(DrCalcTexStmObj *&object) {
  if (object != nullptr) {
    clearMemObj(object);
    object = nullptr;
  }
}

void DrCalcTexStreamMan::jobWait() { WorkCpu::sync(unknown2498, WKCPU_SYNC_NONE, CPUID_ANY); }

void DrCalcTexStreamMan::callHlvTexOptimization() { unknown248c = 0xa0; }

void DrCalcTexStreamMan::callOptimization(u32 value) { unknown2488 += value; }

void DrCalcTexStreamMan::callReadStopMode() {
  unknown2068 |= 0x20;
  unknown248c = 0xa0;
  unknown248e = 0x000a;
}

} // namespace ml

template mm::mtl::PtrSingleton<ml::DrCalcTexStreamMan>::~PtrSingleton();
template void ml::DrResChankList<ml::DrCalcTexStmObj>::changeList(ml::DrCalcTexStmObj *first, ml::DrCalcTexStmObj *second);
