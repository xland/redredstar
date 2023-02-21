#pragma once
#include "include/core/SkImageInfo.h"
#include "include/core/SkSurfaceProps.h"
#include "include/gpu/GrContextOptions.h"

namespace sk_app {

struct DisplayParams {
    DisplayParams()
        : fColorType(kN32_SkColorType)
        , fColorSpace(nullptr)
        , fMSAASampleCount(1)
        , fSurfaceProps(0, kRGB_H_SkPixelGeometry)
        , fDisableVsync(false)
        , fDelayDrawableAcquisition(false)
        , fEnableBinaryArchive(false)
    {}

    SkColorType         fColorType;
    sk_sp<SkColorSpace> fColorSpace;
    int                 fMSAASampleCount;
    GrContextOptions    fGrContextOptions;
    SkSurfaceProps      fSurfaceProps;
    bool                fDisableVsync;
    bool                fDelayDrawableAcquisition;
    bool                fEnableBinaryArchive;
};

}
