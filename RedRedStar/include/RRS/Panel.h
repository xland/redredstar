#pragma once
#include "Element.h"
#include <vector>
namespace RRS {
	class Panel : public Element
	{
	public:
		Panel();
		void Paint(SkCanvas* canvas) override;
		/// <summary>
		/// Add an element to the window
		/// </summary>
		void AddElement(Element* element);
		/// <summary>
		/// 
		/// </summary>
		std::vector<Element*> Children;
	};
}


