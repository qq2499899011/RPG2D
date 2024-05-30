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

	static uint8_t s_GLFWWindowCount = 0;//判断当前窗口数量，对glfw进行初始化和终止操作。创建窗口的时候如果=0需要初始化，关闭窗口的时候如果=0则终止glfw

	static void GLFWErrorCallback(int error, const char* description)
	{
		RPG2D_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	//新建的时候自动进行初始化操作。
	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		

		Shutdown();
	}
	//初始化窗口
	/*
		1.设置窗口属性：
		将传入的窗口属性（如标题、宽度和高度）存储到内部数据成员中。

		2.初始化 GLFW：
		检查当前是否已经初始化过 GLFW，如果没有则进行初始化，并设置错误回调函数。

		3.创建 GLFW 窗口：
		使用 GLFW 的函数创建一个窗口，同时更新窗口计数器。

		4.创建图形上下文：
		使用 GraphicsContext 类的静态函数创建一个与窗口关联的图形上下文，并进行初始化。

		5.设置 GLFW 回调函数：
		设置窗口大小变化、窗口关闭、键盘输入、鼠标点击、滚动和移动等事件的回调函数。
	*/
	void WindowsWindow::Init(const WindowProps& props)
	{
		//1.设置窗口属性
		

		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		RPG2D_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		//2.初始化glfw
		if (s_GLFWWindowCount == 0)
		{
			//如果当前
			int success = glfwInit();
			RPG2D_CORE_ASSERT(success, "Could not initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
		}
		//3.创建glfw窗口
		{
			#if defined(RPG2D_DEBUG)
			if (RendererManager::GetAPI() == RendererAPI::API::OpenGL)
				glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);//捕获Opengl的调试信息。
			#endif
			m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
			++s_GLFWWindowCount;
		}

		//4.创建上下文,加载opengl函数 并初始化 TODO:上下文暂时禁用
		/*
		m_Context = GraphicsContext::Create(m_Window);
		m_Context->Init();
		*/
		//HACK:暂时
		glfwMakeContextCurrent(m_Window);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		//

		glfwSetWindowUserPointer(m_Window, &m_Data);//将m_Data绑定到用户指针上面，从而更方便地获取数据
		SetVSync(true);

		//5.设置回调函数 其实就是注册监听函数，对各种事件发生时进行处理。 调用已经设置好的回调函数进行处理，事件产生中心，产生之后发送到事件处理核心函数
		// Set GLFW callbacks
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		{
			//窗口大小变化事件
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;

			WindowResizeEvent event(width, height);
			//直接呼叫OnEvent,出发了相应函数
			GlobalContext::GetInstance()->m_EventSystem->TriggerEvent(EventType::WindowResize,&event);
			//data.EventCallback(event);
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			//直接调用TriggerEvent
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
			//调用相应的事件处理函数
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
		

		glfwDestroyWindow(m_Window);//销毁glfw窗口
		--s_GLFWWindowCount;

		if (s_GLFWWindowCount == 0)
		{
			glfwTerminate();//关闭glfw
		}
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();// 处理窗口事件
		//渲染结束后交换缓冲
		glfwSwapBuffers(m_Window);
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		

		if (enabled)
			glfwSwapInterval(1);//启用垂直同步 图像渲染与垂直同步信号同步
		else
			glfwSwapInterval(0);//不启用

		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}

}
