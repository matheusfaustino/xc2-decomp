// No game string names this file: named after its header, like the other dr_pixl_* sources of this directory.
#include "dr/dr_pixl_mem_man.h"

namespace ml {

DrPixlMemMan::~DrPixlMemMan() {}

} // namespace ml

template mm::mtl::PtrSingleton<ml::DrPixlMemMan>::~PtrSingleton();
