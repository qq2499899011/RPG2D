#include "RPG2Dpch.h"
#include "Application.h"
#include "LogSystem.h"
#include "Timestep.h"
#include "RPG2D/Function/Renderer/RenderCommand.h"
namespace RPG2D {


	Application* Application::s_Instance = nullptr;
	Application::Application() {
		//判断并指定单例
		RPG2D_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;
		//创建窗口
		m_Window = Window::Create();
		m_Window->SetEventCallback(RPG2D_BIND_EVENT_FN(Application::OnEvent));//这里是把当前的app对象捕获进去了。
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
	
	void Application::Init()
	{
		//做一些准备
		//初始化GlobalContext
		GlobalContext::Create();
		//生成GetInstace;
		GlobalContext::GetInstance()->Init();
	}

	void Application::Update(Timestep ts)
	{
		//调用更新函数
		GlobalContext::GetInstance()->Update(ts);
	}
	void Application::Run() {
		//初始化
		GlobalContext::GetInstance()->m_RendererManager->Init();
		//globalContext获取所有system。
		//物理->脚本->动画->渲染->UI,对场景进行处理。
		//记录时间。
		while (m_Running) {
			//获取时间间隔
			float time = GetTime();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;
			//HACK:暂时放在这里
			Update(timestep);
			
			for (Layer* layer : m_LayerStack)
				layer->OnUpdate(1);
			m_ImGuiLayer->Begin();
			{
				RPG2D_PROFILE_SCOPE("LayerStack OnImGuiRender");
				for (Layer* layer : m_LayerStack)
					layer->OnImGuiRender();
			}
			//HACK:渲染游戏 是不是应该clearbit一下
			m_ImGuiLayer->End();
			
			
			//处理事件+交换上下文
			m_Window->OnUpdate();
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
	
	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		RPG2D_PROFILE_FUNCTION();

		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		GlobalContext::GetInstance()->m_RendererManager->OnWindowResize(e.GetWidth(), e.GetHeight());
		return false;
	}
}