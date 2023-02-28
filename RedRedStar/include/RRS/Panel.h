#pragma once
#include "Element.h"
#include <vector>
#include <memory>
namespace RRS {
	class Panel : public Element
	{
	public:
		Panel();
		~Panel();
		void Paint(SkCanvas* canvas) override;
		void SetIsMouseEnter(int x, int y) override;
		/// <summary>
		/// Add an element to the window
		/// </summary>
		void AddElement(std::shared_ptr<Element> element);
		void SetBackgroundColor(Color color);
		void SetBackgroundColorHover(Color color);
	private:
		Color backgroundColor;
		Color backgroundColorHover;
		/// <summary>
		/// 
		/// </summary>
		std::vector<std::shared_ptr<Element>> Children;
	};
}


