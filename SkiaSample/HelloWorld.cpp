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

    // Clear background
    canvas->clear(SK_ColorWHITE);

    SkPaint paint;
    paint.setColor(SK_ColorRED);

    // Draw a rectangle with red paint
    SkRect rect = SkRect::MakeXYWH(10, 10, 128, 128);
    canvas->drawRect(rect, paint);

    // Set up a linear gradient and draw a circle
    {
        SkPoint linearPoints[] = { { 0, 0 }, { 300, 300 } };
        SkColor linearColors[] = { SK_ColorGREEN, SK_ColorBLACK };
        paint.setShader(SkGradientShader::MakeLinear(linearPoints, linearColors, nullptr, 2, SkTileMode::kMirror));
        paint.setAntiAlias(true);
        canvas->drawCircle(200, 200, 64, paint);
        // Detach shader
        paint.setShader(nullptr);
    }
    canvas->save();

    // Draw a message with a nice black paint
    SkFont font;
    font.setSubpixel(true);
    font.setSize(20);
    paint.setColor(SK_ColorBLACK);
    static const char message[] = "Hello World ";

    // Translate and rotate
    canvas->translate(300, 300);//这是位移
    fRotationAngle += 0.2f;
    if (fRotationAngle > 360) {
        fRotationAngle -= 360;
    }
    canvas->rotate(fRotationAngle); //这是旋转
    // Draw the text
    canvas->drawSimpleText(message, strlen(message), SkTextEncoding::kUTF8, 0, 0, font, paint);

    canvas->restore();
}

void HelloWorld::onIdle() {
    // Just re-paint continuously
    fWindow->inval();
}
/// <summary>
/// 这里原本是用来切换渲染引擎的，精简过之后，就不起作用了
/// </summary>
/// <param name="c"></param>
/// <param name="modifiers"></param>
/// <returns></returns>
bool HelloWorld::onChar(SkUnichar c, skui::ModifierKey modifiers) {
    if (' ' == c) {
        fWindow->detach();
        fWindow->attach();
    }
    return true;
}
