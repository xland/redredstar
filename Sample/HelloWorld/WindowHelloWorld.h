#pragma once
#include <RRS/Window.h>
class WindowHelloWorld : public RRS::Window
{
public:
	WindowHelloWorld();
	void OnLoad() override;
	void OnClosed() override;
};

