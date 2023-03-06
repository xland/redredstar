#pragma once
#include "CommonType.h"
#include "Color.h"
#include "EventListener.h"
#include "Layout.h"
class SkCanvas;
namespace RRS {
	class Window;
	class Element:public EventListener,public Layout
	{
	public:
		Element();
		~Element();
		void SetWidth(float width) override;
		void SetHeight(float height) override;
		float GetWidth() override;
		float GetHeight() override;
		virtual void Paint(SkCanvas* canvas);
		/// <summary>
		/// show the element
		/// </summary>
		void Show();
		/// <summary>
		/// show the element
		/// </summary>
		void Hide();
		void SetIsMouseEnter(int x, int y);
		void Click();
		void AddChild(std::shared_ptr<Element> child);
		void SetBackgroundColor(Color color);
		void SetBackgroundColorHover(Color color);
		void SetDirty(bool flag) override;
		bool IsOutOfView();
		Element* ParentElement = nullptr;
		bool IsMouseEnter = false;		
		Window* OwnerWindow = nullptr;
		float BorderRadius = 0.f;
	protected:
	private:

		float width = 0.f;
		float height = 0.f;
		int hoverId = -1;
		int hoverOffId = -1;
		Color backgroundColorHover = GetColor(255, 255, 255);
		Color backgroundColor = GetColor(255, 255, 255);
		bool isHide = false;
	};
}