#include "RPG2Dpch.h"
#include "Platform/OpenGL/OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace RPG2D {

	/// <summary>
	/// 通过传入glfw句柄进行初始化
	/// </summary>
	/// <param name="windowHandle"></param>
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		RPG2D_CORE_ASSERT(windowHandle, "Window handle is null!")
	}

	/// <summary>
	/// 关联上下文，加载opengl函数。并输出当前显卡和opengl信息。
	/// </summary>
	void OpenGLContext::Init()
	{
		RPG2D_PROFILE_FUNCTION();
		//指定上下文，将当前窗口关联到opengl的上下文，这样接下来的所有渲染都会渲染到当前窗口上
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		RPG2D_CORE_ASSERT(status, "Failed to initialize Glad!");

		RPG2D_CORE_INFO("OpenGL Info:");
		RPG2D_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
		RPG2D_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		RPG2D_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));

		RPG2D_CORE_ASSERT(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5), "RPG2D requires at least OpenGL version 4.5!");
	}
	/// <summary>
	/// 交换缓冲，简单的对glfwSwapBuffer进行封装。
	/// </summary>
	void OpenGLContext::SwapBuffers()
	{
		RPG2D_PROFILE_FUNCTION();

		glfwSwapBuffers(m_WindowHandle);
	}

}
