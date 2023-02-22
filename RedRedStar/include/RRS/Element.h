#pragma once
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
		/// element position x
		/// ShowInCenterScreen must be set to false
		/// </summary>
		int X = CW_USEDEFAULT;
		/// <summary>
		/// element position y
		/// ShowInCenterScreen must be set to false
		/// </summary>
		int Y = CW_USEDEFAULT;
		/// <summary>
		/// element width
		/// </summary>
		int Width = 1000;
		/// <summary>
		/// element height
		/// </summary>
		int Height = 700;
		/// <summary>
		/// The window pointer this element belong to
		/// </summary>
		WindowBase* OwnerWindow;
		/// <summary>
		/// 
		/// </summary>
		std::vector<Element*> Children;
	private:

	};
}