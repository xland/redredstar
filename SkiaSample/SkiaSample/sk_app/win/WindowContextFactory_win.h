#pragma once

#include <Windows.h>

#include <memory>

namespace sk_app {

class WindowContext;
struct DisplayParams;

namespace window_context_factory {

std::unique_ptr<WindowContext> MakeVulkanForWin(HWND, const DisplayParams&);

std::unique_ptr<WindowContext> MakeGLForWin(HWND, const DisplayParams&);

std::unique_ptr<WindowContext> MakeANGLEForWin(HWND, const DisplayParams&);

std::unique_ptr<WindowContext> MakeRasterForWin(HWND, const DisplayParams&);

}  // namespace window_context_factory

}  // namespace sk_app
