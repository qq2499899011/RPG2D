#include "RPG2Dpch.h"
#include "Application.h"
#include "LogSystem.h"
#include "Timestep.h"
#include "RPG2D/Function/Renderer/RenderCommand.h"
namespace RPG2D {


	Application* Application::s_Instance = nullptr;
	Application::Application() {
		//�жϲ�ָ������
		RPG2D_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;
	}
	Application::~Application() {
		//��Ҫ����һЩ����
	}

	//��װ
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
		//��һЩ׼��
		//��ʼ��GlobalContext
		GlobalContext::Create();
		//��������
		m_Window = Window::Create();
		//�൱��ֻҪm_Window�������¼����͵���App::OnEvent������
		//ע���¼�����
		RegisterEvent();
		m_Window->SetEventCallback(RPG2D_BIND_EVENT_FN(Application::OnEvent));//�����ǰѵ�ǰ��app���󲶻��ȥ�ˡ�
		//��ʼ��;
		GlobalContext::GetInstance()->Init();
		/*
		//�½�imgui�㣬ͬʱ��imguiLayer���뵽layerStack����
		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
		*/
	}

	void Application::Update(Timestep ts)
	{
		//���ø��º���
		GlobalContext::GetInstance()->Update(ts);
	}
	//��Ӧ�ó�������Ҫ�������¼�����ע��
	void Application::RegisterEvent()
	{
		GlobalContext::GetInstance()->m_EventSystem->RegisterEventHandler(EventType::WindowClose,RPG2D_BIND_EVENT_FN(OnWindowClose));
		GlobalContext::GetInstance()->m_EventSystem->RegisterEventHandler(EventType::WindowResize,RPG2D_BIND_EVENT_FN(OnWindowResize));
	}
	void Application::Run() {
		//��ʼ������
		GlobalContext::GetInstance()->m_RendererManager->Init();
		//globalContext��ȡ����system��
		//����->�ű�->����->��Ⱦ->UI,�Գ������д���
		//��¼ʱ�䡣
		while (m_Running) {
			//��ȡʱ����
			float time = GetTime();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;
			//HACK:��ʱ��������
			Update(timestep);
			//���Խ�����Ⱦ
			/*
			
			for (Layer* layer : m_LayerStack)
				layer->OnUpdate(1);
			m_ImGuiLayer->Begin();
			{
				RPG2D_PROFILE_SCOPE("LayerStack OnImGuiRender");
				for (Layer* layer : m_LayerStack)
					layer->OnImGuiRender();
			}
			//HACK:��Ⱦ��Ϸ �ǲ���Ӧ��clearbitһ��
			m_ImGuiLayer->End();
			*/
			
			//�����¼�+����������
			m_Window->OnUpdate();
		}
	}
	//�¼��ַ��������ڷֱ����ע��
	//�������¼�֮���ٽ��зַ�����,ÿ�ζ���������һ��OnEvent,ֻͨ��Layer�ķ�ʽ�����¼���
	void Application::OnEvent(Event& e) {
		//���ܵ�Event֮��
		//�����¼�����
		RPG2D_CORE_INFO(e);
		EventDispatcher dispatcher(e);
		//Ӧ����һЩ�еĴ�����,��������һ������
		//dispatcher.Dispatch<WindowCloseEvent>(RPG2D_BIND_EVENT_FN(Application::OnWindowClose));
		//dispatcher.Dispatch<WindowResizeEvent>(RPG2D_BIND_EVENT_FN(Application::OnWindowResize));
		//�ַ������¼�
		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			//λ��layer_stackԽ�������ȼ�Խ��
			if (e.Handled)
				break;
			(*it)->OnEvent(e);
		}
	}

	bool Application::OnWindowClose(Event* e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(Event* event)
	{
		RPG2D_CORE_INFO("WindowResize!!!!");
		//��ѡ���¼�ת����Ϊ��Ӧ������
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
	
	
	/*
	
	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		RPG2D_CORE_INFO("WindowResize");
		//��ѡ���¼�ת����Ϊ��Ӧ������
		//WindowResizeEvent* e = dynamic_cast<WindowResizeEvent*>(event);
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}
		m_Minimized = false;
		GlobalContext::GetInstance()->m_RendererManager->OnWindowResize(e.GetWidth(), e.GetHeight());
		return false;
	}
	
	*/
}