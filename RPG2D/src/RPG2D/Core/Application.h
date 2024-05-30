#pragma once
#include "Window.h"
#include "RPG2D/Events/ApplicationEvent.h"
#include "RPG2D/Events/KeyEvent.h"
namespace RPG2D {
	class Application
	{
	public:
		Application();
		virtual ~Application();
		virtual void Init();
		void Run();
		void OnEvent(Event& e);
		void RegisterEvent();
		static Application& Get() { return *s_Instance; }
		Window& GetWindow() { return *m_Window; }
		float GetTime() { return m_Window->GetTime(); }
		Scope<Window> m_Window;
		//所有manager和system。
	private:
		void Update(Timestep ts);
		bool m_Running = true;
		bool m_Minimized = false;
		float m_LastFrameTime = 0.0f;
		bool OnWindowResize(Event* e);
		bool OnWindowClose(Event* e);
		bool OnKeyPress(Event* e);
		static Application* s_Instance;//单例模式
	};
	//在sandbox中定义
	Application* CreateApplication();
}


