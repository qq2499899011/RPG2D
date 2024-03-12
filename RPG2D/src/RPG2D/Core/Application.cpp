#include "RPG2Dpch.h"
#include "Application.h"
#include "LogSystem.h"
#include "Timestep.h"

namespace RPG2D {


	Application* Application::s_Instance = nullptr;
	Application::Application() {
		//判断并指定单例
		RPG2D_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;
		//创建窗口
		m_Window = Window::Create();
		m_Window->SetEventCallback(RPG2D_BIND_EVENT_FN(Application::OnEvent));//这里是把当前的app对象捕获进去了。
		//创建游戏
		m_Game = new Game(m_Window->GetWidth(), m_Window->GetHeight());
		m_Game->Init();
		
		//新建imgui层，同时将imguiLayer加入到layerStack里面
		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}
	Application::~Application() {
		//需要销毁一些东西
	}

	//包装
	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
	}

	void Application::Run() {
		//就在这里对整个游戏的核心进行处理
		//窗口更新;

		//不同子系统进行更新;

		//逻辑处理 输入 物理 动画 
		
		//渲染处理 UI texture


		while (m_Running) {
			m_Window->OnUpdate();
			//HACK:渲染游戏 是不是应该clearbit一下
			m_Game->Render();
			//HACK:暂时放在这里
			for (Layer* layer : m_LayerStack)
				layer->OnUpdate(1);
			m_ImGuiLayer->Begin();
			{
				RPG2D_PROFILE_SCOPE("LayerStack OnImGuiRender");

				for (Layer* layer : m_LayerStack)
					layer->OnImGuiRender();
			}
			m_ImGuiLayer->End();
		}
		
	}
	void Application::OnEvent(Event& e) {
		//处理事件中心
		RPG2D_CORE_INFO(e);
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(RPG2D_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(RPG2D_BIND_EVENT_FN(Application::OnWindowResize));
		//分发处理事件
		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			//位于layer_stack越靠后优先级越高
			if (e.Handled)
				break;
			(*it)->OnEvent(e);
		}
	}
	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}
	//HACK:部分内容没实现，先保留函数并注释
	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		/*
		* 		RPG2D_PROFILE_FUNCTION();

		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		RendererManager::OnWindowResize(e.GetWidth(), e.GetHeight());
		*/
		return false;//不会过滤，后面需要对event进行再处理
	}
}