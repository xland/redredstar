#pragma once
class SkCanvas;
class YGNode;
namespace RRS {
	class WindowBase;
	class Element
	{
	public:
		Element();
		~Element();
		void Paint(SkCanvas* canvas);
		/// <summary>
		/// show the element
		/// </summary>
		void Show();
		/// <summary>
		/// show the element
		/// </summary>
		void Hide();
		/// <summary>
		/// Add an element to the window
		/// </summary>
		void AddElement(Element* element);
		void SetLayoutPadding(int padding);
		void SetLayoutPadding(int left, int top, int right, int bottom);
		void SetLayoutMargin(int margin);
		void SetLayoutMargin(int left, int top, int right, int bottom);
		void SetBackgroundColor(unsigned r, unsigned g, unsigned b, unsigned a);
		/// <summary>
		/// element position x
		/// ShowInCenterScreen must be set to false
		/// </summary>
		int X = 0;
		/// <summary>
		/// element position y
		/// ShowInCenterScreen must be set to false
		/// </summary>
		int Y = 0;
		/// <summary>
		/// element width
		/// </summary>
		int Width = 200;
		/// <summary>
		/// element height
		/// </summary>
		int Height = 200;
		uint32_t BackgroundColor;
		/// <summary>
		/// The window pointer this element belong to
		/// </summary>
		WindowBase* OwnerWindow = nullptr;
		/// <summary>
		/// 
		/// </summary>
		Element* ParentElement;
		/// <summary>
		/// 
		/// </summary>
		std::vector<Element*> Children;
		/// <summary>
		/// 
		/// </summary>
		YGNode* Layout;
	private:

	};
}