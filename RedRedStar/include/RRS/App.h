#pragma once
#include <Windows.h>
#include <vector>
namespace RRS {
	class WindowBase;
	class App
	{
		public:
			App(const App&) = delete;
			App& operator=(const App&) = delete;
			~App();
			/// <summary>
			/// Get Global App Instance
			/// </summary>
			/// <returns></returns>
			static App* Get();
			/// <summary>
			/// Initialize Global App object
			/// </summary>
			static void Init(HINSTANCE hInstance);
			/// <summary>
			/// Start message loop
			/// </summary>
			/// <returns></returns>
			static int Exec();
			/// <summary>
			/// Quit Application
			/// </summary>
			static void Quit();
			void RemoveWindow(WindowBase* window);
			/// <summary>
			/// Native application instance
			/// </summary>
			HINSTANCE HInstance;
			std::vector<WindowBase*> Windows;

		private:
			App(HINSTANCE hInstance);
			
			inline static App* app{ nullptr };
	};
}