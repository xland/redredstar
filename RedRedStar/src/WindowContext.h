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
    WindowContext(HWND, RRS::DisplayParams*);
    virtual ~WindowContext();
    sk_sp<SkSurface> getBackbufferSurface(int w,int h);
    void swapBuffers();
    void setDisplayParams(DisplayParams* params);

protected:
    void initializeContext();
    // This should be called by subclass destructor. It is also called when window/display
    // parameters change prior to initializing a new GL context. This will in turn call
    // onDestroyContext().
    void destroyContext();
    sk_sp<const GrGLInterface> onInitializeContext();
    sk_sp<GrDirectContext> fContext;
    DisplayParams*    displayParams;
    // parameters obtained from the native window
    // Note that the platform .cpp file is responsible for
    // initializing fSampleCount and fStencilBits!
    int               fSampleCount = 1;
    int               fStencilBits = 0;
    HWND              fHWND;
    HGLRC             fHGLRC;
    sk_sp<const GrGLInterface> fBackendContext;
};

}
