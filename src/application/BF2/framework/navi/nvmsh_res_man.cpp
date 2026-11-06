#include "navi/nvmsh_res_man.h"

namespace navi {

void NvmshResMan::tail() {
  u16 value = unknown16ec == 0;
  u16 inverse = unknown16ec != 0;
  unknown16ec = value;
  unknown16ea = inverse;
}

} // namespace navi
