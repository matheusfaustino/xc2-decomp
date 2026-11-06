#include "navi/nvmsh_ac_man.h"

#include "navi/nvmsh_ac_obj_base.h"

namespace navi {

void NvmshAcMan::tail() { unknown842 = unknown842 == 0; }

void NvmshAcMan::beginUpdate() { unknown840 = unknown842 == 0; }

void NvmshAcMan::releaseObj(NvmshAcObjBase *object) { object->unknown65 |= 1; }

} // namespace navi
