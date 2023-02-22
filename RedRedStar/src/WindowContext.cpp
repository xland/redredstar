#include "WindowContext.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkSurface.h"
#include "include/gpu/GrBackendSurface.h"
#include "include/gpu/GrDirectContext.h"
#include "src/core/SkMathPriv.h"
#include "src/gpu/GrCaps.h"
#include "src/gpu/GrDirectContextPriv.h"
#include "src/gpu/gl/GrGLDefines.h"
#include "src/gpu/gl/GrGLUtil.h"
#include "src/image/SkImage_Base.h"
#include "src/utils/win/SkWGL.h"
#include <GL/gl.h>


namespace RRS {

WindowContext::WindowContext(HWND wnd, DisplayParams* params)
        : displayParams(params), fBackendContext(nullptr) , fSurface(nullptr) , fHWND(wnd) , fHGLRC(nullptr)
{
    displayParams->fMSAASampleCount = GrNextPow2(displayParams->fMSAASampleCount);
    // any config code here (particularly for msaa)?
    this->initializeContext();
}

WindowContext::~WindowContext() {
    destroyContext();
}

void WindowContext::destroyContext() {
    fSurface.reset(nullptr);
    if (fContext) {
        // in case we have outstanding refs to this (lua?)
        fContext->abandonContext();
        fContext.reset();
    }
    fBackendContext.reset(nullptr);
    wglDeleteContext(fHGLRC);
    fHGLRC = NULL;
}

void WindowContext::initializeContext() {
    SkASSERT(!fContext);
    fBackendContext = this->onInitializeContext();
    fContext = GrDirectContext::MakeGL(fBackendContext, displayParams->fGrContextOptions);
    if (!fContext && displayParams->fMSAASampleCount > 1) {
        displayParams->fMSAASampleCount /= 2;
        this->initializeContext();
        return;
    }
}
bool WindowContext::isValid() { 
    return SkToBool(fBackendContext.get());
}
void WindowContext::swapBuffers() {
    HDC dc = GetDC((HWND)fHWND);
    SwapBuffers(dc);
    ReleaseDC((HWND)fHWND, dc);
}

void WindowContext::resize(int w, int h) {
    this->destroyContext();
    this->initializeContext();
}

void WindowContext::setDisplayParams(DisplayParams* params) {
    displayParams = params;
    this->destroyContext();
    this->initializeContext();
}

sk_sp<SkSurface> WindowContext::getBackbufferSurface() {
    if (nullptr == fSurface) {
        if (fContext) {
            GrGLint buffer;
            GR_GL_CALL(fBackendContext.get(), GetIntegerv(GR_GL_FRAMEBUFFER_BINDING, &buffer));

            GrGLFramebufferInfo fbInfo;
            fbInfo.fFBOID = buffer;
            fbInfo.fFormat = GR_GL_RGBA8;

            GrBackendRenderTarget backendRT(fWidth,
                fHeight,
                fSampleCount,
                fStencilBits,
                fbInfo);
            fSurface = SkSurface::MakeFromBackendRenderTarget(fContext.get(), backendRT,
                kBottomLeft_GrSurfaceOrigin,
                kRGBA_8888_SkColorType,
                displayParams->fColorSpace,
                &displayParams->fSurfaceProps);
        }
    }

    return fSurface;
}

sk_sp<const GrGLInterface> WindowContext::onInitializeContext() {
    HDC dc = GetDC(fHWND);
    fHGLRC = SkCreateWGLContext(dc, displayParams->fMSAASampleCount, false /* deepColor */,
        kGLPreferCompatibilityProfile_SkWGLContextRequest);
    if (nullptr == fHGLRC) {
        return nullptr;
    }
    SkWGLExtensions extensions;
    if (extensions.hasExtension(dc, "WGL_EXT_swap_control")) {
        extensions.swapInterval(displayParams->fDisableVsync ? 0 : 1);
    }
    // Look to see if RenderDoc is attached. If so, re-create the context with a core profile
    if (wglMakeCurrent(dc, fHGLRC)) {
        auto interfaceObj = GrGLMakeNativeInterface();
        bool renderDocAttached = interfaceObj->hasExtension("GL_EXT_debug_tool");
        interfaceObj.reset(nullptr);
        if (renderDocAttached) {
            wglDeleteContext(fHGLRC);
            fHGLRC = SkCreateWGLContext(dc, displayParams->fMSAASampleCount, false /* deepColor */,
                kGLPreferCoreProfile_SkWGLContextRequest);
            if (nullptr == fHGLRC) {
                return nullptr;
            }
        }
    }
    if (wglMakeCurrent(dc, fHGLRC)) {
        glClearStencil(0);
        glClearColor(0, 0, 0, 0);
        glStencilMask(0xffffffff);
        glClear(GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        // use DescribePixelFormat to get the stencil and color bit depth.
        int pixelFormat = GetPixelFormat(dc);
        PIXELFORMATDESCRIPTOR pfd;
        DescribePixelFormat(dc, pixelFormat, sizeof(pfd), &pfd);
        fStencilBits = pfd.cStencilBits;

        // Get sample count if the MSAA WGL extension is present
        if (extensions.hasExtension(dc, "WGL_ARB_multisample")) {
            static const int kSampleCountAttr = SK_WGL_SAMPLES;
            extensions.getPixelFormatAttribiv(dc,
                pixelFormat,
                0,
                1,
                &kSampleCountAttr,
                &fSampleCount);
            fSampleCount = std::max(fSampleCount, 1);
        }
        else {
            fSampleCount = 1;
        }

        RECT rect;
        GetClientRect(fHWND, &rect);
        fWidth = rect.right - rect.left;
        fHeight = rect.bottom - rect.top;
        glViewport(0, 0, fWidth, fHeight);
    }
    return GrGLMakeNativeInterface();
}


}   //namespace sk_app
