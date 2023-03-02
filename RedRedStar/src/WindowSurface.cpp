#include "../include/RRS/Window.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkSurface.h"
#include "include/gpu/GrBackendSurface.h"
#include "include/gpu/GrDirectContext.h"
#include "src/core/SkMathPriv.h"
#include "src/gpu/GrCaps.h"
#include "src/gpu/GrDirectContextPriv.h"
#include "src/gpu/gl/GrGLDefines.h"
#include "src/image/SkImage_Base.h"
#include "src/utils/win/SkWGL.h"
#include <GL/gl.h>
#include "src/gpu/gl/GrGLUtil.h"
#include "DisplayParams.h"

namespace RRS
{
	static DisplayParams displayParam;
	void Window::initSurface()
	{
        HDC dc = GetDC(Hwnd);
        hglrc = SkCreateWGLContext(dc, displayParam.fMSAASampleCount, false, kGLPreferCompatibilityProfile_SkWGLContextRequest);
        if (nullptr == hglrc) {
            return;
        }
        SkWGLExtensions extensions;
        if (extensions.hasExtension(dc, "WGL_EXT_swap_control")) {
            extensions.swapInterval(displayParam.fDisableVsync ? 0 : 1);
        }
        if (wglMakeCurrent(dc, hglrc)) {
            glClearStencil(0);
            glClearColor(0, 0, 0, 0);
            glStencilMask(0xffffffff);
            glClear(GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
            // use DescribePixelFormat to get the stencil and color bit depth.
            int pixelFormat = GetPixelFormat(dc);
            PIXELFORMATDESCRIPTOR pfd;
            DescribePixelFormat(dc, pixelFormat, sizeof(pfd), &pfd);
            stencilBits = pfd.cStencilBits;
        }
        auto param = GrGLMakeNativeInterface();
        backendContext = param.release();
        auto context = GrDirectContext::MakeGL(param, displayParam.fGrContextOptions);
        directContext = context.release();
	}

    SkSurface* Window::getSurface()
    {
        glViewport(0, 0, widthClient, heightClient);
        GrGLint buffer;
        GR_GL_CALL(backendContext, GetIntegerv(GR_GL_FRAMEBUFFER_BINDING, &buffer));
        GrGLFramebufferInfo fbInfo;
        fbInfo.fFBOID = buffer;
        fbInfo.fFormat = GR_GL_RGBA8;
        GrBackendRenderTarget backendRT(widthClient, heightClient, displayParam.fMSAASampleCount, stencilBits, fbInfo);
        auto fSurface = SkSurface::MakeFromBackendRenderTarget(directContext, backendRT,
            kBottomLeft_GrSurfaceOrigin,
            kRGBA_8888_SkColorType,
            displayParam.fColorSpace,
            &displayParam.fSurfaceProps);
        return fSurface.release();
    }
    void Window::disposeSurfaceResource() 
    {
        if (directContext) {
            directContext->abandonContext();
            delete directContext;
        }
        delete backendContext;
        wglDeleteContext(hglrc);
        hglrc = nullptr;
    }
}