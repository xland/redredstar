#pragma once
#include "CommonType.h"
#include "Color.h"
#include "EventListener.h"
class SkCanvas;
namespace RRS {
	class Window;
	class Element:public EventListener
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
		std::vector<std::shared_ptr<Element>> Children;
		void SetBackgroundColor(Color color);
		void SetBackgroundColorHover(Color color);
		Window* OwnerWindow = nullptr;
		Element* ParentElement = nullptr;
		bool IsMouseEnter = false;
		float X = 0.f;
		float Y = 0.f;
		float Width = 0.f;
		float Height = 0.f;
		bool IsWidthPercent = false;
		bool IsHeightPercent = false;
		bool IsFlexDirectionRow = true;
		unsigned AlignmentVertical = 0;
		unsigned AlignmentHorizontal = 0;
		float Flex = 0.f;
		float BorderRadius = 0.f;
	protected:
	private:
		void regMouseHoverEvent();
		int hoverId = -1;
		int hoverOffId = -1;
		Color backgroundColorHover = GetColor(255, 255, 255);
		Color backgroundColor = GetColor(255, 255, 255);
		bool isHide = false;
	};
}