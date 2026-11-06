#include "dr/dr_mdo_ctl_seam.h"

#include "dr/dr_ap_cache.h"
#include "dr/dr_man.h"
#include "dr/dr_mdl_base.h"
#include "dr/dr_mdo_ctl.h"
#include "dr/dr_mdo_mat.h"
#include "dr/dr_res_mdo_mat.h"
#include "dr/dr_res_mdo_seam_coli.h"
#include "dr/dr_res_mdo_seam_res.h"
#include "mm_math/mm_col4.h"

namespace ml {

DrMdoCtlSeam::DrMdoCtlSeam(int viewNum)
    : DrMdlObj(viewNum), unknown3e0(nullptr), unknown3e8(nullptr), unknown3f0(nullptr), unknown3f8(nullptr), unknown400(nullptr), unknown408(nullptr), unknown410(nullptr), unknown418(nullptr),
      unknown630(nullptr), unknown638(nullptr), unknown658(nullptr), unknown6d8(nullptr), unknownb30(nullptr), unknownb40(nullptr), unknownc00(0), unknownc02(0), unknownc04(0), unknownc06(0),
      unknownc08(0), unknownc0a(0), unknownc0c(0), unknownc0e(0), unknownc10(0), unknownc12(0), unknownc14(0), unknownc16(0), unknownc18(0), unknownc1a(0), unknownc1c(0), unknownc1e(0), unknownc20(0),
      unknownc22(0), unknownc24(0), unknownc26(0), unknownc28(0), unknownc2e(0), unknownc2f(0), unknownc30(0), unknownc34(0), unknownc38(0), unknownc3c(0), unknownc40(0), unknownc44(0), unknownc48(0),
      unknownc4c(0), unknownc50(0), unknownc54(0), unknownc58(0), unknownc5c(0), unknownc60(0), unknownc64(0), unknownc68(0), unknownc6c(0), unknownc70(0), unknownc74(0), unknownc78(0), unknownc7c(0),
      unknownc80(0), unknownc84(0), unknownc88(1), unknownc90(0), unknownc98(0), unknownca0(0), unknownca2(0), unknownca4(0), unknownca6(0), unknowncc8(false), unknown14b0(0), unknown14c0(0),
      unknown14c8(0) {
  unknown650[0] = 0;
  unknown650[1] = 0;
  unknowncac = 0;
  for (int i = 0; i < 4; i++) {
    unknowncae[i] = -1;
    unknowncb8[i] = 0;
  }
  unknown1c = unknown037_3;
  unknown420.clear();
  unknown528.clear();
}

void *DrMdoCtlSeam::getColiListTbl() { return unknown640[DrMan::s_nDblIdx]; }

u32 DrMdoCtlSeam::getColiListMax() { return unknown650[DrMan::s_nDblIdx]; }

DrResMdoSeamColi *DrMdoCtlSeam::getColiRes(int index) { return &unknown258.unknown20[static_cast<u32>(index)]; }

u32 DrMdoCtlSeam::isRead(int index) const {
  if (((unknownc7c >> 28) & 1) == 0) {
    return unknowncd0.unknown4a & (1 << index);
  }
  return 0;
}

void DrMdoCtlSeam::setErrCode(int code, int param) {
  unknowncae[unknowncac] = code;
  unknowncb8[unknowncac] = param;
  s16 next = unknowncac + 1;
  if (next > 3) {
    next = 0;
  }
  unknowncac = next;
}

void DrMdoCtlSeam::releaseFreeSeamTexture(DrResMdoSeamTexture *texture, bool isAll) {
  if (isAll) {
    texture->release();
    unknown1b0.unknown002(texture);
  } else {
    DrResMdoSeamMemObj *mem = texture->unknown88;
    DrResMdoSeamMemObj *heightMem = texture->unknown90;
    unknownbe0[texture->unknown9e] = 0;
    texture->release();
    texture->unknown88 = nullptr;
    unknown1b0.unknown002(texture);
    if (mem != nullptr) {
      unknown6e0.releaseFreeObj(mem);
    }
    if (heightMem != nullptr) {
      unknown6e0.releaseFreeObj(heightMem);
    }
  }
}

int DrMdoCtlSeam::calcSemaAlignment(u32 size, u32 alignment) {
  u32 remainder = size % alignment;
  int padding = 0;
  if (remainder != 0) {
    padding = alignment - remainder;
  }
  return padding + size;
}

void DrMdoCtlSeam::releaseFreeSeamMemObj(DrResMdoSeamMemObj *mem) { unknown6e0.releaseFreeObj(mem); }

void DrMdoCtlSeam::releaseFreeSeamTexList(DrResMdoSeamTexList *texList, bool isAll) {
  if (isAll) {
    texList->release();
    unknown1e8.unknown002(texList);
  } else {
    DrResMdoSeamMemObj *mem = texList->unknown18;
    unknownbf0[texList->unknown3c] = 0;
    texList->release();
    texList->unknown18 = nullptr;
    unknown1e8.unknown002(texList);
    if (mem != nullptr) {
      unknown6e0.releaseFreeObj(mem);
    }
  }
}

void DrMdoCtlSeam::calcEnd() {
  bool isRenderSafe = false;
  if (unknownc7c & 0x10000) {
    unknownc7c &= ~0x8000;
    unknownc12 = 0;
  } else if (++unknownc12 > 4) {
    unknownc12 = 4;
    unknownc7c |= 0x8000;
    isRenderSafe = true;
  } else {
    unknownc7c &= ~0x8000;
  }
  if (isRenderSafe) {
    unknownc7c &= ~0x20000;
    unknownc14 = 0;
  } else {
    unknownc14++;
    if (unknownc14 >= 900) {
      unknownc14 = 900;
      unknownc7c |= 0x20000;
    } else {
      unknownc7c &= ~0x20000;
    }
  }
  if (unknownc7c & 0x20000) {
    unknownc7c |= 0x8000;
    unknownc12 = 4;
  }
  if (unknownc7c & 0x200000) {
    unknownc7c |= 0x400000;
    unknownc2a = 30;
  }
  bool isReadSafe = false;
  if (unknownc7c & 0x400) {
    unknownc7c &= ~0x800;
    unknownc0e = 0;
  } else if (++unknownc0e > 4) {
    unknownc0e = 4;
    unknownc7c |= 0x800;
    isReadSafe = true;
  } else {
    unknownc7c &= ~0x800;
  }
  if (isReadSafe) {
    unknownc7c &= ~0x100000;
    unknownc10 = 0;
  } else {
    unknownc10++;
    if (unknownc10 >= 900) {
      unknownc10 = 900;
      unknownc7c |= 0x100000;
    } else {
      unknownc7c &= ~0x100000;
    }
  }
  if (unknownc7c & 0x100000) {
    unknownc7c |= 0x800;
    unknownc0e = 4;
  }
  if (unknownc7c & 0x800) {
    m_drMan->setSeamMapStmOk(false);
  } else {
    m_drMan->setSeamMapStmOk(true);
  }
}

void DrMdoCtlSeam::updateTexutreSeam() {
  if (unknowncc8) {
    DrResMdoSeamTexture::SEAMRES_HEIGHT_CNTMAX = 3;
    DrResMdoSeamTexture::SEAMRES_NOM_CNTMAX = 10;
    DrResMdoSeamTexList::setListCnt(10);
  } else {
    if (unknownd30.unknown40 == 0) {
      DrResMdoSeamTexList::setListCnt(60);
      DrResMdoSeamTexture::SEAMRES_NOM_CNTMAX = 120;
    } else {
      DrResMdoSeamTexList::setListCnt(10);
      DrResMdoSeamTexture::SEAMRES_NOM_CNTMAX = 10;
    }
    if (unknownc7c & 0x20) {
      DrResMdoSeamTexture::SEAMRES_HEIGHT_CNTMAX = 3;
    } else {
      DrResMdoSeamTexture::SEAMRES_HEIGHT_CNTMAX = 60;
    }
  }
  DrResMdoSeamTexture *texture = unknown1b0.unknown10.unknown00;
  if (unknowncc8) {
    while (texture != nullptr) {
      DrResMdoSeamTexture *next = texture->unknown08;
      releaseFreeSeamTexture(texture, false);
      texture = next;
    }
    unknownc34 = 0;
  } else {
    while (texture != nullptr) {
      DrResMdoSeamTexture *next;
      if (texture->update()) {
        next = texture->unknown08;
        const MdsSeamOfsTexTblData *table = reinterpret_cast<const MdsSeamOfsTexTblData *>(reinterpret_cast<const u8 *>(unknown1a8) + unknown1a8->unknown5c);
        unknownc34 -= table[texture->unknown9e].unknown04;
        if (texture->unknowna3 == 1) {
          unknownc34 -= table[texture->unknown9e].unknown0c;
        }
        releaseFreeSeamTexture(texture, false);
      } else {
        if (texture->unknowna1) {
          const MdsSeamOfsTexTblData *table = reinterpret_cast<const MdsSeamOfsTexTblData *>(reinterpret_cast<const u8 *>(unknown1a8) + unknown1a8->unknown5c);
          const MdsSeamOfsTexTblData *data = &table[texture->unknown9e];
          if (texture->unknowna3 == 1) {
            texture->changeMemTexture(calcSemaAlignment(data->unknown0c, 0x1000), data->unknown04);
          } else {
            texture->changeMemTexture(data->unknown04);
          }
        } else if (texture->unknowna3 == 1 && texture->updateHeight()) {
          const MdsSeamOfsTexTblData *table = reinterpret_cast<const MdsSeamOfsTexTblData *>(reinterpret_cast<const u8 *>(unknown1a8) + unknown1a8->unknown5c);
          texture->unknowna3 = 0;
          const MdsSeamOfsTexTblData *data = &table[texture->unknown9e];
          u32 heightSize = data->unknown0c;
          int alignedSize = calcSemaAlignment(heightSize, 0x1000);
          unknownc34 -= heightSize;
          DrResMdoSeamMemObj *mem = texture->unknown88;
          texture->unknown88 = unknown6e0.createParentFreeObj(mem, alignedSize);
          texture->resetTexture(data->unknown04);
          unknown6e0.releaseFreeObj(mem);
        }
        next = texture->unknown08;
      }
      texture = next;
    }
  }
  DrResMdoSeamTexList *texList = unknown1e8.unknown10.unknown00;
  if (unknowncc8) {
    while (texList != nullptr) {
      DrResMdoSeamTexList *next = texList->unknown08;
      releaseFreeSeamTexList(texList, false);
      texList = next;
    }
    unknownc34 = 0;
  } else {
    while (texList != nullptr) {
      bool isEnd = texList->update(&unknownd30);
      DrResMdoSeamTexList *next = texList->unknown08;
      if (isEnd) {
        const unknown136 *table = reinterpret_cast<const unknown136 *>(reinterpret_cast<const u8 *>(unknown1a8) + unknown1a8->unknown94);
        unknownc34 -= table[texList->unknown3c].unknown04;
        releaseFreeSeamTexList(texList, false);
      }
      texList = next;
    }
  }
}

void DrMdoCtlSeam::releaseFreeSeamVertex(DrResMdoSeamVertex *vertex, bool isAll) {
  if (isAll) {
    vertex->release();
    unknown220.unknown002(vertex);
  } else {
    DrResMdoSeamMemObj *mem = vertex->unknown18;
    unknownbd0[vertex->unknown2c] = 0;
    vertex->release();
    vertex->unknown18 = nullptr;
    unknown220.unknown002(vertex);
    if (mem != nullptr) {
      unknown6e0.releaseFreeObj(mem);
    }
  }
}

DrResMdoSeamMemObj *DrMdoCtlSeam::createFreeSeamMemObj4K(u32 size) {
  u32 alignedSize = calcSemaAlignment(size, 0x1000);
  return unknown6e0.createFreeObj(alignedSize);
}

DrResMdoSeamMemObj *DrMdoCtlSeam::createFreeSeamMemObjBack4K(u32 size) {
  u32 alignedSize = calcSemaAlignment(size, 0x1000);
  return unknown6e0.createBackFreeObj(alignedSize);
}

DrResMdoSeamTexture *DrMdoCtlSeam::createFreeSeamTexture() { return unknown1b0.unknown001(); }

DrResMdoSeamTexList *DrMdoCtlSeam::createFreeSeamTexList() { return unknown1e8.unknown001(); }

DrResMdoSeamVertex *DrMdoCtlSeam::createFreeSeamVertex() { return unknown220.unknown001(); }

DrResMdoSeamColi *DrMdoCtlSeam::createFreeSeamCollision() { return unknown258.unknown001(); }

void DrMdoCtlSeam::seamZPrePassBegin() {
  unknownf2 = 0;
  unknown001();
  m_drMan->unknown5c0->setupCulling(DR_AP_CULL_TYPE_OFF);
}

void DrMdoCtlSeam::seamZPrePassEnd() {}

void DrMdoCtlSeam::seamThicknessPassBegin(bool isFirst) {
  unknownf2 = 0;
  unknown001();
  if (isFirst) {
    m_drMan->unknown5c0->beginThicknessRenderFirst();
  } else {
    m_drMan->unknown5c0->beginThicknessRenderSecond();
  }
  m_drMan->unknown5c0->setupCulling(isFirst ? DR_AP_CULL_TYPE_FRONT : DR_AP_CULL_TYPE_BACK);
}

void DrMdoCtlSeam::seamThicknessPassEnd(bool value) {}

void DrMdoCtlSeam::calcSeamMapRender(int renderType, u32 flags) {
  unknownf4 = renderType;
  unknownf2 = unknown098[renderType];
  beginSetup();
  const DrResMdoMat *mats = unknown30->unknownc0;
  const MdsMdlTopHeader *entry = unknown30->unknown168;
  for (int i = 0; i < unknown30->unknown170; i++, entry++) {
    const MdsMdlInfoHeader *header = reinterpret_cast<const MdsMdlInfoHeader *>(unknown30->unknown18 + entry->unknown00);
    int count = entry->unknown04;
    for (int j = 0; j < count; j++, header++) {
      if ((header->unknown04 & flags) != 0 && mats[header->unknown0e].unknown08.a > 0.0f) {
        partsDefRender(header, 0, 0, false);
      }
    }
  }
}

} // namespace ml

// The game builds these files in one unit with this one, in this order.
// clang-format off
#include "dr_mdo_ctl.cpp"
#include "dr_mdo_setup.cpp"
#include "dr_mdo_mat.cpp"
#include "dr_mdo_zsort.cpp"
#include "dr_mdo_ctl_nom.cpp"
#include "dr_mdo_ctl_pos.cpp"
#include "dr_mdo_ctl_seam_all.cpp"
#include "dr_mdo_ctl_seam_base_map.cpp"
#include "dr_mdo_ctl_seam_env_map.cpp"
#include "dr_mdo_ctl_seam_inst_map.cpp"
#include "dr_mdo_ctl_seam_light.cpp"
#include "dr_mdo_ctl_seam_low_map.cpp"
#include "dr_mdo_ctl_seam_mpf_map.cpp"
#include "dr_mdo_ctl_seam_read.cpp"
// clang-format on
