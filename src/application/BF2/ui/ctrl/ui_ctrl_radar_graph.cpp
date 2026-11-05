#include "ui/ctrl/ui_ctrl_radar_graph.h"

#include <math.h>

#include "fw/fw_update_info.h"
#include "layer/layer_acc_poly_obj.h"
#include "mm_math/mm_math.h"
#include "mm_mtl/rtti.h"
#include "ui/plugin/ui_object_number_plugin.h"
#include "ui/shape/ui_shape_simple_face.h"
#include "ui/ui_object.h"
#include "ui/ui_object_acc.h"
#include "ui/ui_util.h"

namespace ui {

UICtrlRadarGraph::UICtrlRadarGraph(u32 object) : UICtrlObject(object), unknown30(), unknown48() { unknown30.setAllocator(UIUtil::getProgAllocator()); }

UICtrlRadarGraph::~UICtrlRadarGraph() {
  if (unknown30.size() != 0) {
    unknown30.destroy();
  }
}

void UICtrlRadarGraph::setNumberPlugin(u32 index, u32 handle) {
  if (index < unknown30.size()) {
    UIObject *object = UIUtil::getObject(handle);
    if (object != nullptr && object->unknowne0 != nullptr) {
      UIObjectNumberPlugin *plugin = mm::mtl::dynamicCast<UIObjectNumberPlugin>(object->unknowne0);
      if (plugin != nullptr) {
        unknown30[index].unknown00 = plugin->unknown7c;
        unknown30[index].unknown08 = plugin;
      }
    }
  }
}

void UICtrlRadarGraph::update(const fw::UpdateInfo &info) {
  UIObjectAcc acc(unknown20);
  if (acc.isValid()) {
    UIShapeSimpleFace *shape = mm::mtl::dynamicCast<UIShapeSimpleFace>(acc.getShape());
    u32 vertexNum = shape->unknown08.size();
    if (vertexNum != 0) {
      s16 width;
      s16 height;
      layer::LayerPolyVertex vertex;
      vertex.unknown0c = 0.0f;
      vertex.unknown08 = 0xff;
      vertex.unknown09 = 0xff;
      vertex.unknown0a = 0xff;
      vertex.unknown0b = 0xff;
      vertex.unknown10 = 0.0f;
      acc.getSize(false, width, height);
      if (unknown30.size() != 0) {
        u32 step = 360 / vertexNum;
        f32 angle = 1.5707964f;
        for (u32 i = 0; i < unknown30.size(); i++) {
          Value &value = unknown30[i];
          if (value.unknown10.isBusy()) {
            value.unknown10.update(info.unknown04);
            value.unknown00 = value.unknown10.values[0];
          } else {
            value.unknown00 = value.unknown08->unknown7c;
          }
          f32 rate = static_cast<f32>(value.unknown00) / static_cast<f32>(value.unknown08->unknown84);
          f32 radius = rate == 0.0f ? 0.05f : rate;
          f32 x = cosf(angle) * radius;
          f32 y = sinf(angle) * radius;
          vertex.unknown00 = x;
          vertex.unknown04 = -y;
          shape->writeFaceVertex(&vertex, 1, i);
          angle = step * mm::DEG_TO_RAD + angle;
        }
      }
    }
  }
}

} // namespace ui
