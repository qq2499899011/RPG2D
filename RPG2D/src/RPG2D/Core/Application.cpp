#include "RPG2Dpch.h"
#include "Application.h"
#include "LogSystem.h"
#include "Timestep.h"

namespace RPG2D {


	Application* Application::s_Instance = nullptr;
	Application::Application() {
		//�жϲ�ָ������
		RPG2D_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;
		//��������
		m_Window = Window::Create();
		m_Window->SetEventCallback(RPG2D_BIND_EVENT_FN(Application::OnEvent));//�����ǰѵ�ǰ��app���󲶻��ȥ�ˡ�
		//������Ϸ
		m_Game = new Game(m_Window->GetWidth(), m_Window->GetHeight());
		m_Game->Init();
		
		//�½�imgui�㣬ͬʱ��imguiLayer���뵽layerStack����
		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
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

	void Application::Run() {
		//���������������Ϸ�ĺ��Ľ��д���
		//���ڸ���;

		//��ͬ��ϵͳ���и���;

		//�߼����� ���� ���� ���� 
		
		//��Ⱦ���� UI texture


		while (m_Running) {
			m_Window->OnUpdate();
			//HACK:��Ⱦ��Ϸ �ǲ���Ӧ��clearbitһ��
			m_Game->Render();
			//HACK:��ʱ��������
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
		//�����¼�����
		RPG2D_CORE_INFO(e);
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(RPG2D_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(RPG2D_BIND_EVENT_FN(Application::OnWindowResize));
		//�ַ������¼�
		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			//λ��layer_stackԽ�������ȼ�Խ��
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
	//HACK:��������ûʵ�֣��ȱ���������ע��
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
		return false;//������ˣ�������Ҫ��event�����ٴ���
	}
}