#pragma once
#include "include/private/SkBitmaskEnum.h"

namespace skui {
enum class ModifierKey {
    kNone       = 0,
    kShift      = 1 << 0,
    kControl    = 1 << 1,
    kOption     = 1 << 2,   // same as ALT
    kCommand    = 1 << 3,
    kFirstPress = 1 << 4,
};
}  // namespace skui

namespace sknonstd {
template <> struct is_bitmask_enum<skui::ModifierKey> : std::true_type {};
}
