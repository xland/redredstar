#pragma once
#include <RRS/WindowBase.h>
class WindowHelloWorld : public RRS::WindowBase
{
	WindowHelloWorld();
	void OnLoad() override;
	bool IsMouseInCaptionArea(int x, int y) override;
};

