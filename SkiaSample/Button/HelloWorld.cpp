#include "HelloWorld.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkFont.h"
#include "include/core/SkGraphics.h"
#include "include/core/SkSurface.h"
#include "include/effects/SkGradientShader.h"

using namespace sk_app;

HelloWorld::HelloWorld(HINSTANCE hinstance):fRotationAngle(0) {
    SkGraphics::Init();
    fWindow = Window::CreateNativeWindow(hinstance);
    fWindow->setRequestedDisplayParams(DisplayParams());
    // register callbacks
    fWindow->pushLayer(this);
    fWindow->attach();
}

HelloWorld::~HelloWorld() {
    fWindow->detach();
    delete fWindow;
}

void HelloWorld::updateTitle() {
    if (!fWindow || fWindow->sampleCount() <= 1) {
        return;
    }
    SkString title("Hello World OpenGL");
    fWindow->setTitle(title.c_str());
}

void HelloWorld::onBackendCreated() {
    this->updateTitle();
    fWindow->show();
    fWindow->inval();
}

void HelloWorld::onPaint(SkSurface* surface) {
    auto canvas = surface->getCanvas();
    canvas->clear(SK_ColorWHITE);
    SkPaint paint;
    paint.setColor(SkColorSetARGB(255,220,220,220));
    paint.setStrokeJoin(SkPaint::Join::kRound_Join);
    SkRect rect = SkRect::MakeXYWH(20, 20, 180, 50);
    canvas->drawRoundRect(rect, 12,32,paint);
}

void HelloWorld::onIdle() {
    // Just re-paint continuously
    fWindow->inval();
}