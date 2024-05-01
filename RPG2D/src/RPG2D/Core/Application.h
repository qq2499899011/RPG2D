#pragma once
#include "Window.h"
#include "RPG2D/Events/ApplicationEvent.h"
#include "RPG2D/Core/LayerStack.h"
#include "RPG2D/Function/Renderer/game.h"
#include "RPG2D/ImGui/ImGuiLayer.h"
namespace RPG2D {
	class Application
	{
	public:
		Application();
		virtual ~Application();
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
		void Run();
		void OnEvent(Event& e);
		static Application& Get() { return *s_Instance; }
		Window& GetWindow() { return *m_Window; }
		Scope<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		Game* m_Game = nullptr;
		//所有manager和system。
	private:
		void Update(Timestep ts);
		void Init();
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
		static Application* s_Instance;//单例模式
	};
	//在sandbox中定义
	Application* CreateApplication();
}


