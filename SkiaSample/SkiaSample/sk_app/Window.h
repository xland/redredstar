#pragma once
#include "include/core/SkRect.h"
#include "include/core/SkTypes.h"
#include "include/private/SkTDArray.h"
#include "DisplayParams.h"
#include "tools/skui/InputState.h"
#include "tools/skui/Key.h"
#include "tools/skui/ModifierKey.h"
#include <functional>
#include <Windows.h>
class GrDirectContext;
class SkCanvas;
class SkSurface;
class SkSurfaceProps;
class SkString;

namespace skgpu {
class Context;
}

namespace sk_app {

class WindowContext;

class Window {
public:
    static Window* CreateNativeWindow(HINSTANCE hInstance);
    virtual ~Window();
    virtual void setTitle(const char*) = 0;
    virtual void show() = 0;
    virtual void setClipboardText(const char*) {}

    // Schedules an invalidation event for window if one is not currently pending.
    // Make sure that either onPaint or markInvalReceived is called when the client window consumes
    // the the inval event. They unset fIsContentInvalided which allow future onInval.
    void inval();
    virtual bool scaleContentToFit() const { return false; }

    virtual bool attach() = 0;
    void detach();

    // input handling

    class Layer {
    public:
        Layer() : fActive(true) {}
        virtual ~Layer() = default;

        bool getActive() { return fActive; }
        void setActive(bool active) { fActive = active; }

        // return value of 'true' means 'I have handled this event'
        virtual void onBackendCreated() {}
        virtual void onAttach(Window* window) {}
        virtual bool onChar(SkUnichar c, skui::ModifierKey) { return false; }
        virtual bool onKey(skui::Key, skui::InputState, skui::ModifierKey) { return false; }
        virtual bool onMouse(int x, int y, skui::InputState, skui::ModifierKey) { return false; }
        virtual bool onMouseWheel(float delta, skui::ModifierKey) { return false; }
        virtual bool onTouch(intptr_t owner, skui::InputState, float x, float y) { return false; }
        // Platform-detected gesture events
        virtual bool onFling(skui::InputState state) { return false; }
        virtual bool onPinch(skui::InputState state, float scale, float x, float y) { return false; }
        virtual void onUIStateChanged(const SkString& stateName, const SkString& stateValue) {}
        virtual void onPrePaint() {}
        virtual void onPaint(SkSurface*) {}
        virtual void onResize(int width, int height) {}

    private:
        friend class Window;
        bool fActive;
    };

    void pushLayer(Layer* layer) {
        layer->onAttach(this);
        fLayers.push_back(layer);
    }

    void onBackendCreated();
    bool onChar(SkUnichar c, skui::ModifierKey modifiers);
    bool onKey(skui::Key key, skui::InputState state, skui::ModifierKey modifiers);
    bool onMouse(int x, int y, skui::InputState state, skui::ModifierKey modifiers);
    bool onMouseWheel(float delta, skui::ModifierKey modifiers);
    bool onTouch(intptr_t owner, skui::InputState state, float x, float y);  // multi-owner = multi-touch
    // Platform-detected gesture events
    bool onFling(skui::InputState state);
    bool onPinch(skui::InputState state, float scale, float x, float y);
    void onUIStateChanged(const SkString& stateName, const SkString& stateValue);
    void onPaint();
    void onResize(int width, int height);
    void onActivate(bool isActive);

    int width() const;
    int height() const;
    virtual float scaleFactor() const { return 1.0f; }

    virtual const DisplayParams& getRequestedDisplayParams() { return fRequestedDisplayParams; }
    virtual void setRequestedDisplayParams(const DisplayParams&, bool allowReattach = true);

    // Actual parameters in effect, obtained from the native window.
    int sampleCount() const;
    int stencilBits() const;

    // Returns null if there is not a GPU backend or if the backend is not yet created.
    GrDirectContext* directContext() const;
    skgpu::Context* graphiteContext() const;

protected:
    Window();

    SkTDArray<Layer*>      fLayers;
    DisplayParams          fRequestedDisplayParams;
    bool                   fIsActive = true;

    std::unique_ptr<WindowContext> fWindowContext;

    virtual void onInval() = 0;

    // Uncheck fIsContentInvalided to allow future inval/onInval.
    void markInvalProcessed();

    bool fIsContentInvalidated = false;  // use this to avoid duplicate invalidate events

    void visitLayers(std::function<void(Layer*)> visitor);
    bool signalLayers(std::function<bool(Layer*)> visitor);
};

}
