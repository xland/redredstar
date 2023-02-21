#include "WindowContext.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkSurface.h"
#include "include/gpu/GrBackendSurface.h"
#include "include/gpu/GrDirectContext.h"
#include "src/gpu/gl/GrGLDefines.h"
#include "src/gpu/gl/GrGLUtil.h"
namespace RRS {
    WindowContext::WindowContext(HWND wnd, const DisplayParams& params)
        : displayParams(params) 
    {
    }
    WindowContext::~WindowContext() 
    {
    }
    sk_sp<SkSurface> WindowContext::getBackbufferSurface() {
        if (nullptr == surface) {
            if (context) {
                GrGLint buffer;
                GR_GL_CALL(backendContext.get(), GetIntegerv(GR_GL_FRAMEBUFFER_BINDING, &buffer));
                GrGLFramebufferInfo fbInfo;
                fbInfo.fFBOID = buffer;
                fbInfo.fFormat = GR_GL_RGBA8;
                GrBackendRenderTarget backendRT(width, height, sampleCount, stencilBits, fbInfo);
                surface = SkSurface::MakeFromBackendRenderTarget(context.get(), backendRT,
                    kBottomLeft_GrSurfaceOrigin,
                    kRGBA_8888_SkColorType,
                    displayParams.fColorSpace,
                    &displayParams.fSurfaceProps);
            }
        }
        return surface;
    }

}