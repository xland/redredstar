#pragma once
#include "DisplayParams.h"
#include <Windows.h>
namespace RRS {
	class WindowContext
	{
	public:
		WindowContext(HWND wnd, const DisplayParams&);
		~WindowContext();
		sk_sp<SkSurface> getBackbufferSurface();
	private:
		DisplayParams displayParams;
		sk_sp<GrDirectContext> context;
		sk_sp<const GrGLInterface> backendContext;
		sk_sp<SkSurface> surface;
		int width;
		int height;
		int sampleCount = 1;
		int stencilBits = 0;
	};
}

