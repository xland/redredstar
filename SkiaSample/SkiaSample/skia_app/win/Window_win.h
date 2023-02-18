#pragma once
#include "tools/sk_app/Window.h"

#include <windows.h>

namespace sk_app {

class Window_win : public Window {
public:
    Window_win() : Window() {}
    ~Window_win() override;

    bool init(HINSTANCE instance);

    void setTitle(const char*) override;
    void show() override;

    bool attach() override;

    void onInval() override;

    void setRequestedDisplayParams(const DisplayParams&, bool allowReattach) override;

private:
    void closeWindow();

    HINSTANCE   fHInstance;
    HWND        fHWnd;
    bool        fInitializedBackend = false;

    using INHERITED = Window;
};

}
