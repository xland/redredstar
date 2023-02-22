#include "Window.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkSurface.h"
#include "src/utils/SkUTF.h"
#include "WindowContext.h"
#include <tchar.h>
#include <windows.h>
#include <windowsx.h>

namespace sk_app {

    LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


Window::Window(HINSTANCE hInstance, const DisplayParams& params):fHInstance{ hInstance },fRequestedDisplayParams{ params }
{
    // The main window class name
    static const TCHAR gSZWindowClass[] = _T("SkiaApp");

    static WNDCLASSEX wcex;
    static bool wcexInit = false;
    if (!wcexInit) {
        wcex.cbSize = sizeof(WNDCLASSEX);

        wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
        wcex.lpfnWndProc = WndProc;
        wcex.cbClsExtra = 0;
        wcex.cbWndExtra = 0;
        wcex.hInstance = fHInstance;
        wcex.hIcon = LoadIcon(fHInstance, (LPCTSTR)IDI_WINLOGO);
        wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wcex.lpszMenuName = nullptr;
        wcex.lpszClassName = gSZWindowClass;
        wcex.hIconSm = LoadIcon(fHInstance, (LPCTSTR)IDI_WINLOGO);

        if (!RegisterClassEx(&wcex)) {
            return;
        }
        wcexInit = true;
    }

    /*
     if (fullscreen)
     {
         DEVMODE dmScreenSettings;
         // If full screen set the screen to maximum size of the users desktop and 32bit.
         memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
         dmScreenSettings.dmSize = sizeof(dmScreenSettings);
         dmScreenSettings.dmPelsWidth = (unsigned long)width;
         dmScreenSettings.dmPelsHeight = (unsigned long)height;
         dmScreenSettings.dmBitsPerPel = 32;
         dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

         // Change the display settings to full screen.
         ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

         // Set the position of the window to the top left corner.
         posX = posY = 0;
     }
     */
     //   gIsFullscreen = fullscreen;

    fHWnd = CreateWindow(gSZWindowClass, nullptr, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        nullptr, nullptr, fHInstance, nullptr);
    if (!fHWnd)
    {
        return;
    }

    SetWindowLongPtr(fHWnd, GWLP_USERDATA, (LONG_PTR)this);
    RegisterTouchWindow(fHWnd, 0);

    return;
}

static skui::Key get_key(WPARAM vk) {
    static const struct {
        WPARAM      fVK;
        skui::Key fKey;
    } gPair[] = {
        { VK_BACK,    skui::Key::kBack     },
        { VK_CLEAR,   skui::Key::kBack     },
        { VK_RETURN,  skui::Key::kOK       },
        { VK_UP,      skui::Key::kUp       },
        { VK_DOWN,    skui::Key::kDown     },
        { VK_LEFT,    skui::Key::kLeft     },
        { VK_RIGHT,   skui::Key::kRight    },
        { VK_TAB,     skui::Key::kTab      },
        { VK_PRIOR,   skui::Key::kPageUp   },
        { VK_NEXT,    skui::Key::kPageDown },
        { VK_HOME,    skui::Key::kHome     },
        { VK_END,     skui::Key::kEnd      },
        { VK_DELETE,  skui::Key::kDelete   },
        { VK_ESCAPE,  skui::Key::kEscape   },
        { VK_SHIFT,   skui::Key::kShift    },
        { VK_CONTROL, skui::Key::kCtrl     },
        { VK_MENU,    skui::Key::kOption   },
        { 'A',        skui::Key::kA        },
        { 'C',        skui::Key::kC        },
        { 'V',        skui::Key::kV        },
        { 'X',        skui::Key::kX        },
        { 'Y',        skui::Key::kY        },
        { 'Z',        skui::Key::kZ        },
    };
    for (size_t i = 0; i < SK_ARRAY_COUNT(gPair); i++) {
        if (gPair[i].fVK == vk) {
            return gPair[i].fKey;
        }
    }
    return skui::Key::kNONE;
}

static skui::ModifierKey get_modifiers(UINT message, WPARAM wParam, LPARAM lParam) {
    skui::ModifierKey modifiers = skui::ModifierKey::kNone;

    switch (message) {
    case WM_UNICHAR:
    case WM_CHAR:
        if (0 == (lParam & (1 << 30))) {
            modifiers |= skui::ModifierKey::kFirstPress;
        }
        if (lParam & (1 << 29)) {
            modifiers |= skui::ModifierKey::kOption;
        }
        break;

    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
        if (0 == (lParam & (1 << 30))) {
            modifiers |= skui::ModifierKey::kFirstPress;
        }
        if (lParam & (1 << 29)) {
            modifiers |= skui::ModifierKey::kOption;
        }
        break;

    case WM_KEYUP:
    case WM_SYSKEYUP:
        if (lParam & (1 << 29)) {
            modifiers |= skui::ModifierKey::kOption;
        }
        break;

    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_MOUSEMOVE:
    case WM_MOUSEWHEEL:
        if (wParam & MK_CONTROL) {
            modifiers |= skui::ModifierKey::kControl;
        }
        if (wParam & MK_SHIFT) {
            modifiers |= skui::ModifierKey::kShift;
        }
        break;
    }

    return modifiers;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;

    Window* window = (Window*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

    bool eventHandled = false;

    switch (message) {
    case WM_PAINT:
        BeginPaint(hWnd, &ps);
        window->onPaint();
        EndPaint(hWnd, &ps);
        eventHandled = true;
        break;

    case WM_CLOSE:
        PostQuitMessage(0);
        eventHandled = true;
        break;

    case WM_ACTIVATE:
        // disable/enable rendering here, depending on wParam != WA_INACTIVE
        break;

    case WM_SIZE:
        window->onResize(LOWORD(lParam), HIWORD(lParam));
        eventHandled = true;
        break;

    case WM_UNICHAR:
        eventHandled = window->onChar((SkUnichar)wParam,
            get_modifiers(message, wParam, lParam));
        break;

    case WM_CHAR: {
        const uint16_t* cPtr = reinterpret_cast<uint16_t*>(&wParam);
        SkUnichar c = SkUTF::NextUTF16(&cPtr, cPtr + 2);
        eventHandled = window->onChar(c, get_modifiers(message, wParam, lParam));
    } break;

    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
        eventHandled = window->onKey(get_key(wParam), skui::InputState::kDown,
            get_modifiers(message, wParam, lParam));
        break;

    case WM_KEYUP:
    case WM_SYSKEYUP:
        eventHandled = window->onKey(get_key(wParam), skui::InputState::kUp,
            get_modifiers(message, wParam, lParam));
        break;

    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP: {
        int xPos = GET_X_LPARAM(lParam);
        int yPos = GET_Y_LPARAM(lParam);

        //if (!gIsFullscreen)
        //{
        //    RECT rc = { 0, 0, 640, 480 };
        //    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
        //    xPos -= rc.left;
        //    yPos -= rc.top;
        //}

        skui::InputState istate = ((wParam & MK_LBUTTON) != 0) ? skui::InputState::kDown
            : skui::InputState::kUp;

        eventHandled = window->onMouse(xPos, yPos, istate,
            get_modifiers(message, wParam, lParam));
    } break;

    case WM_MOUSEMOVE: {
        int xPos = GET_X_LPARAM(lParam);
        int yPos = GET_Y_LPARAM(lParam);

        //if (!gIsFullscreen)
        //{
        //    RECT rc = { 0, 0, 640, 480 };
        //    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
        //    xPos -= rc.left;
        //    yPos -= rc.top;
        //}

        eventHandled = window->onMouse(xPos, yPos, skui::InputState::kMove,
            get_modifiers(message, wParam, lParam));
    } break;

    case WM_MOUSEWHEEL:
        eventHandled = window->onMouseWheel(GET_WHEEL_DELTA_WPARAM(wParam) > 0 ? +1.0f : -1.0f,
            get_modifiers(message, wParam, lParam));
        break;

    case WM_TOUCH: {
        uint16_t numInputs = LOWORD(wParam);
        std::unique_ptr<TOUCHINPUT[]> inputs(new TOUCHINPUT[numInputs]);
        if (GetTouchInputInfo((HTOUCHINPUT)lParam, numInputs, inputs.get(),
            sizeof(TOUCHINPUT))) {
            POINT topLeft = { 0, 0 };
            ClientToScreen(hWnd, &topLeft);
            for (uint16_t i = 0; i < numInputs; ++i) {
                TOUCHINPUT ti = inputs[i];
                skui::InputState state;
                if (ti.dwFlags & TOUCHEVENTF_DOWN) {
                    state = skui::InputState::kDown;
                }
                else if (ti.dwFlags & TOUCHEVENTF_MOVE) {
                    state = skui::InputState::kMove;
                }
                else if (ti.dwFlags & TOUCHEVENTF_UP) {
                    state = skui::InputState::kUp;
                }
                else {
                    continue;
                }
                // TOUCHINPUT coordinates are in 100ths of pixels
                // Adjust for that, and make them window relative
                LONG tx = (ti.x / 100) - topLeft.x;
                LONG ty = (ti.y / 100) - topLeft.y;
                eventHandled = window->onTouch(ti.dwID, state, tx, ty) || eventHandled;
            }
        }
    } break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return eventHandled ? 0 : 1;
}


Window::Window() {}

Window::~Window() {
    DestroyWindow(fHWnd);
}

void Window::detach() { fWindowContext = nullptr; }

void Window::visitLayers(std::function<void(Layer*)> visitor) {
    for (int i = 0; i < fLayers.count(); ++i) {
        if (fLayers[i]->fActive) {
            visitor(fLayers[i]);
        }
    }
}

bool Window::signalLayers(std::function<bool(Layer*)> visitor) {
    for (int i = fLayers.count() - 1; i >= 0; --i) {
        if (fLayers[i]->fActive && visitor(fLayers[i])) {
            return true;
        }
    }
    return false;
}

void Window::onBackendCreated() {
    this->visitLayers([](Layer* layer) { layer->onBackendCreated(); });
}

bool Window::onChar(SkUnichar c, skui::ModifierKey modifiers) {
    return this->signalLayers([=](Layer* layer) { return layer->onChar(c, modifiers); });
}

bool Window::onKey(skui::Key key, skui::InputState state, skui::ModifierKey modifiers) {
    return this->signalLayers([=](Layer* layer) { return layer->onKey(key, state, modifiers); });
}

bool Window::onMouse(int x, int y, skui::InputState state, skui::ModifierKey modifiers) {
    return this->signalLayers([=](Layer* layer) { return layer->onMouse(x, y, state, modifiers); });
}

bool Window::onMouseWheel(float delta, skui::ModifierKey modifiers) {
    return this->signalLayers([=](Layer* layer) { return layer->onMouseWheel(delta, modifiers); });
}

bool Window::onTouch(intptr_t owner, skui::InputState state, float x, float y) {
    return this->signalLayers([=](Layer* layer) { return layer->onTouch(owner, state, x, y); });
}

bool Window::onFling(skui::InputState state) {
    return this->signalLayers([=](Layer* layer) { return layer->onFling(state); });
}

bool Window::onPinch(skui::InputState state, float scale, float x, float y) {
    return this->signalLayers([=](Layer* layer) { return layer->onPinch(state, scale, x, y); });
}

void Window::onUIStateChanged(const SkString& stateName, const SkString& stateValue) {
    this->visitLayers([=](Layer* layer) { layer->onUIStateChanged(stateName, stateValue); });
}

void Window::onPaint() {
    if (!fWindowContext) {
        return;
    }
    if (!fIsActive) {
        return;
    }
    sk_sp<SkSurface> backbuffer = fWindowContext->getBackbufferSurface();
    if (backbuffer == nullptr) {
        printf("no backbuffer!?\n");
        // TODO: try recreating testcontext
        return;
    }
    fIsContentInvalidated = false;
    // draw into the canvas of this surface
    this->visitLayers([](Layer* layer) { layer->onPrePaint(); });
    this->visitLayers([=](Layer* layer) { layer->onPaint(backbuffer.get()); });

    backbuffer->flushAndSubmit();

    fWindowContext->swapBuffers();
}

void Window::onResize(int w, int h) {
    if (!fWindowContext) {
        return;
    }
    fWindowContext->resize(w, h);
    this->visitLayers([=](Layer* layer) { layer->onResize(w, h); });
}

void Window::onActivate(bool isActive) {
    fIsActive = isActive;
}

int Window::width() const {
    if (!fWindowContext) {
        return 0;
    }
    return fWindowContext->width();
}

int Window::height() const {
    if (!fWindowContext) {
        return 0;
    }
    return fWindowContext->height();
}

int Window::sampleCount() const {
    if (!fWindowContext) {
        return 0;
    }
    return fWindowContext->sampleCount();
}

int Window::stencilBits() const {
    if (!fWindowContext) {
        return -1;
    }
    return fWindowContext->stencilBits();
}

GrDirectContext* Window::directContext() const {
    if (!fWindowContext) {
        return nullptr;
    }
    return fWindowContext->directContext();
}

void Window::inval() {
    if (!fWindowContext) {
        return;
    }
    if (!fIsContentInvalidated) {
        fIsContentInvalidated = true;
        InvalidateRect(fHWnd, nullptr, false);
    }
}

void Window::setTitle(const char* title) {
    SetWindowTextA(fHWnd, title);
}
void Window::show() {
    ShowWindow(fHWnd, SW_SHOW);
}

bool Window::attach() {
    fWindowContext = std::unique_ptr<WindowContext>(new WindowContext(fHWnd, fRequestedDisplayParams));
    if (!fWindowContext->isValid()) {
        return false;
    }
    this->onBackendCreated();
    return (SkToBool(fWindowContext));
}

}   // namespace sk_app
