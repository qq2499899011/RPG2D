#include "RPG2Dpch.h"
#include "Platform/Windows/WindowsWindow.h"

//#include "RPG2D/Core/Input.h"

#include "RPG2D/Events/ApplicationEvent.h"
#include "RPG2D/Events/MouseEvent.h"
#include "RPG2D/Events/KeyEvent.h"
#include <glad/glad.h>
//#include "RPG2D/Function/RendererManager/RendererManager.h"

//#include "Platform/OpenGL/OpenGLContext.h"

namespace RPG2D {

	static uint8_t s_GLFWWindowCount = 0;//�жϵ�ǰ������������glfw���г�ʼ������ֹ�������������ڵ�ʱ�����=0��Ҫ��ʼ�����رմ��ڵ�ʱ�����=0����ֹglfw

	static void GLFWErrorCallback(int error, const char* description)
	{
		RPG2D_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	//�½���ʱ���Զ����г�ʼ��������
	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		

		Shutdown();
	}
	//��ʼ������
	/*
		1.���ô������ԣ�
		������Ĵ������ԣ�����⡢��Ⱥ͸߶ȣ��洢���ڲ����ݳ�Ա�С�

		2.��ʼ�� GLFW��
		��鵱ǰ�Ƿ��Ѿ���ʼ���� GLFW�����û������г�ʼ���������ô���ص�������

		3.���� GLFW ���ڣ�
		ʹ�� GLFW �ĺ�������һ�����ڣ�ͬʱ���´��ڼ�������

		4.����ͼ�������ģ�
		ʹ�� GraphicsContext ��ľ�̬��������һ���봰�ڹ�����ͼ�������ģ������г�ʼ����

		5.���� GLFW �ص�������
		���ô��ڴ�С�仯�����ڹرա��������롢��������������ƶ����¼��Ļص�������
	*/
	void WindowsWindow::Init(const WindowProps& props)
	{
		//1.���ô�������
		

		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		RPG2D_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		//2.��ʼ��glfw
		if (s_GLFWWindowCount == 0)
		{
			//�����ǰ
			int success = glfwInit();
			RPG2D_CORE_ASSERT(success, "Could not initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
		}
		//3.����glfw����
		{
			#if defined(RPG2D_DEBUG)
			if (RendererManager::GetAPI() == RendererAPI::API::OpenGL)
				glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);//����Opengl�ĵ�����Ϣ��
			#endif
			m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
			++s_GLFWWindowCount;
		}

		//4.����������,����opengl���� ����ʼ�� TODO:��������ʱ����
		/*
		m_Context = GraphicsContext::Create(m_Window);
		m_Context->Init();
		*/
		//HACK:��ʱ
		glfwMakeContextCurrent(m_Window);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		//

		glfwSetWindowUserPointer(m_Window, &m_Data);//��m_Data�󶨵��û�ָ�����棬�Ӷ�������ػ�ȡ����
		SetVSync(true);

		//5.���ûص����� ��ʵ����ע������������Ը����¼�����ʱ���д��� �����Ѿ����úõĻص��������д����¼��������ģ�����֮���͵��¼�������ĺ���
		// Set GLFW callbacks
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		{
			//���ڴ�С�仯�¼�
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;

			WindowResizeEvent event(width, height);
			//ֱ�Ӻ���OnEvent,��������Ӧ����
			GlobalContext::GetInstance()->m_EventSystem->TriggerEvent(EventType::WindowResize,&event);
			//data.EventCallback(event);
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			//ֱ�ӵ���TriggerEvent
			GlobalContext::GetInstance()->m_EventSystem->TriggerEvent(EventType::WindowClose,&event);
			//data.EventCallback(event);
		});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			
			switch (action)
			{
			case GLFW_PRESS:
			{
				KeyPressedEvent event(key, 0);
				//data.EventCallback(event);
				GlobalContext::GetInstance()->m_EventSystem->TriggerEvent(EventType::KeyPressed,&event);
				break;
			}
			case GLFW_RELEASE:
			{
				KeyReleasedEvent event(key);
				GlobalContext::GetInstance()->m_EventSystem->TriggerEvent(EventType::KeyReleased,&event);
				//data.EventCallback(event);
				break;
			}
			case GLFW_REPEAT:
			{
				KeyPressedEvent event(key, true);
				GlobalContext::GetInstance()->m_EventSystem->TriggerEvent(EventType::KeyPressed,&event);
				//data.EventCallback(event);
				break;
			}
			}
		});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			KeyTypedEvent event(keycode);
			GlobalContext::GetInstance()->m_EventSystem->TriggerEvent(EventType::KeyTyped,&event);
			//data.EventCallback(event);
		});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
		{
			//������Ӧ���¼�������
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
			case GLFW_PRESS:
			{
				MouseButtonPressedEvent event(button);
				GlobalContext::GetInstance()->m_EventSystem->TriggerEvent(EventType::MouseButtonPressed,&event);
				//data.EventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				MouseButtonReleasedEvent event(button);
				GlobalContext::GetInstance()->m_EventSystem->TriggerEvent(EventType::MouseButtonReleased,&event);
				//data.EventCallback(event);
				break;
			}
			}
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			GlobalContext::GetInstance()->m_EventSystem->TriggerEvent(EventType::MouseScrolled,&event);
			//data.EventCallback(event);
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseMovedEvent event((float)xPos, (float)yPos);
			GlobalContext::GetInstance()->m_EventSystem->TriggerEvent(EventType::MouseMoved,&event);
			//data.EventCallback(event);
		});
	}

	void WindowsWindow::Shutdown()
	{
		

		glfwDestroyWindow(m_Window);//����glfw����
		--s_GLFWWindowCount;

		if (s_GLFWWindowCount == 0)
		{
			glfwTerminate();//�ر�glfw
		}
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();// �������¼�
		//��Ⱦ�����󽻻�����
		glfwSwapBuffers(m_Window);
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		

		if (enabled)
			glfwSwapInterval(1);//���ô�ֱͬ�� ͼ����Ⱦ�봹ֱͬ���ź�ͬ��
		else
			glfwSwapInterval(0);//������

		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}

}
