#pragma once
#include <Windows.h>
#include <vector>
#include <functional>
namespace RRS {
	class Window;
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
			static void OnAllWindowClosed(std::function<void()>&& cb);
			void RemoveWindow(Window* window);
			void AddWindow(Window* window);
			/// <summary>
			/// Native application instance
			/// </summary>
			HINSTANCE HInstance;
		private:
			std::vector<Window*> windows;
			App(HINSTANCE hInstance);
			std::function<void()> onAllWindowClosed;
			inline static App* app{ nullptr };
	};
}