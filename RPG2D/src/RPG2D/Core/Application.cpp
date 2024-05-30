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
	}

	void Application::Update(Timestep ts)
	{
		//���ø��º���
		GlobalContext::GetInstance()->Update(ts);
		//�������ʱ��
	}
	//��Ӧ�ó�������Ҫ�������¼�����ע��
	void Application::RegisterEvent()
	{
		GlobalContext::GetInstance()->m_EventSystem->RegisterEventHandler(EventType::WindowClose, RPG2D_BIND_EVENT_FN(OnWindowClose));
		GlobalContext::GetInstance()->m_EventSystem->RegisterEventHandler(EventType::WindowResize, RPG2D_BIND_EVENT_FN(OnWindowResize));
		//GlobalContext::GetInstance()->m_EventSystem->RegisterEventHandler(EventType::KeyPressed,RPG2D_BIND_EVENT_FN(OnKeyPress));
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
			m_Window->OnUpdate();
		}
	}
	//�¼��ַ��������ڷֱ����ע��
	void Application::OnEvent(Event& e) {
		//���ܵ�Event֮��
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

	bool Application::OnKeyPress(Event* e)
	{
		//��ѡ���¼�ת����Ϊ��Ӧ������
		KeyPressedEvent* event = dynamic_cast<KeyPressedEvent*>(e);
		//������µİ���
		RPG2D_CORE_TRACE("key pressed:{0}", event->GetKeyCode());
		return false;
	}

}