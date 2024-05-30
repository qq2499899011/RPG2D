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
	}
	Application::~Application() {
		//需要销毁一些东西
	}

	void Application::Init()
	{
		//做一些准备
		//初始化GlobalContext
		GlobalContext::Create();
		//创建窗口
		m_Window = Window::Create();
		//相当于只要m_Window产生了事件，就调用App::OnEvent函数。
		//注册事件函数
		RegisterEvent();
		m_Window->SetEventCallback(RPG2D_BIND_EVENT_FN(Application::OnEvent));//这里是把当前的app对象捕获进去了。
		//初始化;
		GlobalContext::GetInstance()->Init();
	}

	void Application::Update(Timestep ts)
	{
		//调用更新函数
		GlobalContext::GetInstance()->Update(ts);
		//输出播放时间
	}
	//对应用程序中需要监听的事件进行注册
	void Application::RegisterEvent()
	{
		GlobalContext::GetInstance()->m_EventSystem->RegisterEventHandler(EventType::WindowClose, RPG2D_BIND_EVENT_FN(OnWindowClose));
		GlobalContext::GetInstance()->m_EventSystem->RegisterEventHandler(EventType::WindowResize, RPG2D_BIND_EVENT_FN(OnWindowResize));
		//GlobalContext::GetInstance()->m_EventSystem->RegisterEventHandler(EventType::KeyPressed,RPG2D_BIND_EVENT_FN(OnKeyPress));
	}
	void Application::Run() {
		//初始化内容
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
			m_Window->OnUpdate();
		}
	}
	//事件分发处理。现在分别进行注册
	void Application::OnEvent(Event& e) {
		//接受到Event之后
		RPG2D_CORE_INFO(e);
		//dispatcher.Dispatch<WindowCloseEvent>(RPG2D_BIND_EVENT_FN(Application::OnWindowClose));
		//dispatcher.Dispatch<WindowResizeEvent>(RPG2D_BIND_EVENT_FN(Application::OnWindowResize));
	}

	bool Application::OnWindowClose(Event* e)
	{
		m_Running = false;
		GlobalContext::GetInstance()->DeInit();
		return true;
	}

	bool Application::OnWindowResize(Event* event)
	{
		RPG2D_CORE_INFO("WindowResize!!!!");
		//首选将事件转化成为对应的类型
		WindowResizeEvent* e = dynamic_cast<WindowResizeEvent*>(event);
		if (e->GetWidth() == 0 || e->GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}
		m_Minimized = false;
		GlobalContext::GetInstance()->m_RendererManager->OnWindowResize(e->GetWidth(), e->GetHeight());
		return false;
	}

	bool Application::OnKeyPress(Event* e)
	{
		//首选将事件转化成为对应的类型
		KeyPressedEvent* event = dynamic_cast<KeyPressedEvent*>(e);
		//输出按下的按键
		RPG2D_CORE_TRACE("key pressed:{0}", event->GetKeyCode());
		return false;
	}

}