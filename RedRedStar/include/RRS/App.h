#pragma once
#include <Windows.h>
namespace RRS {
	class App
	{
		public:
			App(const App&) = delete;
			App& operator=(const App&) = delete;
			/// <summary>
			/// 获取App全局实例
			/// </summary>
			/// <returns></returns>
			static App* Get();
			/// <summary>
			/// 初始化App对象
			/// </summary>
			static void Init(HINSTANCE hInstance);
			~App();
			static int Exec();
			HINSTANCE hInstance;
		private:
			App(HINSTANCE hInstance);
			inline static App* app{ nullptr };
	};
}