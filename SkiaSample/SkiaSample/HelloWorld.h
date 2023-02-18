#pragma once
#include <Windows.h>
#include "tools/sk_app/Application.h"
#include "tools/sk_app/Window.h"

class HelloWorld : sk_app::Window::Layer {
public:
    HelloWorld(HINSTANCE hinstance);
    ~HelloWorld();
    void onIdle();
    void onBackendCreated() override;
    void onPaint(SkSurface*) override;
    bool onChar(SkUnichar c, skui::ModifierKey modifiers) override;

private:
    void updateTitle();
    sk_app::Window* fWindow;
    SkScalar fRotationAngle;
};
