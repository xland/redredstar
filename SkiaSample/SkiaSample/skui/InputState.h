#pragma once
namespace skui {
enum class InputState {
    kDown,
    kUp,
    kMove,   // only valid for mouse
    kRight,  // only valid for fling
    kLeft,   // only valid for fling
};
}
