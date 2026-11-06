#include "dr/dr_mdo_node.h"

#include "dr/dr_man.h"
#include "mm_math/mm_mat44.h"

namespace ml {

const mm::Mat44 &DrMdoNode::getNodeMatrix(int index) const { return unknown60[DrMan::s_nDblStockIdx][index]; }

mm::Mat44 *DrMdoNode::getNodeMatrixPtr(int index) { return &unknown60[DrMan::s_nDblStockIdx][index]; }

} // namespace ml
