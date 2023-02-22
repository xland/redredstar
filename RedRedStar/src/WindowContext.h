#pragma once
#include "include/gpu/gl/GrGLInterface.h"
#include "include/core/SkSurface.h"
#include "include/core/SkRefCnt.h"
#include "include/core/SkSurfaceProps.h"
#include "include/gpu/GrTypes.h"
#include "DisplayParams.h"
#include <Windows.h>

class GrDirectContext;
class SkSurface;

namespace RRS {

class WindowContext {
public:
    WindowContext(HWND, const DisplayParams&);
    virtual ~WindowContext();
    sk_sp<SkSurface> getBackbufferSurface();
    void swapBuffers();
    bool isValid();
    void resize(int w, int h);
    const DisplayParams& getDisplayParams() { return fDisplayParams; }
    void setDisplayParams(const DisplayParams& params);
    GrDirectContext* directContext() const { return fContext.get(); }
    int width() const { return fWidth; }
    int height() const { return fHeight; }
    SkISize dimensions() const { return {fWidth, fHeight}; }
    int sampleCount() const { return fSampleCount; }
    int stencilBits() const { return fStencilBits; }

protected:
    void initializeContext();
    // This should be called by subclass destructor. It is also called when window/display
    // parameters change prior to initializing a new GL context. This will in turn call
    // onDestroyContext().
    void destroyContext();
    sk_sp<const GrGLInterface> onInitializeContext();
    sk_sp<GrDirectContext> fContext;
    int               fWidth;
    int               fHeight;
    DisplayParams     fDisplayParams;
    // parameters obtained from the native window
    // Note that the platform .cpp file is responsible for
    // initializing fSampleCount and fStencilBits!
    int               fSampleCount = 1;
    int               fStencilBits = 0;
    HWND              fHWND;
    HGLRC             fHGLRC;
    sk_sp<const GrGLInterface> fBackendContext;
    sk_sp<SkSurface>           fSurface;
};

}
