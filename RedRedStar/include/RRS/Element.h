#pragma once
#include "CommonType.h"
#include "Color.h"
#include "Layout.h"
#include "EventListener.h"
class SkCanvas;
namespace RRS {
	class Window;
	class Element:public Layout,public EventListener
	{
	public:
		Element();
		~Element();
		virtual void Paint(SkCanvas* canvas) = 0;
		/// <summary>
		/// show the element
		/// </summary>
		void Show();
		/// <summary>
		/// show the element
		/// </summary>
		void Hide();
		virtual void SetIsMouseEnter(int x, int y);
		virtual void CalculatePosition();
		bool GetIsMouseEnter();
		/// <summary>
		/// 当子元素被添加到父元素内，再把父元素添加到窗口内，此时子元素的ownerWindow为空
		/// 我们通过子元素的GetOwnerWindow方法获取ownerWindow时，会遍历它的父元素，直到找到ownerWindow为止
		/// 获取到子元素的ownerWindow后，这个指针会被缓存下来，下次就不用再执行遍历操作了
		/// </summary>
		/// <returns></returns>
		Window* GetOwnerWindow();
		Element* GetParentElement();
		virtual void EmitClickEvent();
		void SetParentElement(Element* element);
	protected:
	private:
		friend Window;
		bool isMouseEnter = false;
		Window* ownerWindow;
		Element* parentElement;
	};
}