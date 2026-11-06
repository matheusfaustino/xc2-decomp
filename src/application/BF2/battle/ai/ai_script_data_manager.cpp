// Original file name inferred from the ai manager sources: the game calls AiScriptDataManager out of line.
#include "battle/ai/ai_manager.h"
#include "bdat.h"
#include "gf/data/gf_data_bdat.h"
#include "mm_mtl/fixed_list.h"
#include "types.h"

namespace ai {

void AiScriptDataManager::_init() {
  u8 *bdat = gf::GfDataBdat::getFP(0);
  int top = Bdat::getIdTop(bdat);
  int count = Bdat::getIdCount(bdat);
  int num = count < static_cast<int>(SCRIPT_COUNT) ? count : static_cast<int>(SCRIPT_COUNT);
  for (int i = 0; i < num; i++) {
    int id = top + i;
    const char *name = reinterpret_cast<const char *>(Bdat::getValCheck(bdat, "Script", id, 7));
    if (name == nullptr || name[0] == '\0') {
      continue;
    }
    ScriptData data;
    data.unknown00 = id;
    data.unknown08 = name;
    unknown08.push_back(data);
  }
}

} // namespace ai
