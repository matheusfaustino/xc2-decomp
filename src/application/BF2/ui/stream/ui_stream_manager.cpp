#include "ui/stream/ui_stream_manager.h"

#include "bdat.h"
#include "framework/memory/framework_memory.h"
#include "fw/fw_render_resource.h"
#include "mm_mtl/fixstr.h"
#include "mm_mtl/hash.h"
#include "ui/ui_util.h"

namespace ui {

UIStreamManager::UIStreamManager(mm::mtl::IAllocator *allocator) : unknown08(allocator), unknown58(nullptr) {
  mm::mtl::FixStr<128> fileName;
  UIUtil::makeMenuImageFileName(fileName, "cmn_stream_dummy");
  unknown58 = fw::RenderResource::request(unknown08, fileName.c_str(), 10);
}

u32 UIStreamManager::regist(u8 *bdat) {
  for (Content *it = unknown10.begin().get(); it != unknown10.end().get(); it = static_cast<Content *>(it->unknown00)) {
    if (it->unknown18 == bdat) {
      return it->unknown10;
    }
  }
// __LINE__ is the line argument of the operator new call.
#line 223
  Content *content = new (UIUtil::getProgAllocator(), 16, __FILE__, __LINE__) Content();
  if (content != nullptr) {
    content->unknown18 = bdat;
    content->unknown10 = mm::mtl::hashStrCrc(Bdat::getSheetName(bdat));
    unknown10.push_back(content);
    return content->unknown10;
  }
  return 0;
}

u32 UIStreamManager::getContentsNum(u32 streamId) {
  Content *it = unknown10.begin().get();
  Content *end = unknown10.end().get();
  while (it != end) {
    if (it->unknown10 == streamId) {
      int top = Bdat::getIdTop(it->unknown18);
      int count = Bdat::getIdCount(it->unknown18);
      return count + top;
    }
    it = static_cast<Content *>(it->unknown00);
  }
  return 0;
}

} // namespace ui
