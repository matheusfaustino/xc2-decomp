// Original file name unknown: the shapes have no assertion string naming their source file.
#include "wnd/wnd_shape.h" // IWYU pragma: keep

namespace mm {
struct Vec3;
} // namespace mm

namespace wnd {

BoxShape::BoxShape(const mm::Vec3 &extent) : Shape(unknown001_1), unknown10(extent) {}

} // namespace wnd
