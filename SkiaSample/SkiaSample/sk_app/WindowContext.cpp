#include "WindowContext.h"
#include "include/gpu/GrDirectContext.h"

namespace sk_app {

WindowContext::WindowContext(const DisplayParams& params)
        : fDisplayParams(params) {}

WindowContext::~WindowContext() {}

}   //namespace sk_app
