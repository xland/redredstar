#pragma once
#include "CommonType.h"
#include "Color.h"
#include "EventListener.h"
#include "Layout.h"
class SkCanvas;
class SkPaint;
class SkRect;
namespace RRS {
	class Window;
	class Element:public EventListener,public Layout
	{
	public:
		Element();
		~Element();		
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
		Color GetBackgroundColor() { return backgroundColor; };
		Color GetBackgroundColorHover() { return backgroundColorHover; };
		void SetDirty(bool flag) override;
		virtual void OnMouseEnter() {};
		virtual void OnMouseOut() {};
		virtual void OnClick() {};
		virtual Color GetCurrentBackgroundColor();
		bool IsOutOfView();
		Element* ParentElement{ nullptr };
		bool IsMouseEnter{ false };
		Window* OwnerWindow{ nullptr };
		float BorderRadius{ 0.f };
	protected:
	private:
		void drawBorder(SkPaint& paint, SkCanvas* canvas, SkRect& rect);
		int hoverId{-1};
		int hoverOffId{-1};
		Color backgroundColorHover{ UINT32_MAX };
		Color backgroundColor{ GetColor(255, 255, 255) };
		bool isHide{ false };
	};
}